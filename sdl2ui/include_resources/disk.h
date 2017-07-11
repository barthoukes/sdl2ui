/*
 * disk.h
 *
 *  Created on: Dec 31, 2010
 *      Author: mensfort
 */

#pragma once

#include <string>
#include <vector>

typedef std::vector<std::string> fileList;
typedef fileList::iterator fileIterator;

typedef struct
{
	std::string file;
	int length;
	std::string flags;
	std::string date;
} fileStruct;


typedef std::vector<fileStruct> fileDetailList;


class Cdisk
{
public:
	Cdisk() {}
	virtual ~Cdisk() {}
	//bool save();
	void close( FILE *f);
	static bool fileCopy(const std::string &dest, const std::string &source);
	static bool getdir(std::string dir, fileList &files);
	static bool getdir(std::string dir, const std::string &find, std::vector<std::string> &files);
	static bool fileExists( const std::string &file);
	static int fileSize( const std::string &file);
	static bool fileDelete( const std::string &name);
	static void getDir( const char *files, fileDetailList &list);
};

/* DISK_H_ */
