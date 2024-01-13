/***********************************************************************
 * Header:
 *    Test
 * Summary:
 *    Driver to test priority_queue.h
 * Author
 *    Br. Helfrich
 ************************************************************************/

#include <iostream>
#include <string>

#ifndef DEBUG
#define DEBUG   
#endif
 //#undef DEBUG  // Remove this comment to disable unit tests

#include "testPriorityQueue.h"  // for the priority queue unit tests
#include "testSpy.h"            // for the spy unit tests
#include "testVector.h"         // for the vector unit tests
int Spy::counters[] = {};

/**********************************************************************
 * MAIN
 * This is just a simple menu to launch a collection of tests
 ***********************************************************************/
int main()
{
#ifdef DEBUG
   // unit tests
   TestSpy().run();
   TestVector().run();
   TestPQueue().run();
#endif // DEBUG
   
   return 0;
}
