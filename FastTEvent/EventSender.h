//
//  EventSender.h
//  TEvent
//
//  Created by Miguel Martin on 3/02/13.
//  Copyright (c) 2013 Anax Creations. All rights reserved.
//

#ifndef TEvent_EventSender_h
#define TEvent_EventSender_h

#include <vector>

#include "priv/FastDelegate.h"

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

#endif
