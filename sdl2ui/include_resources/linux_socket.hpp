// Definition of the Socket class
#pragma once

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#include "x86_64-linux-gnu/bits/pthreadtypes.h"
#else
#include "i386-linux-gnu/bits/pthreadtypes.h"
#endif
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <memory>
#include <string>
#include <arpa/inet.h>
#include "my_thread.hpp"

const int MAXHOSTNAME =200;
const int MAXCONNECTIONS =10;
const int MAXRECV =128 * 1024;

class Csocket : public CmyLock
{
public:
	Csocket();
	virtual ~Csocket();

	// Server initialization
	bool create( int type=0);
	bool bind( const int port);
	bool listen() const;
	bool accept( Csocket&) const;
	void close();
	// Client initialization
	bool connect( const std::string host, const int port);

	// Data Transimission
	bool send_bytes( const std::string &s) const;
	bool send_to( const std::string &ip, int port, const std::string &s) const;
	int send_bytes( const char *data, int length) const;
	bool return_to( const std::string &s, int port=-1);
	bool read_bytes( std::string& s, int timeout=10000);

	void set_non_blocking( const bool);

	bool is_valid() const
	{
		return m_sock != -1;
	}
	bool m_blocking;

private:

	int m_sock;
	sockaddr_in m_addr;
	int m_timeout;
	char m_buffer[MAXRECV];
	int m_bufSize;
	int m_totalReads;
	sockaddr_in m_returnAddress;
	std::string m_host; ///< what host.
	int m_port; ///< what port.
	bool m_wait; ///< r we waiting?
};

typedef std::shared_ptr<Csocket>   CsocketPtr;
