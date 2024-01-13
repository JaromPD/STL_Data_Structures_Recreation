/***********************************************************************
 * Header:
 *    Test
 * Summary:
 *    Driver to test list.h
 * Author
 *    Br. Helfrich
 ************************************************************************/

#ifndef DEBUG
#define DEBUG   // Remove this to skip the unit tests
#define PRIVATE private
#else
#define PRIVATE public
#endif

#include "testList.h"       // for the list unit tests
#include "testSpy.h"        // for the spy unit tests
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
   TestList().run();
#endif // DEBUG
   
   return 0;
}
