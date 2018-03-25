#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "Module.h"

class Network : public Module
{
public:
	Network(const char* name);
	~Network();

	bool Init();
	bool CleanUp();

private:

};
#endif // !__NETWORK_H__

