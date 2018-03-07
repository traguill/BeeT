#ifndef __FILEDIALOG_H__
#define __FILEDIALOG_H__

#include <string>

class FileDialog
{
public:
	FileDialog();
	~FileDialog();

	void Draw();

private:
	std::string path;
	std::string filename;
};
#endif // !__FILEDIALOG_H__

