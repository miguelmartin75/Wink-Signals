///
/// FastTEvent - Benchmark
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

#include <iostream>
#include <cmath>
#include <chrono> // to measure time

#include "FastTEvent/EventSender.h"
#include "FastTEvent/EventQueue.h"

#if !(defined(DEBUG) || defined(_DEBUG))
#	define NDEBUG // just-incase it is not defined
#endif

/// Handles an event that sends an integer
void handleEvent(int);
/// Different function for a queue, since it uses a different prototype,
/// but it does contain the same implemenation
void queueHandleEvent(const int&);

/// \return The time since epoch, in seconds
double GetTimeNow()
{
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;
	
	return duration_cast<duration<double, std::ratio<1> > >(high_resolution_clock::now().time_since_epoch()).count();
}

/// Prints the time taken to do a task
void printTimeTaken(double start, double end)
{
	std::cout << "Took: " << (end - start) << " seconds\n";
}

int main(int argc, char* argv[])
{
	// the amount of times to send the event out
	const int AMOUNT_OF_TIMES_TO_SEND_EVENT = 100000000;
	double start = 0;	// starting time
	double end = 0;		// ending time
	
	// create some integers to send
	std::vector<int> numbersToSend((rand() % 100) + 1);
	
	// randomize the integers
	for(auto& i : numbersToSend)
	{
		i = rand();
	}
	
	std::cout << "BENCH MARK TO SEND " << AMOUNT_OF_TIMES_TO_SEND_EVENT << " EVENTS\n\n";
	
	{
		std::cout << "Using regular function calls to handle events:\n";
		start = GetTimeNow();
		for(int i = 0; i < AMOUNT_OF_TIMES_TO_SEND_EVENT; ++i)
		{
			for(auto& number : numbersToSend)
			{
				// call the event handler directly
				// (this will probably be in-lined, with optimization)
				
				handleEvent(number);
			}
		}
		end = GetTimeNow();
		// print the time taken
		printTimeTaken(start, end);
	}
	
	
	{
		EventSender<void (int)> sender;
		sender.add(&handleEvent);
		
		std::cout << "Using EventSender<void(int)> to handle events:\n";
		start = GetTimeNow();
		for(int i = 0; i < AMOUNT_OF_TIMES_TO_SEND_EVENT; ++i)
		{
			for(auto& number : numbersToSend)
			{
				// emit the event
				sender.emit(number);
			}
		}
		end = GetTimeNow();
		
		// print the time taken
		printTimeTaken(start, end);
	}
	
	
	{
		EventQueue<int> sender;
		sender.add(&queueHandleEvent);
		
		std::cout << "Using EventQueue<int> to handle events:\n";
		start = GetTimeNow();
		
		for(int i = 0; i < AMOUNT_OF_TIMES_TO_SEND_EVENT; ++i)
		{
			sender.reserve(numbersToSend.size());
			for(auto& i : numbersToSend)
			{
				sender.push(int(i));
			}
			
			// emit the event
			sender.emit();
		}
		end = GetTimeNow();
		
		// print the time taken
		printTimeTaken(start, end);
	}
	
	return 0;
}

void handleEvent(int x)
{
	// if we're not in release mode
#ifndef NDEBUG
	std::cout << "Event recieved x = " << x << '\n';
#endif
	
	int y = rand();
	// simple computation
	int z = std::sqrt(y) * y + x;
}

void queueHandleEvent(const int& x)
{
	// if we're not in release mode
#ifndef NDEBUG
	std::cout << "Event recieved x = " << x << '\n';
#endif
	
	int y = rand();
	// simple computation
	int z = std::sqrt(y) * y + x;
}