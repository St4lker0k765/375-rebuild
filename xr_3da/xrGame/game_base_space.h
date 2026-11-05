#pragma once

enum
{
	GAME_ANY							= 0,
	GAME_SINGLE							= 1,
	GAME_DEATHMATCH						= 2,
	GAME_CS								= 5,
	GAME_TEAMDEATHMATCH					= 6,
	GAME_ARTEFACTHUNT					= 7,

	//identifiers in range [100...254] are registered for script game type
	GAME_DUMMY							= 255	// temporary game type
};

enum EGamePlayerFlags
{
	GAME_PLAYER_FLAG_LOCAL				= (1<<0),

	GAME_PLAYER_FLAG_FORCEDWORD			= u32(-1)
};
