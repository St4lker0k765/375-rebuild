//////////////////////////////////////////////////////////////////////////
// character_info.h			шаблон, для представления абстрактного песонажа
// 
//////////////////////////////////////////////////////////////////////////

#pragma		once

#include "character_info_defs.h"
#include "shared_data.h"

class NET_Packet;

#ifdef XRGAME_EXPORTS
	#include "character_community.h"
	#include "character_rank.h"
	#include "character_reputation.h"
#endif


//////////////////////////////////////////////////////////////////////////
// SCharacterProfile: данные профиля персонажа
//////////////////////////////////////////////////////////////////////////
struct SCharacterProfile : CSharedResource
{
	SCharacterProfile ();
	virtual ~SCharacterProfile ();

	//требуемые параметры персонажа
	CHARACTER_CLASS					m_Class;
	CHARACTER_RANK_VALUE			m_Rank;
	CHARACTER_REPUTATION_VALUE		m_Reputation;
};


class CInventoryOwner;
class CSE_ALifeTraderAbstract;

class CCharacterInfo: public CSharedClass<SCharacterProfile, PROFILE_ID, false>
{
private:
	typedef CSharedClass	<SCharacterProfile, PROFILE_ID, false>	inherited_shared;

	friend CInventoryOwner;
	friend CSE_ALifeTraderAbstract;
public:



	CCharacterInfo();
	~CCharacterInfo();

	virtual void Load	(PROFILE_ID id);

#ifdef XRGAME_EXPORTS
	void load	(IReader&);
	void save	(NET_Packet&);
#endif

protected:
	const SCharacterProfile* data() const	{ VERIFY(inherited_shared::get_sd()); return inherited_shared::get_sd();}
	SCharacterProfile* data()				{ VERIFY(inherited_shared::get_sd()); return inherited_shared::get_sd();}

	//индекс загруженного профиля
	PROFILE_ID m_ProfileId;
	
public:


#ifdef XRGAME_EXPORTS
	PROFILE_ID					Profile()			const;
	LPCSTR						Name()				const;

	const CHARACTER_COMMUNITY&	Community()			const;
	const CHARACTER_RANK&		Rank()				const;
	const CHARACTER_REPUTATION&	Reputation()		const;

	//доступут только у InventoryOwner
protected:
	void	SetRank			(CHARACTER_RANK_VALUE			rank);
	void	SetReputation	(CHARACTER_REPUTATION_VALUE		reputation);
	void	SetCommunity	(const CHARACTER_COMMUNITY&		community);
#endif

protected:


#ifdef XRGAME_EXPORTS
	CHARACTER_RANK					m_CurrentRank;
	CHARACTER_REPUTATION			m_CurrentReputation;
	CHARACTER_COMMUNITY				m_CurrentCommunity;
#endif
};