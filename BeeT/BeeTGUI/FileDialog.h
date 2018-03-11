#ifndef __FILEDIALOG_H__
#define __FILEDIALOG_H__

#include <string>

enum FileDialogMode
{
	OPEN_FILE,
	SAVE_FILE
};

class FileDialog
{
public:
	FileDialog();
	~FileDialog();

	void Draw();

	//Don't need to specify the "."
	void SetFormat(const char* format);
	void EnableWindow(FileDialogMode mode);
	void SetAcceptFunctionCallback(void(*fc)(void*, const char*), void* obj);
	void ClearLastOpenedFileName();

private:
	void CleanUpBeforeClosingWindow();
	void ShowFileName();

private:
	std::string path;
	std::string filename;
	std::string format;
	std::string newFilenameName;
	std::string lastOpenedFile;

	FileDialogMode mode = OPEN_FILE;
	bool show = false;
	
	void* object; // Class to call the acceptFunc
	void(*acceptFunc)(void* obj, const char* filename); // Function to call when Accept is pressed

	std::string fileSelected;
};
#endif // !__FILEDIALOG_H__

