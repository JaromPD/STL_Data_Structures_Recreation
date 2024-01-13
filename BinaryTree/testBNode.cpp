/***********************************************************************
 * Header:
 *    Test
 * Summary:
 *    Driver to test bnode.h
 * Author
 *    Br. Helfrich
 ************************************************************************/


#ifndef DEBUG
#define DEBUG   
#endif
 //#undef DEBUG  // Remove this comment to disable unit tests

#include "testBNode.h"       // for the BST unit tests
#include "testSpy.h"         // for the spy unit tests
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
   TestBNode().run();
#endif // DEBUG
   
   
   return 0;
}

