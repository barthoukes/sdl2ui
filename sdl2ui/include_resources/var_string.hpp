#pragma once

#include <string>
#include <string.h>

class var_string
{
public:
	var_string(const char *cmd, ...);
	explicit var_string(const std::string &name ="");
	~var_string();
	operator std::string();
	std::string string();
	operator const char*();
	int length();
	const char *c_str();
	int system();
	void toLower();
	std::string to_string();
    var_string& operator<<(unsigned char by);
    var_string& operator<<(short w);
    var_string& operator<<(int l);
	var_string& operator<<(unsigned int l);
	var_string& operator<<(long l);
	var_string& operator<<(unsigned long l);
	var_string& operator<<(double f);
	var_string& operator<<(char ch);
	var_string& operator<<(char *ch);
	var_string& operator<<(const char *ch);
	var_string& operator<<(const std::string &ch);
	var_string& operator<<(bool u);
	size_t find_last_not_of(const char *str, size_t pos=-1) const;
	void erase(size_t index);

private:
	//char buffer[4096];
	std::string m_buffer;
};

