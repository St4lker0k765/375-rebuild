#ifndef _INCDEF_XRMESSAGES_H_
#define _INCDEF_XRMESSAGES_H_

#pragma once

// CL	== client 2 server message
// SV	== server 2 client message

enum {
	M_UPDATE			= 0,	// DUAL: Update state
	M_SPAWN,					// DUAL: Spawning, full state

	M_SV_CONFIG_NEW_CLIENT,
	M_SV_CONFIG_GAME,
	M_SV_CONFIG_FINISHED,

	M_MIGRATE_DEACTIVATE,		// TO:   Changing server, just deactivate
	M_MIGRATE_ACTIVATE,			// TO:   Changing server, full state

	M_EVENT,					// Game Event
	//----------- for E3 -----------------------------
	M_CL_UPDATE,
	//-------------------------------------------------
	M_CLIENTREADY,				// Client has finished to load level and are ready to play
	
	M_CHANGE_LEVEL,				// changing level
	M_LOAD_GAME,
	M_RELOAD_GAME,
	M_SAVE_GAME,
	M_SAVE_PACKET,

	M_SWITCH_DISTANCE,
	M_EVENT_PACK,					// Pack of M_EVENT

	//-----------------------------------------------------
	M_CLIENT_REQUEST_CONNECTION_DATA,

	M_CHANGE_LEVEL_GAME,
	//-----------------------------------------------------
	M_CL_PING_CHALLENGE,
	M_CL_PING_CHALLENGE_RESPOND,

	MSG_FORCEDWORD				= u32(-1)
};

enum {
	GE_OWNERSHIP_TAKE,			// DUAL: Client request for ownership of an item
	GE_OWNERSHIP_REJECT,		// DUAL: Client request ownership rejection
	GE_TRANSFER_AMMO,			// DUAL: Take ammo out of weapon for our weapon
	GE_HIT,						//
	GE_DIE,						//
	GE_ASSIGN_KILLER,			//
	GE_DESTROY,					// authorative client request for entity-destroy
	GE_DESTROY_REJECT,			// GE_DESTROY + GE_OWNERSHIP_REJECT
	GE_TELEPORT_OBJECT,

	GE_ADD_RESTRICTION,
	GE_REMOVE_RESTRICTION,
	GE_REMOVE_ALL_RESTRICTIONS,

	GE_BUY,

	GE_PDA,						//a PDA message sent from one PDA to another

	GE_INFO_TRANSFER,			//transfer _new_ info on PDA
	
	GE_TRADE_SELL,
	GE_TRADE_BUY,

	GE_WPN_AMMO_ADD,
	GE_WPN_STATE_CHANGE,

	GE_ADDON_ATTACH,
	GE_ADDON_DETACH,
	GE_ADDON_CHANGE,
	
	GE_GRENADE_EXPLODE,
	GE_INV_ACTION,				//a action beign taken on inventory

	GE_ZONE_STATE_CHANGE,

	GE_MOVE_ACTOR,				//move actor to desired position instantly

	GE_CHANGE_POS,

	GE_GAME_EVENT,

	GE_CHANGE_VISUAL,
/*
	GEG_SIGNAL,
	GEG_PLAYER_READY,
	GEG_PLAYER_CHANGE_TEAM,
	GEG_PLAYER_KILL,			//player wants to die
	GEG_PLAYER_BUY_FINISHED,	//player end to buy items
	GEG_PLAYER_CHANGE_SKIN,
*/	
	GEG_PLAYER_ITEM2SLOT,
	GEG_PLAYER_ITEM2BELT,
	GEG_PLAYER_ITEM2RUCK,
	GEG_PLAYER_ITEMDROP,
	GEG_PLAYER_ITEM_EAT,

	GEG_PLAYER_INVENTORYMENU_OPEN,
	GEG_PLAYER_INVENTORYMENU_CLOSE,
	GEG_PLAYER_BUYMENU_OPEN,
	GEG_PLAYER_BUYMENU_CLOSE,
	GEG_PLAYER_DEACTIVATE_CURRENT_SLOT,
	GEG_PLAYER_RESTORE_CURRENT_SLOT,
	GEG_PLAYER_SPRINT_START,
	GEG_PLAYER_SPRINT_END,

	GE_FORCEDWORD				= u32(-1)
};


enum EGameMessages {  //game_cl <----> game_sv messages	
	GAME_EVENT_CREATE_CLIENT,
	GAME_EVENT_ON_HIT,

	GAME_EVENT_FORCEDWORD				= u32(-1)
};

enum
{
	M_SPAWN_OBJECT_LOCAL		= (1<<0),	// after spawn it becomes local (authorative)
	M_SPAWN_OBJECT_HASUPDATE	= (1<<2),	// after spawn info it has update inside message
	M_SPAWN_OBJECT_ASPLAYER		= (1<<3),	// after spawn it must become viewable
	M_SPAWN_OBJECT_PHANTOM		= (1<<4),	// after spawn it must become viewable
	M_SPAWN_VERSION				= (1<<5),	// control version
	M_SPAWN_UPDATE				= (1<<6),	// + update packet

	M_SPAWN_OBJECT_FORCEDWORD	= u32(-1)
};

enum
{
	M_UPDATE_WEAPON_wfWorking	= (1<<0),
	M_UPDATE_WEAPON_wfVisible	= (1<<1),

	M_UPDATE_WEAPON_FORCEDWORD	= u32(-1)
};

#endif /*_INCDEF_XRMESSAGES_H_*/
