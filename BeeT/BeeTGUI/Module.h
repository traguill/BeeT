#ifndef __MODULE_H__
#define __MODULE_H__

#include <string>

class Module
{
public:
	Module(const char* name);
	~Module();

	virtual bool Init();
	virtual bool CleanUp();
	virtual bool PreUpdate();
	virtual bool Update();
	virtual bool PostUpdate();

	const char* GetName()const;

private:
	std::string name;
};
#endif
