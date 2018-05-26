#ifndef __BEET_DEBUGGER_H__
#define __BEET_DEBUGGER_H__

#include "Packet.h"

class BeeTDebugger
{
public:
	BeeTDebugger();
	~BeeTDebugger();

	void Init();
	bool Update();
	void CleanUp();

	void HandleIncomingData(char* buf, int size, PacketType type);

private:

};

#endif // !__BEET_DEBUGGER_H__
