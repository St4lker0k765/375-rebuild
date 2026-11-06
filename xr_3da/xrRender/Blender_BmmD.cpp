// BlenderDefault.cpp: implementation of the CBlender_BmmD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#include "blender_BmmD.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlender_BmmD::CBlender_BmmD	()
{
	description.CLS		= B_BmmD;
	strcpy				(oT2_Name,	"$null");
	strcpy				(oT2_xform,	"$null");
}

CBlender_BmmD::~CBlender_BmmD	()
{
	
}

void	CBlender_BmmD::Save		(IWriter& fs )
{
	IBlender::Save	(fs);
	xrPWRITE_MARKER	(fs,"Detail map");
	xrPWRITE_PROP	(fs,"Name",				xrPID_TEXTURE,	oT2_Name);
	xrPWRITE_PROP	(fs,"Transform",		xrPID_MATRIX,	oT2_xform);
}

void	CBlender_BmmD::Load		(IReader& fs, u16 version )
{
	IBlender::Load	(fs,version);
	xrPREAD_MARKER	(fs);
	xrPREAD_PROP	(fs,xrPID_TEXTURE,	oT2_Name);
	xrPREAD_PROP	(fs,xrPID_MATRIX,	oT2_xform);
}

#include "uber_deffer.h"
void	CBlender_BmmD::Compile	(CBlender_Compile& C)
{
	IBlender::Compile		(C);
	// codepath is the same, only the shaders differ
	// ***only pixel shaders differ***
	switch(C.iElement) 
	{
	case SE_R2_NORMAL_HQ: 		// deffer
		uber_deffer		(C,true,"base","impl",false,oT2_Name[0]?oT2_Name:0);
		break;
	case SE_R2_NORMAL_LQ: 		// deffer
		uber_deffer		(C,false,"base","impl",false,oT2_Name[0]?oT2_Name:0);
		break;
	case SE_R2_SHADOW:			// smap
		if (RImplementation.o.HW_smap)	C.r_Pass	("shadow_direct_base","dumb",				FALSE,TRUE,TRUE,FALSE);
		else							C.r_Pass	("shadow_direct_base","shadow_direct_base",	FALSE);
		C.r_Sampler		("s_base",C.L_textures[0]);
		C.r_End			();
		break;
	}
}
