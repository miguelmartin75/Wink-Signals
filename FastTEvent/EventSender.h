///
/// FastTEvent
/// Copyright (C) 2012 Anax Creations. All rights reserved.
///
///
/// This software is provided 'as-is', without any express or implied warranty.
/// In no event will the authors be held liable for any damages arising from the
/// use of this software.
///
/// Permission is hereby granted, free of charge, to any person
/// obtaining a copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// 1. The origin of this software must not be misrepresented;
///    you must not claim that you wrote the original software.
///    If you use this software in a product, an acknowledgment
///    in the product documentation would be appreciated but is not required.
///
/// 2. Altered source versions must be plainly marked as such,
///	   and must not be misrepresented as being the original software.
///
/// 3. The above copyright notice and this permission notice shall be included in
///    all copies or substantial portions of the Software.
///

#ifndef __EVENTSENDER_H__
#define __EVENTSENDER_H__

#include <vector>

#include "FastDelegate.h"

/// It is reccomended for events to
/// not return anything
template <class Fn>
class EventSender
{
	/// An event delegate, which is used to
	typedef fastdelegate::FastDelegate<Fn> EventDelegate;
	
	/// Defines an array of call-backs
	typedef std::vector<EventDelegate> EventDelegateArray;
	
public:
	
	/// A static function pointer with the correct signature
	typedef Fn StaticFnPtr;
	
	/// Adds a call-back to a static function pointer
	/// \param ptr The listener you wish to add
	void add(StaticFnPtr fn)
	{
		_delegates.push_back(EventDelegate(fn));
	}
	
	/// Adds a call-back to a member function pointer
	/// \param obj The object that has the function
	/// \param ptr A pointer to the member function pointer of the object
	template <typename T, class MemFnPtr>
	void add(T* obj, MemFnPtr ptr)
	{
		_delegates.push_back(EventDelegate(obj, ptr));
	}
	
	/// Removes a call-back to a static function pointer
	/// \param ptr The function pointer you wish to remove
	void remove(StaticFnPtr fn)
	{
		EventDelegate tester(fn);
		_delegates.erase(std::find(_delegates.begin(), _delegates.end(), tester));
	}
	
	/// Removes a call-back to a member function pointer
	/// \param obj The object that has the function
	/// \param ptr A pointer to the member function pointer of the object
	template <typename T, class MemFnPtr>
	void remove(T* obj, MemFnPtr ptr)
	{
		EventDelegate tester(obj, ptr);
		_delegates.erase(std::find(_delegates.begin(), _delegates.end(), tester));
	}
	
	/// Emits the events you wish to send to the call-backs
	template <class ...Args>
	void emit(Args&&... args)
	{
		for(auto& i : _delegates)
		{
			i(args...);
		}
	}
	
private:
	
	EventDelegateArray _delegates;
};

#endif // __EVENTSENDER_H__