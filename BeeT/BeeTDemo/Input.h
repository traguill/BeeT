#ifndef __INPUT_H__
#define __INPUT_H__

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input
{
public:
	Input();
	~Input();

	bool Update();

	KEY_STATE GetKey(int id)const
	{
		return keyboard[id];
	}

	int GetMouseX() const
	{
		return mouseX;
	}

	int GetMouseY() const
	{
		return mouseY;
	}

private:
	KEY_STATE * keyboard;
	int mouseX;
	int mouseY;

};
extern Input* g_Input;
#endif // !__INPUT_H__

