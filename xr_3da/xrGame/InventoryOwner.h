/////////////////////////////////////////////////////
// Для персонажей, имеющих инвентарь
// InventoryOwner.h
//////////////////////////////////////////////////////

#pragma once
#include "pda_space.h"
#include "attachment_owner.h"
#include "script_space_forward.h"
#include "character_info_defs.h"
#include "inventory_space.h"

class CSE_Abstract;
class CInventory;
class CInventoryItem;
class CPda;
class CGameObject;
class CEntityAlive;
class CCustomZone;
class NET_Packet;

class CCharacterInfo;


class CInventoryOwner : public CAttachmentOwner {							
public:
					CInventoryOwner				();
	virtual			~CInventoryOwner			();

public:
	virtual CInventoryOwner*	cast_inventory_owner	()						{return this;}
public:

	//////////////////////////////////////////////////////////////////////////
	// общие функции

	virtual DLL_Pure	*_construct				();
	virtual BOOL		net_Spawn				(CSE_Abstract* DC);
	virtual void		net_Destroy				();
			void		Init					();
	virtual void		Load					(LPCSTR section);
	virtual void		reinit					();
	virtual void		reload					(LPCSTR section);
	virtual void		OnEvent					(NET_Packet& P, u16 type);

	//serialization
	virtual void	save						(NET_Packet &output_packet);
	virtual void	load						(IReader &input_packet);

	
	//обновление
	virtual void	UpdateInventoryOwner		(u32 deltaT);
	virtual bool	CanPutInSlot				(PIItem item, u32 slot){return true;};
	// свойства
	u32					m_dwMoney;

	// инвентарь
	CInventory	*m_inventory;			

	//игровое имя 
	virtual LPCSTR	Name        () const;

protected:
	//////////////////////////////////////////////////////////////////////////
	// инвентарь 
public:
	const CInventory &inventory() const {VERIFY (m_inventory); return(*m_inventory);}
	CInventory		 &inventory()		{VERIFY (m_inventory); return(*m_inventory);}

	//возвращает текуший разброс стрельбы (в радианах) с учетом движения
	virtual float GetWeaponAccuracy			() const;
	//максимальный переносимы вес
	virtual float MaxCarryWeight			() const;

	//////////////////////////////////////////////////////////////////////////
	//игровые характеристики персонажа
public:
	virtual CCharacterInfo&				CharacterInfo		() const {VERIFY(m_pCharacterInfo); return *m_pCharacterInfo;}

	//установка группировки на клиентском и серверном объкте
	virtual void			SetCommunity	(CHARACTER_COMMUNITY_INDEX);
	virtual void			SetRank			(CHARACTER_RANK_VALUE);
	virtual void			ChangeRank		(CHARACTER_RANK_VALUE);
	virtual void			SetReputation	(CHARACTER_REPUTATION_VALUE);
	virtual void			ChangeReputation(CHARACTER_REPUTATION_VALUE);

	//для работы с relation system
	u16								object_id	() const;
	CHARACTER_COMMUNITY_INDEX		Community	() const;
	CHARACTER_RANK_VALUE			Rank		() const;
	CHARACTER_REPUTATION_VALUE		Reputation	() const;

protected:
	CCharacterInfo*		m_pCharacterInfo;

public:
	virtual void			renderable_Render		();
	virtual void			OnItemTake				(CInventoryItem *inventory_item);
	
	virtual void			OnItemBelt				(CInventoryItem *inventory_item, EItemPlace previous_place);
	virtual void			OnItemRuck				(CInventoryItem *inventory_item, EItemPlace previous_place);
	virtual void			OnItemSlot				(CInventoryItem *inventory_item, EItemPlace previous_place);
	
	virtual void			OnItemDrop				(CInventoryItem *inventory_item);

	virtual void			OnItemDropUpdate		();
	virtual bool			use_bolts				() const {return(true);}
	virtual	void			spawn_supplies			();

	CInventoryItem* CInventoryOwner::GetCurrentOutfit() const;

	//////////////////////////////////////////////////////////////////////////
	// связь со скриптами
	//////////////////////////////////////////////////////////////////////////
};

#include "inventory_owner_inline.h"