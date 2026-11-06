// Blender_Vertex_aref.cpp: implementation of the CBlender_Detail_Still class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#include "Blender_Detail_still.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlender_Detail_Still::CBlender_Detail_Still()
{
	description.CLS		= B_DETAIL;
	description.version	= 0;
}

CBlender_Detail_Still::~CBlender_Detail_Still()
{

}

void	CBlender_Detail_Still::Save		(IWriter& fs )
{
	IBlender::Save		(fs);
	xrPWRITE_PROP		(fs,"Alpha-blend",	xrPID_BOOL,		oBlend);
}

void	CBlender_Detail_Still::Load		(IReader& fs, u16 version )
{
	IBlender::Load		(fs,version);
	xrPREAD_PROP		(fs,xrPID_BOOL,		oBlend);
}

#include "uber_deffer.h"
void	CBlender_Detail_Still::Compile	(CBlender_Compile& C)
{
	IBlender::Compile	(C);

	switch(C.iElement) 
	{
	case SE_R2_NORMAL_HQ: 		// deffer wave
		uber_deffer				(C,false,"detail_w","base",true);
		break;
	case SE_R2_NORMAL_LQ: 		// deffer still
		uber_deffer				(C,false,"detail_s","base",true);
		break;
	}
}
