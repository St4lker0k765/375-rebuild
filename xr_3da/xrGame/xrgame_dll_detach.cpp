#include "stdafx.h"
#include "ai_space.h"
#include "object_factory.h"
#include "ai/monsters/ai_monster_squad_manager.h"

#include "entity_alive.h"
#include "UI/UIXmlInit.h"

#include "character_info.h"
#include "character_community.h"
#include "character_rank.h"
#include "character_reputation.h"

#include "profiler.h"



extern void show_smart_cast_stats					();
extern void clear_smart_cast_stats					();
extern void release_smart_cast_stats				();

void clean_game_globals()
{	
	// destroy ai space
	xr_delete										(g_ai_space);
	// destroy object factory
	xr_delete										(g_object_factory);
	// destroy monster squad global var
	xr_delete										(g_monster_squad);

	//XML indexes
	CCharacterInfo::DeleteSharedData				();
	CCharacterInfo::DeleteIdToIndexData				();
	
	CHARACTER_COMMUNITY::DeleteIdToIndexData		();
	CHARACTER_RANK::DeleteIdToIndexData				();
	CHARACTER_REPUTATION::DeleteIdToIndexData		();


	//static shader for blood
	CEntityAlive::UnloadBloodyWallmarks				();
	CEntityAlive::UnloadFireParticles				();
	// Очищение таблицы цветов
	CUIXmlInit::DeleteColorDefs						();

#ifdef DEBUG
	xr_delete										(g_profiler);
	release_smart_cast_stats						();
#endif
}