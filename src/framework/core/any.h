/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include <type_traits>
#include <utility>
#include <typeinfo>
#include <string>
#include <cassert>

namespace Framework
{

    // Based from https://codereview.stackexchange.com/questions/20058/c11-any-class
    // https://github.com/antlr/antlr4/blob/master/runtime/Cpp/runtime/src/support/Any.h
    //This is a polymorphic wrapper capable of holding any type. (It is loosely based on boost::any)

    //In particular, this is useful when you want to store a heterogeneous collection, such as vector<Any>.


    template<class T>
    using StorageType = typename std::decay<T>::type;

    struct Any
    {
        bool IsNull() const { return mPtr == nullptr; }
        bool IsNotNull() const { return mPtr != nullptr; }

        Any() : mPtr(nullptr) {
        }

        //Any(Any& aThat) : mPtr(aThat.Clone()) {
        //}

        Any(Any&& aThat) : mPtr(aThat.mPtr) {
            aThat.mPtr = nullptr;
        }

        Any(const Any& that) : mPtr(that.Clone()) {
        }

        //Any(const Any&& that) : mPtr(that.Clone()) {
        //}

        template<typename U>
        Any(U&& aValue) : mPtr(new Derived<StorageType<U>>(std::forward<U>(aValue))) {
        }

        template<class U>
        bool Is() const 
        {
            typedef StorageType<U> T;

            auto lDerived = dynamic_cast<Derived<T> *>(mPtr);

            return lDerived != nullptr;
        }

        template<class U>
        StorageType<U>& As()
        {
            typedef StorageType<U> T;

            auto lDerived = dynamic_cast<Derived<T>*>(mPtr);

            if (!lDerived)
                throw std::bad_cast();

            return lDerived->value;
        }

        template<class U>
        operator U()
        {
            return As<StorageType<U>>();
        }

        Any& operator = (const Any& a) {
            if (mPtr == a.mPtr)
                return *this;

            auto lOldmPtr = mPtr;
            mPtr = a.Clone();

            if (lOldmPtr)
                delete lOldmPtr;

            return *this;
        }

        Any& operator = (Any&& a) {
            if (mPtr == a.mPtr)
                return *this;

            std::swap(mPtr, a.mPtr);

            return *this;
        }

        virtual ~Any()
        {
            if (mPtr)
            {
                delete mPtr;
            }
        }

        virtual bool Equals(Any other) const
        {
            return mPtr == other.mPtr;
        }

    private:
        struct Base {
            virtual ~Base() {};
            virtual Base* Clone() const = 0;
        };

        template<typename T>
        struct Derived : Base
        {
            template<typename U> Derived(U&& value_) : value(std::forward<U>(value_)) 
            {
            }

            T value;

            Base* Clone() const
            {
                return Clone<>();
            }

        private:
            template<int N = 0, typename std::enable_if<N == N && std::is_nothrow_copy_constructible<T>::value, int>::type = 0>
            Base* Clone() const
            {
                return new Derived<T>(value);
            }

            template<int N = 0, typename std::enable_if<N == N && !std::is_nothrow_copy_constructible<T>::value, int>::type = 0>
            Base* Clone() const
            {
                return nullptr;
            }

        };

        Base* Clone() const
        {
            if (mPtr)
                return mPtr->Clone();
            else
                return nullptr;
        }

        Base *mPtr;

    };

    template<> inline
    Any::Any(std::nullptr_t&&) : mPtr(nullptr) 
    {

    }

}