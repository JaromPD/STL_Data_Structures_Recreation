/***********************************************************************
 * Header:
 *    TEST PRIORITY QUEUE
 * Summary:
 *    Unit tests for the priority queue
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include "priority_queue.h"
#include <queue>
#include "unitTest.h"
#include "spy.h"

#include <cassert>
#include <memory>
#include <deque>
#include <vector>


class TestPQueue : public UnitTest
{

public:
   void run()
   {
      reset();

      // Construct
      test_construct_default();
      test_constructCopy_empty();
      test_constructCopy_standard();
      test_constructMove_empty();
      test_constructMove_standard();
      test_constructRange_empty();
      test_constructRange_one();
      test_constructRange_staandard();
      test_constructMoveInit_empty();
      test_constructMoveInit_one();
      test_constructMoveInit_standard();
      test_destructor_empty();
      test_destructor_standard();
      test_destructor_partiallyFilled();
      
      // Assign
      test_swap_emptyEmpty();
      test_swap_standardEmpty();
      test_swap_emptyStandard();
      test_swap_standardStandard();

      // Access
      test_top_empty();
      test_top_standard();

      // Insert
      test_push_empty();
      test_push_levelZero();
      test_push_levelOne();
      test_push_levelTwo();
      test_push_levelThree();
      test_pushMove_empty();
      test_pushMove_levelZero();
      test_pushMove_levelOne();
      test_pushMove_levelTwo();
      test_pushMove_levelThree();

      // Remove : NEEDS PERCOLATE
      test_pop_empty();
      test_pop_one();
      test_pop_two();
      test_pop_standard();

      // Status
      test_size_empty();
      test_size_standard();
      test_empty_empty();
      test_empty_standard();
      
      // Utility
      test_percolateDown_nothing();
      test_percolateDown_oneLevel();
      test_percolateDown_twoLevels();
      test_percolateDown_nothingReversed();
      test_percolateDown_oneLevelReversed();
      test_percolateDown_twoLevelsReversed();

      report("PQueue");
   }

   /***************************************
    * CONSTRUCTOR
    ***************************************/

   // default constructor, no allocations
   void test_construct_default()
   {  // setup
      Spy::reset();
      // exercise
      custom::priority_queue <int> pq;
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(pq.container.empty());
   }  // teardown


   /***************************************
    * DESTRUCTOR
    ***************************************/
   
   // destructor of an empty priority queue
   void test_destructor_empty()
   {  // setup
      {
         custom::priority_queue<Spy> pq;
         Spy::reset();
      } // exercise
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
   }  // verify
   
   // destructor of a 7-element collection
   void test_destructor_standard()
   {  // setup
      {
         //  +---+---+---+---+---+---+---+---+---+
         //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
         //  +---+---+---+---+---+---+---+---+---+
         //                10
         //          8            9
         //       4     3      7     5
         custom::priority_queue<Spy> pq;
         setupStandardFixture(pq);
         Spy::reset();
      } // exercise
      // verify
      assertUnit(Spy::numDestructor() == 7); // destructor for [10,8,9,4,3,7,5]
      assertUnit(Spy::numDelete() == 7);     // delete [10,8,9,4,3,7,5]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssign() == 0);
   }
   
   // destructor of a 3-element, 6-capacity priority queue
   void test_destructor_partiallyFilled()
   {  // setup
      {
         //  +---+---+---+---+---+---+
         //  | 10| 8 | 9 |   |   |   |
         //  +---+---+---+---+---+---+
         //                10
         //          8            9
         custom::priority_queue<Spy> pq;
         pq.container.reserve(6);
         pq.container.push_back(Spy(10));
         pq.container.push_back(Spy(8));
         pq.container.push_back(Spy(9));
         Spy::reset();
      }  // exercise
      assertUnit(Spy::numDestructor() == 3); // destructor for [10, 8, 9]
      assertUnit(Spy::numDelete() == 3);     // delete [10, 8, 9]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssign() == 0);
   }
   
   /***************************************
    * COPY CONSTRUCTOR
    ***************************************/
   
   // copy constructor of an empty vector
   void test_constructCopy_empty()
   {  // setup
      custom::priority_queue<Spy> pqSrc;
      Spy::reset();
      // exercise
      custom::priority_queue<Spy> pqDest(pqSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(pqSrc);
      assertEmptyFixture(pqDest);
   }  // teardown
   
   // copy constructor of a 4-element collection
   void test_constructCopy_standard()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue<Spy> pqSrc;
      setupStandardFixture(pqSrc);
      Spy::reset();
      // exercise
      custom::priority_queue<Spy> pqDest(pqSrc);
      // verify
      assertUnit(Spy::numCopy() == 7);     // copy constructor for [10,8,9,4,3,7,5]
      assertUnit(Spy::numAlloc() == 7);    // allocate [10,8,9,4,3,7,5]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(pqSrc.container.data != pqDest.container.data);
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      assertStandardFixture(pqSrc);
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      assertStandardFixture(pqDest);
      // teardown
      teardownStandardFixture(pqSrc);
      teardownStandardFixture(pqDest);
   }
   
   /***************************************
    * MOVE CONSTRUCTOR
    ***************************************/
   
   // move constructor of an empty container
   void test_constructMove_empty()
   {  // setup
      custom::priority_queue<Spy> pqSrc;
      Spy::reset();
      // exercise
      custom::priority_queue<Spy> pqDest(std::move(pqSrc));
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(pqSrc);
      assertEmptyFixture(pqDest);
   }  // teardown
   
   // move constructor of a standard fixture
   void test_constructMove_standard()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue<Spy> pqSrc;
      setupStandardFixture(pqSrc);
      Spy::reset();
      // exercise
      custom::priority_queue<Spy> pqDest(std::move(pqSrc));
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(pqSrc);
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      assertStandardFixture(pqDest);
      // teardown
      teardownStandardFixture(pqDest);
   }
   
   /***************************************
    * RANGE CONSTRUCTOR
    ***************************************/
   
   // priority_queue({})
   void test_constructRange_empty()
   {  // setup
      std::initializer_list<Spy> il;
      Spy::reset();
      // exercise
      custom::priority_queue<Spy> pq(il.begin(), il.end());
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(pq);
      // teardown
      teardownStandardFixture(pq);
   }
   
   // priority_queue({50})
   void test_constructRange_one()
   {  // setup
      //   il = {50}
      std::initializer_list<Spy> il{Spy(50)};
      Spy::reset();
      // exercise
      custom::priority_queue<Spy> pq(il.begin(), il.end());
      // verify
      assertUnit(Spy::numCopy() == 1);     // copy [50]
      assertUnit(Spy::numAlloc() == 1);    // allocate [50]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(pq.container.size() == 1);
      assertUnit(pq.container.capacity() == 1);
      if (pq.container.size() == 1)
         assertUnit(pq.container[0] == Spy(50));
      // teardown
      teardownStandardFixture(pq);
   }

   // priority_queue({10, 9, 8})
   void test_constructRange_staandard()
   {  // setup
      //  il = {10, 9, 8}
      std::initializer_list<Spy> il{Spy(10), Spy(9), Spy(8)};
      Spy::reset();
      // exercise
      custom::priority_queue<Spy> pq(il.begin(), il.end());
      // verify
      assertUnit(Spy::numCopy() == 3);     // copy [10][9][8]
      assertUnit(Spy::numAlloc() == 3);    // allocate [10][9][8]
      assertUnit(Spy::numLessthan() == 3); // [9<10] [9<8][9<10]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //  +---+---+---+
      //  | 10| 9 | 8 |
      //  +---+---+---+
      //                10
      //          9            8
      assertUnit(pq.container.size() == 3);
      assertUnit(pq.container.capacity() == 3);
      if (pq.container.size() == 3)
      {
         assertUnit(pq.container[0] == Spy(10));
         assertUnit(pq.container[1] == Spy(9));
         assertUnit(pq.container[2] == Spy(8));
      }
      // teardown
      teardownStandardFixture(pq);
   }
   
   /***************************************
    * MOVE CONTAINER INITIALIZE CONSTRUCTOR
    ***************************************/
   
   // priority_queue(<, {})
   void test_constructMoveInit_empty()
   {  // setup
      custom::vector <Spy> v;
      Spy::reset();
      // exercise
      custom::priority_queue <Spy> pq(std::less<Spy>(), std::move(v));
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(pq);
      assertUnit(v.size() == 0);
      // teardown
      teardownStandardFixture(pq);
   }
   
   // priority_queue(<, {50})
   void test_constructMoveInit_one()
   {  // setup
      //   v = [50]
      custom::vector <Spy> v{Spy(50)};
      Spy::reset();
      // exercise
      custom::priority_queue <Spy> pq(std::less<Spy>(), v);
      // verify
      assertUnit(Spy::numCopy() == 1);     // copy [50]
      assertUnit(Spy::numAlloc() == 1);    // allocate [50]
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //  +----+
      //  | 50 |
      //  +----+
      assertUnit(pq.container.size() == 1);
      if (pq.container.size() == 1)
         assertUnit(pq.container[0] == Spy(50));
      assertUnit(v.size() == 1);
      if (v.size() == 1)
         assertUnit(v[0] == Spy(50));
      // teardown
      teardownStandardFixture(pq);
   }
   
   // priority_queue(<, {9, 10, 8})
   void test_constructMoveInit_standard()
   {  // setup
      //   v = [9,10,8]
      custom::vector <Spy> v{Spy(9), Spy(10), Spy(8)};
      Spy::reset();
      // exercise
      custom::priority_queue <Spy> pq(std::less<Spy>(), v);
      // verify
      assertUnit(Spy::numCopy() == 3);     // copy [10][9][8]
      assertUnit(Spy::numAlloc() == 3);    // allocate [10][9][8]
      assertUnit(Spy::numLessthan() == 3); // [9<10] [9<8][9<10]
      assertUnit(Spy::numSwap() == 1);     // [9 - 10]
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //  +---+---+---+
      //  | 10| 9 | 8 |
      //  +---+---+---+
      //                10
      //          9            8
      assertUnit(pq.container.size() == 3);
      assertUnit(pq.container.capacity() == 3);
      if (pq.container.size() == 3)
      {
         assertUnit(pq.container[0] == Spy(10));
         assertUnit(pq.container[1] == Spy(9));
         assertUnit(pq.container[2] == Spy(8));
      }
      assertUnit(v.size() == 3);
      if (v.size() == 3)
      {
         assertUnit(v[0] == Spy(9));
         assertUnit(v[1] == Spy(10));
         assertUnit(v[2] == Spy(8));
      }
      // teardown
      teardownStandardFixture(pq);
   }
   
   /***************************************
    * SIZE EMPTY
    ***************************************/

   // size of a priority queue that is empty
   void test_size_empty()
   {  // setup
      custom::priority_queue <Spy> pq;
      Spy::reset();
      // exercise
      size_t size = pq.size();
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(size == 0);
      assertEmptyFixture(pq);
   }  // teardown

   // size of a priority queue that is standard
   void test_size_standard()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy::reset();
      // exercise
      size_t size = pq.size();
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(size == 7);
      assertStandardFixture(pq);
      // teardown
      teardownStandardFixture(pq);
   }

   // whether an empty priority queue is empty
   void test_empty_empty()
   {  // setup
      custom::priority_queue <Spy> pq;
      Spy::reset();
      // exercise
      bool empty = pq.empty();
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(empty == true);
      assertEmptyFixture(pq);
   }  // teardown

   // whether a standard priority queue is empty
   void test_empty_standard()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy::reset();
      // exercise
      bool empty = pq.empty();
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(empty == false);
      assertStandardFixture(pq);
      // teardown
      teardownStandardFixture(pq);
   }

   /***************************************
    * SWAP
    ***************************************/
   
   // swap(empty, empty)
   void test_swap_emptyEmpty()
   {  // setup
      custom::priority_queue <Spy> pqLHS;
      custom::priority_queue <Spy> pqRHS;
      Spy::reset();
      // exercise
      swap(pqLHS, pqRHS);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(pqLHS);
      assertEmptyFixture(pqRHS);
      // teardown
      teardownStandardFixture(pqLHS);
      teardownStandardFixture(pqRHS);
   }
   
   // swap(standard, empty)
   void test_swap_standardEmpty()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   | = pqLHS
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pqLHS;
      setupStandardFixture(pqLHS);
      custom::priority_queue <Spy> pqRHS;
      Spy::reset();
      // exercise
      swap(pqLHS, pqRHS);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   | = pqRHS
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      assertStandardFixture(pqRHS);
      assertEmptyFixture(pqLHS);
      // teardown
      teardownStandardFixture(pqLHS);
      teardownStandardFixture(pqRHS);
   }
      
   // swap(empty, standard
   void test_swap_emptyStandard()
   {  // setup
      custom::priority_queue <Spy> pqLHS;
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   | = pqRHS
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pqRHS;
      setupStandardFixture(pqRHS);
      Spy::reset();
      // exercise
      swap(pqLHS, pqRHS);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   | = pqLHS
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      assertStandardFixture(pqLHS);
      assertEmptyFixture(pqRHS);
      // teardown
      teardownStandardFixture(pqLHS);
      teardownStandardFixture(pqRHS);
   }
   
   // swap(standard, stuff)
   void test_swap_standardStandard()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   | = pqLHS
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pqLHS;
      setupStandardFixture(pqLHS);
      custom::priority_queue <Spy> pqRHS;
      //  +----+----+----+
      //  | 99 | 88 | 77 | = pqRHS
      //  +----+----+----+
      //                99
      //          88            77
      pqRHS.container = {Spy(99), Spy(88), Spy(77)};
      Spy::reset();
      // exercise
      swap(pqLHS, pqRHS);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   | = pqRHS
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      assertStandardFixture(pqRHS);
      //  +----+----+----+
      //  | 99 | 88 | 77 | = pqLHS
      //  +----+----+----+
      //                99
      //          88            77
      assertUnit(pqLHS.container.size() == 3);
      if (pqLHS.container.size() == 3)
      {
         assertUnit(pqLHS.container[0] == Spy(99));
         assertUnit(pqLHS.container[1] == Spy(88));
         assertUnit(pqLHS.container[2] == Spy(77));
      }
      // teardown
      teardownStandardFixture(pqLHS);
      teardownStandardFixture(pqRHS);
   }
   
   
   /***************************************
    * PERCOLATE
    ***************************************/

   // test percolate down with no change
   void test_percolateDown_nothing()
   {  // setup
      //    0   1   2   3   4   5   6
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 7 | 4 | 3 | 9 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //               10
      //         8            7
      //      4     3      9     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy::reset();
      // Exercise
      bool returnValue = pq.percolateDown(1 /*indexHeap*/);
      // Verify
      assertUnit(Spy::numLessthan() == 2);    // compare [8<7][8<10]
      assertUnit(Spy::numSwap() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    1   2   3   4   5   6   7   8   9
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //               10
      //         8            9
      //      4     3      7     5
      assertUnit(returnValue == false);
      assertStandardFixture(pq);
      // Teardown
      teardownStandardFixture(pq);
   }
   
   // test percolate down on a one-level change
   void test_percolateDown_oneLevel()
   {  // setup
      //    0   1   2   3   4   5   6
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 7 | 4 | 3 | 9 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //               10
      //         8            7
      //      4     3      9     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      pq.container[3-1] = Spy(7);
      pq.container[6-1] = Spy(9);
      Spy::reset();
      // Exercise
      bool returnValue = pq.percolateDown(3 /*indexHeap*/);
      // Verify
      assertUnit(Spy::numLessthan() == 2);    // compare [9<5][9<7]
      assertUnit(Spy::numSwap() == 1);        // swap of [7,9]
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      //    1   2   3   4   5   6   7   8   9
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //               10
      //         8            9
      //      4     3      7     5
      assertUnit(returnValue == true);
      assertStandardFixture(pq);
      // Teardown
      teardownStandardFixture(pq);
   }

   // test percolate down on a one-level change
   void test_percolateDown_twoLevels()
   {  // setup
      //    1   2   3   4   5   6   7   8   9
      //  +---+---+---+---+---+---+---+---+---+
      //  | 5 | 8 | 10| 4 | 3 | 7 | 9 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //               5
      //         8            10
      //      4     3      7     9
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      pq.container[1-1] = Spy(5);
      pq.container[3-1] = Spy(10);
      pq.container[7-1] = Spy(9);
      Spy::reset();
      // Exercise
      bool returnValue = pq.percolateDown(1 /*indexHeap*/);
      // Verify
      assertUnit(Spy::numLessthan() == 4);    // compare [8<10][5<10] [7<9][9<5]
      assertUnit(Spy::numSwap() == 2);        // swap of [10,5] [5,9]
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      //    1   2   3   4   5   6   7   8   9
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //               10
      //         8            9
      //      4     3      7     5
      assertUnit(returnValue == true);
      assertStandardFixture(pq);
      // Teardown
      teardownStandardFixture(pq);
   }

   // test percolate down reversed when there is no change
   void test_percolateDown_nothingReversed()
   {  // setup
      //    0   1   2   3   4   5   6
      //  +---+---+---+---+---+---+---+
      //  | 1 | 5 | 2 | 7 | 6 | 4 | 3 |
      //  +---+---+---+---+---+---+---+
      //                1
      //         5           2
      //      7     6      4     3
      custom::priority_queue <int, custom::vector<int>, std::greater<int>> pq;
      pq.container = {1,5,2,7,6,4,3};
      Spy::reset();
      // Exercise
      bool returnValue = pq.percolateDown(1 /*indexHeap*/);
      // Verify
      //    0   1   2   3   4   5   6
      //  +---+---+---+---+---+---+---+
      //  | 1 | 5 | 2 | 7 | 6 | 4 | 3 |
      //  +---+---+---+---+---+---+---+
      assertUnit(returnValue == false);
      assertUnit(pq.container.size() == 7);
      if (pq.container.size() == 7)
      {
         assertUnit(pq.container[0] == 1);
         assertUnit(pq.container[1] == 5);
         assertUnit(pq.container[2] == 2);
         assertUnit(pq.container[3] == 7);
         assertUnit(pq.container[4] == 6);
         assertUnit(pq.container[5] == 4);
         assertUnit(pq.container[6] == 3);
      }
      // Teardown
      pq.container.clear();
   }
   
   // test percolate down on a one-level change
   void test_percolateDown_oneLevelReversed()
   {  // setup
      //    0   1   2   3   4   5   6
      //  +---+---+---+---+---+---+---+
      //  | 2 | 5 | 1 | 7 | 6 | 4 | 3 |
      //  +---+---+---+---+---+---+---+
      //                2
      //         5           1
      //      7     6      4     3
      custom::priority_queue <int, custom::vector<int>, std::greater<int>> pq;
      pq.container = {2,5,1,7,6,4,3};
      Spy::reset();
      // Exercise
      bool returnValue = pq.percolateDown(1 /*indexHeap*/);
      // Verify
      //    0   1   2   3   4   5   6
      //  +---+---+---+---+---+---+---+
      //  | 1 | 5 | 2 | 7 | 6 | 4 | 3 |
      //  +---+---+---+---+---+---+---+
      assertUnit(returnValue == true);
      assertUnit(pq.container.size() == 7);
      if (pq.container.size() == 7)
      {
         assertUnit(pq.container[0] == 1);
         assertUnit(pq.container[1] == 5);
         assertUnit(pq.container[2] == 2);
         assertUnit(pq.container[3] == 7);
         assertUnit(pq.container[4] == 6);
         assertUnit(pq.container[5] == 4);
         assertUnit(pq.container[6] == 3);
      }
      // Teardown
      pq.container.clear();
   }
   
   // test percolate down on a one-level change
   void test_percolateDown_twoLevelsReversed()
   {  // setup
      //    0   1   2   3   4   5   6
      //  +---+---+---+---+---+---+---+
      //  | 3 | 5 | 1 | 7 | 6 | 4 | 2 |
      //  +---+---+---+---+---+---+---+
      //                3
      //         5           1
      //      7     6      4     2
      custom::priority_queue <int, custom::vector<int>, std::greater<int>> pq;
      pq.container = {3,5,1,7,6,4,2};
      Spy::reset();
      // Exercise
      bool returnValue = pq.percolateDown(1 /*indexHeap*/);
      // Verify
      //    0   1   2   3   4   5   6
      //  +---+---+---+---+---+---+---+
      //  | 1 | 5 | 2 | 7 | 6 | 4 | 3 |
      //  +---+---+---+---+---+---+---+
      assertUnit(returnValue == true);
      assertUnit(pq.container.size() == 7);
      if (pq.container.size() == 7)
      {
         assertUnit(pq.container[0] == 1);
         assertUnit(pq.container[1] == 5);
         assertUnit(pq.container[2] == 2);
         assertUnit(pq.container[3] == 7);
         assertUnit(pq.container[4] == 6);
         assertUnit(pq.container[5] == 4);
         assertUnit(pq.container[6] == 3);
      }
      // Teardown
      pq.container.clear();
   }

   /***************************************
    * TOP
    ***************************************/

   // top of an empty priority queue
   void test_top_empty()
   {  // setup
      custom::priority_queue <Spy> pq;
      Spy value(99);
      Spy::reset();
      // exercise
      try
      {
         value = pq.top();
         // verify
         assertUnit(false);
      }
      catch (const std::out_of_range & error)
      {
         assertUnit(error.what() == std::string("std:out_of_range"));
      }
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(value == Spy(99));
      assertEmptyFixture(pq);
   }  // teardown

   // top of the standard fixture
   void test_top_standard()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy value(99);
      Spy::reset();
      // exercise
      value = pq.top();
      // verify
      assertUnit(Spy::numAssign() == 1);       // assign-copy [10]
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(value == Spy(10));
      assertStandardFixture(pq);
      // teardown
      teardownStandardFixture(pq);
   }  

  /***************************************
   * POP
   ***************************************/

   // pop an empty priority queue
   void test_pop_empty()
   {  // setup
      custom::priority_queue <Spy> pq;
      Spy::reset();
      // exercise
      pq.pop();
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertEmptyFixture(pq);
   }  // teardown

   // pop a priority queue with a single element
   void test_pop_one()
   {  // setup
      //  +----+
      //  | 99 |
      //  +----+
      custom::priority_queue <Spy> pq;
      pq.container.push_back(Spy(99));
      Spy::reset();
      // exercise
      pq.pop();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [99]
      assertUnit(Spy::numDelete() == 1);       // delete [99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numEquals() == 0);
      //  +----+
      //  |    |
      //  +----+
      assertEmptyFixture(pq);
      // teardown
   }

   // pop a priority queue with two elements
   void test_pop_two()
   {  // setup
      //               10
      //         8
      //  +---+---+
      //  | 10| 8 |
      //  +---+---+
      custom::priority_queue <Spy> pq;
      pq.container.push_back(Spy(10));
      pq.container.push_back(Spy(8));
      Spy::reset();
      // exercise
      pq.pop();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [10]
      assertUnit(Spy::numDelete() == 1);       // delete  [10]
      assertUnit(Spy::numSwap() == 1);         // swap [10,8]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numEquals() == 0);
      //  +---+
      //  | 8 |
      //  +---+
      assertUnit(pq.container.size() == 1);
      if (pq.container.size() == 1)
         assertUnit(pq.container[0] == Spy(8));
      // teardown
      teardownStandardFixture(pq);
   }

   // pop a priority queue with three elements
   void test_pop_three()
   {  // setup
      //               10
      //         8            9
      //  +---+---+---+
      //  | 10| 8 | 9 |
      //  +---+---+---+
      custom::priority_queue <Spy> pq;
      pq.container.push_back(Spy(10));
      pq.container.push_back(Spy(8));
      pq.container.push_back(Spy(9));
      Spy::reset();
      // exercise
      pq.pop();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [10]
      assertUnit(Spy::numDelete() == 1);       // delete  [10]
      assertUnit(Spy::numLessthan() == 1);     // compare [8<9]
      assertUnit(Spy::numSwap() == 1);         // swap[8,9]
      assertUnit(Spy::numAssignMove() == 1);   // assign-move [9]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numEquals() == 0);
      //               9
      //         8
      //  +---+---+
      //  | 9 | 8 |
      //  +---+---+
      assertUnit(pq.container.size() == 2);
      if (pq.container.size() == 2)
      {
         assertUnit(pq.container[0] == Spy(9));
         assertUnit(pq.container[1] == Spy(8));
      }
      // teardown
      teardownStandardFixture(pq);
   }
   
   // pop from the standard fixture
   void test_pop_standard()
   {  // setup
      //               10
      //         8            9
      //      4     3      7     5
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy::reset();
      // exercise
      pq.pop();
      // verify
      assertUnit(Spy::numSwap() == 3);         // swap [10,5] [9,5] [7,5]
      assertUnit(Spy::numDestructor() == 1);   // destroy [10]
      assertUnit(Spy::numDelete() == 1);       // delete [10]
      assertUnit(Spy::numLessthan() == 3);     // compare [8<9][9<5] [7<5]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numEquals() == 0);
      //    0   1   2   3   4   5
      //  +---+---+---+---+---+---+---+---+---+
      //  | 9 | 8 | 7 | 4 | 3 | 5 |   |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                9
      //          8            7
      //       4     3      5
      assertUnit(pq.container.size() == 6);
      if (pq.container.size() == 6)
      {
         assertUnit(pq.container[0] == Spy(9));
         assertUnit(pq.container[1] == Spy(8));
         assertUnit(pq.container[2] == Spy(7));
         assertUnit(pq.container[3] == Spy(4));
         assertUnit(pq.container[4] == Spy(3));
         assertUnit(pq.container[5] == Spy(5));
      }
      // teardown
      teardownStandardFixture(pq);
   }

   

   /***************************************
    * PUSH
    ***************************************/

   void test_push_empty()
   {  // setup
      custom::priority_queue <Spy> pq;
      Spy s(10);
      Spy::reset();
      // exercise
      pq.push(s);
      // verify
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 1);    // copy-create [10]
      assertUnit(Spy::numAlloc() == 1);   // allocate    [10]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numEquals() == 0);
      //  +----+
      //  | 10 |
      //  +----+
      assertUnit(pq.container.size() == 1);
      if (pq.container.size() == 1)
         assertUnit(pq.container[0] == Spy(10));
      // teardown
      teardownStandardFixture(pq);
   }

   // push an element to the bottom of a priority queue - level zero
   void test_push_levelZero()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy s(1);
      Spy::reset();
      // exercise
      pq.push(s);
      // verify
      assertUnit(Spy::numCopy() == 1);     // copy-create [1]
      assertUnit(Spy::numAlloc() == 1);    // allocate    [1]
      assertUnit(Spy::numLessthan() == 1); // compare [4<1]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      //    1   2   3   4   5   6   7   8
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 | 1 |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      //      1
      assertUnit(pq.container.size() == 8);
      if (pq.container.size() == 8)
      {
         assertUnit(pq.container[0] == Spy(10));
         assertUnit(pq.container[1] == Spy(8));
         assertUnit(pq.container[2] == Spy(9));
         assertUnit(pq.container[3] == Spy(4));
         assertUnit(pq.container[4] == Spy(3));
         assertUnit(pq.container[5] == Spy(7));
         assertUnit(pq.container[6] == Spy(5));
         assertUnit(pq.container[7] == Spy(1));
      }
      // teardown
      teardownStandardFixture(pq);
   }

   // push an element to a priority queue which goes up one level
   void test_push_levelOne()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy s(6);
      Spy::reset();
      // exercise
      pq.push(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy-create [6]
      assertUnit(Spy::numAlloc() == 1);      // allocate    [6]
      assertUnit(Spy::numLessthan() == 3);   // [4<6] [6<3][6<8]
      assertUnit(Spy::numSwap() == 1);       // swap [4,6]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      //    1   2   3   4   5   6   7   8
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 6 | 3 | 7 | 5 | 4 |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       6     3      7     5
      //      4
      assertUnit(pq.container.size() == 8);
      if (pq.container.size() == 8)
      {
         assertUnit(pq.container[0] == Spy(10));
         assertUnit(pq.container[1] == Spy(8));
         assertUnit(pq.container[2] == Spy(9));
         assertUnit(pq.container[3] == Spy(6));
         assertUnit(pq.container[4] == Spy(3));
         assertUnit(pq.container[5] == Spy(7));
         assertUnit(pq.container[6] == Spy(5));
         assertUnit(pq.container[7] == Spy(4));
      }
      // teardown
      teardownStandardFixture(pq);
   }   
   
   // push an element to a priorty queue that goes up two levels
   void test_push_levelTwo()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy s(9);
      Spy::reset();
      // exercise
      pq.push(s);
      // verify
      assertUnit(Spy::numCopy() == 1);      // copy-create [9]
      assertUnit(Spy::numAlloc() == 1);     // allocate    [9]
      assertUnit(Spy::numLessthan() == 6);  // compare [4<9] [9<3][9<8] [8<4] [9<9][9<10]
      assertUnit(Spy::numSwap() == 2);      // swap [4,9] [8,9]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      //    1   2   3   4   5   6   7   8
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 9 | 9 | 8 | 3 | 7 | 5 | 4 |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          9            9
      //       8     3      7     5
      //      4
      assertUnit(pq.container.size() == 8);
      if (pq.container.size() == 8)
      {
         assertUnit(pq.container[0] == Spy(10));
         assertUnit(pq.container[1] == Spy(9));
         assertUnit(pq.container[2] == Spy(9));
         assertUnit(pq.container[3] == Spy(8));
         assertUnit(pq.container[4] == Spy(3));
         assertUnit(pq.container[5] == Spy(7));
         assertUnit(pq.container[6] == Spy(5));
         assertUnit(pq.container[7] == Spy(4));
      }
      // teardown
      teardownStandardFixture(pq);
   }

   // push an element to the top of a priority queue
   void test_push_levelThree()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy s(11);
      Spy::reset();
      // exercise
      pq.push(s);
      // verify
      assertUnit(Spy::numCopy() == 1);      // copy-create [11]
      assertUnit(Spy::numAlloc() == 1);     // allocate    [11]
      assertUnit(Spy::numLessthan() == 8);  // compare [4<11] [11<3][11<8] [8<4] [11<9][11<10] [8<3] [10<8]
      assertUnit(Spy::numSwap() == 3);      // swap [4,11] [8,11] [10,11]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      //    1   2   3   4   5   6   7   8
      //  +---+---+---+---+---+---+---+---+---+
      //  | 11| 10| 9 | 8 | 3 | 7 | 5 | 4 |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                11
      //          10            9
      //       8     3      7     5
      //      4
      assertUnit(pq.container.size() == 8);
      if (pq.container.size() == 8)
      {
         assertUnit(pq.container[0] == Spy(11));
         assertUnit(pq.container[1] == Spy(10));
         assertUnit(pq.container[2] == Spy(9));
         assertUnit(pq.container[3] == Spy(8));
         assertUnit(pq.container[4] == Spy(3));
         assertUnit(pq.container[5] == Spy(7));
         assertUnit(pq.container[6] == Spy(5));
         assertUnit(pq.container[7] == Spy(4));
      }      // teardown
      teardownStandardFixture(pq);
   }

   // move push an element on an empty priority queue
   void test_pushMove_empty()
   {  // setup
      custom::priority_queue <Spy> pq;
      Spy s(10);
      Spy::reset();
      // exercise
      pq.push(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1); // move [10]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);    
      assertUnit(Spy::numAlloc() == 0); 
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numEquals() == 0);
      //  +----+
      //  | 10 |
      //  +----+
      assertUnit(pq.container.size() == 1);
      if (pq.container.size() == 1)
         assertUnit(pq.container[0] == Spy(10));
      // teardown
      teardownStandardFixture(pq);
   }

   // move push an element to the bottom of a priority queue - level zero
   void test_pushMove_levelZero()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy s(1);
      Spy::reset();
      // exercise
      pq.push(std::move(s));
      // verify
      assertUnit(Spy::numLessthan() == 1); // compare [4<1]
      assertUnit(Spy::numCopyMove() == 1);  // move [1]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);   
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      //    1   2   3   4   5   6   7   8
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 | 1 |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      //      1
      assertUnit(pq.container.size() == 8);
      if (pq.container.size() == 8)
      {
         assertUnit(pq.container[0] == Spy(10));
         assertUnit(pq.container[1] == Spy(8));
         assertUnit(pq.container[2] == Spy(9));
         assertUnit(pq.container[3] == Spy(4));
         assertUnit(pq.container[4] == Spy(3));
         assertUnit(pq.container[5] == Spy(7));
         assertUnit(pq.container[6] == Spy(5));
         assertUnit(pq.container[7] == Spy(1));
      }
      // teardown
      teardownStandardFixture(pq);
   }

   // move push an element to a priority queue which goes up one level
   void test_pushMove_levelOne()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy s(6);
      Spy::reset();
      // exercise
      pq.push(std::move(s));
      // verify
      assertUnit(Spy::numLessthan() == 3);    // [4<6] [6<3][6<8]
      assertUnit(Spy::numSwap() == 1);        // swap [4,6]
      assertUnit(Spy::numCopyMove() == 1);    // move [6]
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      //    1   2   3   4   5   6   7   8
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 6 | 3 | 7 | 5 | 4 |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       6     3      7     5
      //      4
      assertUnit(pq.container.size() == 8);
      if (pq.container.size() == 8)
      {
         assertUnit(pq.container[0] == Spy(10));
         assertUnit(pq.container[1] == Spy(8));
         assertUnit(pq.container[2] == Spy(9));
         assertUnit(pq.container[3] == Spy(6));
         assertUnit(pq.container[4] == Spy(3));
         assertUnit(pq.container[5] == Spy(7));
         assertUnit(pq.container[6] == Spy(5));
         assertUnit(pq.container[7] == Spy(4));
      }
      // teardown
      teardownStandardFixture(pq);
   }

   // move push an element to a priorty queue that goes up two levels
   void test_pushMove_levelTwo()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy s(9);
      Spy::reset();
      // exercise
      pq.push(std::move(s));
      // verify
      assertUnit(Spy::numLessthan() == 6);  // compare [4<9] [9<3][9<8] [8<4] [9<9][9<10]
      assertUnit(Spy::numSwap() == 2);      // swap [4,9] [8,9]
      assertUnit(Spy::numCopyMove() == 1);  // copy-move [9]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      //    1   2   3   4   5   6   7   8
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 9 | 9 | 8 | 3 | 7 | 5 | 4 |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          9            9
      //       8     3      7     5
      //      4
      assertUnit(pq.container.size() == 8);
      if (pq.container.size() == 8)
      {
         assertUnit(pq.container[0] == Spy(10));
         assertUnit(pq.container[1] == Spy(9));
         assertUnit(pq.container[2] == Spy(9));
         assertUnit(pq.container[3] == Spy(8));
         assertUnit(pq.container[4] == Spy(3));
         assertUnit(pq.container[5] == Spy(7));
         assertUnit(pq.container[6] == Spy(5));
         assertUnit(pq.container[7] == Spy(4));
      }
      // teardown
      teardownStandardFixture(pq);
   }

   // move push an element to the top of a priority queue
   void test_pushMove_levelThree()
   {  // setup
      //  +---+---+---+---+---+---+---+---+---+
      //  | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                10
      //          8            9
      //       4     3      7     5
      custom::priority_queue <Spy> pq;
      setupStandardFixture(pq);
      Spy s(11);
      Spy::reset();
      // exercise
      pq.push(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);  // copy-move [11]
      assertUnit(Spy::numLessthan() == 8);  // compare [4<11] [11<3][11<8] [8<4] [11<9][11<10] [8<3] [10<8]
      assertUnit(Spy::numSwap() == 3);      // swap [4,11] [8,11] [10,11]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numEquals() == 0);
      //    1   2   3   4   5   6   7   8
      //  +---+---+---+---+---+---+---+---+---+
      //  | 11| 10| 9 | 8 | 3 | 7 | 5 | 4 |   |
      //  +---+---+---+---+---+---+---+---+---+
      //                11
      //          10            9
      //       8     3      7     5
      //      4
      assertUnit(pq.container.size() == 8);
      if (pq.container.size() == 8)
      {
         assertUnit(pq.container[0] == Spy(11));
         assertUnit(pq.container[1] == Spy(10));
         assertUnit(pq.container[2] == Spy(9));
         assertUnit(pq.container[3] == Spy(8));
         assertUnit(pq.container[4] == Spy(3));
         assertUnit(pq.container[5] == Spy(7));
         assertUnit(pq.container[6] == Spy(5));
         assertUnit(pq.container[7] == Spy(4));
      }      // teardown
      teardownStandardFixture(pq);
   }

   /***************************************************
    * SETUP STANDARD FIXTURE
    *                 10
    *           8            9
    *        4     3      7     5
    * 
    *   +---+---+---+---+---+---+---+---+---+
    *   | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
    *   +---+---+---+---+---+---+---+---+---+
    ***************************************************/
   void setupStandardFixture(custom::priority_queue <Spy>& pq)
   {
      pq.container = {Spy(10), Spy(8), Spy(9), Spy(4), Spy(3), Spy(7), Spy(5)};
      pq.container.reserve(9);
   }
   void setupStandardFixture(std::priority_queue <Spy>& pq)
   {
      std::initializer_list<Spy> il{Spy(10), Spy(8), Spy(9), Spy(4), Spy(3), Spy(7), Spy(5)};
      std::priority_queue <Spy> pqTemp(il.begin(), il.end());
      pq = std::move(pqTemp);
   }
   
   /***************************************************
    * VERIFY EMPTY FIXTURE
    ***************************************************/
   void assertEmptyFixtureParameters(const custom::priority_queue <Spy>& pq, int line, const char* function)
   {
      assertIndirect(pq.container.empty());
   }
   void assertEmptyFixtureParameters(const std::priority_queue <Spy>& pq, int line, const char* function)
   {
      assertIndirect(pq.empty());
   }
   
   /***************************************************
    * VERIFY STANDARD FIXTURE
    *                 10
    *           8            9
    *        4     3      7     5
    *
    *   +---+---+---+---+---+---+---+---+---+
    *   | 10| 8 | 9 | 4 | 3 | 7 | 5 |   |   |
    *   +---+---+---+---+---+---+---+---+---+
    ***************************************************/
   void assertStandardFixtureParameters(const custom::priority_queue <Spy>& pq, int line, const char* function)
   {
      assertIndirect(pq.container.size() == 7);
      
      if (pq.container.size() >= 7)
      {
         assertIndirect(pq.container[1-1] == Spy(10));
         assertIndirect(pq.container[2-1] == Spy(8));
         assertIndirect(pq.container[3-1] == Spy(9));
         assertIndirect(pq.container[4-1] == Spy(4));
         assertIndirect(pq.container[5-1] == Spy(3));
         assertIndirect(pq.container[6-1] == Spy(7));
         assertIndirect(pq.container[7-1] == Spy(5));
      }
   }
   void assertStandardFixtureParameters(const std::priority_queue <Spy>& pq, int line, const char* function)
   {
      assertIndirect(pq.size() == 7);

   }
   
   /***************************************************
    * VERIFY TIGHT FIXTURE 
    *                 10
    *           8            9
    *        4     3      7     5
    *
    *   +---+---+---+---+---+---+---+
    *   | 10| 8 | 9 | 4 | 3 | 7 | 5 |
    *   +---+---+---+---+---+---+---+
    ***************************************************/
   void assertTightFixtureParametersX(const custom::priority_queue <Spy>& pq, int line, const char* function)
   {
      assertIndirect(pq.container.size() == 7);

      if (pq.container.size() >= 7)
      {
         assertIndirect(pq.container[1-1] == Spy(10));
         assertIndirect(pq.container[2-1] == Spy(8));
         assertIndirect(pq.container[3-1] == Spy(9));
         assertIndirect(pq.container[4-1] == Spy(4));
         assertIndirect(pq.container[5-1] == Spy(3));
         assertIndirect(pq.container[6-1] == Spy(7));
         assertIndirect(pq.container[7-1] == Spy(5));
      }
   }
   void assertTightFixtureParameters(const std::priority_queue <Spy>& pq, int line, const char* function)
   {
      assertIndirect(pq.size() == 7);
   }
   
   /***************************************************
    * TEARDOWN STANDARD FIXTURE
    *   +---+---+---+---+---+---+---+---+---+---+
    *   |   |   |   |   |   |   |   |   |   |   |
    *   +---+---+---+---+---+---+---+---+---+---+
    ***************************************************/
   void teardownStandardFixture(custom::priority_queue <Spy>& pq)
   {
      pq.container.empty();
   }
   void teardownStandardFixture(std::priority_queue <Spy>& pq)
   {
   }
};

#endif // DEBUG
