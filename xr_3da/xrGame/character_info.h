//////////////////////////////////////////////////////////////////////////
// character_info.h			шаблон, для представления абстрактного песонажа
// 
//////////////////////////////////////////////////////////////////////////

#pragma		once

#include "character_info_defs.h"
#include "shared_data.h"
#include "xml_str_id_loader.h"

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

class CCharacterInfo: public CSharedClass<SCharacterProfile, PROFILE_ID, false>,
					  public CXML_IdToIndex<PROFILE_ID, int, CCharacterInfo>
{
private:
	typedef CSharedClass	<SCharacterProfile, PROFILE_ID, false>	inherited_shared;
	typedef CXML_IdToIndex	<PROFILE_ID, int, CCharacterInfo>		id_to_index;

	friend id_to_index;
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

	static void	 InitXmlIdToIndex	();


	//загрузка из XML файла
	virtual void load_shared		(LPCSTR);

	//индекс загруженного профиля
	PROFILE_ID m_ProfileId;
	
public:


#ifdef XRGAME_EXPORTS
	PROFILE_ID					Profile()			const;
	LPCSTR						Name()				const;
	LPCSTR						Bio()				const;


	const CHARACTER_COMMUNITY&	Community()			const;
	const CHARACTER_RANK&		Rank()				const;
	const CHARACTER_REPUTATION&	Reputation()		const;

	//доступут только у InventoryOwner
protected:
	void	SetRank			(CHARACTER_RANK_VALUE			rank);
	void	SetReputation	(CHARACTER_REPUTATION_VALUE		reputation);
	void	SetCommunity	(const CHARACTER_COMMUNITY&		community);

public:
	int		TradeIconX	()	const;
	int		TradeIconY	()	const;
//	int		MapIconX	()	const;
//	int		MapIconY	()	const;
#endif

protected:


#ifdef XRGAME_EXPORTS
	CHARACTER_RANK					m_CurrentRank;
	CHARACTER_REPUTATION			m_CurrentReputation;
	CHARACTER_COMMUNITY				m_CurrentCommunity;
#endif
};