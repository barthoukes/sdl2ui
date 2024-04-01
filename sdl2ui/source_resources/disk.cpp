 /*============================================================================*/
/**  @file		disk.cpp
 *   @ingroup   zhongcan
 *   @brief     Disk functions.
 *
 *   File open, close, copy.
 *
 *   @author    Bart Houkes
 *============================================================================*/

#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include "disk.hpp"

/*function to get disk files*/
bool Cdisk::getDir(const std::string &dir, fileList &files)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL)
	{
		return false;
	}

	while ((dirp = readdir(dp)) != NULL)
	{
		if (dirp->d_name[0]!='.' && dirp->d_type!=DT_DIR)
		{
		    files.push_back(std::string(dirp->d_name));
		}
	}
	closedir( dp);
	return true;
}

bool Cdisk::getDir(const std::string &dir, const std::string &find, fileList &files)
{
    std::vector<std::string> a;
    return getDevDir(dir, find, files, a);
}

/*function... might want it in some class?*/
bool Cdisk::getDirectories(const std::string &dir, fileList &files)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL)
	{
		//cout << "Error(" << errno << ") opening " << dir << endl;
		return false;
	}

	while ((dirp = readdir(dp)) != NULL)
	{
		if (dirp->d_name[0]!='.' &&
		    (dirp->d_type==DT_DIR || dirp->d_type==DT_LNK))
		{
		    files.push_back(std::string(dirp->d_name));
		}
	}
	closedir( dp);
	return true;
}

/* @brief Get the directory files
 * @param dir [in] What directory to find
 * @param find [in] What to search for
 * @param files [out] What files found
 * */
bool Cdisk::getDevDir(std::string dir, const std::string &find, std::vector<std::string> &files,
		 const std::vector<std::string> &excludeFiles)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL)
	{
		//cout << "Error(" << errno << ") opening " << dir << endl;
		return false;
	}

	while ((dirp = readdir(dp)) != NULL)
	{
		std::string found =dirp->d_name;
		if ( found.find(find) !=std::string::npos)
		{
			bool valid = true;
			for ( const std::string &x : excludeFiles)
			{
				if (found.find(x) !=std::string::npos)
				{
					valid =false;
				}
			}
			if (valid)
			{
				files.push_back(std::string(dirp->d_name));
			}
		}
	}
	closedir( dp);
	return true;
}

/*--Bart Houkes--08/12/1994--------------------------------Status:Debugged--*/
/** @brief Check if file exists
 *  @param dest [out] Destination file
 *  @param source [in] Source file
 *  @return true on success
 */
bool Cdisk::fileCopy(const std::string &dest, const std::string &source)
{
	FILE *Fs;
	FILE *Fd;
	char buffer[256]; // Temporary data buffer
	size_t size;

	Fs = fopen(source.c_str(), "rb");
	if (!Fs)
		return false;
	Fd = fopen(dest.c_str(), "wb");
	if (!Fd)
	{
		fclose(Fs);
		return false;
	}
	size = fread(buffer, 1, 256, Fs);
	while (size)
	{
		fwrite(buffer, 1, size, Fd);
		size = fread(buffer, 1, 256, Fs);
	}
	fclose(Fd);
	fclose(Fs);
	return true;
}

/** @brief Check if file exists
 *  @param name [in] What file to inspect
 *  @return true on success, false if the file is not there
 */
bool Cdisk::fileExists( const std::string &name)
{
	FILE *F =fopen(name.c_str(), "r");
	if (!F)
	{
		return false;
	}
	fclose(F);
	return true;
}

/** @brief Safe way to remove file.
 *  @param name [in] What file to remove
 *  @return true on success
 */
bool Cdisk::fileDelete( const std::string &name)
{
	bool retVal =false;
	try
	{
		retVal =!unlink( name.c_str());
	}
	catch (...)
	{
	}
	return retVal;
}

/** @brief Safe way to remove file.
 *  @param name [in] What file to remove
 *  @return true on success
 */
int Cdisk::fileSize( const std::string &name)
{
	int retVal =0;
	try
	{
		FILE *F=fopen(name.c_str(), "rb");
		if (!F)
		{
			return 0;
		}
		// set the file pointer to end of file
		fseek( F, 0, SEEK_END );
		retVal = ftell( F );
		fclose( F );
	}
	catch (...)
	{
	}
	return retVal;
}

#define SKIPSPACE(x) while(*(x)!=' ' and *(x)) (x)++; while (*(x)==' ' && *(x)) (x)++;
#define SKIPLINE(x) while(*(x)>=' ' and *(x)) (x)++; while (*(x)<' ' && *(x)) (x)++;

void Cdisk::getDetailedDir(const std::string &fileList, fileDetailList &list)
{
	int mfiles=0;
	int mlines=0;
	list.clear();
	const char *files = fileList.c_str();
	while ( *files)
	{
		if (!*files)
		{
			break;
		}
		mlines++;
		if (files[1]=='r' || files[1]=='-')
		{
			// Valid line
			SKIPSPACE(files); // xr...
			SKIPSPACE(files); // 1
			SKIPSPACE(files); // mensfort
			SKIPSPACE(files); // mensfort
			fileStruct fs;
			fs.length=atoi(files);
			SKIPSPACE(files);
			SKIPSPACE(files); // month
			SKIPSPACE(files); // day
			SKIPSPACE(files); // hour:day
			int length=0;
			while (*(files+length)>=' ') length++;
			fs.file.assign(files, length);
			files+=length;
			if (length>=1)
			{
				list.push_back(fs);
				mfiles++;
			}
		}
		SKIPLINE(files);
	}
	printf("Lines=%d, files=%d\n", mlines,mfiles);
}

std::string Cdisk::getFileData( const std::string fileName)
{
    std::string retVal;
    try
    {
        FILE *F=fopen(fileName.c_str(), "rb");
        if (!F)
        {
            return 0;
        }
        // set the file pointer to end of file
        char data[4096];
        int n= fread(data, 1,4092, F);
        fclose( F );
        if (n>0)
        {
            data[n--] = 0;
        }
        while(n>0 && data[n]<32)
        {
            data[n--]=0;
        }
        retVal = data;
    }
    catch (...)
    {
    }
    return retVal;

}
