/***********************************************************************
 * Header:
 *    TEST PAIR
 * Summary:
 *    Unit tests for the pair
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once
#ifdef DEBUG

#include "pair.h"       // class under test
#include "unitTest.h"   // unit test baseclass
#include "spy.h"        // spy is a mock class to monitor the class under test

/***********************************************
 * TEST PAIR
 * Unit tests for the Pair class
 ***********************************************/
class TestPair : public UnitTest
{
   
public:
   void run()
   {
      reset();
      
      // Create
      test_create_default();
      test_create_nondefault();
      test_create_nondefaultMove();
      
      // Make Pair
      test_makePair_default();
      test_makePair_nondefault();
      
      // Delete
      test_delete_default();
      test_delete_standard();
            
      // Copy
      test_copy_default();
      test_copy_standard();

      // Copy move
      test_copyMove_default();
      test_copyMove_standard();
      
      // Assign
      test_assign_defaultToDefault();
      test_assign_standardToDefault();
      test_assign_defaultToStandard();
      test_assign_standardToStandard();
      
      // Assign Move
      test_assignMove_defaultToDefault();
      test_assignMove_standardToDefault();
      test_assignMove_defaultToStandard();
      test_assignMove_standardToStandard();

      // Direct Access
      test_directAccess_namedRead();
      test_directAccess_namedWrite();

      // Equivalence
      test_equivalence_same();
      test_equivalence_firstSmaller();
      test_equivalence_firstLarger();
      
      // Swap
      test_swap_defaultToDefault();
      test_swap_standardToDefault();
      test_swap_defaultToStandard();
      test_swap_standardToStandard();
      test_swapStandalone_defaultToDefault();
      test_swapStandalone_standardToDefault();
      test_swapStandalone_defaultToStandard();
      test_swapStandalone_standardToStandard();
  
      // Get
      test_get_firstRead();
      
      report("Pair");
   }
   
   /***************************************
    * GET
    ***************************************/
   
   void test_get_firstRead()
   {  // setup
      custom::pair <Spy, int> p;
      setupStandardFixture(p);
      Spy s;
      Spy::reset();
      // exercise
//      s = custom::get<0>(p);
//      int answer = custom::test<0>(p);
//      answer = custom::test<1>(p);
   }
   
   /***************************************
    * CREATE
    ***************************************/
   
   // create a default pair
   void test_create_default()
   {  // setup
      Spy::reset();
      // exercise
      custom::pair <Spy, int> p;
      // verify
      // (nullptr, 0)
      assertUnit(Spy::numAlloc() == 0);   // nothing allocated
      assertUnit(Spy::numDelete() == 0);  // nothing deleted
      assertUnit(Spy::numDefault() == 1); // p.first
      assertEmptyFixture(p);
   }  // teardown
   
   // create with the nondefault constructor. 
   void test_create_nondefault()
   {  // setup
      Spy s(99);
      Spy::reset();
      // exercise
      custom::pair <Spy, int> p(s, 100);
      // verify
      assertUnit(Spy::numAlloc() == 1);      // p.first
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numCopy() == 1);       // p.first
      // ( Spy(99), 100 )
      assertStandardFixture(p);
      // Spy(99)
      assertUnit(s.p != nullptr);
      assertUnit(s.get() == 99);
   }  // teardown

   // create with the nondefault constructor moving the values
   void test_create_nondefaultMove()
   {  // setup
      Spy s(99);
      Spy::reset();
      // exercise
      custom::pair <Spy, int> p(std::move(s), std::move(100));
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numCopy() == 0);       // nothing copied
      assertUnit(Spy::numCopyMove() == 1);   // s -> p.first
      assertUnit(Spy::numAssignMove() == 0); // s -> p.first
      // ( Spy(99), 100 )
      assertStandardFixture(p);
      // Spy()
      assertUnit(s.empty());
   }  // teardown
   
   /***************************************
    * MAKE PAIR
    ***************************************/
   
   // make a pair with a default Spy
   void test_makePair_default()
   {  // setup
      custom::pair <Spy, int> p;
      Spy s;
      Spy::reset();
      // exercise
      p = custom::make_pair(s, 0);
      // verify
      // (nullptr, 0)
      assertUnit(Spy::numAlloc() == 0);   // nothing allocated
      assertUnit(Spy::numDelete() == 0);  // nothing deleted
      assertUnit(Spy::numDefault() == 0); // default not created
      assertUnit(Spy::numCopy() == 1);    // p.first
      assertEmptyFixture(p);
   }  // teardown
   
   // make a pair with an allocated spy
   void test_makePair_nondefault()
   {  // setup
      custom::pair <Spy, int> p;
      Spy s(99);
      Spy::reset();
      // exercise
      p = custom::make_pair(s, 100);
      // verify
      assertUnit(Spy::numAlloc() == 1);      // p.first
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default not created
      assertUnit(Spy::numCopy() == 1);       // p.first
      // ( Spy(99), 100 )
      assertStandardFixture(p);
      // Spy(99)
      assertUnit(s.p != nullptr);
      assertUnit(s.get() == 99);
   }  // teardown
   
   /***************************************
    * DELETE
    ***************************************/
   
   // delete a default pair
   void test_delete_default()
   {  // setup
      Spy::reset();
      {
         custom::pair <Spy, int> p;
      }  // exercise
      // verify
      assertUnit(Spy::numAlloc() == 0);       // nothing allocated
      assertUnit(Spy::numDelete() == 0);      // nothing deleted
      assertUnit(Spy::numDefault() == 1);     // p.first
      assertUnit(Spy::numDestructor() == 1);  // p.first
   }  // teardown
   
   // delete the standard fixture
   void test_delete_standard()
   {  // setup
      {
         custom::pair <Spy, int> p;
         setupStandardFixture(p);
         Spy::reset();
      } // exercise
      // verify
      assertUnit(Spy::numAlloc() == 0);       // nothing allocated
      assertUnit(Spy::numDelete() == 1);      // p.first
      assertUnit(Spy::numDefault() == 0);     // nothing created
      assertUnit(Spy::numDestructor() == 1);  // p.first
   }  // teardown
   
   /***************************************
    * ASSIGN - MOVE
    * The move (steal) edition of the assignment operator
    ***************************************/
   
   // assign-move a default onto a default
   void test_assignMove_defaultToDefault()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      // ( Spy(), 0 )
      custom::pair <Spy, int> pDes;
      Spy::reset();
      // exercise
      pDes = std::move(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numAssignMove() == 1); // pSrc.first --> pDes.first
      // ( Spy(), 0 )
      assertEmptyFixture(pDes);
      // ( Spy(), 0 )
      assertEmptyFixture(pSrc);
   }  // teardown

   // assign-move a default onto a standard
   void test_assignMove_defaultToStandard()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pDes;
      setupStandardFixture(pDes);
      Spy::reset();
      // exercise
      pDes = std::move(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 1);     // nothing deleted
      assertUnit(Spy::numAssignMove() == 1); // pSrc.first --> pDes.first
      assertEmptyFixture(pDes);
      assertEmptyFixture(pSrc);
   }  // teardown
   
   // assign-move a standard onto a default
   void test_assignMove_standardToDefault()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      custom::pair <Spy, int> pDes;
      Spy::reset();
      // exercise
      pDes = std::move(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);         // nothing allocated
      assertUnit(Spy::numDelete() == 0);        // nothing destroyed
      assertUnit(Spy::numAssignMove() == 1);    // pSrc.first --> pDes.first
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
      // ( Spy(), 100 )
      assertUnit(pSrc.first.empty());
      assertUnit(pSrc.second == 100); // moving an integer leaves it unchanged
   }  // teardown
   
   // assign-move a standard onto a standard
   void test_assignMove_standardToStandard()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      // ( Spy(9), 10 )
      custom::pair <Spy, int> pDes;
      pDes.first.set(9);
      pDes.second = 10;
      Spy::reset();
      // exercise
      pDes = std::move(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);         // nothing allocated
      assertUnit(Spy::numDelete() == 1);        // pSrc.first
      assertUnit(Spy::numAssignMove() == 1);    // pSrc.first --> pDes.first
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
      // ( Spy(), 100 )
      assertUnit(pSrc.first.empty());
      assertUnit(pSrc.second == 100); // moving an integer leaves it unchanged
   }  // teardown
   
   /***************************************
    * COPY
    * The standard (non-move) edition of the copy constructor
    ***************************************/
   
   // copy a default pair
   void test_copy_default()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      Spy::reset();
      // exercise
      custom::pair <Spy, int> pDes(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);    // nothing allocated
      assertUnit(Spy::numDelete() == 0);   // nothing deleted
      assertUnit(Spy::numCopy() == 1);     // pSrc.first --> pDes.first
      // ( Spy(), 0 )
      assertEmptyFixture(pDes);
      // ( Spy(), 0 )
      assertEmptyFixture(pSrc);
   }  // teardown
   
   // copy the standard fixture
   void test_copy_standard()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      Spy::reset();
      // exercise
      custom::pair <Spy, int> pDes(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 1);    // pDes.first
      assertUnit(Spy::numDelete() == 0);   // nothing deleted
      assertUnit(Spy::numCopy() == 1);     // pSrc.first --> pDes.first
      assertUnit(pDes.first.p != pSrc.first.p);
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
      // ( Spy(99), 100 )
      assertStandardFixture(pSrc);
   }  // teardown
   
   /***************************************
    * COPY MOVE
    * The move edition of the copy constructor
    ***************************************/
   
   // copy-move a default pair
   void test_copyMove_default()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      Spy::reset();
      // exercise
      custom::pair <Spy, int> pDes(std::move(pSrc));
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // no default constructor called
      assertUnit(Spy::numAssignMove() == 0); // nothing assigned
      assertUnit(Spy::numCopyMove() == 1);   // pSrc.first --> pDes.first
      // ( Spy(), 0 )
      assertEmptyFixture(pDes);
      // ( Spy(), 0 )
      assertEmptyFixture(pSrc);
   }  // teardown
   
   // copy-move the standard fixture
   void test_copyMove_standard()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      Spy::reset();
      // exercise
      custom::pair <Spy, int> pDes(std::move(pSrc));
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // nothing default created
      assertUnit(Spy::numAssignMove() == 0); // nothing assigned
      assertUnit(Spy::numCopyMove() == 1);   // pSrc.first --> pDes.first
      assertUnit(pDes.first.p != pSrc.first.p);
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
      // ( Spy(), 100 )
      assertUnit(pSrc.first.empty());
   }  // teardown
   
   
   /***************************************
    * ASSIGN
    * The standard (non-move) edition of the assignment operator
    ***************************************/
   
   // assign a default onto a default
   void test_assign_defaultToDefault()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      // ( Spy(), 0 )
      custom::pair <Spy, int> pDes;
      Spy::reset();
      // exercise
      pDes = pSrc;
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numAssign() == 1);     // pSrc.first --> pDes.first
      // ( Spy(), 0 )
      assertEmptyFixture(pDes);
      // ( Spy(), 0 )
      assertEmptyFixture(pSrc);
   }  // teardown
   
   // assign a default onto a standard
   void test_assign_defaultToStandard()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pDes;
      setupStandardFixture(pDes);
      Spy::reset();
      // exercise
      pDes = pSrc;
      // verify
      assertUnit(Spy::numAlloc() == 0);        // nothing allocated
      assertUnit(Spy::numDelete() == 1);       // nothing deleted
      assertUnit(Spy::numAssign() == 1);       // pSrc.first --> pDes.first
      // ( Spy(), 0 )
      assertEmptyFixture(pDes);
      // ( Spy(), 0 )
      assertEmptyFixture(pSrc);
   }  // teardown
   
   // assign a standard onto a default
   void test_assign_standardToDefault()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      custom::pair <Spy, int> pDes;
      Spy::reset();
      // exercise
      pDes = pSrc;
      // verify
      assertUnit(pDes.first.p != pSrc.first.p);
      assertUnit(Spy::numAlloc() == 1);         // pDes.first
      assertUnit(Spy::numDelete() == 0);        // nothing deleted
      assertUnit(Spy::numAssign() == 1);        // pSrc.first --> pDes.first
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
      // ( Spy(99), 100 )
      assertStandardFixture(pSrc);
   }  // teardown
   
   // assign a standard onto a standard
   void test_assign_standardToStandard()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      // ( Spy(9), 10 )
      custom::pair <Spy, int> pDes;
      pDes.first.set(9);
      pDes.second = 10;
      Spy::reset();
      // exercise
      pDes = pSrc;
      // verify
      assertUnit(pDes.first.p != pSrc.first.p);
      assertUnit(Spy::numAlloc() == 0);        // nothing allocated
      assertUnit(Spy::numDelete() == 0);       // nothing deleted
      assertUnit(Spy::numAssign() == 1);        // pSrc.first --> pDes.first
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
      // ( Spy(99), 100 )
      assertStandardFixture(pSrc);
   }  // teardown
   
  
   /***************************************
    * DIRECT ACCESS
    * accessing the member variables
    ***************************************/
   
   // direct access to the member variables - read
   void test_directAccess_namedRead()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> p;
      setupStandardFixture(p);
      Spy::reset();
      // exercise & verify
      assertUnit(p.first.empty() == false);
      assertUnit(p.first.get() == 99);
      assertUnit(p.second == 100);
      // verify
      assertUnit(Spy::numAlloc() == 0);        // nothing allocated
      assertUnit(Spy::numDelete() == 0);       // nothing deleted
      assertUnit(Spy::numAssign() == 0);       // assignment not called
      assertUnit(Spy::numCopy() == 0);         // copy constructor not called
      assertUnit(Spy::numCopyMove() == 0);     // move constructor not called
   }  // teardown
   
   // direct access to the named variables - write
   void test_directAccess_namedWrite()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> p;
      setupStandardFixture(p);
      Spy::reset();
      // exercise
      p.first.set(9);
      p.second = 10;
      // verify
      assertUnit(p.first.empty() == false);
      assertUnit(p.first.get() == 9);
      assertUnit(p.second == 10);
      assertUnit(Spy::numAlloc() == 0);        // nothing allocated
      assertUnit(Spy::numDelete() == 0);       // nothing deleted
      assertUnit(Spy::numAssign() == 0);       // assignment not called
      assertUnit(Spy::numCopy() == 0);         // copy constructor not called
      assertUnit(Spy::numCopyMove() == 0);     // move constructor not called
   }  // teardown
   
   /***************************************
    * EQUIVALENCE
    * All of these ==  !=  <  > <= >=
    ***************************************/
   
   // ( Spy(99), 100 ) == ( Spy(99), 100 )
   void test_equivalence_same()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pLeft;
      setupStandardFixture(pLeft);
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pRight;
      setupStandardFixture(pRight);
      // exercise
      bool equivalent   = (pLeft == pRight);
      bool notequals    = (pLeft != pRight);
      bool lessthan     = (pLeft <  pRight);
      bool greaterthan  = (pLeft >  pRight);
      bool lessequal    = (pLeft <= pRight);
      bool greaterequal = (pLeft >= pRight);
      // verify
      assertUnit(equivalent   == true);
      assertUnit(notequals    == false);
      assertUnit(lessthan     == false);
      assertUnit(greaterthan  == false);
      assertUnit(lessequal    == true);
      assertUnit(greaterequal == true);
      assertStandardFixture(pLeft);
      assertStandardFixture(pRight);
   }  // teardown
   
   // ( Spy(0), 0 ) == ( Spy(99), 100 )
   void test_equivalence_firstSmaller()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pLeft;
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pRight;
      setupStandardFixture(pRight);
      // exercise
      bool equivalent   = (pLeft == pRight);
      bool notequals    = (pLeft != pRight);
      bool lessthan     = (pLeft <  pRight);
      bool greaterthan  = (pLeft >  pRight);
      bool lessequal    = (pLeft <= pRight);
      bool greaterequal = (pLeft >= pRight);
      // verify
      assertUnit(equivalent   == false);
      assertUnit(notequals    == true);
      assertUnit(lessthan     == true);
      assertUnit(greaterthan  == false);
      assertUnit(lessequal    == true);
      assertUnit(greaterequal == false);
      assertEmptyFixture(pLeft);
      assertStandardFixture(pRight);
   }  // teardown
   
   // ( Spy(99), 100 ) == ( Spy(), 0 )
   void test_equivalence_firstLarger()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pLeft;
      setupStandardFixture(pLeft);
      // ( Spy(), 0 )
      custom::pair <Spy, int> pRight;
      // exercise
      bool equivalent   = (pLeft == pRight);
      bool notequals    = (pLeft != pRight);
      bool lessthan     = (pLeft <  pRight);
      bool greaterthan  = (pLeft >  pRight);
      bool lessequal    = (pLeft <= pRight);
      bool greaterequal = (pLeft >= pRight);
      // verify
      assertUnit(equivalent   == false);
      assertUnit(notequals    == true);
      assertUnit(lessthan     == false);
      assertUnit(greaterthan  == true);
      assertUnit(lessequal    == false);
      assertUnit(greaterequal == true);
      assertStandardFixture(pLeft);
      assertEmptyFixture(pRight);
   }  // teardown
   

   /***************************************
    * SWAP
    * swap two elements
    ***************************************/
   
   // swap a default with a default
   void test_swap_defaultToDefault()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      // ( Spy(), 0 )
      custom::pair <Spy, int> pDes;
      Spy::reset();
      // exercise
      pDes.swap(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numAssignMove() == 2); // swap does two assign-moves
      assertUnit(Spy::numAssign() == 0);     // nothing copy-assigned
      assertUnit(Spy::numCopyMove() == 1);   // swap copy-moves the temp
      assertUnit(Spy::numCopy() == 0);       // copy-constructor never called
      // ( Spy(), 0 )
      assertEmptyFixture(pDes);
      // ( Spy(), 0 )
      assertEmptyFixture(pSrc);
   }  // teardown
   
   // swap a default with a standard
   void test_swap_defaultToStandard()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pDes;
      setupStandardFixture(pDes);
      Spy::reset();
      // exercise
      pDes.swap(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);        // nothing allocated
      assertUnit(Spy::numDelete() == 0);       // nothing deleted
      assertUnit(Spy::numAssignMove() == 2);   // swap does two assign-moves
      assertUnit(Spy::numAssign() == 0);       // nothing copy-assigned
      assertUnit(Spy::numCopyMove() == 1);     // swap copy-moves the temp
      assertUnit(Spy::numCopy() == 0);         // copy-constructor never called
      // ( Spy(), 0 )
      assertEmptyFixture(pDes);
      // ( Spy(99), 100 )
      assertStandardFixture(pSrc);
   }  // teardown
   
   // swap a standard with a default
   void test_swap_standardToDefault()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      // ( Spy(), 0 )
      custom::pair <Spy, int> pDes;
      Spy::reset();
      // exercise
      pDes.swap(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);         // pDes.first
      assertUnit(Spy::numDelete() == 0);        // nothing deleted
      assertUnit(Spy::numAssignMove() == 2);    // swap does two assign-moves
      assertUnit(Spy::numAssign() == 0);        // nothing copy-assigned
      assertUnit(Spy::numCopyMove() == 1);      // swap copy-moves the temp
      assertUnit(Spy::numCopy() == 0);          // copy-constructor never called
      // ( Spy(), 0 )
      assertEmptyFixture(pSrc);
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
   }  // teardown
   
   // swap a standard with a standard
   void test_swap_standardToStandard()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      // ( Spy(9), 10 )
      custom::pair <Spy, int> pDes;
      pDes.first.set(9);
      pDes.second = 10;
      Spy::reset();
      // exercise
      pDes.swap(pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);         // nothing allocated
      assertUnit(Spy::numDelete() == 0);        // nothing deleted
      assertUnit(Spy::numAssignMove() == 2);    // swap does two assign-moves
      assertUnit(Spy::numAssign() == 0);        // nothing copy-assigned
      assertUnit(Spy::numCopyMove() == 1);      // swap copy-moves the temp
      assertUnit(Spy::numCopy() == 0);          // copy-constructor never called
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
      // ( Spy(9), 10 )
      assertUnit(pSrc.first == Spy(9));
      assertUnit(pSrc.second == 10);
   }  // teardown
   
   // swap a default with a default - standalone edition
   void test_swapStandalone_defaultToDefault()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      // ( Spy(), 0 )
      custom::pair <Spy, int> pDes;
      Spy::reset();
      // exercise
      swap(pSrc, pDes);
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numAssignMove() == 2); // swap does two assign-moves
      assertUnit(Spy::numAssign() == 0);     // nothing copy-assigned
      assertUnit(Spy::numCopyMove() == 1);   // swap copy-moves the temp
      assertUnit(Spy::numCopy() == 0);       // copy-constructor never called
      // ( Spy(), 0 )
      assertEmptyFixture(pDes);
      // ( Spy(), 0 )
      assertEmptyFixture(pSrc);
   }  // teardown
   
   // swap a default with a standard - standalone edition
   void test_swapStandalone_defaultToStandard()
   {  // setup
      // ( Spy(), 0 )
      custom::pair <Spy, int> pSrc;
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pDes;
      setupStandardFixture(pDes);
      Spy::reset();
      // exercise
      swap(pDes, pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);        // nothing allocated
      assertUnit(Spy::numDelete() == 0);       // nothing deleted
      assertUnit(Spy::numAssignMove() == 2);   // swap does two assign-moves
      assertUnit(Spy::numAssign() == 0);       // nothing copy-assigned
      assertUnit(Spy::numCopyMove() == 1);     // swap copy-moves the temp
      assertUnit(Spy::numCopy() == 0);         // copy-constructor never called
      // ( Spy(), 0 )
      assertEmptyFixture(pDes);
      // ( Spy(99), 100 )
      assertStandardFixture(pSrc);
   }  // teardown
   
   // swap a standard with a default - standalone
   void test_swapStandalone_standardToDefault()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      // ( Spy(), 0 )
      custom::pair <Spy, int> pDes;
      Spy::reset();
      // exercise
      swap(pDes, pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);         // pDes.first
      assertUnit(Spy::numDelete() == 0);        // nothing deleted
      assertUnit(Spy::numAssignMove() == 2);    // swap does two assign-moves
      assertUnit(Spy::numAssign() == 0);        // nothing copy-assigned
      assertUnit(Spy::numCopyMove() == 1);      // swap copy-moves the temp
      assertUnit(Spy::numCopy() == 0);          // copy-constructor never called
      // ( Spy(), 0 )
      assertEmptyFixture(pSrc);
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
   }  // teardown
   
   // swap a standard with a standard - standalone
   void test_swapStandalone_standardToStandard()
   {  // setup
      // ( Spy(99), 100 )
      custom::pair <Spy, int> pSrc;
      setupStandardFixture(pSrc);
      // ( Spy(9), 10 )
      custom::pair <Spy, int> pDes;
      pDes.first.set(9);
      pDes.second = 10;
      Spy::reset();
      // exercise
      swap(pDes, pSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);         // nothing allocated
      assertUnit(Spy::numDelete() == 0);        // nothing deleted
      assertUnit(Spy::numAssignMove() == 2);    // swap does two assign-moves
      assertUnit(Spy::numAssign() == 0);        // nothing copy-assigned
      assertUnit(Spy::numCopyMove() == 1);      // swap copy-moves the temp
      assertUnit(Spy::numCopy() == 0);          // copy-constructor never called
      // ( Spy(99), 100 )
      assertStandardFixture(pDes);
      // ( Spy(9), 10 )
      assertUnit(pSrc.first == Spy(9));
      assertUnit(pSrc.second == 10);
   }  // teardown
   
   /*************************************************************
    * VERIFY EMPTY FIXTURE
    * (nullptr, 0)
    *************************************************************/
   void assertEmptyFixtureParameters(custom::pair<Spy, int>& p, int line, const char* function)
   {
      assertIndirect(p.first.empty());
      assertIndirect(p.second == 0);
   }
   
   /*************************************************************
    * VERIFY STANDAR FIXTURE
    * { 99, 100 }
    *************************************************************/
   void assertStandardFixtureParameters(custom::pair<Spy, int>& p, int line, const char* function)
   {
      assertIndirect(!p.first.empty());
      if (!p.first.empty())
         assertIndirect(p.first.get() == 99);
      assertIndirect(p.second == 100);
   }
   
   /****************************************************************
    * Setup Standard Fixture
    *   (Spy(99), 100)
    ****************************************************************/
   void setupStandardFixture(custom::pair<Spy, int>& p)
   {
      p.first.set(99);
      p.second = 100;
   }

};





#endif // DEBUG

