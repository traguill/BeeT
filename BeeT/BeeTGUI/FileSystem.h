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
	bool CleanUp();

	unsigned int Load(const char* path, char** buffer)const;
	unsigned int Save(const char* path, const void* buffer, unsigned int size)const;

	std::string GetDirectory()const;
	std::string GetFileNameFromPath(const std::string& path)const;
	std::string GetDirectoryFromPath(const std::string& path)const;
	unsigned int EnumerateFiles(const char* path, std::vector<std::string>& files);
	
	unsigned int FilterFiles(const std::vector<std::string>& filenames, std::vector<std::string>& filtered, const std::string& filter)const;
	bool IsDirectory(const char* fname)const;

private:
	bool AddPath(const char* path, const char* mountPoint = nullptr);

private:
	std::string currentDirectory;
};
#endif