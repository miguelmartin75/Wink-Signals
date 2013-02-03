#ifndef TEvent_EventQueue_h
#define TEvent_EventQueue_h

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



#endif
