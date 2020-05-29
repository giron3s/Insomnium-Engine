/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once


#include "core/serialization/serializableobject.h"

namespace Framework
{
    class ObjectFactory
    {
	    NON_COPYABLE_CLASS(ObjectFactory);

    private:
	    class ObjectCreator
	    {
	    public:
		    ObjectCreator() = default;
		    virtual ~ObjectCreator() = default;
		    shared_ptr<SerializableObject> Create() { return CreateT(); }
	    protected:
		    virtual shared_ptr<SerializableObject> CreateT() = 0;
	    };

	    template <typename T>
	    class ObjectCreatorT : public ObjectCreator
	    {
	    public:
		    ObjectCreatorT() = default;
		    virtual ~ObjectCreatorT() = default;
	    protected:
		    virtual shared_ptr<SerializableObject> CreateT() override { return shared_ptr<SerializableObject>(new T); }
	    };

	    ObjectFactory();
	    ~ObjectFactory() = default;

	    friend class Engine;

    public:

        template <typename T>
        void Register()
        {
            std::string lObjectName = Framework::Utils::Demangling(typeid(T).name());
            mFactories[lObjectName.c_str()] = shared_ptr<ObjectCreator>(new ObjectCreatorT<T>());
        }

        shared_ptr<SerializableObject> Create(const string& type)
        {
            if (mFactories.find(type) != mFactories.end())
            {
                auto& lObjcreator = mFactories[type];
                return lObjcreator->Create();
            }

            return nullptr;
        }

    private:
        std::map <string, shared_ptr<ObjectCreator>> mFactories;
    };
}
