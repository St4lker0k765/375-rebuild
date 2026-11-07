////////////////////////////////////////////////////////////////////////////
//	Module 		: alife_registry_wrappers.h
//	Created 	: 20.10.2004
//  Modified 	: 20.10.2004
//	Author		: Dmitriy Iassenev
//	Description : ALife registry wrappers
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "alife_registry_container.h"
#include "alife_registry_wrapper.h"

//реестр контактов общения с другими персонажами
typedef CALifeRegistryWrapper<CKnownContactsRegistry> KNOWN_CONTACTS_REGISTRY;

template <typename T>
class CALifeRegistryWrapperObject {
	T				*m_registry;

public:
	IC				CALifeRegistryWrapperObject		()
	{
		m_registry	= xr_new<T>();
	}

	virtual			~CALifeRegistryWrapperObject	()
	{
		xr_delete	(m_registry);
	}

	IC		T		&registry						() const
	{
		VERIFY		(m_registry);
		return		(*m_registry);
	}
};

class CKnownContactsRegistryWrapper :	public CALifeRegistryWrapperObject<KNOWN_CONTACTS_REGISTRY> {};
class CRelationRegistryWrapper :		public CALifeRegistryWrapperObject<CALifeRegistryWrapper<CRelationRegistry> > {};
