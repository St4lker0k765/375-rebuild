////////////////////////////////////////////////////////////////////////////
//	Module 		: game_sv_base_script.cpp
//	Created 	: 28.06.2004
//  Modified 	: 28.06.2004
//	Author		: Dmitriy Iassenev
//	Description : Base server game script export
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "game_sv_base.h"
#include "script_space.h"
#include <luabind/iterator_policy.hpp>
#include <luabind/operator.hpp>
#include <luabind/out_value_policy.hpp>
#include "xrMessages.h"
#include "ui/UIInventoryUtilities.h"
#include "xr_time.h"
#include "../../xrNetServer/net_utils.h"

using namespace luabind;

void game_sv_GameState::script_register(lua_State *L)
{

	module(L,"game")
	[
	class_< xrTime >("CTime")
		.enum_("date_format")
		[
			value("DateToDay",		int(InventoryUtilities::edpDateToDay)),
			value("DateToMonth",	int(InventoryUtilities::edpDateToMonth)),
			value("DateToYear",		int(InventoryUtilities::edpDateToYear))
		]
		.enum_("time_format")
		[
			value("TimeToHours",	int(InventoryUtilities::etpTimeToHours)),
			value("TimeToMinutes",	int(InventoryUtilities::etpTimeToMinutes)),
			value("TimeToSeconds",	int(InventoryUtilities::etpTimeToSeconds)),
			value("TimeToMilisecs",	int(InventoryUtilities::etpTimeToMilisecs))
		]
		.def(						constructor<>()				)
		.def(						constructor<const xrTime&>())
		.def(const_self <			xrTime()					)
		.def(const_self <=			xrTime()					)
		.def(const_self >			xrTime()					)
		.def(const_self >=			xrTime()					)
		.def(const_self ==			xrTime()					)
		.def(self +					xrTime()					)
		.def(self -					xrTime()					)

		.def("diffSec"				,&xrTime::diffSec)
		.def("add"					,&xrTime::add)
		.def("sub"					,&xrTime::sub)

		.def("setHMS"				,&xrTime::setHMS)
		.def("setHMSms"				,&xrTime::setHMSms)
		.def("set"					,&xrTime::set)
		.def("get"					,&xrTime::get, out_value(_2) + out_value(_3) + out_value(_4) + out_value(_5) + out_value(_6) + out_value(_7) + out_value(_8))
		.def("dateToString"			,&xrTime::dateToString)
		.def("timeToString"			,&xrTime::timeToString),
		// declarations
		def("time",					get_time),
		def("get_game_time",		get_time_struct),
//		def("get_surge_time",	Game::get_surge_time),
//		def("get_object_by_name",Game::get_object_by_name),
	
	class_< game_sv_GameState, game_GameState >("game_sv_GameState")

	.def("get_eid",				&game_sv_GameState::get_eid)
	.def("get_id",				&game_sv_GameState::get_id)
	.def("get_it",				&game_sv_GameState::get_it)
	.def("get_it_2_id",			&game_sv_GameState::get_it_2_id)
	.def("get_name_it",			&game_sv_GameState::get_name_it)
	.def("get_name_id",			&game_sv_GameState::get_name_id)
	
	.def("get_players_count",	&game_sv_GameState::get_players_count)
	.def("get_id_2_eid",		&game_sv_GameState::get_id_2_eid)

	.def("get_option_i",		&game_sv_GameState::get_option_i)
	.def("get_option_s",		&game_sv_GameState::get_option_s)
	.def("u_EventSend",			&game_sv_GameState::u_EventSend)

	.def("getRPcount",			&game_sv_GameState::getRPcount)
	];
	
	module(L)
	[
	class_<enum_exporter<EGamePlayerFlags> >("game_player_flags")
		.enum_("flags")
		[
			value("GAME_PLAYER_FLAG_LOCAL",						int(GAME_PLAYER_FLAG_LOCAL)),
			value("GAME_PLAYER_FLAG_READY",						int(GAME_PLAYER_FLAG_READY)),
			value("GAME_PLAYER_FLAG_VERY_VERY_DEAD",			int(GAME_PLAYER_FLAG_VERY_VERY_DEAD)),
			value("GAME_PLAYER_FLAG_CS_SPECTATOR",				int(GAME_PLAYER_FLAG_CS_SPECTATOR)),
			value("GAME_PLAYER_FLAG_SCRIPT_BEGINS_FROM",		int(GAME_PLAYER_FLAG_SCRIPT_BEGINS_FROM))
		]
	];
}
