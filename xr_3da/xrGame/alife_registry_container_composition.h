////////////////////////////////////////////////////////////////////////////
//	Module 		: alife_registry_container_composition.h
//	Created 	: 01.07.2004
//  Modified 	: 01.07.2004
//	Author		: Dmitriy Iassenev
//	Description : ALife registry container class, composition of the registries
////////////////////////////////////////////////////////////////////////////

#pragma once

//#include "alife_abstract_registry.h"
#include "PdaMsg.h"

#include "character_info_defs.h"
#include "relation_registry_defs.h"


#include "map_location_defs.h"

#pragma warning(push)
#pragma warning(disable:4005)

template <typename _index_type, typename _data_type>
class CALifeAbstractRegistry;

//для всех персонажей, отношения с другими персонажами
typedef CALifeAbstractRegistry<u16, RELATION_DATA > CRelationRegistry;
add_to_registry_type_list(CRelationRegistry);
#define character_relations define_constant(CRelationRegistry) 
#define registry_type_list save_registry_type_list(CRelationRegistry)


//для актеров, список персонажей с которыми были разговоры
typedef CALifeAbstractRegistry<u16, TALK_CONTACT_VECTOR > CKnownContactsRegistry;
add_to_registry_type_list(CKnownContactsRegistry);
#define known_contacts define_constant(CKnownContactsRegistry) 
#define registry_type_list save_registry_type_list(CKnownContactsRegistry)


//map locations for actor
add_to_registry_type_list(CMapLocationRegistry);
#define map_locations define_constant(CMapLocationRegistry) 
#define registry_type_list save_registry_type_list(CMapLocationRegistry)

#pragma warning(pop)