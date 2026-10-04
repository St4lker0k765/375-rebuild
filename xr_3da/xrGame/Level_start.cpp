#include "stdafx.h"
#include "level.h"
//#include "LevelFogOfWar.h"
#include "Level_Bullet_Manager.h"
#include "xrserver.h"
#include "game_cl_base.h"
#include "xrmessages.h"
#include "../x_ray.h"
#include "clsid_game.h"

BOOL CLevel::net_Start	( LPCSTR op_server, LPCSTR op_client )
{
	BOOL bResult				= FALSE;

	pApp->LoadBegin				();

	//make Client Name if options doesn't have it
	if (!strstr(op_client,"/name="))
	{
		string512 tmp;
		strcpy(tmp, op_client);
		strcat(tmp, "/name=");
		strcat(tmp, Core.CompName);
		m_caClientOptions			= tmp;
	} else {
		m_caClientOptions			= op_client;
	};
	m_caServerOptions			    = op_server;

	// Start client and server if need it
	if (op_server)
	{
		pApp->LoadTitle			("SERVER: Starting...");

		// Connect
		Server					= xr_new<xrServer>();

		if (!strstr(*m_caServerOptions,"/alife")) {
			string64			l_name = "";
			const char* SOpts = *m_caServerOptions;
			strncpy(l_name, *m_caServerOptions, strchr(SOpts, '/') - SOpts);
//			strcpy				(l_name,*m_caServerOptions);
			// Activate level
			if (strchr(l_name,'/'))
				*strchr(l_name,'/')	= 0;

			m_name				= l_name;

			int					id = pApp->Level_ID(l_name);

			if (id<0) {
				pApp->LoadEnd	();
				Log				("Can't find level: ",l_name);
				return			FALSE;
			}
			pApp->Level_Set		(id);
		}
		
		Server->Connect			(m_caServerOptions);	
		Server->SLS_Default		();
		m_name					= Server->level_name(m_caServerOptions);
	}

	// Start client
	bResult						= net_Start_client(*m_caClientOptions);
	// Send Ready message to server
	if (bResult)
	{
		NET_Packet		NP;
		NP.w_begin		(M_CLIENTREADY);
		Send(NP,net_flags(TRUE,TRUE));

		if (OnClient() && Server)
		{
			Server->SLS_Clear();
		};
	};

	
	//init bullet manager
	BulletManager().Clear		();
	BulletManager().Load		();

	pApp->LoadEnd				();

	return						bResult;
}

void CLevel::InitializeClientGame	(NET_Packet& P)
{	
	xr_delete(game);
	game					= smart_cast<game_cl_GameState*> ( NEW_INSTANCE ( CLSID_CL_GAME_SINGLE ) );
	game->Init();
}

