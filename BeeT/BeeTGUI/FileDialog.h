#ifndef __FILEDIALOG_H__
#define __FILEDIALOG_H__

#include <string>

class FileDialog
{
public:
	FileDialog();
	~FileDialog();

	void Draw();

	//Don't need to specify the "."
	void SetFormat(const char* format);

private:
	std::string path;
	std::string filename;
	std::string format;
};
#endif // !__FILEDIALOG_H__

