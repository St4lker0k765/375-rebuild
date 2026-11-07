////////////////////////////////////////////////////////////////////////////
//	Module 		: object_factory_register.cpp
//	Created 	: 27.05.2004
//  Modified 	: 27.05.2004
//	Author		: Dmitriy Iassenev
//	Description : Object factory
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#include "object_factory_impl.h"

// server entities includes
#include "xrServer_Objects_ALife_All.h"
#include "clsid_game.h"

// client entities includes
#ifndef NO_XR_GAME
#	include "../std_classes.h"
#	include "level.h"
#	include "gamepersistent.h"
#	include "hudmanager.h"
#	include "actor.h"

#	include "weaponAK74.h"
#	include "weaponAMMO.h"
#	include "physicobject.h"
#endif

#	define ADD(a,b,c,d) add<a,b>(c,d)

void CObjectFactory::register_classes	()
{
	// client entities
	add<CLevel>													(CLSID_GAME_LEVEL				,"level");
	add<CGamePersistent>										(CLSID_GAME_PERSISTANT			,"game");
	add<CHUDManager>											(CLSID_HUDMANAGER				,"hud_manager");

	ADD(CActor					,CSE_ALifeCreatureActor			,CLSID_OBJECT_ACTOR				,"actor");

	ADD(CWeaponAK74				,CSE_ALifeItemWeapon			,CLSID_OBJECT_W_AK74			,"wpn_ak74");
	ADD(CWeaponAmmo				,CSE_ALifeItemAmmo				,CLSID_OBJECT_A_PM				,"wpn_ammo");
	ADD(CPhysicObject			,CSE_ALifeObjectPhysic			,CLSID_OBJECT_PHYSIC			,"obj_physic");
}