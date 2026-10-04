#pragma once

enum	EKeyBinding
{
	kFWD			=	1,
	kBACK			,
	kL_STRAFE		,
	kR_STRAFE		,
	kLEFT			,
	kRIGHT			,
	kUP				,
	kDOWN			,
	kJUMP			,
	kCROUCH			,
	kCROUCH_TOGGLE	,
	kSPRINT_TOGGLE	,
	kACCEL			,
	kL_LOOKOUT		,
	kR_LOOKOUT		,

	kCAM_1			,
	kCAM_2			,
	kCAM_3			,
	kCAM_ZOOM_IN	,
	kCAM_ZOOM_OUT	,

	kWPN_1			,
	kWPN_2			,
	kWPN_3			,
	kWPN_4			,
	kWPN_FIRE		,
	kWPN_RELOAD		,
	kWPN_LIGHT		,

	kPAUSE			,
	kDROP			,
	kUSE			,
	kSCREENSHOT		,
	kQUIT			,
	kCONSOLE		,

	kFORCEDWORD		= u32(-1)
};

struct _keybind		{
	char *	name;
	int		DIK;
};

extern _keybind	keybind			[];
extern _keybind	keynames		[];
extern int		key_binding		[];
extern void		CCC_RegisterInput();

struct _conCmd{
	string512	cmd;
};

class ConsoleBindCmds{
public:
	xr_map<int,_conCmd>	m_bindConsoleCmds;
	void bind(int dik, LPCSTR N);
	void unbind(int dik);
	bool execute(int dik);
	void clear();
	void save(IWriter* F);
};
void GetActionBinding(LPCSTR action, char* dst_buff);

extern ConsoleBindCmds	bindConsoleCmds;

#define MOUSE_1		0x100
#define MOUSE_2		0x200
#define MOUSE_3		0x400
