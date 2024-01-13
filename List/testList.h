/***********************************************************************
 * Header:
 *    TEST LIST
 * Summary:
 *    Unit tests for list
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include "list.h"
#include <list>
#include "unitTest.h"
#include "spy.h"

#include <vector>
#include <cassert>
#include <memory>
#include <iostream>

class TestList : public UnitTest
{
public:
   void run()
   {
      reset();

      // Construct
      test_construct_default();
      test_construct_sizeZero();
      test_construct_sizeThree();
      test_construct_sizeThreeFill();
      test_constructCopy_empty();
      test_constructCopy_standard();
      test_constructMove_empty();
      test_constructMove_standard();
      test_constructInit_empty();
      test_constructInit_standard();
      test_constructRange_empty();
      test_constructRange_standard();
      test_destructor_empty();
      test_destructor_standard();

      // Assign
      test_assign_emptyToEmpty();
      test_assign_standardToEmpty();
      test_assign_emptyToStandard();
      test_assign_smallToBig();
      test_assign_bigToSmall();
      test_assignInit_empty();
      test_assignInit_sameSize();
      test_assignInit_rightBigger();
      test_assignInit_leftBigger();
      test_assignMove_emptyToEmpty();
      test_assignMove_standardToEmpty();
      test_assignMove_emptyToStandard();
      test_assignMove_bigToSmall();
      test_swap_emptyToEmpty();
      test_swap_standardToEmpty();
      test_swap_emptyToStandard();
      test_swap_bigToSmall();

      // Iterator
      test_iterator_begin_empty();
      test_iterator_begin_standard();
      test_iterator_end_standard();
      test_iterator_increment_standardMiddle();
      test_iterator_increment_standardEnd();
      test_iterator_incrementPost_standardMiddle();
      test_iterator_decrement_standardMiddle();
      test_iterator_decrement_standardBegin();
      test_iterator_decrementPost_standardMiddle();
      test_iterator_dereference_read();
      test_iterator_dereference_update();
      
      // Access
      test_front_empty();
      test_front_standardRead();
      test_front_standardWrite();
      test_back_empty();
      test_back_standardRead();
      test_back_standardWrite();
      
      // Insert
      test_pushback_empty();
      test_pushback_standard();
      test_pushback_moveEmpty();
      test_pushback_moveStandard();
      test_pushfront_empty();
      test_pushfront_standard();
      test_pushfront_moveEmpty();
      test_pushfront_moveStandard();
      test_insert_empty();
      test_insert_standardFront();
      test_insert_standardMiddle();
      test_insert_standardEnd();
      test_insertMove_empty();
      test_insertMove_standardFront();
      test_insertMove_standardMiddle();

      // Remove
      test_clear_empty();
      test_clear_standard();
      test_popback_empty();
      test_popback_standard();
      test_popback_single();
      test_popfront_empty();
      test_popfront_standard();
      test_popfront_single();
      test_erase_empty();
      test_erase_standardFront();
      test_erase_standardMiddle();
      test_erase_standardEnd();

      // Status
      test_size_empty();
      test_size_three();
      test_empty_empty();
      test_empty_three();

      report("List");
   }

   /***************************************
    * CONSTRUCTOR
    ***************************************/

   // default constructor, no allocations
   void test_construct_default()
   {  // setup
      std::allocator<custom::list<Spy>> alloc;
      custom::list<Spy> l;
      l.pHead = (custom::list<Spy>::Node*)0xBADF00D1;
      l.pTail = (custom::list<Spy>::Node*)0xBADF00D2;
      l.numElements = 99;
      Spy::reset();
      // exercise
      alloc.construct(&l); // the constructor is called explicitly
      // verify
      assertUnit(Spy::numDefault() == 0);     
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(l);
   }  // teardown

   // construct with a size of zero
   void test_construct_sizeZero()
   {  // setup
      std::allocator<custom::list<Spy>> alloc;
      custom::list<Spy> l;
      l.pHead = (custom::list<Spy>::Node*)0xBADF00D1;
      l.pTail = (custom::list<Spy>::Node*)0xBADF00D2;
      l.numElements = 99;
      Spy::reset();
      // exercise
      alloc.construct(&l, 0); // the constructor is called explicitly
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(l);
   }  // teardown

   // construct with a size of three
   void test_construct_sizeThree()
   {  // setup
      std::allocator<custom::list<Spy>> alloc;
      custom::list<Spy> l;
      l.pHead = (custom::list<Spy>::Node*)0xBADF00D1;
      l.pTail = (custom::list<Spy>::Node*)0xBADF00D2;
      l.numElements = 99;
      Spy::reset();
      // exercise
      alloc.construct(&l, 3); // the constructor is called explicitly
      // verify
      assertUnit(Spy::numDefault() == 3);    // Create [00][00][00]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //    +----+   +----+   +----+
      //    | 00 | - | 00 | - | 00 |
      //    +----+   +----+   +----+      
      assertUnit(l.numElements == 3);
      assertUnit(l.pHead != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy());
         assertUnit(l.pHead->pPrev == nullptr);
         assertUnit(l.pHead->pNext != nullptr);
         if (l.pHead->pNext != nullptr)
         {
            assertUnit(l.pHead->pNext->data == Spy());
            assertUnit(l.pHead->pNext->pPrev == l.pHead);
            assertUnit(l.pHead->pNext->pNext != nullptr);
            if (l.pHead->pNext->pNext != nullptr)
            {
               assertUnit(l.pHead->pNext->pNext->data == Spy());
               assertUnit(l.pHead->pNext->pNext->pPrev == l.pHead->pNext);
               assertUnit(l.pHead->pNext->pNext->pNext == nullptr);
            }
         }
      }
      // teardown
      teardownStandardFixture(l);
   }

   // construct with a size of three and fill
   void test_construct_sizeThreeFill()
   {  // setup
      Spy s(99);
      std::allocator<custom::list<Spy>> alloc;
      custom::list<Spy> l;
      l.pHead = (custom::list<Spy>::Node*)0xBADF00D1;
      l.pTail = (custom::list<Spy>::Node*)0xBADF00D2;
      l.numElements = 99;
      Spy::reset();
      // exercise
      alloc.construct(&l, 3, s); // the constructor is called explicitly
      // verify
      assertUnit(Spy::numCopy() == 3);       // copy [99][99][99]
      assertUnit(Spy::numAlloc() == 3);      // allocate [99][99][99]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //    +----+   +----+   +----+
      //    | 99 | - | 99 | - | 99 |
      //    +----+   +----+   +----+      
      assertUnit(l.numElements == 3);
      assertUnit(l.pHead != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(99));
         assertUnit(l.pHead->pPrev == nullptr);
         assertUnit(l.pHead->pNext != nullptr);
         if (l.pHead->pNext != nullptr)
         {
            assertUnit(l.pHead->pNext->data == Spy(99));
            assertUnit(l.pHead->pNext->pPrev == l.pHead);
            assertUnit(l.pHead->pNext->pNext != nullptr);
            if (l.pHead->pNext->pNext != nullptr)
            {
               assertUnit(l.pHead->pNext->pNext->data == Spy(99));
               assertUnit(l.pHead->pNext->pNext->pPrev == l.pHead->pNext);
               assertUnit(l.pHead->pNext->pNext->pNext == nullptr);
            }
         }
      }
      // teardown
      teardownStandardFixture(l);
   }

   /***************************************
    * DESTRUCTOR
    ***************************************/

    // destructor of an empty list
   void test_destructor_empty()
   {  // setup
      {
         custom::list<Spy> v;
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
   }  // teardown

   // destructor of a 3-element collection
   void test_destructor_standard()
   {  // setup
      {
         //    +----+   +----+   +----+
         //    | 11 | - | 26 | - | 31 |
         //    +----+   +----+   +----+      
         custom::list<Spy> l;
         setupStandardFixture(l);
         Spy::reset();
      } // exercise
      // verify
      assertUnit(Spy::numDestructor() == 3); // destructor for [11][26][31]
      assertUnit(Spy::numDelete() == 3);     // delete [11][26][31]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssign() == 0);
   }



   /***************************************
    * COPY CONSTRUCTOR
    ***************************************/

    // copy constructor of an empty list
   void test_constructCopy_empty()
   {  // setup
      custom::list<Spy> lSrc;
      Spy::reset();
      // exercise
      custom::list<Spy> lDest(lSrc);
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
      assertEmptyFixture(lSrc);
      assertEmptyFixture(lDest);
   }  // teardown

   // copy constructor of a 3-element collection
   void test_constructCopy_standard()
   {  // setup
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::list<Spy> lSrc;
      setupStandardFixture(lSrc);
      Spy::reset();
      // exercise
      custom::list<Spy> lDest(lSrc);
      // verify
      assertUnit(Spy::numCopy() == 3);     // copy constructor for [11][26][31]
      assertUnit(Spy::numAlloc() == 3);    // allocate [11][26][31]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(lSrc.pHead != lDest.pHead);
      if (lSrc.pHead && lDest.pHead)
      {
         assertUnit(lSrc.pHead->pNext != lDest.pHead->pNext);
         if (lSrc.pHead->pNext && lDest.pHead->pNext)
            assertUnit(lSrc.pHead->pNext->pNext != lDest.pHead->pNext->pNext);
      }
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      assertStandardFixture(lSrc);
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      assertStandardFixture(lDest);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDest);
   }

   /***************************************
    * MOVE CONSTRUCTOR
    ***************************************/

    // move constructor of an empty list
   void test_constructMove_empty()
   {  // setup
      custom::list<Spy> lSrc;
      Spy::reset();
      // exercise
      custom::list<Spy> lDest(std::move(lSrc));
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
      assertEmptyFixture(lSrc);
      assertEmptyFixture(lDest);
   }  // teardown

   // move constructor of a 3-element collection
   void test_constructMove_standard()
   {  // setup
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::list<Spy> lSrc;
      setupStandardFixture(lSrc);
      custom::list<Spy>::Node *p = lSrc.pHead;
      Spy::reset();
      // exercise
      custom::list<Spy> lDest(std::move(lSrc));
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
      assertEmptyFixture(lSrc);
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      assertStandardFixture(lDest);
      assertUnit(p == lDest.pHead);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDest);
   }

   /***************************************
    * CONSTRUCTOR INITIALIZE LIST
    ***************************************/

    // empty initialization list
   void test_constructInit_empty()
   {  // setup
      Spy::reset();
      // exercise
      custom::list<Spy> l{};
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
      assertEmptyFixture(l);
   }  // teardown

   // use the initialization list to create the standard fixture
   void test_constructInit_standard()
   {  // setup
      std::initializer_list<Spy> il{ Spy(11),Spy(26),Spy(31) };
      Spy::reset();
      // exercise
      custom::list<Spy> l(il); // same as list<Spy> l{...}
      // verify
      assertUnit(Spy::numCopy() == 3);       // copy-construct [11][26][31]
      assertUnit(Spy::numAlloc() == 3);      // allocate [11][26][31]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   /***************************************
    * CONSTRUCTOR RANGE
    ***************************************/

    // empty range
   void test_constructRange_empty()
   {  // setup
      std::initializer_list <Spy> il{};
      Spy::reset();
      // exercise
      custom::list<Spy> l(il.begin(), il.end());
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
      assertEmptyFixture(l);
   }  // teardown

   // use the initialization list to create the standard fixture
   void test_constructRange_standard()
   {  // setup
      std::initializer_list<Spy> il{ Spy(11),Spy(26),Spy(31) };
      Spy::reset();
      // exercise
      custom::list<Spy> l(il.begin(), il.end());
      // verify
      assertUnit(Spy::numCopy() == 3);       // copy-construct [11][26][31]
      assertUnit(Spy::numAlloc() == 3);      // allocate [11][26][31]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }


   /***************************************
    * SIZE EMPTY 
    ***************************************/

    // size of graph with one node
   void test_size_empty()
   {  // setup
      custom::list<Spy> l;
      Spy::reset();
      // exercise
      size_t size = l.size();
      // verify
      assertUnit(0 == size);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(l);
   }  // teardown

   // size of graph with four nodes
   void test_size_three()
   {  // setup
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy::reset();
      // exercise
      size_t size = l.size();
      // verify
      assertUnit(3 == size);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // graph with one node
   void test_empty_empty()
   {  // setup
      custom::list<Spy> l;
      Spy::reset();
      // exercise
      bool empty = l.empty();
      // verify
      assertUnit(true == empty);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(l);
   }  // teardown

   // graph with four nodes
   void test_empty_three()
   {  // setup
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy::reset();
      // exercise
      bool empty = l.empty();
      // verify
      assertUnit(false == empty);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }


   /***************************************
    * ASSIGN
    ***************************************/

   // use the assignment operator when both are empty
   void test_assign_emptyToEmpty()
   {  // setup
      custom::list<Spy> lSrc;
      custom::list<Spy> lDes;
      Spy::reset();
      // exercise
      lDes = lSrc;
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(lSrc);
      assertEmptyFixture(lDes);
   }  // teardown

   // From the standard to fixture to an empty list
   void test_assign_standardToEmpty()
   {  // setup
      //     pHead             pTail
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::list<Spy> lSrc;
      setupStandardFixture(lSrc);
      custom::list<Spy> lDes;
      Spy::reset();
      // exercise
      lDes = lSrc;
      // verify
      assertUnit(Spy::numCopy() == 3);       // copy [11][26][31]
      assertUnit(Spy::numAlloc() == 3);      // allocate [11][26][31]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(lSrc.pHead != lDes.pHead);
      if (lSrc.pHead && lDes.pHead)
      {
         assertUnit(lSrc.pHead->pNext != lDes.pHead->pNext);
         if (lSrc.pHead->pNext && lDes.pHead->pNext)
            assertUnit(lSrc.pHead->pNext->pNext != lDes.pHead->pNext->pNext);
      } 
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lSrc);
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lDes);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDes);
   } 

   // From the empty list to the standard to fixture
   void test_assign_emptyToStandard()
   {  // setup
      custom::list<Spy> lSrc;
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> lDes;
      setupStandardFixture(lDes);
      Spy::reset();
      // exercise
      lDes = lSrc;
      // verify
      assertUnit(Spy::numDestructor() == 3);   // destroy [11][26][31]
      assertUnit(Spy::numDelete() == 3);       // delete [11][26][31]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);      
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(lSrc);
      assertEmptyFixture(lDes);
   }  // teardown

   void test_assign_bigToSmall()
   {  // setup
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> lSrc;
      setupStandardFixture(lSrc);
      // lDes   pHead    pTail
      //       +----+   +----+
      //       | 85 | - | 99 |
      //       +----+   +----+
      custom::list<Spy> lDes;
      custom::list<Spy>::Node* pDes1 = new custom::list<Spy>::Node(Spy(85));
      custom::list<Spy>::Node* pDes2 = new custom::list<Spy>::Node(Spy(99));
      pDes1->pNext = pDes2;
      pDes2->pPrev = pDes1;
      lDes.pHead = pDes1;
      lDes.pTail = pDes2;
      lDes.numElements = 2;
      Spy::reset();
      // exercise
      lDes = lSrc;
      // verify
      assertUnit(Spy::numAssign() == 2);       // assign [11][26]
      assertUnit(Spy::numCopy() == 1);         // copy construct [31]
      assertUnit(Spy::numAlloc() == 1);        // allocate [31]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);       
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(lSrc.pHead != lDes.pHead);
      if (lSrc.pHead && lDes.pHead)
      {
         assertUnit(lSrc.pHead->pNext != lDes.pHead->pNext);
         if (lSrc.pHead->pNext && lDes.pHead->pNext)
            assertUnit(lSrc.pHead->pNext->pNext != lDes.pHead->pNext->pNext);
      }
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lSrc);
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lDes);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDes);
   }  

   // assign a small list of 3 onto a larger one of 4
   void test_assign_smallToBig()
   {  // setup
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> lSrc;
      setupStandardFixture(lSrc);
      // lDes   pHead                      pTail
      //       +----+   +----+   +----+   +----+
      //       | 61 | - | 73 | - | 85 | - | 99 |
      //       +----+   +----+   +----+   +----+
      custom::list<Spy> lDes;
      custom::list<Spy>::Node* lDes1 = new custom::list<Spy>::Node(Spy(61));
      custom::list<Spy>::Node* lDes2 = new custom::list<Spy>::Node(Spy(73));
      custom::list<Spy>::Node* lDes3 = new custom::list<Spy>::Node(Spy(85));
      custom::list<Spy>::Node* lDes4 = new custom::list<Spy>::Node(Spy(99));
      lDes1->pNext = lDes2;
      lDes2->pNext = lDes3;
      lDes3->pNext = lDes4;
      lDes4->pPrev = lDes3;
      lDes3->pPrev = lDes2;
      lDes2->pPrev = lDes1;
      lDes.pHead = lDes1;
      lDes.pTail = lDes4;
      lDes.numElements = 4;
      Spy::reset();
      // exercise
      lDes = lSrc;
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [99]
      assertUnit(Spy::numDelete() == 1);       // delete [99]
      assertUnit(Spy::numAssign() == 3);       // assign [11][26][31]
      assertUnit(Spy::numCopy() == 0);         
      assertUnit(Spy::numAlloc() == 0);        
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      if (lSrc.pHead && lDes.pHead)
      {
         assertUnit(lSrc.pHead->pNext != lDes.pHead->pNext);
         if (lSrc.pHead->pNext && lDes.pHead->pNext)
            assertUnit(lSrc.pHead->pNext->pNext != lDes.pHead->pNext->pNext);
      }
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lSrc);
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lDes);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDes);
   }


   /***************************************
    * ASSIGN INIT
    ***************************************/

    // assignment-init when there is nothing to copy
   void test_assignInit_empty()
   {  // setup
      custom::list<Spy> l;
      Spy::reset();
      // exercise
      l = {};
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
      assertEmptyFixture(l);
   }  // teardown

   // assignment-init when source and destination are same size
   void test_assignInit_sameSize()
   {  // setup
      // l      pHead             pTail
      //       +----+   +----+   +----+
      //       | 99 | - | 99 | - | 99 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      l.pHead->data.set(99);
      l.pHead->pNext->data.set(99);
      l.pHead->pNext->pNext->data.set(99);
      std::initializer_list<Spy> il{ Spy(11),Spy(26),Spy(31) };
      Spy::reset();
      // exercise
      l = il;   // l = {Spy(11), Spy(26), Spy(31) }
      // verify
      assertUnit(Spy::numAssign() == 3);     // [11][26][31] are assigned over
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // assignment-init when the destination is smaller than the source
   void test_assignInit_rightBigger()
   {  // setup
      // l      pHead/pTail
      //       +----+
      //       | 99 |
      //       +----+
      custom::list<Spy> l;
      l.pHead = l.pTail = new custom::list<Spy>::Node(Spy(99));
      l.numElements = 1;
      std::initializer_list<Spy> il{ Spy(11),Spy(26),Spy(31) };
      Spy::reset();
      // exercise
      l = il;   // l = {Spy(11), Spy(26), Spy(31) }
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [11] onto the [99] spot
      assertUnit(Spy::numCopy() == 2);       // copy [26][31]
      assertUnit(Spy::numAlloc() == 2);      // allocate [26][31]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);     
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // assignment-init when the destination is bigger than the source
   void test_assignInit_leftBigger()
   {  // setup
      // l      pHead                     pTail
      //       +----+   +----+   +----+   +----+
      //       | 61 | - | 73 | - | 85 | - | 99 |
      //       +----+   +----+   +----+   +----+
      custom::list<Spy> l;
      custom::list<Spy>::Node* p1 = new custom::list<Spy>::Node(Spy(61));
      custom::list<Spy>::Node* p2 = new custom::list<Spy>::Node(Spy(73));
      custom::list<Spy>::Node* p3 = new custom::list<Spy>::Node(Spy(85));
      custom::list<Spy>::Node* p4 = new custom::list<Spy>::Node(Spy(99));
      p1->pNext = p2;
      p2->pNext = p3;
      p3->pNext = p4;
      p4->pPrev = p3;
      p3->pPrev = p2;
      p2->pPrev = p1;
      l.pHead = p1;
      l.pTail = p4;
      l.numElements = 4;
      std::initializer_list<Spy> il{ Spy(11),Spy(26),Spy(31) };
      Spy::reset();
      // exercise
      l = il;   // l = {Spy(11), Spy(26), Spy(31) }
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [99]
      assertUnit(Spy::numDelete() == 1);       // delete [99]
      assertUnit(Spy::numAssign() == 3);       // assign [11][26][31] onto [61][73][85]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   /***************************************
    * ASSIGN MOVE
    ***************************************/

    // use the assignment operator when both are empty
   void test_assignMove_emptyToEmpty()
   {  // setup
      custom::list<Spy> lSrc;
      custom::list<Spy> lDes;
      Spy::reset();
      // exercise
      lDes = std::move(lSrc);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(lSrc);
      assertEmptyFixture(lDes);
   }  // teardown

   // From the standard to fixture to an empty list
   void test_assignMove_standardToEmpty()
   {  // setup
      //     pHead             pTail
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::list<Spy> lSrc;
      setupStandardFixture(lSrc);
      custom::list<Spy> lDes;
      auto pHeadSrc = lSrc.pHead;
      auto pTailSrc = lSrc.pTail;

      Spy::reset();
      // exercise
      lDes = std::move(lSrc);
      // verify
      assertUnit(Spy::numCopy() == 0);    
      assertUnit(Spy::numAlloc() == 0);   
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);

      assertUnit(lDes.pHead == pHeadSrc);
      assertUnit(lDes.pTail == pTailSrc);

      assertEmptyFixture(lSrc);
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lDes);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDes);
   }

   // From the empty list to the standard to fixture
   void test_assignMove_emptyToStandard()
   {  // setup
      custom::list<Spy> lSrc;
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> lDes;
      setupStandardFixture(lDes);
      Spy::reset();
      // exercise
      lDes = std::move(lSrc);
      // verify
      assertUnit(Spy::numDestructor() == 3);   // destroy [11][26][31]
      assertUnit(Spy::numDelete() == 3);       // delete [11][26][31]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(lSrc);
      assertEmptyFixture(lDes);
   }  // teardown

   void test_assignMove_bigToSmall()
   {  // setup
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> lSrc;
      setupStandardFixture(lSrc);
      // lDes   pHead    pTail
      //       +----+   +----+
      //       | 85 | - | 99 |
      //       +----+   +----+
      custom::list<Spy> lDes;
      custom::list<Spy>::Node* pDes1 = new custom::list<Spy>::Node(Spy(85));
      custom::list<Spy>::Node* pDes2 = new custom::list<Spy>::Node(Spy(99));
      pDes1->pNext = pDes2;
      pDes2->pPrev = pDes1;
      lDes.pHead = pDes1;
      lDes.pTail = pDes2;
      lDes.numElements = 2;
      auto pHeadSrc = lSrc.pHead;
      auto pTailSrc = lSrc.pTail;
      Spy::reset();
      // exercise
      lDes = std::move(lSrc);
      // verify
      assertUnit(Spy::numAssign() == 0);      
      assertUnit(Spy::numCopy() == 0);        
      assertUnit(Spy::numAlloc() == 0);        
      assertUnit(Spy::numDestructor() == 2);   // destroy [85][99]
      assertUnit(Spy::numDelete() == 2);       // delete [85][99]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);

      assertUnit(lDes.pHead == pHeadSrc);
      assertUnit(lDes.pTail == pTailSrc);

      assertEmptyFixture(lSrc);
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lDes);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDes);
   }


   /***************************************
    * SWAP
    ***************************************/

   // use swap when both are empty
   void test_swap_emptyToEmpty()
   {  // setup
      custom::list<Spy> lSrc;
      custom::list<Spy> lDes;
      Spy::reset();
      // exercise
      lDes.swap(lSrc);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(lSrc);
      assertEmptyFixture(lDes);
   }  // teardown

   // From the standard to fixture to an empty list
   void test_swap_standardToEmpty()
   {  // setup
      //     pHead             pTail
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::list<Spy> lSrc;
      setupStandardFixture(lSrc);
      custom::list<Spy> lDes;
      Spy::reset();
      auto pHeadSrc = lSrc.pHead;
      auto pTailSrc = lSrc.pTail;
      auto pHeadDes = lDes.pHead;
      auto pTailDes = lDes.pTail;
      // exercise
      lDes.swap(lSrc);
      // verify
      assertUnit(Spy::numCopy() == 0);       
      assertUnit(Spy::numAlloc() == 0);      
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      
      assertUnit(pHeadSrc == lDes.pHead);
      assertUnit(pHeadDes == lSrc.pHead);
      assertUnit(pTailSrc == lDes.pTail);
      assertUnit(pTailDes == lSrc.pTail);
      assertEmptyFixture(lSrc);
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lDes);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDes);
   }

   // From the empty list to the standard to fixture
   void test_swap_emptyToStandard()
   {  // setup
      custom::list<Spy> lSrc;
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> lDes;
      setupStandardFixture(lDes);
      auto pHeadSrc = lSrc.pHead;
      auto pTailSrc = lSrc.pTail;
      auto pHeadDes = lDes.pHead;
      auto pTailDes = lDes.pTail;
      Spy::reset();
      // exercise
      lDes.swap(lSrc);
      // verify
      assertUnit(Spy::numDestructor() == 0); 
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);

      assertUnit(pHeadSrc == lDes.pHead);
      assertUnit(pHeadDes == lSrc.pHead);
      assertUnit(pTailSrc == lDes.pTail);
      assertUnit(pTailDes == lSrc.pTail);
      assertEmptyFixture(lDes);
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lSrc);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDes);
   }

   void test_swap_bigToSmall()
   {  // setup
      // lSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> lSrc;
      setupStandardFixture(lSrc);
      // lDes   pHead    pTail
      //       +----+   +----+
      //       | 85 | - | 99 |
      //       +----+   +----+
      custom::list<Spy> lDes;
      custom::list<Spy>::Node* pDes1 = new custom::list<Spy>::Node(Spy(85));
      custom::list<Spy>::Node* pDes2 = new custom::list<Spy>::Node(Spy(99));
      pDes1->pNext = pDes2;
      pDes2->pPrev = pDes1;
      lDes.pHead = pDes1;
      lDes.pTail = pDes2;
      lDes.numElements = 2;
      auto pHeadSrc = lSrc.pHead;
      auto pTailSrc = lSrc.pTail;
      auto pHeadDes = lDes.pHead;
      auto pTailDes = lDes.pTail;
      Spy::reset();
      // exercise
      lDes.swap(lSrc);
      // verify
      assertUnit(Spy::numAssign() == 0);  
      assertUnit(Spy::numCopy() == 0);    
      assertUnit(Spy::numAlloc() == 0);   
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);

      assertUnit(pHeadSrc == lDes.pHead);
      assertUnit(pHeadDes == lSrc.pHead);
      assertUnit(pTailSrc == lDes.pTail);
      assertUnit(pTailDes == lSrc.pTail);
      // lSrc   pHead    pTail
      //       +----+   +----+
      //       | 85 | - | 99 |
      //       +----+   +----+
      assertUnit(lSrc.pHead != nullptr);
      if (lSrc.pHead)
      {
         assertUnit(lSrc.pHead->data == Spy(85));
         assertUnit(lSrc.pHead->pNext == lSrc.pTail);
      }
      assertUnit(lSrc.pTail != nullptr);
      if (lSrc.pTail)
      {
         assertUnit(lSrc.pTail->data == Spy(99));
         assertUnit(lSrc.pTail->pPrev == lSrc.pHead);
      }
      assertUnit(lSrc.numElements == 2);
      // lDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(lDes);
      // teardown
      teardownStandardFixture(lSrc);
      teardownStandardFixture(lDes);
   }

   /***************************************
    * CLEAR
    ***************************************/

   // clear an empty fixture
   void test_clear_empty()
   {  // setup
      custom::list<Spy> l;
      Spy::reset();
      // exercise
      l.clear();
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
      assertEmptyFixture(l);
   }  // teardown

   void test_clear_standard()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy::reset();
      // exercise
      l.clear();
      // verify
      assertUnit(Spy::numDestructor() == 3);   // destroy [11][26][31]
      assertUnit(Spy::numDelete() == 3);       // delete [11][26][31]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(l);
   }  // teardown


   /***************************************
    * PUSH BACK
    ***************************************/

   // push back to an empty list
   void test_pushback_empty()
   {  // setup
      custom::list<Spy> l;
      Spy s(99);
      Spy::reset();
      // exercise
      l.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy [99]
      assertUnit(Spy::numAlloc() == 1);      // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+
      //       | 99 |
      //       +----+
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.pTail == l.pHead);
      assertUnit(l.numElements == 1);
      if (l.pHead)
      { 
         assertUnit(l.pHead->data == Spy(99));
         assertUnit(l.pHead->pNext == nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
      }
      // teardown
      assertUnit(s == Spy(99));
      teardownStandardFixture(l);
   }

   // push an element onto the back of the standard fixture
   void test_pushback_standard()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy s(99);
      Spy::reset();
      // exercise
      l.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy [99]
      assertUnit(Spy::numAlloc() == 1);      // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //        pHead                      pTail
      //       +----+   +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 | - | 99 |
      //       +----+   +----+   +----+   +----+
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.numElements == 4);
      if (l.pTail)
      {
         assertUnit(l.pTail->data == Spy(99));
         assertUnit(l.pTail->pNext == nullptr);
         assertUnit(l.pTail->pPrev != nullptr);
         if (l.pTail->pPrev)
         {
            assertUnit(l.pTail->pPrev->pNext == l.pTail);
            l.pTail = l.pTail->pPrev;
            delete l.pTail->pNext;
            l.numElements--;
            l.pTail->pNext = nullptr;
         }
      }
      assertUnit(s == Spy(99));
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }   

   // push back to an empty list
   void test_pushback_moveEmpty()
   {  // setup
      custom::list<Spy> l;
      Spy s(99);
      Spy::reset();
      // exercise
      l.push_back(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);       // copy-move [99]
      assertUnit(Spy::numAlloc() == 0);       
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+
      //       | 99 |
      //       +----+
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.pTail == l.pHead);
      assertUnit(l.numElements == 1);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(99));
         assertUnit(l.pHead->pNext == nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
      }
      assertUnit(s == Spy());
      // teardown
      teardownStandardFixture(l);
   }

   // push an element onto the back of the standard fixture
   void test_pushback_moveStandard()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy s(99);
      Spy::reset();
      // exercise
      l.push_back(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);       // copy-move [99]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //        pHead                      pTail
      //       +----+   +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 | - | 99 |
      //       +----+   +----+   +----+   +----+
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.numElements == 4);
      if (l.pTail)
      {
         assertUnit(l.pTail->data == Spy(99));
         assertUnit(l.pTail->pNext == nullptr);
         assertUnit(l.pTail->pPrev != nullptr);
         if (l.pTail->pPrev)
         {
            assertUnit(l.pTail->pPrev->pNext == l.pTail);
            l.pTail = l.pTail->pPrev;
            delete l.pTail->pNext;
            l.numElements--;
            l.pTail->pNext = nullptr;
         }
      }
      assertUnit(s == Spy());
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   /***************************************
    * PUSH FRONT
    ***************************************/

   // push an element onto the front of an empty list
   void test_pushfront_empty()
   {  // setup
      custom::list<Spy> l;
      Spy s(99);
      Spy::reset();
      // exercise
      l.push_front(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy [99]
      assertUnit(Spy::numAlloc() == 1);      // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+
      //       | 99 |
      //       +----+
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.pTail == l.pHead);
      assertUnit(l.numElements == 1);
      if (l.pTail)
      {
         assertUnit(l.pTail->data == Spy(99));
         assertUnit(l.pTail->pNext == nullptr);
         assertUnit(l.pTail->pPrev == nullptr);
      }
      assertUnit(s == Spy(99));
      // teardown
      teardownStandardFixture(l);
   }

   // push an element onto the front of the standard fixture
   void test_pushfront_standard()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy s(99);
      Spy::reset();
      // exercise
      l.push_front(s);
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy [99]
      assertUnit(Spy::numAlloc() == 1);      // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //        pHead                      pTail
      //       +----+   +----+   +----+   +----+
      //       | 99 | - | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.numElements == 4);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(99));
         assertUnit(l.pHead->pNext != nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->pPrev == l.pHead);
            l.pHead = l.pHead->pNext;
            delete l.pHead->pPrev;
            l.numElements--;
            l.pHead->pPrev = nullptr;
         }
      }
      assertUnit(s == Spy(99));
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // push an element onto the front of an empty list
   void test_pushfront_moveEmpty()
   {  // setup
      custom::list<Spy> l;
      Spy s(99);
      Spy::reset();
      // exercise
      l.push_front(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);       // copy-move [99]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+
      //       | 99 |
      //       +----+
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.pTail == l.pHead);
      assertUnit(l.numElements == 1);
      if (l.pTail)
      {
         assertUnit(l.pTail->data == Spy(99));
         assertUnit(l.pTail->pNext == nullptr);
         assertUnit(l.pTail->pPrev == nullptr);
      }
      assertUnit(s == Spy());
      // teardown
      teardownStandardFixture(l);
   }

   // push an element onto the front of the standard fixture
   void test_pushfront_moveStandard()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy s(99);
      Spy::reset();
      // exercise
      l.push_front(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);       // copy-move [99]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //        pHead                      pTail
      //       +----+   +----+   +----+   +----+
      //       | 99 | - | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.numElements == 4);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(99));
         assertUnit(l.pHead->pNext != nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->pPrev == l.pHead);
            l.pHead = l.pHead->pNext;
            delete l.pHead->pPrev;
            l.numElements--;
            l.pHead->pPrev = nullptr;
         }
      }
      assertUnit(s == Spy());
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   /***************************************
    * POP BACK
    ***************************************/

   // remove an element from the back of an empty list
   void test_popback_empty()
   {  // setup
      custom::list<Spy> l;
      Spy::reset();
      // exercise
      l.pop_back();
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
      assertEmptyFixture(l);
   }  // teardown

   // remove an element from the back of the standard fixture
   void test_popback_standard()
   {  // setup
      //        pHead                      pTail
      //       +----+   +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 | - | 99 |
      //       +----+   +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      custom::list<Spy>::Node* p = new custom::list<Spy>::Node(Spy(99));
      l.pTail->pNext = p;
      p->pPrev = l.pTail;
      l.pTail = p;
      l.numElements++;
      Spy::reset();
      // exercise
      l.pop_back();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [99]
      assertUnit(Spy::numDelete() == 1);       // delete [99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // remove an element from the back of a single element list
   void test_popback_single()
   {  // setup
      //        pTail
      //        pHead
      //       +----+
      //       | 99 |
      //       +----+
      custom::list<Spy> l;
      l.pHead = l.pTail = new custom::list<Spy>::Node(Spy(99));
      l.numElements = 1;
      Spy::reset();
      // exercise
      l.pop_back();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [99]
      assertUnit(Spy::numDelete() == 1);       // delete [99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(l);
      // teardown
   }

   /***************************************
    * POP FRONT
    ***************************************/

   // remove an element from the front of an empty list
   void test_popfront_empty()
   {  // setup
      custom::list<Spy> l;
      Spy::reset();
      // exercise
      l.pop_front();
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
      assertUnit(l.pHead == NULL);
      assertUnit(l.pTail == NULL);
      assertUnit(l.numElements == 0);
      assertUnit(l.size() == 0);
      assertUnit(l.empty() == true);
   }  // teardown

   // remove an element from the front of the standard fixture
   void test_popfront_standard()
   {  // setup
      //        pHead                      pTail
      //       +----+   +----+   +----+   +----+
      //       | 99 | - | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      custom::list<Spy>::Node* p = new custom::list<Spy>::Node(Spy(99));
      l.pHead->pPrev = p;
      p->pNext = l.pHead;
      l.pHead = p;
      l.numElements++;
      Spy::reset();
      // exercise
      l.pop_front();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [99]
      assertUnit(Spy::numDelete() == 1);       // delete [99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // remove an element from the front of a single element list
   void test_popfront_single()
   {  // setup
      //        pTail
      //        pHead
      //       +----+
      //       | 99 |
      //       +----+
      custom::list<Spy> l;
      l.pHead = l.pTail = new custom::list<Spy>::Node(Spy(99));
      l.numElements = 1;
      Spy::reset();
      // exercise
      l.pop_front();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [99]
      assertUnit(Spy::numDelete() == 1);       // delete [99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(l);
      // teardown
   }

   /***************************************
    * FRONT and BACK
    ***************************************/

   // attempt to access the front of an empty list
   void test_front_empty()
   {  // setup
      custom::list<Spy> l;
      // exercise
      Spy s;
      Spy::reset();
      try
      {
         s = l.front();
         // verify
      }
      catch (const char* sError)
      {
         assertUnit(std::string("ERROR: unable to access data from an empty list") ==
                std::string(sError));
      }
      assertEmptyFixture(l);
   }  // teardown

   // read the element off the front of the standard list
   void test_front_standardRead()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy s(99);
      Spy::reset();
      // exercise
      s = l.front();
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign [11] to value
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);       
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(s == Spy(11));
      // l      pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // write the element to the front of the standard list
   void test_front_standardWrite()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy s(99);
      Spy::reset();
      // exercise
      l.front() = s;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign 99 to [11]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // l      pHead             pTail
      //       +----+   +----+   +----+
      //       | 99 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertUnit(l.pHead->data == Spy(99));
      l.pHead->data = Spy(11);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // attempt to access the back of an empty list
   void test_back_empty()
   {  // setup
      custom::list<Spy> l;
      Spy s(99);
      Spy::reset();
      // exercise
      try
      {
         s = l.back();
         // verify
      }
      catch (const char* sError)
      {
         assertUnit(std::string("ERROR: unable to access data from an empty list") ==
            std::string(sError));
      }
      assertEmptyFixture(l);
   }  // teardown

   // read the element off the back of the standard list
   void test_back_standardRead()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy s(99);
      Spy::reset();
      // exercise
      s = l.back();
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign [31]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(s == Spy(31));
      // l      pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // write the element to the back of the standard list
   void test_back_standardWrite()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy s(99);
      Spy::reset();
      // exercise
      l.back() = s;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign 99 to [31]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // l      pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 99 |
      //       +----+   +----+   +----+
      assertUnit(l.pTail->data == Spy(99));
      l.pTail->data = Spy(31);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

    /***************************************
    * INSERT - Copy
    ***************************************/

   // insert an element onto an empty list
   void test_insert_empty()
   {  // setup
      custom::list<Spy> l;
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      it.p = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      itReturn = l.insert(it, s);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+
      //       | 99 |
      //       +----+
      //      itReturn
      assertUnit(it.p == nullptr);
      assertUnit(itReturn.p == l.pHead);
      assertUnit(itReturn == l.begin());
      if (itReturn.p != nullptr)
         assertUnit(*itReturn == Spy(99));
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.pTail == l.pHead);
      assertUnit(l.numElements == 1);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(99));
         assertUnit(l.pHead->pNext == nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
      }
      // teardown
      teardownStandardFixture(l);
   }

   // insert an element onto the front of the standard list
   void test_insert_standardFront()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //         it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead;
      Spy s(99);
      Spy::reset();
      // exercise
      itReturn = l.insert(it, s);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+   +----+   +----+   +----+
      //       | 99 | - | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      //      itReturn    it
      assertUnit(it.p != nullptr);
      if (it.p)
         assertUnit(it.p->data == Spy(11));
      assertUnit(itReturn.p != nullptr);
      if (itReturn.p)
         assertUnit(itReturn.p->data == Spy(99));
      assertUnit(l.numElements == 4);
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(99));
         assertUnit(l.pHead->pNext != nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->data == Spy(11));
            assertUnit(l.pHead->pNext->pNext != nullptr);
            assertUnit(l.pHead->pNext->pPrev == l.pHead);
            if (l.pHead->pNext->pNext)
            {
               assertUnit(l.pHead->pNext->pNext->data == Spy(26));
               assertUnit(l.pHead->pNext->pNext->pNext != nullptr);
               assertUnit(l.pHead->pNext->pNext->pPrev == l.pHead->pNext);
               assertUnit(l.pHead->pNext->pNext->pNext == l.pTail);
               if (l.pHead->pNext->pNext->pNext)
               {
                  assertUnit(l.pHead->pNext->pNext->pNext->data == Spy(31));
                  assertUnit(l.pHead->pNext->pNext->pNext->pNext == nullptr);
                  assertUnit(l.pHead->pNext->pNext->pNext->pPrev == l.pHead->pNext->pNext);
               }
            }
         }
      }
      // teardown
      teardownStandardFixture(l);
   }

   // insert an element onto the middle of the standard list
   void test_insert_standardMiddle()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead->pNext;
      Spy s(99);
      Spy::reset();
      // exercise
      itReturn = l.insert(it, s);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+   +----+   +----+   +----+
      //       | 11 | - | 99 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      //               itReturn    it
      assertUnit(it.p != nullptr);
      if (it.p)
         assertUnit(it.p->data == Spy(26));
      assertUnit(itReturn.p != nullptr);
      if (itReturn.p)
         assertUnit(itReturn.p->data == Spy(99));
      assertUnit(l.numElements == 4);
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(11));
         assertUnit(l.pHead->pNext != nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->data == Spy(99));
            assertUnit(l.pHead->pNext->pNext != nullptr);
            assertUnit(l.pHead->pNext->pPrev == l.pHead);
            if (l.pHead->pNext->pNext)
            {
               assertUnit(l.pHead->pNext->pNext->data == Spy(26));
               assertUnit(l.pHead->pNext->pNext->pNext != nullptr);
               assertUnit(l.pHead->pNext->pNext->pPrev == l.pHead->pNext);
               assertUnit(l.pHead->pNext->pNext->pNext == l.pTail);
               if (l.pHead->pNext->pNext->pNext)
               {
                  assertUnit(l.pHead->pNext->pNext->pNext->data == Spy(31));
                  assertUnit(l.pHead->pNext->pNext->pNext->pNext == nullptr);
                  assertUnit(l.pHead->pNext->pNext->pNext->pPrev == l.pHead->pNext->pNext);
               }
            }
         }
      }
      // teardown
      teardownStandardFixture(l);
   }

   // insert an element onto the end of the standard list
   void test_insert_standardEnd()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                                  it
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy s(99);
      Spy::reset();
      // exercise
      itReturn = l.insert(it, s);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+   +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 | - | 99 |
      //       +----+   +----+   +----+   +----+
      //                                  itReturn    it
      assertUnit(it.p == nullptr);
      assertUnit(itReturn.p != nullptr);
      if (itReturn.p)
         assertUnit(itReturn.p->data == Spy(99));
      assertUnit(l.numElements == 4);
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(11));
         assertUnit(l.pHead->pNext != nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->data == Spy(26));
            assertUnit(l.pHead->pNext->pNext != nullptr);
            assertUnit(l.pHead->pNext->pPrev == l.pHead);
            if (l.pHead->pNext->pNext)
            {
               assertUnit(l.pHead->pNext->pNext->data == Spy(31));
               assertUnit(l.pHead->pNext->pNext->pNext != nullptr);
               assertUnit(l.pHead->pNext->pNext->pPrev == l.pHead->pNext);
               assertUnit(l.pHead->pNext->pNext->pNext == l.pTail);
               if (l.pHead->pNext->pNext->pNext)
               {
                  assertUnit(l.pHead->pNext->pNext->pNext->data == Spy(99));
                  assertUnit(l.pHead->pNext->pNext->pNext->pNext == nullptr);
                  assertUnit(l.pHead->pNext->pNext->pNext->pPrev == l.pHead->pNext->pNext);
               }
            }
         }
      }
      // teardown
      teardownStandardFixture(l);
   }

   /***************************************
   * INSERT - Move
   ***************************************/

   // insert an element onto an empty list
   void test_insertMove_empty()
   {  // setup
      custom::list<Spy> l;
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      it.p = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      itReturn = l.insert(it, std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);      // copy-move [99]
      assertUnit(Spy::numCopy() == 0);          
      assertUnit(Spy::numAlloc() == 0);         
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(s.empty());
      //       +----+
      //       | 99 |
      //       +----+
      //      itReturn
      assertUnit(itReturn.p != nullptr);
      assertUnit(itReturn == l.begin());
      if (itReturn.p != nullptr)
         assertUnit(itReturn.p->data == Spy(99));
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      assertUnit(l.pTail == l.pHead);
      assertUnit(l.numElements == 1);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(99));
         assertUnit(l.pHead->pNext == nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
      }
      // teardown
      teardownStandardFixture(l);
   }

   // insert an element onto the front of the standard list
   void test_insertMove_standardFront()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //         it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead;
      Spy s(99);
      Spy::reset();
      // exercise
      itReturn = l.insert(it, std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);      // copy-move [99]
      assertUnit(Spy::numCopy() == 0);          
      assertUnit(Spy::numAlloc() == 0);         
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+   +----+   +----+   +----+
      //       | 99 | - | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      //      itReturn    it
      assertUnit(s.empty());
      assertUnit(it.p != nullptr);
      if (it.p)
         assertUnit(it.p->data == Spy(11));
      assertUnit(itReturn.p != nullptr);
      if (itReturn.p)
         assertUnit(itReturn.p->data == Spy(99));
      assertUnit(l.numElements == 4);
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(99));
         assertUnit(l.pHead->pNext != nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->data == Spy(11));
            assertUnit(l.pHead->pNext->pNext != nullptr);
            assertUnit(l.pHead->pNext->pPrev == l.pHead);
            if (l.pHead->pNext->pNext)
            {
               assertUnit(l.pHead->pNext->pNext->data == Spy(26));
               assertUnit(l.pHead->pNext->pNext->pNext != nullptr);
               assertUnit(l.pHead->pNext->pNext->pPrev == l.pHead->pNext);
               assertUnit(l.pHead->pNext->pNext->pNext == l.pTail);
               if (l.pHead->pNext->pNext->pNext)
               {
                  assertUnit(l.pHead->pNext->pNext->pNext->data == Spy(31));
                  assertUnit(l.pHead->pNext->pNext->pNext->pNext == nullptr);
                  assertUnit(l.pHead->pNext->pNext->pNext->pPrev == l.pHead->pNext->pNext);
               }
            }
         }
      }
      // teardown
      teardownStandardFixture(l);
   }

   // insert an element onto the middle of the standard list
   void test_insertMove_standardMiddle()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead->pNext;
      Spy s(99);
      Spy::reset();
      // exercise
      itReturn = l.insert(it, std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);   // copy-move [99]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);         
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //       +----+   +----+   +----+   +----+
      //       | 11 | - | 99 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      //               itReturn    it
      assertUnit(s.empty());
      assertUnit(it.p != nullptr);
      if (it.p)
         assertUnit(it.p->data == Spy(26));
      assertUnit(itReturn.p != nullptr);
      if (itReturn.p)
         assertUnit(itReturn.p->data == Spy(99));
      assertUnit(l.numElements == 4);
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pTail != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(11));
         assertUnit(l.pHead->pNext != nullptr);
         assertUnit(l.pHead->pPrev == nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->data == Spy(99));
            assertUnit(l.pHead->pNext->pNext != nullptr);
            assertUnit(l.pHead->pNext->pPrev == l.pHead);
            if (l.pHead->pNext->pNext)
            {
               assertUnit(l.pHead->pNext->pNext->data == Spy(26));
               assertUnit(l.pHead->pNext->pNext->pNext != nullptr);
               assertUnit(l.pHead->pNext->pNext->pPrev == l.pHead->pNext);
               assertUnit(l.pHead->pNext->pNext->pNext == l.pTail);
               if (l.pHead->pNext->pNext->pNext)
               {
                  assertUnit(l.pHead->pNext->pNext->pNext->data == Spy(31));
                  assertUnit(l.pHead->pNext->pNext->pNext->pNext == nullptr);
                  assertUnit(l.pHead->pNext->pNext->pNext->pPrev == l.pHead->pNext->pNext);
               }
            }
         }
      }
      // teardown
      teardownStandardFixture(l);
   }


   /***************************************
    * ERASE
    ***************************************/

   // erase an element from an empty list
   void test_erase_empty()
   {  // setup
      custom::list<Spy> l;
      custom::list<Spy>::iterator itErase;
      custom::list<Spy>::iterator itReturn;
      itErase.p = nullptr;
      Spy::reset();
      // exercise
      itReturn = l.erase(itErase);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(itReturn.p == itErase.p);
      assertUnit(itErase == l.end());
      assertEmptyFixture(l);
   }  // teardown

   // erase the element from the front of a list
   void test_erase_standardFront()
   {  // setup
      custom::list<Spy>::iterator itErase;
      custom::list<Spy>::iterator itReturn;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //         itErase
      custom::list<Spy> l;
      setupStandardFixture(l);
      custom::list<Spy>::Node* p1 = l.pHead;
      custom::list<Spy>::Node* p2 = p1->pNext;
      custom::list<Spy>::Node* p3 = p2->pNext;
      itErase.p = l.pHead;
      Spy::reset();
      // exercise
      itReturn = l.erase(itErase);
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [11]
      assertUnit(Spy::numDelete() == 1);       // free [11]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(itReturn.p == p2);
      //        pHead   pTail
      //       +----+   +----+
      //       | 26 | - | 31 |
      //       +----+   +----+
      //        itReturn
      assertUnit(l.pHead == p2);
      assertUnit(l.pTail == p3);
      assertUnit(l.numElements == 2);
      assertUnit(l.pHead != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(26));
         assertUnit(l.pHead->pNext == l.pTail);
         assertUnit(l.pHead->pNext == l.pTail);
         assertUnit(l.pHead->pNext != nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->data == Spy(31));
            assertUnit(l.pHead->pNext->pNext == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(l);
   }

   // erase the element from the middle of a list
   void test_erase_standardMiddle()
   {  // setup
      custom::list<Spy>::iterator itErase;
      custom::list<Spy>::iterator itReturn;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  itErase
      custom::list<Spy> l;
      setupStandardFixture(l);
      custom::list<Spy>::Node* p1 = l.pHead;
      custom::list<Spy>::Node* p2 = p1->pNext;
      custom::list<Spy>::Node* p3 = p2->pNext;
      itErase.p = p2;
      Spy::reset();
      // exercise
      itReturn = l.erase(itErase);
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [26]
      assertUnit(Spy::numDelete() == 1);       // free [26]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(itReturn.p == p3);
      //        pHead    pTail
      //       +----+   +----+
      //       | 11 | - | 31 |
      //       +----+   +----+
      //                  itReturn
      assertUnit(l.pHead == p1);
      assertUnit(l.pTail == p3);
      assertUnit(l.numElements == 2);
      assertUnit(l.pHead != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(11));
         assertUnit(l.pHead->pNext == l.pTail);
         assertUnit(l.pHead->pNext == l.pTail);
         assertUnit(l.pHead->pNext != nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->data == Spy(31));
            assertUnit(l.pHead->pNext->pNext == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(l);
   }

   // erase the element from the end of a list
   void test_erase_standardEnd()
   {  // setup
      custom::list<Spy>::iterator itErase;
      custom::list<Spy>::iterator itReturn;
      //         p1       p2       p3
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                           itErase
      custom::list<Spy> l;
      setupStandardFixture(l);
      custom::list<Spy>::Node* p1 = l.pHead;
      custom::list<Spy>::Node* p2 = p1->pNext;
      custom::list<Spy>::Node* p3 = p2->pNext;
      itErase.p = p3;
      Spy::reset();
      // exercise
      itReturn = l.erase(itErase);
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [31]
      assertUnit(Spy::numDelete() == 1);       // free [31]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(itReturn.p == NULL);
      assertUnit(itReturn == l.end());
      //        pHead    pTail
      //       +----+   +----+
      //       | 11 | - | 26 |
      //       +----+   +----+
      //                         itErase = NULL
      assertUnit(l.pHead == p1);
      assertUnit(l.pTail == p2);
      assertUnit(l.numElements == 2);
      assertUnit(l.pHead != nullptr);
      if (l.pHead)
      {
         assertUnit(l.pHead->data == Spy(11));
         assertUnit(l.pHead->pNext == l.pTail);
         assertUnit(l.pHead->pNext == l.pTail);
         assertUnit(l.pHead->pNext != nullptr);
         if (l.pHead->pNext)
         {
            assertUnit(l.pHead->pNext->data == Spy(26));
            assertUnit(l.pHead->pNext->pNext == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(l);
   }


   /***************************************
    * ITERATOR
    ***************************************/

   // test the iterator accessing the beginning of an empty list
   void test_iterator_begin_empty()
   {  // setup
      custom::list<Spy> l;
      custom::list<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = l.begin();
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
      assertUnit(it.p == nullptr);
      assertUnit(it == l.end());
      assertEmptyFixture(l);
   }  // teardown

   // test the iterator at the beginning of the standard fixture
   void test_iterator_begin_standard()
   {  // setup
      custom::list<Spy>::iterator it;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy::reset();
      // exercise
      it = l.begin();
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //         it
      assertUnit(it.p == l.pHead);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // test the iterator at the end of the standard fixture
   void test_iterator_end_standard()
   {  // setup
      custom::list<Spy>::iterator it;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::list<Spy> l;
      setupStandardFixture(l);
      Spy::reset();
      // exercise
      it = l.end();
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                                    it = NULL
      assertUnit(it.p == nullptr);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // test the iterator to increment from the middle of the standard fixture
   void test_iterator_incrementPost_standardMiddle()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead->pNext;
      Spy::reset();
      // exercise
      itReturn = it++;
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                           it
      //               itReturn
      assertUnit(it.p == l.pTail);
      assertUnit(itReturn.p == l.pHead->pNext);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // test the iterator to increment from the middle of the standard fixture
   void test_iterator_increment_standardMiddle()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead->pNext;
      Spy::reset();
      // exercise
      itReturn = ++it;
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                           it
      //                          itReturn
      assertUnit(it.p == l.pTail);
      assertUnit(itReturn.p == l.pTail);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // test the iterator to increment from the end of the standard fixture
   void test_iterator_increment_standardEnd()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                           it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead->pNext->pNext;
      Spy::reset();
      // exercise
      itReturn = ++it;
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                                  it
      //                                itReturn
      assertUnit(it.p == nullptr);
      assertUnit(itReturn.p == nullptr);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // test the iterator to increment from the middle of the standard fixture
   void test_iterator_decrementPost_standardMiddle()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead->pNext;
      Spy::reset();
      // exercise
      itReturn = it--;
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //        it
      //               itReturn
      assertUnit(it.p == l.pHead);
      assertUnit(itReturn.p == l.pHead->pNext);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // test the iterator to increment from the middle of the standard fixture
   void test_iterator_decrement_standardMiddle()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead->pNext;
      Spy::reset();
      // exercise
      itReturn = --it;
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //        it
      //      itReturn
      assertUnit(it.p == l.pHead);
      assertUnit(itReturn.p == l.pHead);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // test the iterator to increment from the end of the standard fixture
   void test_iterator_decrement_standardBegin()
   {  // setup
      custom::list<Spy>::iterator it;
      custom::list<Spy>::iterator itReturn;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //         it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead;
      Spy::reset();
      // exercise
      itReturn = --it;
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //  it
      //itReturn
      assertUnit(it.p == nullptr);
      assertUnit(itReturn.p == nullptr);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // the the iterator's dereference operator to access an item from the list
   void test_iterator_dereference_read()
   {  // setup
      custom::list<Spy>::iterator it;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead->pNext;
      Spy s(99);
      Spy::reset();
      // exercise
      s = *it;
      // verify
      assertUnit(Spy::numAssign() == 1);     // assigned [26]
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      assertUnit(s == Spy(26));
      assertUnit(it.p == l.pHead->pNext);
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   // the the iterator's dereference operator to update an item from the list
   void test_iterator_dereference_update()
   {  // setup
      custom::list<Spy>::iterator it;
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::list<Spy> l;
      setupStandardFixture(l);
      it.p = l.pHead->pNext;
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
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 99 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      assertUnit(l.pHead != nullptr);
      assertUnit(l.pHead->pNext != nullptr);
      if (l.pHead != nullptr && l.pHead->pNext != nullptr)
      {
         assertUnit(it.p == l.pHead->pNext);
         assertUnit(l.pHead->pNext->data == Spy(99));
         l.pHead->pNext->data = Spy(26);
      }
      assertStandardFixture(l);
      // teardown
      teardownStandardFixture(l);
   }

   /****************************************************************
    * Setup Standard Fixture
    *        pHead             pTail
    *       +----+   +----+   +----+
    *       | 11 | - | 26 | - | 31 |
    *       +----+   +----+   +----+
    ****************************************************************/
   void setupStandardFixture(custom::list<Spy>& l)
   {
      // allocat
      custom::list<Spy>::Node* p1 = new custom::list<Spy>::Node(Spy(11));
      custom::list<Spy>::Node* p2 = new custom::list<Spy>::Node(Spy(26));
      custom::list<Spy>::Node* p3 = new custom::list<Spy>::Node(Spy(31));

      // hook up pointers
      p1->pNext = p2;
      p2->pNext = p3;
      p3->pPrev = p2;
      p2->pPrev = p1;

      // set up the list
      l.pHead = p1;
      l.pTail = p3;
      l.numElements = 3;
   }

   /****************************************************************
    * Teardown Standard Fixture
    *        pHead             pTail
    *       +----+   +----+   +----+
    *       |    | - |    | - |    |
    *       +----+   +----+   +----+
    ****************************************************************/
   void teardownStandardFixture(custom::list<Spy>& l)
   {
      if (l.pHead != NULL)
      {
         if (l.pHead->pNext)
         {
            if (l.pHead->pNext->pNext)
            {
               if (l.pHead->pNext->pNext->pNext)
               {
                  if (l.pHead->pNext->pNext->pNext->pNext)
                     delete l.pHead->pNext->pNext->pNext->pNext;
                  delete l.pHead->pNext->pNext->pNext;
               }
               delete l.pHead->pNext->pNext;
            }
            delete l.pHead->pNext;
         }
         delete l.pHead;
         l.pHead = l.pTail = nullptr;
         l.numElements = 0;
      }
   }

   /****************************************************************
    * Verify Empty Fixture
    ****************************************************************/
   void assertEmptyFixtureParameters(const custom::list<Spy>& l, int line, const char* function)
   {
      // verify the member variables
      assertIndirect(l.numElements == 0);
      assertIndirect(l.pHead == nullptr);
      assertIndirect(l.pTail == nullptr);
   }

   /****************************************************************
    * Verify Standard Fixture
    *        pHead             pTail
    *       +----+   +----+   +----+
    *       | 11 | - | 26 | - | 31 |
    *       +----+   +----+   +----+
    ****************************************************************/
   void assertStandardFixtureParameters(const custom::list<Spy>& l, int line, const char* function)
   {
      // verify the member variables
      assertIndirect(l.numElements == 3);
      assertIndirect(l.pHead != nullptr);
      assertIndirect(l.pTail != nullptr);

      // verify the linked list 
      if (l.pHead)
      {
         assertIndirect(l.pHead->data == Spy(11));
         assertIndirect(l.pHead->pNext != nullptr);
         assertIndirect(l.pHead->pPrev == nullptr);
         if (l.pHead->pNext)
         {
            assertIndirect(l.pHead->pNext->data == Spy(26));
            assertIndirect(l.pHead->pNext->pNext == l.pTail);
            assertIndirect(l.pHead->pNext->pPrev == l.pHead);
            if (l.pHead->pNext->pNext)
            {
               assertIndirect(l.pHead->pNext->pNext->data == Spy(31));
               assertIndirect(l.pHead->pNext->pNext->pNext == nullptr);
               assertIndirect(l.pHead->pNext->pNext->pPrev != nullptr);
            }
         }
      }
   }


};

#endif // DEBUG
