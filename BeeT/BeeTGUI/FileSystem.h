#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"

#include <vector>
#include <string>

class FileSystem : public Module
{
public:
	FileSystem(const char* name);
	~FileSystem();

	bool Init();

	unsigned int Load(const char* path, char** buffer)const;
	unsigned int Save(const char* path, const void* buffer, unsigned int size)const;

	std::string GetDirectory()const;

	std::string GetFileNameFromPath(const std::string& path)const;
	std::string GetDirectoryFromPath(const std::string& path)const;

	unsigned int GetAllFilesInDirectory(const char* path, std::vector<std::string>& filenames)const; // Returns the number of files found
	unsigned int FilterFiles(const std::vector<std::string>& filenames, std::vector<std::string>& filtered, const std::string& filter)const;

private:
	std::string currentDirectory;
};
#endif