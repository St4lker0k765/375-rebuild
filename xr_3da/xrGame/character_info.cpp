//////////////////////////////////////////////////////////////////////////
// character_info.cpp			игровая информация для персонажей в игре
// 
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "character_info.h"

#ifdef XRGAME_EXPORTS
#	include "ui/xrXMLParser.h"
#else
#	include "xrXMLParser.h"
#endif


//////////////////////////////////////////////////////////////////////////
SCharacterProfile::SCharacterProfile()
{
	m_CharacterId		= NULL;
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
	inherited_shared::load_shared(m_ProfileId, NULL);
}

void CCharacterInfo::load_shared	(LPCSTR)
{
	CUIXml uiXml;
	const id_to_index::ITEM_DATA& item_data = *id_to_index::GetById(m_ProfileId);

	string128 xml_file_full;
	strconcat(xml_file_full, *shared_str(item_data.file_name), ".xml");

	bool xml_result = uiXml.Init(CONFIG_PATH, GAME_PATH, xml_file_full);
	R_ASSERT3(xml_result, "xml file not found", xml_file_full);

	//loading from XML
	XML_NODE* item_node = uiXml.NavigateToNode(id_to_index::tag_name, item_data.pos_in_file);
	R_ASSERT3(item_node, "profile id=", *item_data.id);

	uiXml.SetLocalRoot(item_node);
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

LPCSTR CCharacterInfo::Bio() const
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



void CCharacterInfo::InitXmlIdToIndex()
{
	if(!id_to_index::tag_name)
		id_to_index::tag_name = "character";
	if(!id_to_index::file_str)
		id_to_index::file_str = pSettings->r_string("profiles", "files");
}