/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

namespace Game
{
    enum class CatalogGroup
    {
        /// The different kinds of catalog group
        #define CatalogGroups  \
            X( RoomShapes,            0 ) \
            X( Beds,                  1 ) \
            X( TablesAndChairs,       2 ) \
            X( Storages,              3 ) \
            X( UpholsteredFurnitures, 4 ) \
            X( eINVALID, UINT16_MAX)

        #define X(ID, VALUE) ID = VALUE,
            CatalogGroups
        #undef X
    };

    //Enum to string
    static const char* CatalogGroupToString(CatalogGroup aCatalogGroup)
    {
        switch (aCatalogGroup)
        {
            #define X(ID, VALUE) case CatalogGroup::ID: return #ID;
                CatalogGroups
            #undef X
        };

        return nullptr;
    }

    static std::size_t CatalogGroupCount()
    {
        //TODO
        return 5;
    }


    //String to enum
    static CatalogGroup StringToCatalogGroup(const char *aCatalogGroupName)
    {
        #define X(ID, VALUE) if(strcmp(aCatalogGroupName, #ID) == 0) return CatalogGroup::ID;
            CatalogGroups
        #undef X
        return CatalogGroup::eINVALID;
    }
}
