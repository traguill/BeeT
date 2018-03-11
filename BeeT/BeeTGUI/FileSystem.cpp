#include "FileSystem.h"
#include "Log.h"
#include "../PhysFS/include/physfs.h"
#include "../SDL2/include/SDL.h"

#ifdef _DEBUG
#pragma comment( lib, "../lib/physfs_staticd.lib" )
#else
#pragma comment( lib, "../lib/physfs_static.lib" )
#endif

using namespace std;

FileSystem::FileSystem(const char* name) : Module(name)
{}

FileSystem::~FileSystem()
{}

bool FileSystem::Init()
{

	char* basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);
	currentDirectory = basePath;
	SDL_free(basePath);

	AddPath(currentDirectory.data());
	LOGI("Current directory: %s", currentDirectory.data());

	if (PHYSFS_setWriteDir(currentDirectory.data()) == 0)
	{
		LOGE("Could not set a write directory");
	}

	return true;
}

bool FileSystem::CleanUp()
{
	PHYSFS_deinit();
	return true;
}

unsigned int FileSystem::Load(const char * path, char ** buffer) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(path);

	if (fs_file != NULL)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[(unsigned int)size];
			PHYSFS_sint64 readed = PHYSFS_read(fs_file, *buffer, 1, (PHYSFS_sint32)size);
			if (readed != size)
			{
				LOGE("File System error while reading from file %s: %s\n", path, PHYSFS_getLastError());
				if (buffer)
					delete[] buffer;
			}
			else
				ret = (unsigned int)readed;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOGE("File System error while closing file %s: %s\n", path, PHYSFS_getLastError());
	}
	else
		LOGE("File System error while opening file %s: %s\n", path, PHYSFS_getLastError());

	return ret;
}

unsigned int FileSystem::Save(const char * path, const void * buffer, unsigned int size) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(path);

	if (fs_file != NULL)
	{
		PHYSFS_sint64 written = PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOGE("File System error while writing to file %s: %s\n", path, PHYSFS_getLastError());
		}
		else
			ret = (unsigned int)written;

		if (PHYSFS_close(fs_file) == 0)
			LOGE("File System error while closing file %s: %s\n", path, PHYSFS_getLastError());
	}
	else
		LOGE("File System error while opening file %s: %s\n", path, PHYSFS_getLastError());

	return ret;
}

string FileSystem::GetDirectory() const
{
	return currentDirectory;
}

string FileSystem::GetFileNameFromPath(const string & path) const
{
	size_t begin = path.find_last_of("\\/") + 1;
	size_t end = path.find_last_of(".");
	return path.substr(begin, end - begin);
}

string FileSystem::GetDirectoryFromPath(const string & path) const
{
	return path.substr(0, path.find_last_of("\\/") + 1);
}

unsigned int FileSystem::EnumerateFiles(const char * path, std::vector<std::string>& files)
{
	files.clear();
	char** rc = PHYSFS_enumerateFiles(path);
	char** i;
	for (i = rc; *i != NULL; i++)
		files.push_back(*i);
	PHYSFS_freeList(rc);
	return files.size();
}

void FileSystem::RemoveDirectoriesFromList(std::vector<std::string>& list) const
{
	vector<string> tmp = list;
	list.clear();
	for (auto file : tmp)
	{
		if (IsDirectory(file.data()) == false)
			list.push_back(file);
	}
}

unsigned int FileSystem::FilterFiles(const vector<string>& filenames, vector<string>& filtered, const string & filter) const
{
	filtered.clear();

	string extension;
	for (vector<string>::const_iterator file = filenames.begin(); file != filenames.end(); ++file)
	{
		extension = (*file).substr((*file).find_last_of("."), (*file).size());
		if (extension.compare(filter.data()) == 0)
		{
			filtered.push_back((*file));
		}
	}

	return filtered.size();
}

bool FileSystem::IsDirectory(const char * fname) const
{
	return PHYSFS_isDirectory(fname) == 0 ? false : true;
}

bool FileSystem::AddPath(const char * path, const char * mountPoint)
{
	if (PHYSFS_mount(path, mountPoint, 1) == 0)
	{
		LOGE("Could not add path or zip (%s): %s", path, PHYSFS_getLastError());
		return false;
	}
	return true;
}