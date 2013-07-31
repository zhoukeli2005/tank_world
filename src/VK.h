#ifndef _vk_h_
#define _vk_h_

class VK
{
public:
	enum {
		RETURN			= 0x0D,
		ESCAPE			= 0x1B,
		SPACE			= 0x20,

		LEFT			= 0x25,
		UP,
		RIGHT,
		DOWN,

		NUM0			= 0x30,
		NUM1,
		NUM2,
		NUM3,
		NUM4,
		NUM5,
		NUM6,
		NUM7,
		NUM8,
		NUM9,

		A				= 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		NUMPAD0			= 0x60,
		NUMPAD1,
		NUMPAD2,
		NUMPAD3,
		NUMPAD4,
		NUMPAD5,
		NUMPAD6,
		NUMPAD7,
		NUMPAD8,
		NUMPAD9,

		F1				= 0x70,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		LSHIFT			= 0xA0,
		RSHIFT			= 0xA1,

		LCTL			= 0xA2,
		RCTL			= 0xA3
	};
};

#endif