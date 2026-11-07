//////////////////////////////////////////////////////////////////////////
// character_info.cpp			игровая информация для персонажей в игре
// 
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "character_info.h"

//////////////////////////////////////////////////////////////////////////
SCharacterProfile::SCharacterProfile()
{
	m_Rank				= NO_RANK;
	m_Reputation		= NO_REPUTATION;
}

SCharacterProfile::~SCharacterProfile()
{
}



//////////////////////////////////////////////////////////////////////////

CCharacterInfo::CCharacterInfo()
{
	m_ProfileId = NULL;

#ifdef XRGAME_EXPORTS
	m_CurrentRank.set(NO_RANK);
	m_CurrentReputation.set(NO_REPUTATION);
#endif
}


CCharacterInfo::~CCharacterInfo()
{
}


void CCharacterInfo::Load(PROFILE_ID id)
{
	m_ProfileId = id;
}

#ifdef XRGAME_EXPORTS


PROFILE_ID CCharacterInfo::Profile()			const
{
	return m_ProfileId;
}

LPCSTR CCharacterInfo::Name() const
{
	return "";
}

const CHARACTER_RANK& CCharacterInfo::Rank() const
{
	return	m_CurrentRank;
}

const CHARACTER_COMMUNITY& CCharacterInfo::Community() const
{
	return	m_CurrentCommunity;
}

const CHARACTER_REPUTATION& CCharacterInfo::Reputation() const
{
	return m_CurrentReputation;
}

void CCharacterInfo::SetRank (CHARACTER_RANK_VALUE rank)
{
	m_CurrentRank.set(rank);
}
void CCharacterInfo::SetReputation (CHARACTER_REPUTATION_VALUE reputation)
{
	m_CurrentReputation.set(reputation);
}

void CCharacterInfo::SetCommunity	(const CHARACTER_COMMUNITY& community)
{
	m_CurrentCommunity = community;
}

#endif