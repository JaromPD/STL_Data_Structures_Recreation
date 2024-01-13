/***********************************************************************
 * Header:
 *    TEST DEQUE
 * Summary:
 *    Unit tests for queue
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include "deque.h"
#include "unitTest.h"

#include <iostream>
#include <cassert>
#include <memory>
#include "spy.h"

#include <deque>

class TestDeque : public UnitTest
{
public:
   void run()
   {
      reset();

      //// Utilities
      test_ibFromID_4x1();
      test_ibFromID_3x3();
      test_icFromID_4x1();
      test_icFromID_3x3();
      test_iaFromID_4x1();
      test_iaFromID_3x3();
      test_realloc_emptyToOne();
      test_realloc_oneToTwo();
      test_realloc_shift();
      test_realloc_wrapBetweenBlocks();
      test_realloc_complex();

      //// Construct
      test_construct_default();
      test_constructCopy_empty();
      test_constructCopy_standard();
      test_constructCopy_wrapped();


      //// Assign
      test_assign_emptyToEmpty();
      test_assign_emptyToStandard();
      test_assign_standardToStandard();
      test_assign_standardToEmpty();
      test_assign_wrapped();

      //// Iterator
      test_iterator_begin_empty();
      test_iterator_begin_standard();
      test_iterator_end_standard();
      test_iterator_increment_standardMiddle();
      test_iterator_dereference_read();
      test_iterator_dereference_update();
      test_iterator_add_withinBlock();
      test_iterator_add_betweenBlocks();
      test_iterator_difference_standard();

      //// Access
      test_back_readStandard();
      test_back_readWrapped();
      test_back_writeStandard();
      test_back_writeWrapped();
      test_front_readStandard();
      test_front_writeStandard();
      test_subscript_readStandard();
      test_subscript_readWrapped();
      test_subscript_writeStandard();
      test_subscript_writeWrapped();

      //// Insert
      test_pushback_empty();
      test_pushback_roomNoWrap();
      test_pushback_newBlock();
      test_pushback_wrap();
      test_pushback_complex();
      test_pushfront_empty();
      test_pushfront_roomNoWrap();
      test_pushfront_newBlock();
      test_pushfront_wrap();
      test_pushfront_complex();
      test_pushfront_bigWrap();

      //// Remove
      test_clear_empty();
      test_clear_standard();
      test_popfront_unwrap();
      test_popfront_standard();
      test_popfront_lastElement();
      test_popfront_lastInBlock();
      test_popfront_complex();
      test_popback_unwrap();
      test_popback_standard();
      test_popback_lastElement();
      test_popback_lastInBlock();
      test_popback_complex();

      //// Status
      test_size_empty();
      test_size_standard();
      test_empty_empty();
      test_empty_standard();



      report("Deque");
   }

   /***************************************
    * INDEX TRANSLATORS
    ***************************************/

   // Block from Deque where numBlocks = 4, numCells = 1
   void test_ibFromID_4x1()
   {  // setup
      //                    iaFront
      //    +----+  +----+  +----+  +----+
      //    |    |  |    |  |    |  |    |
      //    +----+  +----+  +----+  +----+
      //        \      \      /     /
      //        +----+----+----+----+
      //        |    |    |    |    |
      //        +----+----+----+----+     
      custom::deque<Spy> d;
      d.numBlocks = 4;
      d.numCells = 1;
      d.iaFront = 2;
      // exercise
      int ib0 = d.ibFromID(0);
      int ib1 = d.ibFromID(1);
      int ib2 = d.ibFromID(2);
      int ib3 = d.ibFromID(3);
      // verify
      assertUnit(ib0 == 2);
      assertUnit(ib1 == 3);
      assertUnit(ib2 == 0);
      assertUnit(ib3 == 1);
      // teardown
      teardownStandardFixture(d);
   }

   // Block from Deque where numBlocks = 3, numCells = 3
   void test_ibFromID_3x3()
   {  // setup
      //                          iaFront
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //    |    |    |    |  |    |    |    |  |    |    |    |
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //                      \      |      / 
      //                      +----+----+----+
      //                      |    |    |    |
      //                      +----+----+----+
      custom::deque<Spy> d;
      d.numBlocks = 3;
      d.numCells = 3;
      d.iaFront = 4;
      // exercise
      int ib0 = d.ibFromID(0);
      int ib1 = d.ibFromID(1);
      int ib2 = d.ibFromID(2);
      int ib3 = d.ibFromID(3);
      int ib4 = d.ibFromID(4);
      int ib5 = d.ibFromID(5);
      int ib6 = d.ibFromID(6);
      int ib7 = d.ibFromID(7);
      int ib8 = d.ibFromID(8);
      // verify
      assertUnit(ib0 == 1);
      assertUnit(ib1 == 1);
      assertUnit(ib2 == 2);
      assertUnit(ib3 == 2);
      assertUnit(ib4 == 2);
      assertUnit(ib5 == 0);
      assertUnit(ib6 == 0);
      assertUnit(ib7 == 0);
      assertUnit(ib8 == 1);
      // teardown
      teardownStandardFixture(d);
   }

   // Cell from Deque where numBlocks = 4, numCells = 1
   void test_icFromID_4x1()
   {  // setup
      //                    iaFront
      //    +----+  +----+  +----+  +----+
      //    |    |  |    |  |    |  |    |
      //    +----+  +----+  +----+  +----+
      //        \      \      /     /
      //        +----+----+----+----+
      //        |    |    |    |    |
      //        +----+----+----+----+     
      custom::deque<Spy> d;
      d.numBlocks = 4;
      d.numCells = 1;
      d.iaFront = 2;
      // exercise
      int ic0 = d.icFromID(0);
      int ic1 = d.icFromID(1);
      int ic2 = d.icFromID(2);
      int ic3 = d.icFromID(3);
      // verify
      assertUnit(ic0 == 0);
      assertUnit(ic1 == 0);
      assertUnit(ic2 == 0);
      assertUnit(ic3 == 0);
      // teardown
      teardownStandardFixture(d);
   }

   // Cell from Deque where numBlocks = 3, numCells = 3
   void test_icFromID_3x3()
   {  // setup
      //                          iaFront
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //    |    |    |    |  |    |    |    |  |    |    |    |
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //                      \      |      / 
      //                      +----+----+----+
      //                      |    |    |    |
      //                      +----+----+----+
      custom::deque<Spy> d;
      d.numBlocks = 3;
      d.numCells = 3;
      d.iaFront = 4;
      // exercise
      int ic0 = d.icFromID(0);
      int ic1 = d.icFromID(1);
      int ic2 = d.icFromID(2);
      int ic3 = d.icFromID(3);
      int ic4 = d.icFromID(4);
      int ic5 = d.icFromID(5);
      int ic6 = d.icFromID(6);
      int ic7 = d.icFromID(7);
      int ic8 = d.icFromID(8);
      // verify
      assertUnit(ic0 == 1);
      assertUnit(ic1 == 2);
      assertUnit(ic2 == 0);
      assertUnit(ic3 == 1);
      assertUnit(ic4 == 2);
      assertUnit(ic5 == 0);
      assertUnit(ic6 == 1);
      assertUnit(ic7 == 2);
      assertUnit(ic8 == 0);
      // teardown
      teardownStandardFixture(d);
   }

   // Array from Deque where numBlocks = 4, numCells = 1
   void test_iaFromID_4x1()
   {  // setup
      //                    iaFront
      //    +----+  +----+  +----+  +----+
      //    |    |  |    |  |    |  |    |
      //    +----+  +----+  +----+  +----+
      //        \      \      /     /
      //        +----+----+----+----+
      //        |    |    |    |    |
      //        +----+----+----+----+     
      custom::deque<Spy> d;
      d.numBlocks = 4;
      d.numCells = 1;
      d.iaFront = 2;
      // exercise
      int ia0 = d.iaFromID(0);
      int ia1 = d.iaFromID(1);
      int ia2 = d.iaFromID(2);
      int ia3 = d.iaFromID(3);
      // verify
      assertUnit(ia0 == 2);
      assertUnit(ia1 == 3);
      assertUnit(ia2 == 0);
      assertUnit(ia3 == 1);
      // teardown
      teardownStandardFixture(d);
   }

   // Array from Deque where numBlocks = 3, numCells = 3
   void test_iaFromID_3x3()
   {  // setup
      //                          iaFront
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //    |    |    |    |  |    |    |    |  |    |    |    |
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //                      \      |      / 
      //                      +----+----+----+
      //                      |    |    |    |
      //                      +----+----+----+
      custom::deque<Spy> d;
      d.numBlocks = 3;
      d.numCells = 3;
      d.iaFront = 4;
      // exercise
      int ia0 = d.iaFromID(0);
      int ia1 = d.iaFromID(1);
      int ia2 = d.iaFromID(2);
      int ia3 = d.iaFromID(3);
      int ia4 = d.iaFromID(4);
      int ia5 = d.iaFromID(5);
      int ia6 = d.iaFromID(6);
      int ia7 = d.iaFromID(7);
      int ia8 = d.iaFromID(8);
      // verify
      assertUnit(ia0 == 4);
      assertUnit(ia1 == 5);
      assertUnit(ia2 == 6);
      assertUnit(ia3 == 7);
      assertUnit(ia4 == 8);
      assertUnit(ia5 == 0);
      assertUnit(ia6 == 1);
      assertUnit(ia7 == 2);
      assertUnit(ia8 == 3);
      // teardown
      teardownStandardFixture(d);
   }
   
   /***************************************
    * INDEX TRANSLATORS
    ***************************************/

   // no blocks to one
   void test_realloc_emptyToOne()
   {  // setup
      custom::deque<Spy> d;
      d.numCells = 4;
      Spy::reset();
      // exercise
      d.reallocate(1);
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //           +----+
      //           | // |
      //           +----+
      assertUnit(d.numElements == 0);
      assertUnit(d.iaFront == 0);
      assertUnit(d.numBlocks == 1);
      assertUnit(d.numCells  == 4);
      assertUnit(d.data != nullptr);
      if (d.data)
         assertUnit(d.data[0] == nullptr);
      // teardown
      teardownStandardFixture(d);
   }

   // one block to two
   void test_realloc_oneToTwo()
   {  // setup
      //     iaFront
      //    +----+----+----+
      //    |    | 99 |    |
      //    +----+----+----+
      //            |  
      //         +----+
      //         |    |
      //         +----+
      custom::deque<Spy> d;
      d.numCells = 3;
      d.numElements = 1;
      d.numBlocks = 1;
      d.data = new Spy * [1];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[0][1], Spy(99));
      d.iaFront = 1;
      Spy * pFirstBlock = d.data[0];
      Spy::reset();
      // exercise
      d.reallocate(2);
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //        iaFront
      //    +----+----+----+    
      //    |    | 99 |    |   
      //    +----+----+----+   
      //                 \     
      //                +----+----+
      //                |    | // |
      //                +----+----+
      assertUnit(d.numElements == 1);
      assertUnit(d.iaFront == 1);
      assertUnit(d.numBlocks == 2);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      { 
         assertUnit(d.data[0] == pFirstBlock);
         if (d.data[0])
            assertUnit(d.data[0][1] == Spy(99));
         assertUnit(d.data[1] == nullptr);
      }
      // teardown
      teardownStandardFixture(d);
   }

   // shift where we started at block 1
   void test_realloc_shift()
   {  // setup
      //    iaFront
      //    +----+----+----+
      //    | 67 | 79 | 85 |
      //    +----+----+----+
      //            |  
      //    +----+----+
      //    | // |    |
      //    +----+----+
      custom::deque<Spy> d;
      d.numCells = 3;
      d.numElements = 3;
      d.numBlocks = 2;
      d.data = new Spy * [2];
      d.data[0] = nullptr;
      d.data[1] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[1][0], Spy(67));
      d.alloc.construct(&d.data[1][1], Spy(79));
      d.alloc.construct(&d.data[1][2], Spy(85));
      d.iaFront = 3;
      Spy* pFirstBlock = d.data[1];
      Spy::reset();
      // exercise
      d.reallocate(3);
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //    iaFront
      //    +----+----+----+
      //    | 67 | 79 | 85 |
      //    +----+----+----+
      //            |  
      //         +----+----+----+
      //         |    | // | // |
      //         +----+----+----+
      assertUnit(d.numElements == 3);
      assertUnit(d.iaFront == 0);
      assertUnit(d.numBlocks == 3);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pFirstBlock);
         if (d.data[0])
         {
            assertUnit(d.data[0][0] == Spy(67));
            assertUnit(d.data[0][1] == Spy(79));
            assertUnit(d.data[0][2] == Spy(85));
         }
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] == nullptr);
      }
      // teardown
      teardownStandardFixture(d);
   }



   // wrapping complete blocks
   void test_realloc_wrapBetweenBlocks()
   {  // setup
      //                   iaFront
      //     +----+----+   +----+----+ 
      //     | 31 |    |   | 11 | 28 | 
      //     +----+----+   +----+----+ 
      //          |             |
      //       +----+----+----+----+
      //       |    | // | // |    |
      //       +----+----+----+----+
      custom::deque<Spy> d;
      d.numCells = 2;
      d.numElements = 3;
      d.numBlocks = 4;
      d.data = new Spy * [4];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = nullptr;
      d.data[2] = nullptr;
      d.data[3] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[3][0], Spy(11));
      d.alloc.construct(&d.data[3][1], Spy(28));
      d.alloc.construct(&d.data[0][0], Spy(31));
      d.iaFront = 6;
      Spy* pFirstBlock = d.data[3];
      Spy* pSecondBlock = d.data[0];
      Spy::reset();
      // exercise
      d.reallocate(5);
      // verify
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //   iaFront
      //   +----+----+   +----+----+ 
      //   | 11 | 28 |   | 31 |    | 
      //   +----+----+   +----+----+ 
      //          \       /      
      //          +----+----+----+----+----+
      //          |    |    | // | // | // |
      //          +----+----+----+----+----+
      assertUnit(d.numElements == 3);
      assertUnit(d.iaFront == 0);
      assertUnit(d.numBlocks == 5);
      assertUnit(d.numCells == 2);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pFirstBlock);
         if (d.data[0] && d.iaFront == 0)
         {
            assertUnit(d.data[0][0] == Spy(11));
            assertUnit(d.data[0][1] == Spy(28));
         }
         assertUnit(d.data[1] == pSecondBlock);
         if (d.data[1])
         {
            assertUnit(d.data[1][0] == Spy(31));
         }
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] == nullptr);
         assertUnit(d.data[4] == nullptr);
      }
      // teardown
      teardownStandardFixture(d);
   }

   // wrapping complex - worst case scenario
   void test_realloc_complex()
   {  // setup
      //                          iaFront
      //   +----+----+----+   +----+----+----+   +----+----+----+
      //   | 67 | 79 | 85 |   |    | 11 | 28 |   | 31 | 49 | 59 |
      //   +----+----+----+   +----+----+----+   +----+----+----+  
      //                    \         |         /             
      //                      +----+----+----+
      //                      |    |    |    |
      //                      +----+----+----+
      custom::deque<Spy> d;
      d.numCells = 3;
      d.numElements = 8;
      d.numBlocks = 3;
      d.data = new Spy * [3];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = d.alloc.allocate(d.numCells);
      d.data[2] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[1][1], Spy(11));
      d.alloc.construct(&d.data[1][2], Spy(28));
      d.alloc.construct(&d.data[2][0], Spy(31));
      d.alloc.construct(&d.data[2][1], Spy(49));
      d.alloc.construct(&d.data[2][2], Spy(59));
      d.alloc.construct(&d.data[0][0], Spy(67));
      d.alloc.construct(&d.data[0][1], Spy(79));
      d.alloc.construct(&d.data[0][2], Spy(85));
      d.iaFront = 4;
      Spy* pFirstBlock = d.data[0];
      Spy* pSecondBlock = d.data[1];
      Spy* pThirdBlock = d.data[2];
      Spy::reset();
      // exercise
      d.reallocate(4);
      // verify
      assertUnit(Spy::numCopyMove() == 0);  
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //        iaFront
      //   +----+----+----+   +----+----+----+   +----+----+----+   
      //   |    | 11 | 28 |   | 31 | 49 | 59 |   | 67 | 79 | 85 |   
      //   +----+----+----+   +----+----+----+   +----+----+----+   
      //                          \         \     /                     
      //                             +----+----+----+----+
      //                             |    |    |    | // |
      //                             +----+----+----+----+
      assertUnit(d.numElements == 8);
      assertUnit(d.iaFront == 1);
      assertUnit(d.numBlocks == 4);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pSecondBlock);
         if (d.data[0] == pSecondBlock)
         {
            assertUnit(d.data[0][1] == Spy(11));
            assertUnit(d.data[0][2] == Spy(28));
         }
         assertUnit(d.data[1] == pThirdBlock);
         if (d.data[1] == pThirdBlock)
         {
            assertUnit(d.data[1][0] == Spy(31));
            assertUnit(d.data[1][1] == Spy(49));
            assertUnit(d.data[1][2] == Spy(59));
         }
         assertUnit(d.data[2] == pFirstBlock);
         if (d.data[2] == pFirstBlock)
         {                    
            assertUnit(d.data[2][0] == Spy(67));
            assertUnit(d.data[2][1] == Spy(79));
            assertUnit(d.data[2][2] == Spy(85));
         }
         assertUnit(d.data[3] == nullptr);
      }
      // teardown
      teardownStandardFixture(d);
   }

   /***************************************
    * CONSTRUCTORS
    ***************************************/
   
   // default constructor, no allocations
   void test_construct_default()
   {  // setup
      std::allocator<custom::deque<Spy>> alloc;
      custom::deque<Spy> d;
      d.iaFront = 66;
      d.numCells = 77;
      d.numBlocks = 88;
      d.numElements = 99;
      d.data = (Spy **)0xBAADF00D;
      Spy::reset();
      // exercise
      alloc.construct(&d);  // just call the constructor by itself
      // verify
      assertUnit(Spy::numAssign() == 0);     
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0); 
      assertEmptyFixture(d);
   }  // teardown

   /***************************************
    * SIZE EMPTY CAPACITY
    ***************************************/

    // size of empty deque
   void test_size_empty()
   {  // setup
      custom::deque<Spy> d;
      Spy::reset();
      // exercise
      size_t s = d.size();
      // verify
      assertUnit(s == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(d);
   }  // teardown

   // size of the standard fixture
   void test_size_standard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy::reset();
      // exercise
      size_t s = d.size();
      // verify
      assertUnit(4 == s);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // whether an empty deque is empty
   void test_empty_empty()
   {  // setup
      custom::deque<Spy> d;
      Spy::reset();
      // exercise
      bool f = d.empty();
      // verify
      assertUnit(f == true);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(d);
   }  // teardown

   // is a standard fixture empty?
   void test_empty_standard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy::reset();
      // exercise
      bool f = d.empty();
      // verify
      assertUnit(f == false);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }


   /***************************************
    * COPY
    ***************************************/

   // copy an empty deque
   void test_constructCopy_empty() 
   {  // setup
      custom::deque<Spy> dSrc;
      Spy::reset();
      // exercise
      custom::deque<Spy> dDes(dSrc);
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(dSrc);
      assertEmptyFixture(dDes);
   }  // teardown

   // copy constructor a standard deque
   void test_constructCopy_standard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> dSrc;
      setupStandardFixture(dSrc);
      Spy::reset();
      // exercise
      custom::deque<Spy> dDes(dSrc);
      // verify
      assertUnit(Spy::numCopy() == 4);    // copy [31, 49, 55, 67]
      assertUnit(Spy::numAlloc() == 4);   // allocate [31, 49, 55, 67]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //   iaFront
      //   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+ 
      //   | 31 | 49 | 55 | 67 |    |    |    |    |    |    |    |    |    |    |    |    | 
      //   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
      //     |             
      //   +----+
      //   |    |
      //   +----+
      assertUnit(dDes.numElements == 4);
      assertUnit(dDes.numBlocks == 1);
      assertUnit(dDes.numCells == 16);
      assertUnit(dDes.data != nullptr);
      if (dDes.data != nullptr && dDes.data[0])
      {
         assertUnit(dDes.data[0][0] == Spy(31));
         assertUnit(dDes.data[0][1] == Spy(49));
         assertUnit(dDes.data[0][2] == Spy(55));
         assertUnit(dDes.data[0][3] == Spy(67));
      }
      assertStandardFixture(dSrc);
      // teardown
      teardownStandardFixture(dSrc);
      teardownStandardFixture(dDes);
   }

   // copy constructor where the source is wrapped. Need to unwrap
   void test_constructCopy_wrapped()
   {  // setup
      //                                iaFront
      //   +----+----+----+    +----+----+----+
      //   | 67 | 79 |    |    |    |    | 59 |
      //   +----+----+----+    +----+----+----+
      //     |                             |
      //   +----+----+----+----+----+----+----+
      //   |    | // | // | // | // | // |    |
      //   +----+----+----+----+----+----+----+
      custom::deque<Spy> dSrc;
      dSrc.numCells = 3;
      dSrc.numElements = 3;
      dSrc.numBlocks = 7;
      dSrc.data = new Spy * [7];
      dSrc.data[0] = dSrc.alloc.allocate(dSrc.numCells);
      dSrc.data[1] = nullptr;
      dSrc.data[2] = nullptr;
      dSrc.data[3] = nullptr;
      dSrc.data[4] = nullptr;
      dSrc.data[5] = nullptr;
      dSrc.data[6] = dSrc.alloc.allocate(dSrc.numCells);
      dSrc.alloc.construct(&dSrc.data[6][2], Spy(59));
      dSrc.alloc.construct(&dSrc.data[0][0], Spy(67));
      dSrc.alloc.construct(&dSrc.data[0][1], Spy(79));
      dSrc.iaFront = 20;
      Spy::reset();
      // exercise
      custom::deque<Spy> dDes(dSrc);
      // verify
      assertUnit(Spy::numCopy() == 3);    // copy [59, 67, 79]
      assertUnit(Spy::numAlloc() == 3);   // allocate [59, 67, 79]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //   iaFront
      //   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+ 
      //   | 59 | 67 | 79 |    |    |    |    |    |    |    |    |    |    |    |    |    | 
      //   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
      //     |             
      //   +----+
      //   |    |
      //   +----+
      assertUnit(dDes.numElements == 3);
      assertUnit(dDes.numBlocks == 1);
      assertUnit(dDes.numCells == 16);
      assertUnit(dDes.data != nullptr);
      if (dDes.data != nullptr && dDes.data[0])
      {
         assertUnit(dDes.data[0][0] == Spy(59));
         assertUnit(dDes.data[0][1] == Spy(67));
         assertUnit(dDes.data[0][2] == Spy(79));
      }
      // teardown
      teardownStandardFixture(dSrc);
      teardownStandardFixture(dDes);
   }

   /***************************************
    * ASSIGNMENT
    ***************************************/
   
   // assignment when there is nothing to copy
   void test_assign_emptyToEmpty()
   {  // setup
      custom::deque <Spy> dSrc;
      custom::deque <Spy> dDes;
      Spy::reset();
      // exercise
      dDes = dSrc;
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(dDes);
      assertEmptyFixture(dSrc);
   }  // teardown

   // assignment when source and destination are same size
   void test_assign_standardToStandard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> dSrc;
      setupStandardFixture(dSrc);
      custom::deque<Spy> dDes;
      setupStandardFixture(dDes);
      Spy::reset();
      // exercise
      dDes = dSrc;
      // verify
      assertUnit(Spy::numAssign() == 4);  // assign [31, 49, 55, 67]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0); 
      assertUnit(Spy::numDelete() == 0); 
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertStandardFixture(dDes);
      assertStandardFixture(dSrc);
      // teardown
      teardownStandardFixture(dSrc);
      teardownStandardFixture(dDes);
   }

   // assignment when the destination is smaller than the source
   void test_assign_standardToEmpty()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> dSrc;
      setupStandardFixture(dSrc);
      custom::deque<Spy> dDes;
      Spy::reset();
      // exercise
      dDes = dSrc;
      // verify
      assertUnit(Spy::numCopy() == 4);    // copy [31, 49, 55, 67]
      assertUnit(Spy::numAlloc() == 4);   // allocate [31, 49, 55, 67]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //   iaFront
      //   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+ 
      //   | 31 | 49 | 55 | 67 |    |    |    |    |    |    |    |    |    |    |    |    | 
      //   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
      //     |             
      //   +----+
      //   |    |
      //   +----+
      assertUnit(dDes.numElements == 4);
      assertUnit(dDes.numBlocks == 1);
      assertUnit(dDes.numCells == 16);
      assertUnit(dDes.data != nullptr);
      if (dDes.data != nullptr && dDes.data[0])
      {
         assertUnit(dDes.data[0][0] == Spy(31));
         assertUnit(dDes.data[0][1] == Spy(49));
         assertUnit(dDes.data[0][2] == Spy(55));
         assertUnit(dDes.data[0][3] == Spy(67));
      }
      assertStandardFixture(dSrc);
      // teardown
      teardownStandardFixture(dSrc);
      teardownStandardFixture(dDes);
   }

   // assignment when the destination is bigger than the source
   void test_assign_emptyToStandard()
   {  // setup
      custom::deque<Spy> dSrc;
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> dDes;
      setupStandardFixture(dDes);
      Spy::reset();
      // exercise
      dDes = dSrc;
      // verify
      assertUnit(Spy::numDelete() == 4); // delete [31, 49, 55, 67]
      assertUnit(Spy::numDestructor() == 4); // destroy [31, 49, 55, 67]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);   
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(dSrc);
      assertUnit(dDes.numElements == 0);
      assertUnit(dDes.numBlocks == 4);
      assertUnit(dDes.numCells == 3);
      assertUnit(dDes.data != nullptr);
      // teardown
      teardownStandardFixture(dSrc);
      teardownStandardFixture(dDes);
   }

   // assign where the source is wrapped. Need to unwrap
   void test_assign_wrapped()
   {  // setup
      //                                iaFront
      //   +----+----+----+    +----+----+----+
      //   | 67 | 79 |    |    |    |    | 59 |
      //   +----+----+----+    +----+----+----+
      //     |                             |
      //   +----+----+----+----+----+----+----+
      //   |    | // | // | // | // | // |    |
      //   +----+----+----+----+----+----+----+
      custom::deque<Spy> dSrc;
      dSrc.numCells = 3;
      dSrc.numElements = 3;
      dSrc.numBlocks = 7;
      dSrc.data = new Spy * [7];
      dSrc.data[0] = dSrc.alloc.allocate(dSrc.numCells);
      dSrc.data[1] = nullptr;
      dSrc.data[2] = nullptr;
      dSrc.data[3] = nullptr;
      dSrc.data[4] = nullptr;
      dSrc.data[5] = nullptr;
      dSrc.data[6] = dSrc.alloc.allocate(dSrc.numCells);
      dSrc.alloc.construct(&dSrc.data[6][2], Spy(59));
      dSrc.alloc.construct(&dSrc.data[0][0], Spy(67));
      dSrc.alloc.construct(&dSrc.data[0][1], Spy(79));
      dSrc.iaFront = 20;
      custom::deque<Spy> dDes;
      Spy::reset();
      // exercise
      dDes = dSrc;
      // verify
      assertUnit(Spy::numCopy() == 3);    // copy [59, 67, 79]
      assertUnit(Spy::numAlloc() == 3);   // allocate [59, 67, 79]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //   iaFront
      //   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+ 
      //   | 59 | 67 | 79 |    |    |    |    |    |    |    |    |    |    |    |    |    | 
      //   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
      //     |             
      //   +----+
      //   |    |
      //   +----+
      assertUnit(dDes.numElements == 3);
      assertUnit(dDes.numBlocks == 1);
      assertUnit(dDes.numCells == 16);
      assertUnit(dDes.data != nullptr);
      if (dDes.data != nullptr && dDes.data[0])
      {
         assertUnit(dDes.data[0][0] == Spy(59));
         assertUnit(dDes.data[0][1] == Spy(67));
         assertUnit(dDes.data[0][2] == Spy(79));
      }
      // teardown
      teardownStandardFixture(dSrc);
      teardownStandardFixture(dDes);
   }

   
   /***************************************
    * CLEAR
    ***************************************/
   
   // clear an empty collection
   void test_clear_empty()
   {  // setup
      custom::deque<Spy> d;
      Spy::reset();
      // exercise
      d.clear();
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(d);
   }  // teardown

   // clear when there are elements
   void test_clear_standard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy::reset();
      // exercise
      d.clear();
      // verify
      assertUnit(Spy::numDelete() == 4);     // delete 31, 49, 55, 67
      assertUnit(Spy::numDestructor() == 4); // destroy 31, 49, 55, 67
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //          +----+----+----+----+
      //          | // | // | // | // |
      //          +----+----+----+----+
      assertUnit(d.numElements == 0);
      assertUnit(d.numBlocks == 4);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data != nullptr)
      {
         assertUnit(d.data[0] == nullptr);
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] == nullptr);
      }
      // teardown
      teardownStandardFixture(d);
   }


   /***************************************
    * PUSH BACK
    ***************************************/

   // add an element when the deque is empty
   void test_pushback_empty()
   {  // setup
      custom::deque<Spy> d;
      d.numCells = 5;
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    iaFront
      //    +----+----+----+----+----+
      //    | 99 |    |    |    |    |
      //    +----+----+----+----+----+
      //                |  
      //             +----+
      //             |    |
      //             +----+
      assertUnit(d.numElements == 1);
      assertUnit(d.numCells == 5);
      assertUnit(d.numBlocks == 1);
      assertUnit(d.iaFront == 0);
      assertUnit(d.data != nullptr);
      if (d.data)
         assertUnit(d.data[0][0] == Spy(99));
      // teardown
      teardownStandardFixture(d);
   }

   // add an element when there is room. Add an element to an existing block
   void test_pushback_roomNoWrap()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 | 99 |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(d.numElements == 5);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[2] != nullptr);
         if (d.data[2] && d.numElements == 5)
         {
            assertUnit(d.data[2][2] == Spy(99));
            d.alloc.destroy(&d.data[2][2]);
            d.numElements = 4;
         }
      }
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // add an element when there is room. Add a new block
   void test_pushback_newBlock()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 | 79 |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      new((void*)(&(d.data[2][2]))) Spy(79);
      d.numElements++;
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0     1    2       0    1    2      0    1    2
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 | 79 |  | 99 |    |    |
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //                      \       |        /   
      //                 +----+----+----+----+
      //                 | // |    |    |    |
      //                 +----+----+----+----+
      assertUnit(d.numElements == 6);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[2] != nullptr);
         if (d.data[2])
         {
            assertUnit(d.data[2][2] == Spy(79));
            d.alloc.destroy(&d.data[2][2]);
            d.numElements--;
         }
         assertUnit(d.data[3] != nullptr);
         if (d.data[3])
         {
            assertUnit(d.data[3][0] == Spy(99));
            d.alloc.destroy(&d.data[3][0]);
            d.numElements--;
            d.alloc.deallocate(d.data[3], d.numCells);
            d.data[3] = nullptr;
         }
      }
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // add an element that involves going into the wrapped state
   void test_pushback_wrap()
   {  // setup
      //               iaFront
      //       0     1    2    3
      //     +----+----+----+----+ 
      //     |    |    | 11 | 28 | 
      //     +----+----+----+----+ 
      //               |  
      //            +----+
      //            |    |
      //            +----+
      custom::deque<Spy> d;
      d.numCells = 4;
      d.numElements = 2;
      d.numBlocks = 1;
      d.data = new Spy * [1];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[0][2], Spy(11));
      d.alloc.construct(&d.data[0][3], Spy(28));
      Spy * pBlock = d.data[0];
      d.iaFront = 2;
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //               iaFront
      //       0     1    2    3
      //     +----+----+----+----+   +----+----+----+----+ 
      //     |    |    | 11 | 28 |   | 99 |    |    |    |
      //     +----+----+----+----+   +----+----+----+----+ 
      //                       \      /
      //                     +----+----+
      //                     |    |    |
      //                     +----+----+
      assertUnit(d.numElements == 3);
      assertUnit(d.iaFront == 2);
      assertUnit(d.numBlocks == 2);
      assertUnit(d.numCells == 4);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pBlock);
         if (d.data[0] == pBlock)
         {
            assertUnit(d.data[0][2] == Spy(11));
            assertUnit(d.data[0][3] == Spy(28));
         }
         assertUnit(d.data[1] != nullptr);
         if (d.data[1] != nullptr)
            assertUnit(d.data[1][0] == Spy(99));
      }
      // teardown
      teardownStandardFixture(d);
   }
   // add an element that will cause the deque to reallocaate. This will cause an unwrap
   void test_pushback_complex()
   {  // setup
         //                          iaFront
      //   +----+----+----+   +----+----+----+   +----+----+----+
      //   | 67 | 79 | 85 |   |    | 11 | 28 |   | 31 | 49 | 59 |
      //   +----+----+----+   +----+----+----+   +----+----+----+  
      //                    \         |         /             
      //                      +----+----+----+
      //                      |    |    |    |
      //                      +----+----+----+
      custom::deque<Spy> d;
      d.numCells = 3;
      d.numElements = 8;
      d.numBlocks = 3;
      d.data = new Spy * [3];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = d.alloc.allocate(d.numCells);
      d.data[2] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[1][1], Spy(11));
      d.alloc.construct(&d.data[1][2], Spy(28));
      d.alloc.construct(&d.data[2][0], Spy(31));
      d.alloc.construct(&d.data[2][1], Spy(49));
      d.alloc.construct(&d.data[2][2], Spy(59));
      d.alloc.construct(&d.data[0][0], Spy(67));
      d.alloc.construct(&d.data[0][1], Spy(79));
      d.alloc.construct(&d.data[0][2], Spy(85));
      d.iaFront = 4;
      Spy* pFirstBlock  = d.data[0];
      Spy* pSecondBlock = d.data[1];
      Spy* pThirdBlock  = d.data[2];
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_back(s);
      // verify
      assertUnit(Spy::numCopyMove() == 0);   
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //        iaFront
      //   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+
      //   |    | 11 | 28 |   | 31 | 49 | 59 |   | 67 | 79 | 85 |   | 99 |    |    |
      //   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+ 
      //                          \         \     /      /            
      //                             +----+----+----+----+----+----+
      //                             |    |    |    |    | // | // |
      //                             +----+----+----+----+----+----+
      assertUnit(d.numElements == 9);
      assertUnit(d.iaFront == 1);
      assertUnit(d.numBlocks == 6);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pSecondBlock);
         if (d.data[0])
         {
            assertUnit(d.data[0][1] == Spy(11));
            assertUnit(d.data[0][2] == Spy(28));
         }
         assertUnit(d.data[1] == pThirdBlock);
         if (d.data[1])
         {
            assertUnit(d.data[1][0] == Spy(31));
            assertUnit(d.data[1][1] == Spy(49));
            assertUnit(d.data[1][2] == Spy(59));
         }
         assertUnit(d.data[2] == pFirstBlock);
         if (d.data[2])
         {
            assertUnit(d.data[2][0] == Spy(67));
            assertUnit(d.data[2][1] == Spy(79));
            assertUnit(d.data[2][2] == Spy(85));
         }
         assertUnit(d.data[3] != nullptr);
         if (d.data[3] && d.numBlocks == 6)
         {
            assertUnit(d.data[3][0] == Spy(99));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }

   /***************************************
    * PUSH FRONT
    ***************************************/
   
    // add an element when the deque is empty
   void test_pushfront_empty()
   {  // setup
      custom::deque<Spy> d;
      d.numCells = 5;
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_front(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                       iaFront
      //      0     1   2    3     4
      //    +----+----+----+----+----+
      //    |    |    |    |    | 99 |
      //    +----+----+----+----+----+
      //                |  
      //             +----+
      //             |    |
      //             +----+
      assertUnit(d.numElements == 1);
      assertUnit(d.numCells == 5);
      assertUnit(d.numBlocks == 1);
      assertUnit(d.iaFront == 4);
      assertUnit(d.data != nullptr);
      if (d.data)
         assertUnit(d.data[0][4] == Spy(99));
      // teardown
      teardownStandardFixture(d);
   }

   // add an element when there is room. Add an element to an existing block
   void test_pushfront_roomNoWrap()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_front(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 99 | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(d.numElements == 5);
      assertUnit(d.iaFront == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[1] != nullptr);
         if (d.data[1] && d.numElements == 5)
         {
            assertUnit(d.data[1][0] == Spy(99));
            d.alloc.destroy(&d.data[1][0]);
            d.numElements = 4;
            d.iaFront = 4;
         }
      }
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // add an element when there is room. Add a new block
   void test_pushfront_newBlock()
   {  // setup
      //    iaFront
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 28 | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      new((void*)(&(d.data[1][0]))) Spy(28);
      d.iaFront--;
      d.numElements++;
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_front(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //              iaFront
      //      0     1    2       0    1    2      0    1    2
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //    |    |    | 99 |  | 28 | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+  +----+----+----+
      //                     \        |       /   
      //                      +----+----+----+----+
      //                      |    |    |    | // |
      //                      +----+----+----+----+
      assertUnit(d.numElements == 6);
      assertUnit(d.iaFront == 2);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] != nullptr);
         if (d.data[0])
         {
            assertUnit(d.data[0][2] == Spy(99));
            d.alloc.destroy(&d.data[0][2]);
            d.alloc.deallocate(d.data[0], d.numCells);
            d.data[0] = nullptr;
            d.iaFront++;
            d.numElements--;
         }
         if (d.data[1])
         {
            assertUnit(d.data[1][0] == Spy(28));
            d.alloc.destroy(&d.data[1][0]);
            d.numElements--;
            d.iaFront++;
         }
         assertUnit(d.data[2] != nullptr);
      }
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // add an element that involves going into the wrapped state
   void test_pushfront_wrap()
   {  // setup
      //    iaFront
      //       0     1    2    3
      //     +----+----+----+----+ 
      //     | 11 | 28 |    |    | 
      //     +----+----+----+----+ 
      //               |  
      //            +----+
      //            |    |
      //            +----+
      custom::deque<Spy> d;
      d.numCells = 4;
      d.numElements = 2;
      d.numBlocks = 1;
      d.data = new Spy * [1];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[0][0], Spy(11));
      d.alloc.construct(&d.data[0][1], Spy(28));
      Spy* pBlock = d.data[0];
      d.iaFront = 0;
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_front(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //               iaFront
      //       0     1    2    3
      //     +----+----+----+----+   +----+----+----+----+ 
      //     | 11 | 28 |    |    |   |    |    |    | 99 |
      //     +----+----+----+----+   +----+----+----+----+ 
      //                        \     /
      //                      +----+----+
      //                      |    |    |
      //                      +----+----+
      assertUnit(d.numElements == 3);
      assertUnit(d.iaFront == 7);
      assertUnit(d.numBlocks == 2);
      assertUnit(d.numCells == 4);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pBlock);
         if (d.data[0] == pBlock)
         {
            assertUnit(d.data[0][0] == Spy(11));
            assertUnit(d.data[0][1] == Spy(28));
         }
         assertUnit(d.data[1] != nullptr);
         if (d.data[1] != nullptr)
         {
            assertUnit(d.data[1][3] == Spy(99));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }

   // add an element that will cause the deque to reallocaate. This will cause an unwrap
   void test_pushfront_complex()
   {  // setup
         //                   iaFront
      //   +----+----+----+   +----+----+----+   +----+----+----+
      //   | 67 | 79 |    |   |  5 | 11 | 28 |   | 31 | 49 | 59 |
      //   +----+----+----+   +----+----+----+   +----+----+----+  
      //                    \         |         /             
      //                      +----+----+----+
      //                      |    |    |    |
      //                      +----+----+----+
      custom::deque<Spy> d;
      d.numCells = 3;
      d.numElements = 8;
      d.numBlocks = 3;
      d.data = new Spy * [3];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = d.alloc.allocate(d.numCells);
      d.data[2] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[1][0], Spy(5));
      d.alloc.construct(&d.data[1][1], Spy(11));
      d.alloc.construct(&d.data[1][2], Spy(28));
      d.alloc.construct(&d.data[2][0], Spy(31));
      d.alloc.construct(&d.data[2][1], Spy(49));
      d.alloc.construct(&d.data[2][2], Spy(59));
      d.alloc.construct(&d.data[0][0], Spy(67));
      d.alloc.construct(&d.data[0][1], Spy(79));
      d.iaFront = 3;
      Spy* pFirstBlock = d.data[0];
      Spy* pSecondBlock = d.data[1];
      Spy* pThirdBlock = d.data[2];
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_front(s);
      // verify
      assertUnit(Spy::numCopyMove() == 0);  
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //   iaFront
      //   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+ 
      //   | 5  | 11 | 28 |   | 31 | 49 | 59 |   | 67 | 79 |    |   |    |    | 99 |
      //   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+    
      //                          \         \     /             /      
      //                        +----+----+----+----+----+----+
      //                        |    |    |    | // | // |    |
      //                        +----+----+----+----+----+----+
      assertUnit(d.numElements == 9);
      assertUnit(d.iaFront == 17);
      assertUnit(d.numBlocks == 6);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pSecondBlock);
         if (d.data[0] == pSecondBlock)
         {
            assertUnit(d.data[0][0] == Spy(5));
            assertUnit(d.data[0][1] == Spy(11));
            assertUnit(d.data[0][2] == Spy(28));
         }
         assertUnit(d.data[1] == pThirdBlock);
         if (d.data[1] == pThirdBlock)
         {
            assertUnit(d.data[1][0] == Spy(31));
            assertUnit(d.data[1][1] == Spy(49));
            assertUnit(d.data[1][2] == Spy(59));
         }
         assertUnit(d.data[2] == pFirstBlock);
         if (d.data[2] == pFirstBlock)
         {
            assertUnit(d.data[2][0] == Spy(67));
            assertUnit(d.data[2][1] == Spy(79));
         }
         assertUnit(d.data[3] == nullptr);
         assertUnit(d.data[4] == nullptr);
         assertUnit(d.data[5] != nullptr);
         if (d.data[5] != nullptr)
            assertUnit(d.data[5][2] == Spy(99));

      }
      // teardown
      teardownStandardFixture(d);
   }
   
   // add an element that will cause the deque to reallocaate. This will cause an unwrap
   void test_pushfront_bigWrap()
   {  // setup
      //   iaFront
      //   +----+----+----+
      //   | 67 | 79 |    |
      //   +----+----+----+
      //     | 
      //   +----+----+----+----+----+----+----+
      //   |    | // | // | // | // | // | // |
      //   +----+----+----+----+----+----+----+
      custom::deque<Spy> d;
      d.numCells = 3;
      d.numElements = 2;
      d.numBlocks = 7;
      d.data = new Spy * [7];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = nullptr;
      d.data[2] = nullptr;
      d.data[3] = nullptr;
      d.data[4] = nullptr;
      d.data[5] = nullptr;
      d.data[6] = nullptr;
      d.alloc.construct(&d.data[0][0], Spy(67));
      d.alloc.construct(&d.data[0][1], Spy(79));
      d.iaFront = 0;
      Spy* pBlock = d.data[0];
      Spy s(99);
      Spy::reset();
      // exercise
      d.push_front(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99
      assertUnit(Spy::numAlloc() == 1);      // allocate 99
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //   iaFront
      //   +----+----+----+    +----+----+----+
      //   | 67 | 79 |    |    |    |    | 99 |
      //   +----+----+----+    +----+----+----+
      //     |                             |
      //   +----+----+----+----+----+----+----+
      //   |    | // | // | // | // | // |    |
      //   +----+----+----+----+----+----+----+
      assertUnit(d.numElements == 3);
      assertUnit(d.iaFront == 20);
      assertUnit(d.numBlocks == 7);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pBlock);
         if (d.data[0])
         {
            assertUnit(d.data[0][0] == Spy(67));
            assertUnit(d.data[0][1] == Spy(79));
         }
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] == nullptr);
         assertUnit(d.data[4] == nullptr);
         assertUnit(d.data[5] == nullptr);
         assertUnit(d.data[6] != nullptr);
         if (d.data[6])
         {
            assertUnit(d.data[6][2] == Spy(99));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }

   /***************************************
    * POP FRONT
    ***************************************/
   
  
   // remove one element from a standard fixture
   void test_popfront_standard()
   {  // setup
      //         iaFront
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy::reset();
      // exercise
      d.pop_front();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 31
      assertUnit(Spy::numDestructor() == 1);    // destroy 31
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);      
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //             iaFront
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    |    | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(d.numElements == 3);
      assertUnit(d.iaFront == 5);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[1] != nullptr);
         if (d.data[1])
         {
            d.alloc.construct(&d.data[1][1], Spy(31));
            d.iaFront--;
            d.numElements++;
         }
      }
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // go from an unwrapped state to a wrapped state
   void test_popfront_unwrap()
   {  // setup
      //    iaFront
      //       0     1    2    3
      //     +----+----+----+----+ 
      //     | 28 | 31 |    | 11 | 
      //     +----+----+----+----+ 
      //               |  
      //            +----+
      //            |    |
      //            +----+
      custom::deque<Spy> d;
      d.numCells = 4;
      d.numElements = 3;
      d.numBlocks = 1;
      d.data = new Spy * [1];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[0][3], Spy(11));
      d.alloc.construct(&d.data[0][0], Spy(28));
      d.alloc.construct(&d.data[0][1], Spy(31));
      Spy* pBlock = d.data[0];
      d.iaFront = 3;
      Spy::reset();
      // exercise
      d.pop_front();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 11
      assertUnit(Spy::numDestructor() == 1);    // destroy 11
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //               iaFront
      //       0     1    2    3
      //     +----+----+----+----+ 
      //     | 28 | 31 |    |    | 
      //     +----+----+----+----+ 
      //               |  
      //            +----+
      //            |    |
      //            +----+
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == 0);
      assertUnit(d.numBlocks == 1);
      assertUnit(d.numCells == 4);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pBlock);
         if (d.data[0])
         {
            assertUnit(d.data[0][0] == Spy(28));
            assertUnit(d.data[0][1] == Spy(31));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }
   // remove last element in a block which is the last block
   void test_popfront_lastElement()
   {  // setup
      //    iaFront
      //       0     1    2    3
      //     +----+----+----+----+ 
      //     |    | 31 |    |    | 
      //     +----+----+----+----+ 
      //               |  
      //            +----+
      //            |    |
      //            +----+
      custom::deque<Spy> d;
      d.numCells = 4;
      d.numElements = 1;
      d.numBlocks = 1;
      d.data = new Spy * [1];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[0][1], Spy(31));
      d.iaFront = 1;
      Spy::reset();
      // exercise
      d.pop_front();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 31
      assertUnit(Spy::numDestructor() == 1);    // destroy 31
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //               iaFront
      //            +----+
      //            | // |
      //            +----+
      assertUnit(d.numElements == 0);
      assertUnit(d.numBlocks == 1);
      assertUnit(d.numCells == 4);
      assertUnit(d.data != nullptr);
      if (d.data)
         assertUnit(d.data[0] == nullptr);
      // teardown
      teardownStandardFixture(d);
   }

   // remove last element from a block, but there are other blocks
   void test_popfront_lastInBlock()
   {  // setup
      //         iaFront
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    |    | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      d.alloc.destroy(&d.data[1][1]);
      d.iaFront++;
      d.numElements--;
      Spy::reset();
      // exercise
      d.pop_front();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 49
      assertUnit(Spy::numDestructor() == 1);    // destroy 49
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //             iaFront
      //                         0    1    2
      //                      +----+----+----+
      //                      | 55 | 67 |    |
      //                      +----+----+----+
      //                        /
      //          +----+----+----+----+
      //          | // | // |    | // |
      //          +----+----+----+----+
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == 6);
      assertUnit(d.numBlocks == 4);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == nullptr);
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] != nullptr);
         if (d.data[2])
         {
            assertUnit(d.data[2][0] == Spy(55));
            assertUnit(d.data[2][1] == Spy(67));
         }
         assertUnit(d.data[3] == nullptr);
      }
      // teardown
      teardownStandardFixture(d);
   }

   // pretty much the worst case scenario
   void test_popfront_complex()
   {  // setup
      //                                iaFront
      //   +----+----+----+    +----+----+----+
      //   | 67 | 79 |    |    |    |    | 59 |
      //   +----+----+----+    +----+----+----+
      //     |                             |
      //   +----+----+----+----+----+----+----+
      //   |    | // | // | // | // | // |    |
      //   +----+----+----+----+----+----+----+
      custom::deque<Spy> d;
      d.numCells = 3;
      d.numElements = 3;
      d.numBlocks = 7;
      d.data = new Spy * [7];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = nullptr;
      d.data[2] = nullptr;
      d.data[3] = nullptr;
      d.data[4] = nullptr;
      d.data[5] = nullptr;
      d.data[6] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[6][2], Spy(59));
      d.alloc.construct(&d.data[0][0], Spy(67));
      d.alloc.construct(&d.data[0][1], Spy(79));
      d.iaFront = 20;
      Spy* pBlock = d.data[0];
      Spy::reset();
      // exercise
      d.pop_front();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 59
      assertUnit(Spy::numDestructor() == 1);    // destroy 59
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //   iaFront
      //   +----+----+----+
      //   | 67 | 79 |    |
      //   +----+----+----+
      //     |             
      //   +----+----+----+----+----+----+----+
      //   |    | // | // | // | // | // | // |
      //   +----+----+----+----+----+----+----+
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == 0);
      assertUnit(d.numBlocks == 7);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pBlock);
         if (d.data[0])
         {
            assertUnit(d.data[0][0] == Spy(67));
            assertUnit(d.data[0][1] == Spy(79));
         }
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] == nullptr);
         assertUnit(d.data[4] == nullptr);
         assertUnit(d.data[5] == nullptr);
         assertUnit(d.data[6] == nullptr);
      }
      // teardown
      teardownStandardFixture(d);
   }

   /***************************************
    * POP BACK
    ***************************************/
   
    // remove one element from a standard fixture
   void test_popback_standard()
   {  // setup
      //         iaFront
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy::reset();
      // exercise
      d.pop_back();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 67
      assertUnit(Spy::numDestructor() == 1);    // destroy 67
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //             iaFront
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 |    |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(d.numElements == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[2] != nullptr);
         if (d.data[2])
         {
            d.alloc.construct(&d.data[2][1], Spy(67));
            d.numElements++;
         }
      }
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // go from an unwrapped state to a wrapped state
   void test_popback_unwrap()
   {  // setup
      //              iaFront
      //       0     1    2    3
      //     +----+----+----+----+ 
      //     |    |    | 11 | 28 | 
      //     +----+----+----+----+ 
      //               |  
      //            +----+
      //            |    |
      //            +----+
      custom::deque<Spy> d;
      d.numCells = 4;
      d.numElements = 2;
      d.numBlocks = 1;
      d.data = new Spy * [1];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[0][2], Spy(11));
      d.alloc.construct(&d.data[0][3], Spy(28));
      Spy* pBlock = d.data[0];
      d.iaFront = 2;
      Spy::reset();
      // exercise
      d.pop_back();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 28
      assertUnit(Spy::numDestructor() == 1);    // destroy 28
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //               iaFront
      //       0     1    2    3
      //     +----+----+----+----+ 
      //     |    |    | 11 |    | 
      //     +----+----+----+----+ 
      //               |  
      //            +----+
      //            |    |
      //            +----+
      assertUnit(d.numElements == 1);
      assertUnit(d.iaFront == 2);
      assertUnit(d.numBlocks == 1);
      assertUnit(d.numCells == 4);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == pBlock);
         if (d.data[0])
         {
            assertUnit(d.data[0][2] == Spy(11));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }
   // remove last element in a block which is the last block
   void test_popback_lastElement()
   {  // setup
      //    iaFront
      //       0     1    2    3
      //     +----+----+----+----+ 
      //     |    | 31 |    |    | 
      //     +----+----+----+----+ 
      //               |  
      //            +----+
      //            |    |
      //            +----+
      custom::deque<Spy> d;
      d.numCells = 4;
      d.numElements = 1;
      d.numBlocks = 1;
      d.data = new Spy * [1];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[0][1], Spy(31));
      d.iaFront = 1;
      Spy::reset();
      // exercise
      d.pop_back();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 31
      assertUnit(Spy::numDestructor() == 1);    // destroy 31
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //               iaFront
      //            +----+
      //            | // |
      //            +----+
      assertUnit(d.numElements == 0);
      assertUnit(d.numBlocks == 1);
      assertUnit(d.numCells == 4);
      assertUnit(d.data != nullptr);
      if (d.data)
         assertUnit(d.data[0] == nullptr);
      // teardown
      teardownStandardFixture(d);
   }

   // remove last element from a block, but there are other blocks
   void test_popback_lastInBlock()
   {  // setup
      //         iaFront
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 |    |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      d.alloc.destroy(&d.data[2][1]);
      d.numElements--;
      Spy::reset();
      // exercise
      d.pop_back();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 55
      assertUnit(Spy::numDestructor() == 1);    // destroy 55
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //         iaFront
      //      0     1    2    
      //    +----+----+----+  
      //    |    | 31 | 49 |  
      //    +----+----+----+  
      //               \      
      //          +----+----+----+----+
      //          | // |    | // | // |
      //          +----+----+----+----+
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == 4);
      assertUnit(d.numBlocks == 4);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == nullptr);
         assertUnit(d.data[1] != nullptr);
         if (d.data[1])
         {
            assertUnit(d.data[1][1] == Spy(31));
            assertUnit(d.data[1][2] == Spy(49));
         }
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] == nullptr);
      }
      // teardown
      teardownStandardFixture(d);
   }

   // pretty much the worst case scenario
   void test_popback_complex()
   {  // setup
      //                            iaFront
      //   +----+----+----+    +----+----+----+
      //   | 79 |    |    |    |    | 59 | 67 |
      //   +----+----+----+    +----+----+----+
      //     |                             |
      //   +----+----+----+----+----+----+----+
      //   |    | // | // | // | // | // |    |
      //   +----+----+----+----+----+----+----+
      custom::deque<Spy> d;
      d.numCells = 3;
      d.numElements = 3;
      d.numBlocks = 7;
      d.data = new Spy * [7];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = nullptr;
      d.data[2] = nullptr;
      d.data[3] = nullptr;
      d.data[4] = nullptr;
      d.data[5] = nullptr;
      d.data[6] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[6][1], Spy(59));
      d.alloc.construct(&d.data[6][2], Spy(67));
      d.alloc.construct(&d.data[0][0], Spy(79));
      d.iaFront = 19;
      Spy* pBlock = d.data[6];
      Spy::reset();
      // exercise
      d.pop_back();
      // verify
      assertUnit(Spy::numDelete() == 1);        // delete 79
      assertUnit(Spy::numDestructor() == 1);    // destroy 79
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //   iaFront
      //                            iaFront
      //                       +----+----+----+
      //                       |    | 59 | 67 |
      //                       +----+----+----+
      //                                   |
      //   +----+----+----+----+----+----+----+
      //   | // | // | // | // | // | // |    |
      //   +----+----+----+----+----+----+----+
      assertUnit(d.numElements == 2);
      assertUnit(d.iaFront == 19);
      assertUnit(d.numBlocks == 7);
      assertUnit(d.numCells == 3);
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[0] == nullptr);
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] == nullptr);
         assertUnit(d.data[4] == nullptr);
         assertUnit(d.data[5] == nullptr);
         assertUnit(d.data[6] == pBlock);
         if (d.data[6])
         {
            assertUnit(d.data[6][1] == Spy(59));
            assertUnit(d.data[6][2] == Spy(67));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }


   /***************************************
    * BACK
    ***************************************/
  
   // read an element from the back of the standard fixture
   void test_back_readStandard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy s(99);
      Spy::reset();
      // exercise
      s = d.back();
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign 67
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy(67) == s);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // read an element from the back of a deque that is wrapped
   void test_back_readWrapped()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 55 | 67 |    |  |    | 31 | 49 |
      //    +----+----+----+  +----+----+----+
      //          \                  /
      //          +----+----+----+----+
      //          |    | // | // |    |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      d.numBlocks = 4;
      d.numCells = 3;
      d.numElements = 4;
      d.iaFront = 10;
      d.data = new Spy * [d.numBlocks];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = nullptr;
      d.data[2] = nullptr;
      d.data[3] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[3][1], Spy(31));
      d.alloc.construct(&d.data[3][2], Spy(49));
      d.alloc.construct(&d.data[0][0], Spy(55));
      d.alloc.construct(&d.data[0][1], Spy(67));
      Spy s(99);
      Spy::reset();
      // exercise
      s = d.back();
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign 67
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy(67) == s);
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 55 | 67 |    |  |    | 31 | 49 |
      //    +----+----+----+  +----+----+----+
      //          \                  /
      //          +----+----+----+----+
      //          |    | // | // |    |
      //          +----+----+----+----+
      assertUnit(d.numBlocks == 4);
      assertUnit(d.numCells == 3);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == 10);
      assertUnit(d.data != nullptr);

      if (d.data)
      {
         assertUnit(d.data[0] != nullptr);
         if (d.data[0])
         {
            assertUnit(d.data[0][0] == Spy(55));
            assertUnit(d.data[0][1] == Spy(67));
         }
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] != nullptr);
         if (d.data[3])
         {
            assertUnit(d.data[3][1] == Spy(31));
            assertUnit(d.data[3][2] == Spy(49));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }

   // write an element to the back of the standard deque
   void test_back_writeStandard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy s(99);
      Spy::reset();
      // exercise
      d.back() = s;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign 99
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 99 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[2] != nullptr);
         if (d.data[2])
         {
            assertUnit(d.data[2][1] == Spy(99));
            d.data[2][1] = Spy(67);
         }
      }
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // write an element to the back of a deque that is wrapped
   void test_back_writeWrapped()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 55 | 67 |    |  |    | 31 | 49 |
      //    +----+----+----+  +----+----+----+
      //          \                  /
      //          +----+----+----+----+
      //          |    | // | // |    |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      d.numBlocks = 4;
      d.numCells = 3;
      d.numElements = 4;
      d.iaFront = 10;
      d.data = new Spy * [d.numBlocks];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = nullptr;
      d.data[2] = nullptr;
      d.data[3] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[3][1], Spy(31));
      d.alloc.construct(&d.data[3][2], Spy(49));
      d.alloc.construct(&d.data[0][0], Spy(55));
      d.alloc.construct(&d.data[0][1], Spy(67));
      Spy s(99);
      Spy::reset();
      // exercise
      d.back() = s;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign 99
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 55 | 99 |    |  |    | 31 | 49 |
      //    +----+----+----+  +----+----+----+
      //          \                  /
      //          +----+----+----+----+
      //          |    | // | // |    |
      //          +----+----+----+----+
      assertUnit(d.numBlocks == 4);
      assertUnit(d.numCells == 3);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == 10);
      assertUnit(d.data != nullptr);

      if (d.data)
      {
         assertUnit(d.data[0] != nullptr);
         if (d.data[0])
         {
            assertUnit(d.data[0][0] == Spy(55));
            assertUnit(d.data[0][1] == Spy(99));
         }
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] != nullptr);
         if (d.data[3])
         {
            assertUnit(d.data[3][1] == Spy(31));
            assertUnit(d.data[3][2] == Spy(49));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }
   
   /***************************************
    * FRONT
    ***************************************/
   

   // read an element from the front of a standard deque
   void test_front_readStandard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy s(99);
      Spy::reset();
      // exercise
      s = d.front();
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign 31
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy(31) == s);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // write an element to the front of the standard fixture
   void test_front_writeStandard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy s(99);
      Spy::reset();
      // exercise
      d.front() = s;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign 99
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 99 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[1] != nullptr);
         if (d.data[1])
         {
            assertUnit(d.data[1][1] == Spy(99));
            d.data[1][1] = Spy(31);
         }
      }
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   /***************************************
    * SUBSCRIPT
    ***************************************/

   // read from a deque using the subscript operator
   void test_subscript_readStandard()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy s0(99);
      Spy s1(99);
      Spy s2(99);
      Spy s3(99);
      Spy::reset();
      // exercise
      s0 = d[0];
      s1 = d[1];
      s2 = d[2];
      s3 = d[3];
      // verify
      assertUnit(Spy::numAssign() == 4);      // assign 31, 49, 55, 67
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(s0 == Spy(31));
      assertUnit(s1 == Spy(49));
      assertUnit(s2 == Spy(55));
      assertUnit(s3 == Spy(67));
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   void test_subscript_readWrapped()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 55 | 67 |    |  |    | 31 | 49 |
      //    +----+----+----+  +----+----+----+
      //          \                  /
      //          +----+----+----+----+
      //          |    | // | // |    |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      d.numBlocks = 4;
      d.numCells = 3;
      d.numElements = 4;
      d.iaFront = 10;
      d.data = new Spy * [d.numBlocks];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = nullptr;
      d.data[2] = nullptr;
      d.data[3] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[3][1], Spy(31));
      d.alloc.construct(&d.data[3][2], Spy(49));
      d.alloc.construct(&d.data[0][0], Spy(55));
      d.alloc.construct(&d.data[0][1], Spy(67));
      Spy s0(99);
      Spy s1(99);
      Spy s2(99);
      Spy s3(99);
      Spy::reset();
      // exercise
      s0 = d[0];
      s1 = d[1];
      s2 = d[2];
      s3 = d[3];
      // verify
      assertUnit(Spy::numAssign() == 4);      // assign 31, 49, 55, 67
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 55 | 99 |    |  |    | 31 | 49 |
      //    +----+----+----+  +----+----+----+
      //          \                  /
      //          +----+----+----+----+
      //          |    | // | // |    |
      //          +----+----+----+----+
      assertUnit(d.numBlocks == 4);
      assertUnit(d.numCells == 3);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == 10);
      assertUnit(d.data != nullptr);

      if (d.data)
      {
         assertUnit(d.data[0] != nullptr);
         if (d.data[0])
         {
            assertUnit(d.data[0][0] == Spy(55));
            assertUnit(d.data[0][1] == Spy(67));
         }
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] != nullptr);
         if (d.data[3])
         {
            assertUnit(d.data[3][1] == Spy(31));
            assertUnit(d.data[3][2] == Spy(49));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }
   void test_subscript_writeStandard()
   {
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy s0(10);
      Spy s1(11);
      Spy s2(12);
      Spy s3(13);
      Spy::reset();
      // exercise
      d[0] = s0;
      d[1] = s1;
      d[2] = s2;
      d[3] = s3;
      // verify
      assertUnit(Spy::numAssign() == 4);      // assign 10, 11, 12, 13
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    |    | 10 | 11 |  | 12 | 13 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(d.numBlocks == 4);
      assertUnit(d.numCells == 3);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == 4);
      assertUnit(d.data != nullptr);

      if (d.data)
      {
         assertUnit(d.data[0] == nullptr);
         assertUnit(d.data[1] != nullptr);
         if (d.data[1])
         {
            assertUnit(d.data[1][1] == Spy(10));
            assertUnit(d.data[1][2] == Spy(11));
         }
         assertUnit(d.data[2] != nullptr);
         if (d.data[2])
         {
            assertUnit(d.data[2][0] == Spy(12));
            assertUnit(d.data[2][1] == Spy(13));
         }
         assertUnit(d.data[3] == nullptr);
      }      // teardown
      teardownStandardFixture(d);
   }

   void test_subscript_writeWrapped()
   {  // setup
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 55 | 67 |    |  |    | 31 | 49 |
      //    +----+----+----+  +----+----+----+
      //          \                  /
      //          +----+----+----+----+
      //          |    | // | // |    |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      d.numBlocks = 4;
      d.numCells = 3;
      d.numElements = 4;
      d.iaFront = 10;
      d.data = new Spy * [d.numBlocks];
      d.data[0] = d.alloc.allocate(d.numCells);
      d.data[1] = nullptr;
      d.data[2] = nullptr;
      d.data[3] = d.alloc.allocate(d.numCells);
      d.alloc.construct(&d.data[3][1], Spy(31));
      d.alloc.construct(&d.data[3][2], Spy(49));
      d.alloc.construct(&d.data[0][0], Spy(55));
      d.alloc.construct(&d.data[0][1], Spy(67));
      Spy s0(10);
      Spy s1(11);
      Spy s2(12);
      Spy s3(13);
      Spy::reset();
      // exercise
      d[0] = s0;
      d[1] = s1;
      d[2] = s2;
      d[3] = s3;
      // verify
      assertUnit(Spy::numAssign() == 4);      // assign 10, 11, 12, 13
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0     1    2       0    1    2
      //    +----+----+----+  +----+----+----+
      //    | 12 | 13 |    |  |    | 10 | 11 |
      //    +----+----+----+  +----+----+----+
      //          \                  /
      //          +----+----+----+----+
      //          |    | // | // |    |
      //          +----+----+----+----+
      assertUnit(d.numBlocks == 4);
      assertUnit(d.numCells == 3);
      assertUnit(d.numElements == 4);
      assertUnit(d.iaFront == 10);
      assertUnit(d.data != nullptr);

      if (d.data)
      {
         assertUnit(d.data[0] != nullptr);
         if (d.data[0])
         {
            assertUnit(d.data[0][0] == Spy(12));
            assertUnit(d.data[0][1] == Spy(13));
         }
         assertUnit(d.data[1] == nullptr);
         assertUnit(d.data[2] == nullptr);
         assertUnit(d.data[3] != nullptr);
         if (d.data[3])
         {
            assertUnit(d.data[3][1] == Spy(10));
            assertUnit(d.data[3][2] == Spy(11));
         }
      }
      // teardown
      teardownStandardFixture(d);
   }


   /***************************************
    * ITERATOR
    ***************************************/

   // test the iterator accessing the beginning of an empty deque
   void test_iterator_begin_empty()
   {  // setup
      custom::deque<Spy> d;
      custom::deque<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = d.begin();
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.d == &d);
      assertUnit(it.id == 0);
      assertUnit(it == d.end());
      assertEmptyFixture(d);
   }  // teardown

   // test the iterator at the beginning of the standard fixture
   void test_iterator_begin_standard()
   {  // setup
      custom::deque<Spy>::iterator it;
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy::reset();
      // exercise
      it = d.begin();
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.id == 0);
      assertUnit(it.d == &d);
      //           it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // test the iterator at the end of the standard fixture
   void test_iterator_end_standard()
   {  // setup
      custom::deque<Spy>::iterator it;
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      Spy::reset();
      // exercise
      it = d.end();
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                                  it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(it.id == 4);
      assertUnit(it.d == &d);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // test the iterator to increment from the middle of the standard fixture
   void test_iterator_increment_standardMiddle()
   {  // setup
      custom::deque<Spy>::iterator it;
      //                it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      it.d = &d;
      it.id = 1;
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                        it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(it.id == 2);
      assertUnit(it.d == &d);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // the the iterator's dereference operator to access an item from the list
   void test_iterator_dereference_read()
   {  // setup
      custom::deque<Spy>::iterator it;
      //                it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      it.d = &d;
      it.id = 1;
      Spy s(99);
      Spy::reset();
      // exercise
      s = *it;
      // verify
      assertUnit(Spy::numAssign() == 1);     // assigned 49
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(s == Spy(49));
      assertUnit(it.id == 1);
      assertUnit(it.d == &d);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // the the iterator's dereference operator to update an item from the list
   void test_iterator_dereference_update()
   {  // setup
      custom::deque<Spy>::iterator it;
      //                it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      it.d = &d;
      it.id = 1;
      Spy s(99);
      Spy::reset();
      // exercise
      *it = s;
      // verify
      assertUnit(Spy::numAssign() == 1);     // assigned [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 99 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(d.data != nullptr);
      if (d.data)
      {
         assertUnit(d.data[1] != nullptr);
         if (d.data[1])
         {
            assertUnit(d.data[1][2] == Spy(99));
            d.data[1][2] = Spy(49);
         }
      }
      assertUnit(it.id == 1);
      assertUnit(it.d == &d);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // the the iterator's dereference operator to update an item from the list
   void test_iterator_add_withinBlock()
   {  // setup
      custom::deque<Spy>::iterator it;
      //           it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      it.d = &d;
      it.id = 0;
      Spy::reset();
      // exercise
      it += 1;
      // verify
      assertUnit(Spy::numAssign() == 0); 
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(it.id == 1);
      assertUnit((*(it.d))[it.id] == Spy(49));
      assertUnit(it.d == &d);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // the the iterator's dereference operator to update an item from the list
   void test_iterator_add_betweenBlocks()
   {  // setup
      custom::deque<Spy>::iterator it;
      //           it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      it.d = &d;
      it.id = 0;
      Spy::reset();
      // exercise
      it += 3;
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                             it
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      assertUnit(it.id == 3);
      assertUnit((*(it.d))[it.id] == Spy(67));
      assertUnit(it.d == &d);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }

   // the the iterator's dereference operator to update an item from the list
   void test_iterator_difference_standard()
   {  // setup
      custom::deque<Spy>::iterator it1;
      custom::deque<Spy>::iterator it2;
      //           it1          it2
      // id        0    1       2     3
      //    +----+----+----+  +----+----+----+
      //    |    | 31 | 49 |  | 55 | 67 |    |
      //    +----+----+----+  +----+----+----+
      //               \        /
      //          +----+----+----+----+
      //          | // |    |    | // |
      //          +----+----+----+----+
      custom::deque<Spy> d;
      setupStandardFixture(d);
      it1.d = &d;
      it1.id = 0;
      it2.d = &d;
      it2.id = 2;
      Spy::reset();
      // exercise
      size_t diff = it2 - it1;
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(diff == 2);
      assertStandardFixture(d);
      // teardown
      teardownStandardFixture(d);
   }


   /*************************************************************
    * SETUP STANDARD FIXTURE
    *    [31, 49, 55, 67]
    *
    *      0     1    2       0    1    2
    *    +----+----+----+  +----+----+----+
    *    |    | 31 | 49 |  | 55 | 67 |    |
    *    +----+----+----+  +----+----+----+
    *               \        /
    *          +----+----+----+----+
    *          | // |    |    | // |
    *          +----+----+----+----+
    *    iaFront     = 4
    *    numElements = 4
    *    numCell     = 3
    *    numBlock    = 4
    *************************************************************/
   void setupStandardFixture(custom::deque<Spy>& d)
   {
      d.numBlocks   = 4;
      d.numCells    = 3;
      d.numElements = 4;
      d.iaFront     = 4;
      d.data = new Spy * [d.numBlocks];

      d.data[0] = nullptr;
      d.data[1] = d.alloc.allocate(d.numCells);
      d.data[2] = d.alloc.allocate(d.numCells);
      d.data[3] = nullptr;

      d.alloc.construct(&d.data[1][1], Spy(31));
      d.alloc.construct(&d.data[1][2], Spy(49));
      d.alloc.construct(&d.data[2][0], Spy(55));
      d.alloc.construct(&d.data[2][1], Spy(67));
   }

   /*************************************************************
    * VERIFY EMPTY FIXTURE
    *************************************************************/
   void assertEmptyFixtureParameters(const custom::deque<Spy>& d, int line, const char* function)
   {
      assertIndirect(d.numBlocks == 0);
      assertIndirect(d.numCells == 16);
      assertIndirect(d.numElements == 0);
      assertIndirect(d.iaFront == 0);
      assertIndirect(d.data == nullptr);
   }

   /*************************************************************
    * VERIFY STANDARD FIXTURE
    *    [31, 49, 55, 67]
    *
    *      0     1    2       0    1    2
    *    +----+----+----+  +----+----+----+
    *    |    | 31 | 49 |  | 55 | 67 |    |
    *    +----+----+----+  +----+----+----+
    *               \        /
    *          +----+----+----+----+
    *          | // |    |    | // |
    *          +----+----+----+----+
    *    iaFront     = 4
    *    numElements = 4
    *    numCell     = 3
    *    numBlock    = 4
    *************************************************************/
   void assertStandardFixtureParameters(const custom::deque<Spy>& d, int line, const char* function)
   {
      assertIndirect(d.numBlocks == 4);
      assertIndirect(d.numCells == 3);
      assertIndirect(d.numElements == 4);
      assertIndirect(d.iaFront == 4);
      assertIndirect(d.data != nullptr);

      if (d.data)
      {
         assertIndirect(d.data[0] == nullptr);
         assertIndirect(d.data[1] != nullptr);
         if (d.data[1])
         {
            assertIndirect(d.data[1][1] == Spy(31));
            assertIndirect(d.data[1][2] == Spy(49));
         }
         assertIndirect(d.data[2] != nullptr);
         if (d.data[2])
         {
            assertIndirect(d.data[2][0] == Spy(55));
            assertIndirect(d.data[2][1] == Spy(67));
         }
         assertIndirect(d.data[3] == nullptr);
      }
   }

   /*************************************************************
    * TEARDOWN STANDARD FIXTURE
    *      0     1    2    3
    *    +----+----+----+----+
    *    |    |    |    |    |
    *    +----+----+----+----+
    *************************************************************/
   void teardownStandardFixture(custom::deque<Spy>& d)
   {
      if (d.data)
      {
         for (size_t id = 0; id < d.numElements; id++)
         {
            int ib = d.ibFromID((int)id);
            int ic = d.icFromID((int)id);
            if (ib != -1 && ic != -1)
               d.alloc.destroy(&d.data[ib][ic]);
         }

         if (d.numBlocks > 0 && d.data[0])
            d.alloc.deallocate(d.data[0], d.numCells);
         if (d.numBlocks > 1 && d.data[1])
            d.alloc.deallocate(d.data[1], d.numCells);
         if (d.numBlocks > 2 && d.data[2])
            d.alloc.deallocate(d.data[2], d.numCells);
         if (d.numBlocks > 3 && d.data[3])
            d.alloc.deallocate(d.data[3], d.numCells);
         if (d.numBlocks > 4 && d.data[4])
            d.alloc.deallocate(d.data[4], d.numCells);

         delete [] d.data;
      }
      d.data = nullptr;
      d.numBlocks = 0;
      d.numElements = 0;
      d.numCells = 0;
   }

};

#endif // DEBUG
