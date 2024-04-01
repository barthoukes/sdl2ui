/*
 * tar.cpp
 *
 *  Created on: 8 mrt. 2016
 *      Author: mensfort
 */

#include "tar.hpp"
#include "stdio.h"
#include "stdlib.h"

tar::tar()
{
	// TODO Auto-generated constructor stub
}


tar::~tar()
{
	// TODO Auto-generated destructor stub
}


void tar::extract( char *tar_file, char *path)
{
	char s[2048];
	sprintf(s, "tar -C \"%s\" -v -xf \"%s\" ", path, tar_file);
	system(s);
}


const char *tar::extract_line( const char *tar_file, const char *path)
{
	sprintf(data, "tar -C \"%s\" -v -xf \"%s\" ", path, tar_file);
	return data;
}

void tar::compress( char *tar_file, char *path, char *source_file)
{
	char s[2048];
	sprintf(s, "tar -v -C \"%s\" -cf \"%s\" \"%s\"", path, tar_file, source_file);
	system(s);
}
