/////////////////////////////////////////////////////
// Для персонажей, имеющих инвентарь
// InventoryOwner.cpp
//////////////////////////////////////////////////////


#include "stdafx.h"
#include "InventoryOwner.h"
#include "entity_alive.h"
#include "actor.h"
#include "inventory.h"
#include "xrserver_objects_alife_items.h"
#include "character_info.h"
#include "script_game_object.h"
#include "script_engine.h"
#include "level.h"
#include "game_base_space.h"
#include "script_space.h"
#include "xrserver.h"
#include "xrServer_Objects_ALife_Monsters.h"
#include "alife_registry_wrappers.h"
#include "relation_registry.h"
#include "ai_object_location.h"
#include "script_callback_ex.h"
#include "game_object_space.h"

//////////////////////////////////////////////////////////////////////////
// CInventoryOwner class 
//////////////////////////////////////////////////////////////////////////
CInventoryOwner::CInventoryOwner			()
{
	m_pCharacterInfo			= NULL;
	
	m_inventory					= xr_new<CInventory>();
	m_pCharacterInfo			= xr_new<CCharacterInfo>();
}

DLL_Pure *CInventoryOwner::_construct		()
{
	return						(0);
}

CInventoryOwner::~CInventoryOwner			() 
{
	xr_delete					(m_inventory);
	xr_delete					(m_pCharacterInfo);
}

void CInventoryOwner::Load					(LPCSTR section)
{
}

void CInventoryOwner::reload				(LPCSTR section)
{
	inventory().Clear			();
	inventory().m_pOwner		= this;
	inventory().SetSlotsUseful (true);

	m_dwMoney					= 0;

	CAttachmentOwner::reload	(section);
}

void CInventoryOwner::reinit				()
{
	CAttachmentOwner::reinit	();
}

//call this after CGameObject::net_Spawn
BOOL CInventoryOwner::net_Spawn		(CSE_Abstract* DC)
{
	//получить указатель на объект, InventoryOwner
	CGameObject			*pThis = smart_cast<CGameObject*>(this);
	if(!pThis) return FALSE;
	CSE_Abstract* E	= (CSE_Abstract*)(DC);

	if(!pThis->Local())  return TRUE;


	return TRUE;
}

void CInventoryOwner::net_Destroy()
{
	CAttachmentOwner::net_Destroy();
	
	inventory().Clear();
	inventory().SetActiveSlot(NO_ACTIVE_SLOT);
}


void	CInventoryOwner::save	(NET_Packet &output_packet)
{
	if(inventory().GetActiveSlot() == NO_ACTIVE_SLOT)
		output_packet.w_u8((u8)(-1));
	else
		output_packet.w_u8((u8)inventory().GetActiveSlot());

	CharacterInfo().save(output_packet);
}
void	CInventoryOwner::load	(IReader &input_packet)
{
	u8 active_slot = input_packet.r_u8();
	if(active_slot == u8(-1))
		inventory().SetActiveSlot(NO_ACTIVE_SLOT);
	else
		inventory().SetActiveSlot(active_slot);

	CharacterInfo().load(input_packet);
}


void CInventoryOwner::UpdateInventoryOwner(u32 deltaT)
{
	inventory().Update();
}

void CInventoryOwner::renderable_Render		()
{
	if (inventory().ActiveItem())
		inventory().ActiveItem()->renderable_Render();

	CAttachmentOwner::renderable_Render();
}

void CInventoryOwner::OnItemTake			(CInventoryItem *inventory_item)
{
	CGameObject	*object = smart_cast<CGameObject*>(this);
	VERIFY		(object);
	object->callback(GameObject::eOnItemTake)(inventory_item->object().lua_game_object());

	attach		(inventory_item);
}

//возвращает текуший разброс стрельбы с учетом движения (в радианах)
float CInventoryOwner::GetWeaponAccuracy	() const
{
	return 0.f;
}

//максимальный переносимы вес
float  CInventoryOwner::MaxCarryWeight () const
{
	return inventory().GetMaxWeight();
}

void CInventoryOwner::spawn_supplies		()
{
	CGameObject								*game_object = smart_cast<CGameObject*>(this);
	VERIFY									(game_object);

	if (use_bolts())
		Level().spawn_item					("bolt",game_object->Position(),game_object->ai_location().level_vertex_id(),game_object->ID());

	if (!ai().get_alife() && GameID()==GAME_SINGLE) {
		CSE_Abstract						*abstract = Level().spawn_item("device_pda",game_object->Position(),game_object->ai_location().level_vertex_id(),game_object->ID(),true);
		CSE_ALifeItemPDA					*pda = smart_cast<CSE_ALifeItemPDA*>(abstract);
		R_ASSERT							(pda);
		pda->m_original_owner				= (u16)game_object->ID();
		NET_Packet							P;
		abstract->Spawn_Write				(P,TRUE);
		Level().Send						(P,net_flags(TRUE));
		F_entity_Destroy					(abstract);
	}
}

//игровое имя 
LPCSTR	CInventoryOwner::Name () const
{
	return CharacterInfo().Name();
}

//////////////////////////////////////////////////////////////////////////
//для работы с relation system
u16 CInventoryOwner::object_id	()  const
{
	return smart_cast<const CGameObject*>(this)->ID();
}


//////////////////////////////////////////////////////////////////////////
//установка группировки на клиентском и серверном объкте

void CInventoryOwner::SetCommunity	(CHARACTER_COMMUNITY_INDEX new_community)
{
	CEntityAlive* EA					= smart_cast<CEntityAlive*>(this); VERIFY(EA);
	CSE_Abstract* e_entity				= Level().Server->game->get_entity_from_eid	(EA->ID()); VERIFY(e_entity);
	CSE_ALifeTraderAbstract* trader		= smart_cast<CSE_ALifeTraderAbstract*>(e_entity);
	if(!trader) return;

	CharacterInfo().m_CurrentCommunity.set(new_community);
	EA->id_Team = CharacterInfo().m_CurrentCommunity.team();
	trader->m_community_index  = new_community;
}

void CInventoryOwner::SetRank			(CHARACTER_RANK_VALUE rank)
{
	CEntityAlive* EA					= smart_cast<CEntityAlive*>(this); VERIFY(EA);
	CSE_Abstract* e_entity				= Level().Server->game->get_entity_from_eid	(EA->ID()); VERIFY(e_entity);
	CSE_ALifeTraderAbstract* trader		= smart_cast<CSE_ALifeTraderAbstract*>(e_entity);
	if(!trader) return;

	CharacterInfo().m_CurrentRank.set(rank);
	trader->m_rank  = rank;
}

void CInventoryOwner::ChangeRank			(CHARACTER_RANK_VALUE delta)
{
	SetRank(Rank()+delta);
}
void CInventoryOwner::SetReputation		(CHARACTER_REPUTATION_VALUE reputation)
{
	CEntityAlive* EA					= smart_cast<CEntityAlive*>(this); VERIFY(EA);
	CSE_Abstract* e_entity				= Level().Server->game->get_entity_from_eid	(EA->ID()); VERIFY(e_entity);
	CSE_ALifeTraderAbstract* trader		= smart_cast<CSE_ALifeTraderAbstract*>(e_entity);
	if(!trader) return;

	CharacterInfo().m_CurrentReputation.set(reputation);
	trader->m_reputation  = reputation;
}

void CInventoryOwner::ChangeReputation	(CHARACTER_REPUTATION_VALUE delta)
{
	SetReputation(Reputation() + delta);
}



CHARACTER_COMMUNITY_INDEX  CInventoryOwner::Community	() const
{
	return CharacterInfo().Community().index();
}

CHARACTER_REPUTATION_VALUE	CInventoryOwner::Rank		() const
{
	return CharacterInfo().Rank().value();
}

CHARACTER_REPUTATION_VALUE	CInventoryOwner::Reputation	() const
{
	return CharacterInfo().Reputation().value();
}

void CInventoryOwner::OnItemDrop			(CInventoryItem *inventory_item)
{
	CGameObject	*object = smart_cast<CGameObject*>(this);
	VERIFY		(object);
	object->callback(GameObject::eOnItemDrop)(inventory_item->object().lua_game_object());

	detach		(inventory_item);
}

void CInventoryOwner::OnItemDropUpdate ()
{
}

void CInventoryOwner::OnItemBelt	(CInventoryItem *inventory_item, EItemPlace previous_place)
{
	attach		(inventory_item);
}
void CInventoryOwner::OnItemRuck	(CInventoryItem *inventory_item, EItemPlace previous_place)
{
	detach		(inventory_item);
}
void CInventoryOwner::OnItemSlot	(CInventoryItem *inventory_item, EItemPlace previous_place)
{
}

CInventoryItem* CInventoryOwner::GetCurrentOutfit() const
{
    return inventory().m_slots[OUTFIT_SLOT].m_pIItem;
}