#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"

class Input : public Module
{
public:
	Input(const char* name);
	~Input();

	bool Init();
	bool PreUpdate();
};
#endif

