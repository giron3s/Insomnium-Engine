#pragma once

#include <vector>
#include "core/any.h"

namespace Framework
{
    namespace GUI
    {
        /*!
            \class UIDragDropData
            \brief UIDragDropData is used to describe information that can be stored in the clipboard, and transferred via the drag and drop mechanism.
            UIDragDropData objects associate the data that they hold with the corresponding MIME types to ensure that information can be safely transferred between applications, and
            copied around within the same application.
        
            UIDragDropData objects are usually created using \c new and supplied
            to QDrag or QClipboard objects. This is to enable Qt to manage
            the memory that they use.
        
            A single UIDragDropData object can store the same data using several
            different formats at the same time. The formats() function
            returns a list of the available formats in order of preference.*/
       
        class UIDragDropData
        {
        
        public:
        
            template<class T>
            void SetData(const T aData)
            {
                RemoveData<T>();
                Any lNewData(aData);
                mDataList.push_back(lNewData);
            }
        
            
            template<class T>
            T GetData() const
            {
                for(auto lIter : mDataList)
                {
                    if(lIter.Is<T>() )
                    {
                        return lIter.As<T>();
                    }
                }

                std::string lTName = typeid(T).name();
                WARNING("Not stored any %s type data into the UIDragDropData!", lTName.c_str());
                if ( std::is_pod<T>::value )
                    return T{0}; // pod: zero-initialized
                else
                    return T{}; // not pod: default-constructed
            }

            template<class T>
            bool HasData() const
            {
                for (auto lIter : mDataList)
                {
                    if (lIter.Is<T>())
                    {
                        return true;
                    }
                }
                return false;
            }

        
            template<class T>
            void RemoveData()
            {
                for (auto lIter : mDataList)
                {
                    if (lIter.Is<T>())
                    {
                        mDataList.erase(lIter);
                        return;
                    }
                }
            }
        
            void Clear()
            {
                mDataList.clear();
            } 
        
        protected:
            std::vector<Any> mDataList;
        };
    }
}
