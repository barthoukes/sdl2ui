// Implementation of the Socket class.


#include <pthread.h>
#include "linux_socket.hpp"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "timeout.hpp"

/** @brief Constructor Csocket */
Csocket::Csocket()
: m_blocking(true)
, m_sock( -1)
, m_bufSize(0)
, m_totalReads(0)
, m_port(-1)
, m_wait(false)
{
	m_timeout =0;
	memset( m_buffer, 0, MAXRECV );
	memset( &m_addr, 0, sizeof(m_addr));
	memset( &m_returnAddress, 0, sizeof( m_returnAddress));
}

/** @brief Destructor */
Csocket::~Csocket()
{
	close();
}

/** @brief Close socket */
void Csocket::close()
{
	if ( m_wait)
	{
		//Log.write("Send socket to finished");
		send_bytes( "hoi");
	}
	if ( m_port>0)
	{
		Csocket socket; ///< Socket to send files.
		socket.create(2);
		socket.connect( "localhost", m_port);
	}
	delay(40);
	if ( is_valid())
	{
		::close( m_sock);
	}
	m_sock =-1;
	m_port =-1;
}

/** @brief Create socket
 *  @param type [in] 0=IPROTO_UDP, 1=PF_INET, 2=AF_INET, 3=SOCK_STREAM+AF_INET
 *  @return true on success
 */
bool Csocket::create( int type)
{
	m_totalReads =0;
	memset( &m_returnAddress, 0, sizeof( m_returnAddress));

	switch (type)
	{
	case 0:
		m_sock =socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
		break;
	case 1:
		m_sock =socket( PF_INET, SOCK_DGRAM, 0);
		break;
	case 4:
		m_sock =socket( AF_INET, SOCK_DGRAM, 0);
		break;
	default:
		m_sock =socket( AF_INET, SOCK_STREAM, 0);
		break;
	}
	if ( !is_valid())
		return false;

	// TIME_WAIT - argh
	int on =1;
	if ( setsockopt( m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*) &on,
			sizeof(on)) == -1)
		return false;

	return true;
}

/** @brief Bind socket to any port
 *  @param port [in] Incoming port to open
 *  @return true on success
 */
bool Csocket::bind( const int port)
{
	//Log.write( "Socket bin %d", port);
	if ( !is_valid())
	{
		return false;
	}
	m_port =port;
	m_addr.sin_family =AF_INET;
	m_addr.sin_addr.s_addr =INADDR_ANY;
	m_addr.sin_port =htons( port);

	int bind_return =::bind( m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));

	if ( bind_return == -1)
	{
		return false;
	}

	return true;
}

/** @brief Listen to a socket
 *  @return true when we're listening
 */
bool Csocket::listen() const
{
	if ( !is_valid())
	{
		return false;
	}

	int listen_return =::listen( m_sock, MAXCONNECTIONS);

	if ( listen_return == -1)
	{
		return false;
	}

	return true;
}

/** @brief Tell to accept another socket
 *  @param new_socket [in] Accept a socket and create a return socket
 *  @return true on success
 */
bool Csocket::accept( Csocket& new_socket) const
{
	try
	{
		new_socket.m_sock =0;
		int addr_length =sizeof(m_addr);
		new_socket.m_sock =::accept( m_sock, (sockaddr *) &m_addr,
				(socklen_t *) &addr_length);
	}
	catch (...)
	{
		return false;
	}
	return ( new_socket.m_sock > 0);
}

/** @brief Send data over a socket
 *  @param s [in] String to send over socket
 *  @return true on success
 */
bool Csocket::send_bytes( const std::string &s) const
{
	if ( s=="")
	{
		return true;
	}
	int status =::send( m_sock, s.c_str(), s.size()+1, MSG_NOSIGNAL );
	return (status != -1);
}

/** @brief Send data over socket
 *  @param data [in] What to send pointer
 *  @param length [in] Lenght of memory to send
 *  @return true on success
 */
int Csocket::send_bytes( const char *data, int length) const
{
	if ( data ==NULL || length==0)
	{
		return true;
	}
	int bytes_sent = ::send( m_sock, data, length, MSG_NOSIGNAL );
	return bytes_sent;
}

/** @brief Send data to a certain port and ip address
 *  @param ip [in] address
 *  @param port [in] port on destination
 *  @param s [in] Content to send
 *  @return true on success
 */
bool Csocket::send_to( const std::string &ip, int port, const std::string &s) const
{
	struct sockaddr_in addr;

	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr( ip.c_str()); //ip.c_str());
	addr.sin_port=htons( port);

	int x=sendto( m_sock, s.c_str(), s.length(), 0, (struct sockaddr*)&addr, sizeof(addr));
	return (x >= 0);
}

/** @brief Return socket
 *  @param s [in] What we send out
 *  @param port [in] Port to use for sending
 *  @return true on success
 */
bool Csocket::return_to( const std::string &s, int port)
{
	lock();
	if ( m_returnAddress.sin_port ==0)
	{
		return false;
	}
	if ( port>0)
	{
		m_returnAddress.sin_port=htons( port);
	}
	int x=sendto( m_sock, s.c_str(), s.length(), 0, (struct sockaddr*)&m_returnAddress, sizeof(m_returnAddress));
	return (x >= 0);
}

/** @brief Read data from any socket
 *  @param s [out] What we read
 *  @param timeout [in] Timeout
 *  @return true on success, false on timeout
 */
bool Csocket::read_bytes( std::string& s, int timeout)
{
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));

	if ( timeout !=m_timeout)
	{
		struct timeval tv;
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		setsockopt( m_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,  sizeof tv);
		m_timeout =timeout;
	}
	set_non_blocking( (timeout>0) ? true:false);
	char buf[MAXRECV + 1];
	s ="";
	memset( buf, 0, MAXRECV + 1);
	socklen_t len=sizeof(addr);
	m_wait =true;
	int status =::recvfrom( m_sock, buf, MAXRECV, 0, (struct sockaddr*)&addr, &len);
	m_wait =false;
	if ( status>0)
	{
		s =buf;
		lock();
		m_totalReads++;
		m_returnAddress =addr;
		unlock();
		return true;
	}
	return false;
}

/** @brief Connect to a host and port
 *  @param host [in] IP-address of destination, which should be listening
 *  @param port [in] Port to transmit data later
 *  @return true when the destination has a port on reading, false if destination is not accepting connections
 */
bool Csocket::connect( const std::string host, const int port)
{
	if ( !is_valid())
		return false;

	m_addr.sin_family =AF_INET;
	m_addr.sin_port =htons( port);

	int status =inet_pton( AF_INET, host.c_str(), &m_addr.sin_addr);

	if ( errno == EAFNOSUPPORT)
		return false;

	status =::connect( m_sock, (sockaddr *) &m_addr, sizeof(m_addr));

	return (status == 0);
}

/** @brief Set a socket to non-blocking. Experimental!
 *  @param b [in] true when the port should not block.
 */
void Csocket::set_non_blocking( const bool b)
{
	int opts;

	//if ( m_blocking ==b)
	//{
	//	return;
	//}
	m_blocking=!b;
	opts =fcntl( m_sock, F_GETFL );
	if ( opts < 0)
	{
		return;
	}
	if ( b)
		opts =(opts | O_NONBLOCK);
	else
		opts =(opts & ~O_NONBLOCK);
	fcntl( m_sock, F_SETFL, opts);
}

/* END */
