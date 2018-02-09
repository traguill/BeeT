#ifndef __BEETGUI_H__
#define __BEETGUI_H__

#include "Module.h"

class BeeTGui : public Module
{
public:
	BeeTGui(const char* name);
	~BeeTGui();

	bool Init();
	bool CleanUp();
	bool Update();

private:

};
#endif

