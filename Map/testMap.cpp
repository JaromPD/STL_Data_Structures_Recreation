/***********************************************************************
 * Header:
 *    Test
 * Summary:
 *    Driver to test map.h
 * Author
 *    Br. Helfrich
 ************************************************************************/

#ifndef DEBUG
#define DEBUG   
#endif
 //#undef DEBUG  // Remove this comment to disable unit tests

#include "testSpy.h"       // for the spy unit tests
#include "testPair.h"      // for the pair unit tests
#include "testBST.h"       // for the BST unit tests
#include "testMap.h"       // for the map unit tests
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
   TestPair().run();
   TestBST().run();
   TestMap().run();
#endif // DEBUG
   
   return 0;
}

