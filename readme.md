# Wink Signals

---

A fast template signal library, for C++, using the [Fastest Possible C++ Delegates](http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible). This library is designed to be fast and easy to use. It is incredibly lightweight and type-safe. This library uses some C++11 features, so it may or may not compile with your compiler. Since it is designed to be easy-to-use, it is quite similar to the famous [boost::signals](http://www.boost.org/doc/libs/1_53_0/doc/html/signals.html) library, but it has it's differences here and there.

Please read the tutorial further down if you wish to use the library.

# Author

---

Miguel Martin - [miguel.martin7.5@hotmail.com](mailto:miguel.martin7.5@hotmail.com)

# Tested Compilers

---

- clang 3.1/Apple Clang Version 4.1 (LLVM 3.1svn)

# Installation

---

This library is a header-only library (mainly because of templates), therefore there is no installation. Just place it in your project, or somewhere your compiler can find it and you're all set!

# Performance

This library is FAST, it uses the [Fastest Possible C++ Delegates](http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible) library in order to achieve this performance. You can read about the performance of that library on [this](http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible) website.

In order to test out the performance yourself, you can compile and run the ``Benchmark.cpp`` file in the ``examples/`` directory.

Here's the EventSender and EventQueue performance compared to regular function calls:

	BENCH MARK TO SEND 100000000 EVENTS

	Using regular function calls to handle events:
	Took: 6.11206 seconds
	Using signal<slot<void(int)> > to handle events:
	Took: 6.20876 seconds
	Using event_queue<int> to handle events:
	Took: 8.82542 seconds

As you can see, this library is quite fast compared to regular function calls.

#### NOTE:

---
This is using just one call-back for the events.

---

# Usage

---

The library is contained within the wink/ directory and is within the ``wink`` namespace.

## Slots

Slots are an alternate way of defining a function pointer. They are almost as fast as a normal function call, since they use the [Fastest Possible C++ Delegates](http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible) library, for their implementation.

### 1. Create a slot

In order to create a slot, you simply ``#include "wink/slot.h"`` and then create an object of type ``wink::slot<T>``, where T is the function prototype. This syntax is the same as ``std::function<T>``, or ``boost::signal<T>``.

#### Example:

```
wink::slot<void (int)> mySlot;
```


### 2. Binding to functions and member functions

In order to bind to a member function or regular function, you must call the static function ``slot<T>::bind()``, or alternatively pass in the member function or function when constructing your slot.

#### Example:

```
// Through the constructor
Foo bar;
wink::slot<void (int)> slotMemberFn(&bar, &Foo::foobar);
slot slotGlobalFn(&foo);

// With the use of slot<T>::bind

typedef wink::slot<void (int)> slot;
Foo bar;
slot slotMemberFn = slot::bind(&bar, &Foo::foobar);
slot slotGlobalFn = slot::bind(&foo);
```

### 3. Calling the slot

To call the slot, simply use the ``operator()``, as you would with a regular function/method. This can take any number of arguments

```
wink::slot<void (int, int, int)> slot(&foo);

slot(3, 4, 5); // call the slot
```

#### NOTE:

---
If binding to an object, when the object is destroyed, it is undefined behaviour. The slot may actually still call the object's method, or it may do something else. I reccomend, if you can, to not call the slot if you destroy an object.

---

## Signals

An event sender is used to send events to multiple call-backs immediately. 

### 1. Create a signal

To create a signal, you create an object of the class ``wink::signal<T>``, where T is a slot.

#### Example:

```
// Create a signal that uses slots with a function prototype of void (int)
wink::signal<wink::slot<void (int)> > signal;
```

#### NOTE:

---
It is reccomended to have a return value that is void, as returning a value with multiple call-backs does not make much sense.

---

### 2. Connecting/Disconnecting slots

To connect, or disconnect to/from a slot, you simply call ``connect(const slot_type&)`` and ``disconnect(const slot_type&)``, respectively. This is almost exactly the same as it is in [boost::signals](http://www.boost.org/doc/libs/1_53_0/doc/html/signals.html).

#### Example:

```
void doSomething(int x)
{
	std::cout << x << '\n';
}

// ...

// Create a signal
typedef wink::slot<void (int)> slot;
wink::signal<slot> sender;

// connect
sender.connect(slot::bind(&doSomething));

// disconnect
sender.disconnect(slot::bind(&doSomething));
```

### 3. Emiiting the slots in your signal

To emit events to your connected slots, you simply call the ``emit(T...)`` function, or use the ``()(T..)`` operator on the signal object, where ``T...`` is the paramters of your events.

#### Example:

```
// create a sender
wink::signal<wink::slot<void (int)>> sender;

// subcsribe to events...
// do other things 

// emit the event
sender.emit(32);
```  

## Event Queues

An event queue is used for when you don't want to send events out immediately.
The basic idea of an event queue is as follows:

1. You push data onto the event queue, when an event occurs
2. Later throughout the program, you call ``emit()`` or ``cemit()``, where ``emit()`` clears the data you pushed on the queue, and ``cemit()`` does not (as it is the constant version of ``emit()``).
3. You start this process all over again

An event queue is usually handy when you are doing something quite expensive and you wish to deal with the events later in your program. Or, where you require to process the event but if you fire the event immediately it will disrupt what you are doing (e.g. looping through a vector and erasing elements).

### 1. Create an Event Data Structure

In order to create an event queue, you must have a data structure that resembles the data you will be sending for your events. Typically, all data in the data structure will be const and assigned through a constructor, and there will be no other methods.

#### Example:

```
struct CollisionEvent
{
	const Entity& obj1, obj2;
	CollisionEvent(const Entity& Obj1, const Entity& Obj2)
			: obj1(Obj1), obj2(Obj2) 
	{
	}
};
```

#### NOTE:

----
Feel free to make the data non-const, especially if you need to modify sent data, please note that the event data sent by an event queue will be a const reference. (i.e. const T&, where T is the event data).

----

### 2. Create an EventQueue Object

To create an EventQueue, you must first ``#include "wink/event_queue.h"``, and then define an object. The template-paramter for ``event_queue<T>`` is the type of data you are going to send.

#### Example:

```
wink::event_queue<CollisionEvent> collisionEventQueue;
```

### 3. Connecting/disconnecting slots

Connecting and disconnecting slots is the same as a signal.

#### NOTE:

---

event_queue actually uses an EventSender object to send out events. The only difference is, an ``event_queue<T>`` uses the following function prototype:

```
void foo(const T&);
```
The name of your method may be anything, as with a signal, but all functions MUST return void. Unlike signals, where it is optional (but doesn't really make sense).

---


### 4. Pushing Data to the Event Queue

To push data to the event queue, simply call the method ``push(const T&)``, where T is the type of your event.

#### Example:

```
wink::event_queue<CollisionEvent> collisionEventQueue;

// ...

// push some data to the event queue
collisionEventQueue.push(CollisionEvent(entity1, entity2));

```

### 5. Emitting your events

Finally, to emit your events you have pushed to the event queue, call ``emit()``, ``cemit()``, or use the ``()`` operator, on your event queue object. ``cemit()`` is a constant version of the ``emit()`` function, and therefore cannot modify your event queue, meaning that it will not ``clear()`` the data you pushed onto the queue.

#### NOTE:

---
The ``()`` operator overload on wink::event_queue objects will clear your pushed data depending if the object is constant. If it is a constant object/reference, it will _not_ clear your data, otherwise it will.

---

#### Example:

```
collisionEventQueue(); 			// may or may not clear pushed data
collisionEventQueue.cemit(); 	// will not clear pushed data
collisionEventQueue.emit(); 	// will clear pushed data
```