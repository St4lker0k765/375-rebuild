#include "stdafx.h"
#include "xrServer.h"
#include "LevelGameDef.h"
#include "script_process.h"
#include "xrServer_Objects_ALife_Monsters.h"
#include "script_engine.h"
#include "script_engine_space.h"
#include "level.h"
#include "xrserver.h"
#include "ai_space.h"
#include "game_sv_event_queue.h"
#include "../XR_IOConsole.h"

//#define		MAPROT_LIST				"maprot_list.ltx"
string_path		MAPROT_LIST		= "";
BOOL	net_sv_control_hit	= TRUE		;

// Main
game_PlayerState*	game_sv_GameState::get_it					(u32 it)
{
	xrClientData*	C	= (xrClientData*)m_server->client_Get			(it);
	if (0==C)			return 0;
	else				return C->ps;
}

game_PlayerState*	game_sv_GameState::get_id					(ClientID id)							
{
	xrClientData*	C	= (xrClientData*)m_server->ID_to_client	(id);
	if (0==C)			return 0;
	else				return C->ps;
}

ClientID				game_sv_GameState::get_it_2_id				(u32 it)
{
	xrClientData*	C	= (xrClientData*)m_server->client_Get		(it);
	if (0==C){
		ClientID clientID;clientID.set(0);
		return clientID;
	}
	else				return C->ID;
}

LPCSTR				game_sv_GameState::get_name_it				(u32 it)
{
	xrClientData*	C	= (xrClientData*)m_server->client_Get		(it);
	if (0==C)			return 0;
	else				return *C->Name;
}

LPCSTR				game_sv_GameState::get_name_id				(ClientID id)							
{
	xrClientData*	C	= (xrClientData*)m_server->ID_to_client	(id);
	if (0==C)			return 0;
	else				return *C->Name;
}

u32					game_sv_GameState::get_players_count		()
{
	return				m_server->client_Count();
}

u16					game_sv_GameState::get_id_2_eid				(ClientID id)
{
	xrClientData*	C	= (xrClientData*)m_server->ID_to_client	(id);
	if (0==C)			return 0xffff;
	CSE_Abstract*	E	= C->owner;
	if (0==E)			return 0xffff;
	return E->ID;
}

game_PlayerState*	game_sv_GameState::get_eid (u16 id) //if exist
{
	CSE_Abstract* entity = get_entity_from_eid(id);
	if (entity && entity->owner->ps->GameID == id)
		return entity->owner->ps;
	return NULL;
}

CSE_Abstract*		game_sv_GameState::get_entity_from_eid		(u16 id)
{
	return				m_server->ID_to_entity(id);
}

// Utilities
u32					game_sv_GameState::get_alive_count			(u32 team)
{
	u32		cnt		= get_players_count	();
	u32		alive	= 0;
	for		(u32 it=0; it<cnt; ++it)	
	{
		game_PlayerState*	ps	=	get_it	(it);
		if (u32(ps->team) == team)	alive	+=	(ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))?0:1;
	}
	return alive;
}

xr_vector<u16>*		game_sv_GameState::get_children				(ClientID id)
{
	xrClientData*	C	= (xrClientData*)m_server->ID_to_client	(id);
	if (0==C)			return 0;
	CSE_Abstract* E	= C->owner;
	if (0==E)			return 0;
	return	&(E->children);
}

s32					game_sv_GameState::get_option_i				(LPCSTR lst, LPCSTR name, s32 def)
{
	string64		op;
	strconcat		(op,"/",name,"=");
	if (strstr(lst,op))	return atoi	(strstr(lst,op)+xr_strlen(op));
	else				return def;
}

string64&			game_sv_GameState::get_option_s				(LPCSTR lst, LPCSTR name, LPCSTR def)
{
	static string64	ret;

	string64		op;
	strconcat		(op,"/",name,"=");
	LPCSTR			start	= strstr(lst,op);
	if (start)		
	{
		LPCSTR			begin	= start + xr_strlen(op); 
		sscanf			(begin, "%[^/]",ret);
	}
	else			
	{
		if (def)	strcpy		(ret,def);
		else		ret[0]=0;
	}
	return ret;
}
void				game_sv_GameState::signal_Syncronize		()
{
	sv_force_sync	= TRUE;
}

// Network
void game_sv_GameState::net_Export_State						(NET_Packet& P, ClientID to)
{
	// Generic
	P.w_clientID	(to);
	P.w_s32			(type);
	P.w_u32			(start_time);
	P.w_u8			(u8(m_bVotingEnabled));
	P.w_u8			(u8(m_bFriendlyIndicators));
	P.w_u8			(u8(net_sv_control_hit));
	P.w_u32			(m_u32ForceRespawn);

	// Players
//	u32	p_count			= get_players_count() - ((g_pGamePersistent->bDedicatedServer)? 1 : 0);
	u32 p_count = 0;
	for (u32 p_it=0; p_it<get_players_count(); ++p_it)
	{
		xrClientData*	C		=	(xrClientData*)	m_server->client_Get	(p_it);
		if (!C->net_Ready || C->ps->Skip) continue;
		p_count++;
	};

	P.w_u16				(u16(p_count));
	game_PlayerState*	Base	= get_id(to);
	for (u32 p_it=0; p_it<get_players_count(); ++p_it)
	{
		string64	p_name;
		xrClientData*	C		=	(xrClientData*)	m_server->client_Get	(p_it);
		game_PlayerState* A		=	get_it			(p_it);
		if (A->Skip || !C->net_Ready) continue;
		if (0==C)	strcpy(p_name,"Unknown");
		else 
		{
			CSE_Abstract* C_e		= C->owner;
			if (0==C_e)		strcpy(p_name,"Unknown");
			else 
			{
				strcpy	(p_name,C_e->name_replace());
			}
		}

		A->setName(p_name);
		u16 tmp_flags = A->flags;

		if (Base==A)	
			A->setFlag(GAME_PLAYER_FLAG_LOCAL);
/*
		if (A->Skip || !C->net_Ready)
		{
			A->flags = tmp_flags;
			continue;
		};		
*/
		ClientID clientID = get_it_2_id	(p_it);
		P.w_clientID			(clientID);
//		P.w_stringZ				(p_name);
		A->net_Export			(P);
		
		A->flags = tmp_flags;
	}

//	P.w_u64(GetGameTime());
//	P.w_float(GetGameTimeFactor());
	net_Export_GameTime(P);
}

void game_sv_GameState::net_Export_Update						(NET_Packet& P, ClientID id_to, ClientID id)
{
	game_PlayerState* A		= get_id		(id);
	if (A)
	{
		u16 bk_flags = A->flags;
		if (id==id_to)	
		{
			A->setFlag(GAME_PLAYER_FLAG_LOCAL);
		}

		P.w_clientID	(id);
		A->net_Export(P);
		A->flags = bk_flags;
	};
};

void game_sv_GameState::net_Export_GameTime						(NET_Packet& P)
{
	//Syncronize GameTime 
	P.w_u64(GetGameTime());
	P.w_float(GetGameTimeFactor());
	//Syncronize EnvironmentGameTime 
	P.w_u64(GetEnvironmentGameTime());
	P.w_float(GetEnvironmentGameTimeFactor());
};


void game_sv_GameState::OnPlayerConnect			(ClientID /**id_who/**/)
{
	signal_Syncronize	();
}

void game_sv_GameState::OnPlayerDisconnect		(ClientID /**id_who/**/, LPSTR, u16 )
{
	signal_Syncronize	();
}

void game_sv_GameState::Create					(shared_str &options)
{
	string256	fn_game;	
	// loading scripts
	ai().script_engine().remove_script_process(ScriptEngine::eScriptProcessorGame);
	string256					S;
	FS.update_path				(S,"$game_config$","script.ltx");
	CInifile					*l_tpIniFile = xr_new<CInifile>(S);
	R_ASSERT					(l_tpIniFile);

	ai().script_engine().add_script_process(ScriptEngine::eScriptProcessorGame,xr_new<CScriptProcess>("game",l_tpIniFile->r_string("single", "script")));

	xr_delete					(l_tpIniFile);

	//---------------------------------------------------------------------
	
	strcpy( MAPROT_LIST, get_option_s(*options, "maprot"));
	if (MAPROT_LIST[0])
	{
		Console->ExecuteScript(MAPROT_LIST);
	};

	m_bVotingEnabled = get_option_i(*options,"vote",0) != 0;
	m_bFriendlyIndicators = get_option_i(*options,"fi",0) != 0;

	m_u32ForceRespawn = get_option_i(*options, "frcrspwn", 0) * 1000;
}

void	game_sv_GameState::assign_RP				(CSE_Abstract* E, game_PlayerState* ps_who)
{
	VERIFY				(E);

	u8					l_uc_team = u8(-1);
	CSE_Spectator		*tpSpectator = smart_cast<CSE_Spectator*>(E);
	if (tpSpectator)
		l_uc_team = tpSpectator->g_team();
	else {
		CSE_ALifeCreatureAbstract	*tpTeamed = smart_cast<CSE_ALifeCreatureAbstract*>(E);
		if (tpTeamed)
			l_uc_team = tpTeamed->g_team();
		else
			R_ASSERT2(tpTeamed,"Non-teamed object is assigning to respawn point!");
	}
	xr_vector<RPoint>&	rp	= rpoints[l_uc_team];
	//-----------------------------------------------------------
	xr_vector<u32>	xrp;//	= rpoints[l_uc_team];
	for (u32 i=0; i<rp.size(); i++)
	{
		if (rp[i].TimeToUnfreeze < Level().timeServer())
			xrp.push_back(i);
	}
	u32 rpoint = 0;
	if (xrp.size() && !tpSpectator)
	{
		rpoint = xrp[::Random.randI((int)xrp.size())];
	}
	else
	{
		if (!tpSpectator)
		{
			for (u32 i=0; i<rp.size(); i++)
			{
				rp[i].TimeToUnfreeze = 0;
			};
		};
		rpoint = ::Random.randI((int)rp.size());
	}
	//-----------------------------------------------------------
	RPoint&				r	= rp[rpoint];
	E->o_Position.set	(r.P);
	E->o_Angle.set		(r.A);
}

void game_sv_GameState::u_EventGen(NET_Packet& P, u16 type, u16 dest)
{
	P.w_begin	(M_EVENT);
	P.w_u32		(Level().timeServer());//Device.TimerAsync());
	P.w_u16		(type);
	P.w_u16		(dest);
}

void game_sv_GameState::u_EventSend(NET_Packet& P)
{
	ClientID clientID; clientID.setBroadcast();
	m_server->SendBroadcast(clientID,P,net_flags(TRUE,TRUE));
}

void game_sv_GameState::Update		()
{
	for (u32 it=0; it<m_server->client_Count(); ++it) {
		xrClientData*	C			= (xrClientData*)	m_server->client_Get(it);
		C->ps->ping					= u16(C->stats.getPing());
	}
	
	if (Level().game) {
		CScriptProcess				*script_process = ai().script_engine().script_process(ScriptEngine::eScriptProcessorGame);
		if (script_process)
			script_process->update	();
	}
}

game_sv_GameState::game_sv_GameState()
{
	m_server					= Level().Server;

	m_event_queue = xr_new<GameEventQueue>();

	for (int i=0; i<TEAM_COUNT; i++) rpoints_MinDist[i] = 1000.0f;
}

game_sv_GameState::~game_sv_GameState()
{
	ai().script_engine().remove_script_process(ScriptEngine::eScriptProcessorGame);
	xr_delete(m_event_queue);
}

bool game_sv_GameState::change_level (NET_Packet &net_packet, ClientID sender)
{
	return						(true);
}

void game_sv_GameState::save_game (NET_Packet &net_packet, ClientID sender)
{
}

bool game_sv_GameState::load_game (NET_Packet &net_packet, ClientID sender)
{
	return						(true);
}

void game_sv_GameState::reload_game (NET_Packet &net_packet, ClientID sender)
{
}

void game_sv_GameState::switch_distance (NET_Packet &net_packet, ClientID sender)
{
}

void game_sv_GameState::OnHit (u16 id_hitter, u16 id_hitted, NET_Packet& P)
{
	CSE_Abstract*		e_hitter		= get_entity_from_eid	(id_hitter	);
	CSE_Abstract*		e_hitted		= get_entity_from_eid	(id_hitted	);
	if (!e_hitter || !e_hitted) return;

	CSE_Abstract*		a_hitter		= smart_cast <CSE_ALifeCreatureActor*> (e_hitter);
	CSE_Abstract*		a_hitted		= smart_cast <CSE_ALifeCreatureActor*> (e_hitted);

	if (a_hitted && a_hitter)
	{
		OnPlayerHitPlayer(id_hitter, id_hitted, P);
		return;
	};
};

void game_sv_GameState::OnEvent (NET_Packet &tNetPacket, u16 type, u32 time, ClientID sender )
{
	switch	(type)
	{	
	case GAME_EVENT_ON_HIT:
		{
			u16		id_dest = tNetPacket.r_u16();
			u16     id_src  = tNetPacket.r_u16();
			CSE_Abstract*	e_src	= get_entity_from_eid	(id_src	);
			if(!e_src) break;
			OnHit(id_src, id_dest, tNetPacket);
			ClientID clientID;clientID.setBroadcast();
			m_server->SendBroadcast		(clientID,tNetPacket,net_flags(TRUE,TRUE));
		}break	;
	case GAME_EVENT_CREATE_CLIENT:
		{
			IClient* P					= m_server->client_Create();
			VERIFY						(P);
			P->ID						= sender;
			//tNetPacket.r_clientID(P->ID);
			tNetPacket.r_stringZ		(P->Name);
			P->flags.bLocal				= !!tNetPacket.r_u8();
			P->flags.bConnected			= TRUE;
			m_server->AttachNewClient	(P);
		}break	;
	default:
		R_ASSERT2	(0,"Game Event not implemented!!!");
	};
};

void game_sv_GameState::AddDelayedEvent(NET_Packet &tNetPacket, u16 type, u32 time, ClientID sender )
{
//	OnEvent(tNetPacket,type,time,sender);
	m_event_queue->Create(tNetPacket,type,time,sender);
}

void game_sv_GameState::ProcessDelayedEvent		()
{
	GameEvent* ge = NULL;
	while ((ge = m_event_queue->Retreive()) != 0) {
		OnEvent(ge->P,ge->type,ge->time,ge->sender);
		m_event_queue->Release();
	}
}

u32 game_sv_GameState::getRPcount (u16 team_idx)
{
	if ( !(team_idx<TEAM_COUNT) )
		return 0;
	else
		return rpoints[team_idx].size();
}

void game_sv_GameState::teleport_object	(NET_Packet &packet, u16 id)
{
}

void game_sv_GameState::add_restriction	(NET_Packet &packet, u16 id)
{
}

void game_sv_GameState::remove_restriction(NET_Packet &packet, u16 id)
{
}

void game_sv_GameState::remove_all_restrictions	(NET_Packet &packet, u16 id)
{
}

shared_str game_sv_GameState::level_name		(const shared_str &server_options) const
{
	string64			l_name = "";
	VERIFY				(_GetItemCount(*server_options,'/'));
	return				(_GetItem(*server_options,0,l_name,'/'));
}
