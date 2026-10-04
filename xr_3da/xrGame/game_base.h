#pragma once

#include "game_base_space.h"
#include "script_export_space.h"
#include "alife_space.h"

#pragma pack(push,1)

struct	game_PlayerState;//fw
class	NET_Packet;

struct		RPoint
{
	Fvector	P;
	Fvector A;
	u32		TimeToUnfreeze;
	RPoint(){P.set(.0f,0.f,.0f);A.set(.0f,0.f,.0f); TimeToUnfreeze = 0;}
	DECLARE_SCRIPT_REGISTER_FUNCTION_STRUCT
};
add_to_type_list(RPoint)
#undef script_type_list
#define script_type_list save_type_list(RPoint)

struct	game_PlayerState 
{
	string64	name;
	s16			team;
	s16			kills;
	s16			deaths;
	s32			money_total;
	u16			flags;

	u16			ping;		//Ping from DirectX
	u16			Rping;		//Ping from message

	u16			GameID;

	//------Dedicated-------------------
	bool		Skip;
	//---------------------------
	u16			lasthitter;
	u16			lasthitweapon;
	u8			skin;
	//---------------------------
	u32			RespawnTime;
	u32			DeathTime;
	//---------------------------

/*
private:
	game_PlayerState(const game_PlayerState&);
	void operator = (const game_PlayerState&);
*/
public:
					game_PlayerState		();
					~game_PlayerState		();
	virtual void	clear					();
			bool	testFlag				(u16 f);
			void	setFlag					(u16 f);
			void	resetFlag				(u16 f);
			LPCSTR	getName					(){return name;}
			void	setName					(LPCSTR s){strcpy(name,s);}

#ifndef AI_COMPILER
	virtual void	net_Export				(NET_Packet& P);
	virtual void	net_Import				(NET_Packet& P);
#endif
	//---------------------------------------
	
	DEF_VECTOR(SPAWN_POINTS_LIST, s16);

	SPAWN_POINTS_LIST	pSpawnPointsList;
	s16					m_s16LastSRoint;

	bool				m_bClearRun;
	DECLARE_SCRIPT_REGISTER_FUNCTION_STRUCT
};

add_to_type_list(game_PlayerState)
#undef script_type_list
#define script_type_list save_type_list(game_PlayerState)


struct	game_TeamState
{
	int			score;
	u16			num_targets;

	game_TeamState();
};


#pragma pack(pop)

class	game_GameState : public DLL_Pure
{
protected:
	s32								type;
	u32								start_time;

public:
	game_GameState();
				u32					Type					() const						{return type;};
				s32					StartTime				() const						{return start_time;};
	virtual		void				Create					(shared_str& options)				{};
	virtual		game_PlayerState*	createPlayerState()		{return xr_new<game_PlayerState>(); };

//moved from game_sv_base (time routines)
private:
	// scripts
	u64								m_qwStartProcessorTime;
	u64								m_qwStartGameTime;
	float							m_fTimeFactor;
	//-------------------------------------------------------
	u64								m_qwEStartProcessorTime;
	u64								m_qwEStartGameTime;
	float							m_fETimeFactor;
	//-------------------------------------------------------
public:

	virtual		ALife::_TIME_ID		GetGameTime				();	
	virtual		float				GetGameTimeFactor		();	
				void				SetGameTimeFactor		(ALife::_TIME_ID GameTime, const float fTimeFactor);
	virtual		void				SetGameTimeFactor		(const float fTimeFactor);
	

	virtual		ALife::_TIME_ID		GetEnvironmentGameTime	();
	virtual		float				GetEnvironmentGameTimeFactor		();
				void				SetEnvironmentGameTimeFactor		(ALife::_TIME_ID GameTime, const float fTimeFactor);
	virtual		void				SetEnvironmentGameTimeFactor		(const float fTimeFactor);


	DECLARE_SCRIPT_REGISTER_FUNCTION
};
add_to_type_list(game_GameState)
#undef script_type_list
#define script_type_list save_type_list(game_GameState)
