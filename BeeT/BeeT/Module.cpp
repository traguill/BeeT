#include "Module.h"

Module::Module(const char* name) : name(name)
{
}

Module::~Module()
{
}

bool Module::Init()
{
	return true;
}

bool Module::CleanUp()
{
	return true;
}

const char * Module::GetName() const
{
	return name.data();
}
