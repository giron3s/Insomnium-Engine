/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <map>
#include "logger.h"

// A signal object may call multiple slots with the
// same signature. You can connect functions to the signal
// which will be called when the emit() method on the
// signal object is invoked. Any argument passed to emit()
// will be passed to the given functions.

namespace Framework
{
    struct EventHandler
    {
        static const int InvalidId = -1;
        static const EventHandler Invalid() { return EventHandler( InvalidId ); }
        EventHandler() : m_Id( InvalidId ) {}
        EventHandler( const int id ) : m_Id( id ) {}

        bool operator<( const EventHandler& other ) const { return m_Id < other.m_Id; }

        int m_Id;
    };

    template <typename... Args>
    class Signal
    {
    public:

        Signal()
            : m_currentId( EventHandler::InvalidId )
        {}

        Signal(Signal const& other)
            : m_currentId( EventHandler::InvalidId )
        {}

        ~Signal()
        {
            if (!m_slots.empty())
            {
                WARNING("A slot has not been disconnected from a signal.");
            }
        }

        // connects a member function of a given object to this Signal
        template <typename F, typename... A>
        EventHandler ConnectMember(F&& f, A&& ... a) const
        {
            m_slots.insert({ ++m_currentId, std::bind(f, a...) });
            return m_currentId;
        }

        // connects a std::function to the signal. The returned
        // value can be used to disconnect the function again
        EventHandler Connect(std::function<void(Args...)> const& slot) const
        {
            m_slots.insert(std::make_pair(++m_currentId, slot));
            return m_currentId;
        }

        // disconnects a previously connected function
        void Disconnect(EventHandler id) const
        {
            ASSERT( id.m_Id != EventHandler::InvalidId );
            m_slots.erase(id);
        }

        // disconnects all previously connected functions
        void DisconnectAll() const
        {
            m_slots.clear();
        }

        // calls all connected functions
        void Emit(Args... p)
        {
            for (auto it : m_slots)
            {
                it.second(p...);
            }
        }

        // assignment creates new Signal
        Signal& operator=(Signal const& other)
        {
            DisconnectAll();
        }

    private:
        mutable std::map<EventHandler, std::function<void(Args...)>> m_slots;
        mutable int m_currentId;
    };

}
#endif /* SIGNAL_HPP */
