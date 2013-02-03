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

#ifndef __EVENTQUEUE_H__
#define __EVENTQUEUE_H__

#include "EventSender.h"

template <typename TEventData>
class EventQueue
{
	/// The implementation of the queue
	typedef EventSender<void (const TEventData&)> Sender;
	
	/// Defines a queue of data
	typedef std::vector<TEventData> EventDataQueue;
	
public:
	
	/// A static function pointer with the correct signature
	typedef typename Sender::StaticFnPtr StaticFnPtr;
	
	
	/// Adds a call-back to a static function pointer
	/// \param ptr The listener you wish to add
	void add(StaticFnPtr ptr)
	{
		_sender.add(ptr);
	}
	
	/// Adds a call-back to a member function pointer
	/// \param obj The object that has the function
	/// \param ptr A pointer to the member function pointer of the object
	template <typename T, class MemFnPtr>
	void add(T* obj, MemFnPtr ptr)
	{
		_sender.add(obj, ptr);
	}
	
	/// Removes a call-back to a static function pointer
	/// \param ptr The function pointer you wish to remove
	void remove(StaticFnPtr ptr)
	{
		_sender.remove(ptr);
	}
	
	/// Removes a call-back to a member function pointer
	/// \param obj The object that has the function
	/// \param ptr A pointer to the member function pointer of the object
	template <typename T, class MemFnPtr>
	void remove(T* obj, MemFnPtr ptr)
	{
		_sender.remove(obj, ptr);
	}
	
	/// Pushes an event in the queue
	/// \param eventData The event data that you wish to push to the queue
	void push(TEventData&& eventData)
	{
		_dataQueue.emplace_back(eventData);
	}
	
	/// Reserves memeroy for the event queue,
	/// call this if you need to add a lot of
	/// data to the queue
	void reserve(size_t amount)
	{
		_dataQueue.reserve(amount);
	}
	
	/// Emits events that are queued up
	void emit()
	{
		// loop through the data queue
		for(auto& i : _dataQueue)
		{
			// emit the event
			_sender.emit(i);
		}
		
		// clear the data queue
		_dataQueue.clear();
	}
	
	
private:
	
	/// A queue of the data which will be sent
	EventDataQueue _dataQueue;
	
	/// The sender object that is used to send the events
	Sender _sender;
};



#endif // __EVENTQUEUE_H__