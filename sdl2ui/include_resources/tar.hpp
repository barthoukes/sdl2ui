/*
 * tar.h
 *
 *  Created on: 8 mrt. 2016
 *      Author: mensfort
 */

#ifndef SOURCE_RESOURCES_TAR_H_
#define SOURCE_RESOURCES_TAR_H_

class tar {
public:
	char data[2048];
	tar();
	virtual ~tar();

	void extract( char *tar_file, char *path);
	const char *extract_line( const char *tar_file, const char *path);
	void compress( char *tar_file, char *path, char *source_file);
};

#endif /* SOURCE_RESOURCES_TAR_H_ */
