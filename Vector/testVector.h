/***********************************************************************
 * Header:
 *    TEST VECTOR
 * Summary:
 *    Unit tests for vector
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include <vector>
#include "vector.h"
#include "unitTest.h"
#include "spy.h"

#include <cassert>
#include <memory>

class TestVector : public UnitTest
{

public:
   void run()
   {
      reset();
      
      // Construct
      test_construct_default();
      test_construct_sizeZero();
      test_construct_sizeFour();
      test_construct_sizeFourFill();
      test_constructCopy_empty();
      test_constructCopy_standard();
      test_constructCopy_partiallyFilled();
      test_constructMove_empty();
      test_constructMove_standard();
      test_constructMove_partiallyFilled();
      test_constructInit_empty();
      test_constructInit_standard();
      test_destructor_empty();
      test_destructor_standard();
      test_destructor_partiallyFilled();

      // Assign
      test_assign_empty();
      test_assign_sameSize();
      test_assign_rightBigger();
      test_assign_leftBigger();
      test_assignMove_empty();
      test_assignMove_sameSize();
      test_assignMove_rightBigger();
      test_assignMove_leftBigger();
      test_assign_fullToFull();
      test_assignMove_fullToFull();
      test_swap_empty();
      test_swap_sameSize();
      test_swap_rightBigger();
      test_swap_leftBigger();

      // Iterator
      test_iterator_beginEmpty();
      test_iterator_beginFull();
      test_iterator_endFull();
      test_iterator_incrementFull();
      test_iterator_dereferenceReadFull();
      test_iterator_dereferenceUpdate();
      test_iterator_construct_default();
      test_iterator_construct_pointer();
      test_iterator_construct_index();

      // Access
      test_subscript_read();
      test_subscript_write();
      test_front_read();
      test_front_write();
      test_back_read();
      test_back_write();
      test_back_partiallyfilled();

      // Insert
      test_pushback_empty();
      test_pushback_excessCapacity();
      test_pushback_requireReallocate();
      test_pushback_moveEmpty();
      test_pushback_moveExcessCapacity();
      test_pushback_moveRequireReallocate();
      test_resize_emptyZero();
      test_resize_emptyFourDefault();
      test_resize_emptyFourValue();
      test_resize_fourZero();
      test_resize_fourSixDefault();
      test_resize_fourSixValue();
      test_reserve_emptyZero();
      test_reserve_emptyTen();
      test_reserve_fourZero();
      test_reserve_fourFour();
      test_reserve_fourTen();
      test_reserve_standardZero();
      test_reserve_standardTen();

      // Remove
      test_popback_empty();
      test_popback_full();
      test_popback_partiallyFilled();
      test_clear_empty();
      test_clear_full();
      test_clear_partiallyFilled();
      test_shrink_empty();
      test_shrink_toEmpty();
      test_shrink_standard();
      test_shrink_twoExtraSlots();
      
      // Status
      test_size_empty();
      test_size_full();
      test_empty_empty();
      test_empty_full();
      test_capacity_empty();
      test_capacity_full();

      report("Vector");
   }
   
   /***************************************
    * CONSTRUCTOR
    ***************************************/
   
   // default constructor, no allocations
   void test_construct_default()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy> v;
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(v);
   }  // teardown

   // allocate space for zero
   void test_construct_sizeZero()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy> v(0);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(v);
   }  // teardown

   // allocate space for four
   void test_construct_sizeFour()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy> v(4);
      // verify
      assertUnit(Spy::numDefault() == 4);   // default-construct [00,00,00,00]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 00 | 00 | 00 | 00 |
      //    +----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data)
      { 
         assertUnit(v.data[0] == Spy());
         assertUnit(v.data[1] == Spy());
         assertUnit(v.data[2] == Spy());
         assertUnit(v.data[3] == Spy());
      }
      assertUnit(v.numElements == 4);
      assertUnit(v.numCapacity == 4);
      // teardown
      teardownStandardFixture(v);
   }

   // allocate space for four and fill with 10
   void test_construct_sizeFourFill()
   {  // setup
      Spy s(99);
      Spy::reset();
      // exercise
      custom::vector<Spy> v(4, s);
      // verify
      assertUnit(Spy::numCopy() == 4);        // copy-construct [99,99,99,99]
      assertUnit(Spy::numAlloc() == 4);       // allocate [99,99,99,99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data)
      { 
         assertUnit(v.data[0] == Spy(99));
         assertUnit(v.data[1] == Spy(99));
         assertUnit(v.data[2] == Spy(99));
         assertUnit(v.data[3] == Spy(99));
      }
      assertUnit(v.numElements == 4);
      assertUnit(v.numCapacity == 4);
      // teardown
      teardownStandardFixture(v);
   }


   /***************************************
    * DESTRUCTOR
    ***************************************/

   // destructor of an empty vector
   void test_destructor_empty()
   {  // setup
      {
         custom::vector<Spy> v;
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

   // destructor of a 4-element collection
   void test_destructor_standard()
   {  // setup
      {
         //      0    1    2    3
         //    +----+----+----+----+
         //    | 26 | 49 | 67 | 89 |
         //    +----+----+----+----+
         custom::vector<Spy> v;
         setupStandardFixture(v);
         Spy::reset();
      } // exercise
      // verify
      assertUnit(Spy::numDestructor() == 4); // destructor for [26,49,67,89]
      assertUnit(Spy::numDelete() == 4);     // delete [26,49,67,89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssign() == 0);
   }

   // destructor of a 2-element, 4-capacity collection
   void test_destructor_partiallyFilled()
   {  // setup
      {
         //      0    1    2    3
         //    +----+----+----+----+
         //    | 26 | 49 |    |    |
         //    +----+----+----+----+
         custom::vector<Spy> v;
         v.data = v.alloc.allocate(4);
         v.alloc.construct(&v.data[0], Spy(99));
         v.alloc.construct(&v.data[1], Spy(99));
         v.numElements = 2;
         v.numCapacity = 4;
         Spy::reset();
      }  // exercise
      assertUnit(Spy::numDestructor() == 2); // destructor for [26,49]
      assertUnit(Spy::numDelete() == 2);     // delete [26,49]
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
      custom::vector<Spy> vSrc;
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(vSrc);
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
      assertEmptyFixture(vSrc);
      assertEmptyFixture(vDest);
   }  // teardown
   
   // copy constructor of a 4-element collection
   void test_constructCopy_standard()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(vSrc);
      // verify
      assertUnit(Spy::numCopy() == 4);     // copy constructor for [26,49,67,89]
      assertUnit(Spy::numAlloc() == 4);    // allocate [26,49,67,89]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(vSrc.data != vDest.data);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   // copy constructor of a 2-element, 4-capacity collection
   void test_constructCopy_partiallyFilled()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      vSrc.data = vSrc.alloc.allocate(4);
      vSrc.alloc.construct(&vSrc.data[0], Spy(26));
      vSrc.alloc.construct(&vSrc.data[1], Spy(49));
      vSrc.numElements = 2;
      vSrc.numCapacity = 4;
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(vSrc);
      // verify
      assertUnit(Spy::numCopy() == 2);     // copy constructor [26,49]
      assertUnit(Spy::numAlloc() == 2);    // allocate [26,49]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 |    |    |
      //    +----+----+----+----+
      assertUnit(vSrc.data != nullptr);
      assertUnit(vSrc.data[0] == Spy(26));
      assertUnit(vSrc.data[1] == Spy(49));
      assertUnit(vSrc.numElements == 2);
      assertUnit(vSrc.numCapacity == 4);
      //      0    1
      //    +----+----+
      //    | 26 | 49 |
      //    +----+----+
      assertUnit(vDest.data != nullptr);
      if (vDest.data)
      {
         assertUnit(vDest.data[0] == Spy(26));
         assertUnit(vDest.data[1] == Spy(49));
      }
      assertUnit(vDest.numElements == 2);
      assertUnit(vDest.numCapacity == 2);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   /***************************************
    * MOVE CONSTRUCTOR
    ***************************************/
   
   // move constructor of an empty vector
   void test_constructMove_empty()
   {  // setup
      custom::vector<Spy> vSrc;
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(std::move(vSrc));
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
      assertEmptyFixture(vSrc);
      assertEmptyFixture(vDest);
   }  // teardown
   
   // move constructor of a 4-element collection
   void test_constructMove_standard()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      Spy * p = vSrc.data;
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(std::move(vSrc));
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
      assertEmptyFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      assertUnit(p == vDest.data);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   // move constructor of a 2-element, 4-capacity collection
   void test_constructMove_partiallyFilled()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      vSrc.data = vSrc.alloc.allocate(4);
      vSrc.alloc.construct(&vSrc.data[0], Spy(26));
      vSrc.alloc.construct(&vSrc.data[1], Spy(49));
      vSrc.numElements = 2;
      vSrc.numCapacity = 4;
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(std::move(vSrc));
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
      assertEmptyFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 |    |    |
      //    +----+----+----+----+
      assertUnit(vDest.data != nullptr);
      if (vDest.data)
      {
         assertUnit(vDest.data[0] == Spy(26));
         assertUnit(vDest.data[1] == Spy(49));
      }
      assertUnit(vDest.numElements == 2);
      assertUnit(vDest.numCapacity == 4);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   /***************************************
    * CONSTRUCTOR INITIALIZE LIST
    ***************************************/
   
   // empty initialization list
   void test_constructInit_empty()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy> v{};
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
      assertEmptyFixture(v);
   }  // teardown
   
   // use the initialization list to create the standard fixture
   void test_constructInit_standard()
   {  // setup
      std::initializer_list<Spy> l{Spy(26),Spy(49),Spy(67),Spy(89)};
      Spy::reset();
      // exercise
      custom::vector<Spy> v(l); // same as vector<Spy> v{...}
      // verify
      assertUnit(Spy::numCopy() == 4);       // copy-construct [26,49,67,89]
      assertUnit(Spy::numAlloc() == 4);      // allocate [26,49,67,89]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   /***************************************
    * RESIZE, RESERVE AND SHRINK-TO-FIT
    ***************************************/
   
   // resize an empty vector with zero elements
   void test_resize_emptyZero()
   {  // setup
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.resize(0);
      // verify
      assertUnit(v.numCapacity == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(v);
   }  // teardown
   
   // start with an empty vector and resize to four
   void test_resize_emptyFourDefault()
   {  // setup
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.resize(4);
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 00 | 00 | 00 | 00 |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 4);
      assertUnit(Spy::numDefault() == 4);    // default [00,00,00,00]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // resize four elements with the provided value
   void test_resize_emptyFourValue()
   {  // setup
      custom::vector<Spy> v;
      Spy s(99);
      Spy::reset();
      // exercise
      v.resize(4, s);
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 4);
      assertUnit(Spy::numCopy() == 4);     // copy [99,99,99,99]
      assertUnit(Spy::numAlloc() == 4);    // allocate [99,99,99,99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // resize the standard fixture to zero
   void test_resize_fourZero()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.resize(0);
      // verify
      assertUnit(Spy::numDestructor() == 4);  // destroy [26,49,67,89]
      assertUnit(Spy::numDelete() == 4);      // free [26,49,67,89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // grow the vector from 4 to 6 with default values
   void test_resize_fourSixDefault()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.resize(6);
      // verify
      assertUnit(Spy::numCopyMove() == 4);  // copy-move [26,49,67,89]
      assertUnit(Spy::numDestructor() == 4);// destory now-empty [26,49,67,89]
      assertUnit(Spy::numDefault() == 2);   // create new [00,00]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numCopy() == 0);      // copy 
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3    4    5
      //    +----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 89 | 00 | 00 |
      //    +----+----+----+----+----+----+
      assertUnit(v.numCapacity == 6);
      assertUnit(v.numElements == 6);
      if (v.data)
      {
         assertUnit(v.data[4] == Spy());
         assertUnit(v.data[5] == Spy());
      }
      v.numCapacity = 4;
      v.numElements = 4;
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // grow the vector from 4 to 6 with provided value
   void test_resize_fourSixValue()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      Spy s(99);
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.resize(6, s);
      // verify
      assertUnit(Spy::numCopyMove() == 4);  // copy-move [26,49,67,89]
      assertUnit(Spy::numDestructor() == 4);// destroy old and moved [26,49,67,89]
      assertUnit(Spy::numCopy() == 2);      // copy-create [99,99]
      assertUnit(Spy::numAlloc() == 2);     // allocate [99,99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3    4    5
      //    +----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 89 | 99 | 99 |
      //    +----+----+----+----+----+----+
      assertUnit(v.numCapacity == 6);
      assertUnit(v.numElements == 6);
      if (v.data && v.numElements >= 6)
      {
         assertUnit(v.data[4] == Spy(99));
         assertUnit(v.data[5] == Spy(99));
      }
      v.numCapacity = 4;
      v.numElements = 4;
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // reserve zero on an empty vector
   void test_reserve_emptyZero()
   {  // setup
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.reserve(0);
      // verify
      assertUnit(v.numCapacity == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(v);
   }  // teardown
   
   // increase the capacity on an empty vector to ten
   void test_reserve_emptyTen()
   {  // setup
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.reserve(10);
      // verify
      //      0    1    2    3    4    5    6    7    8    9
      //    +----+----+----+----+----+----+----+----+----+----+
      //    |    |    |    |    |    |    |    |    |    |    |
      //    +----+----+----+----+----+----+----+----+----+----+
      assertUnit(v.numCapacity == 10);
      assertUnit(v.numElements == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // teardown
      teardownStandardFixture(v);
   }

   // try to decrease the capacity. Nothing changes!
   void test_reserve_fourZero()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.numElements = 0;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.reserve(0);
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
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // change the capacity to its current size
   void test_reserve_fourFour()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.numElements = 0;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.reserve(4);
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
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // increase the capacity
   void test_reserve_fourTen()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.numElements = 0;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.reserve(10);
      // verify
      assertUnit(Spy::numCopyMove() == 0);   
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3    4    5    6    7    8    9
      //    +----+----+----+----+----+----+----+----+----+----+
      //    |    |    |    |    |    |    |    |    |    |    |
      //    +----+----+----+----+----+----+----+----+----+----+
      assertUnit(v.numCapacity == 10);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // attempt to reserve 0 when we already have four elements
   void test_reserve_standardZero()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.reserve(0);
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
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // reserve six extra spots from the standard fixture
   void test_reserve_standardTen()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.reserve(10);
      // verify
      assertUnit(v.numCapacity == 10);
      assertUnit(Spy::numCopyMove() == 4);   // copy-move [26,49,67,89]
      assertUnit(Spy::numDestructor() == 4); // destroy the now-empty [26,49,67,89]
      assertUnit(Spy::numCopy() == 0);  
      assertUnit(Spy::numAlloc() == 0); 
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3    4    5    6    7    8    9
      //    +----+----+----+----+----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 89 |    |    |    |    |    |    |
      //    +----+----+----+----+----+----+----+----+----+----+
      v.numCapacity = 4;
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // shrink an empty fixture
   void test_shrink_empty()
   {  // setup
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.shrink_to_fit();
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
      assertEmptyFixture(v);
   }  // teardown
   
   // remove extra capacity when there are no elements
   void test_shrink_toEmpty()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.numElements = 0;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.shrink_to_fit();
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
      assertEmptyFixture(v);
   }  // teardown
   
   // attempt to shrink when the vector is filled
   void test_shrink_standard()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.shrink_to_fit();
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
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // shrink a vector with two extra slots
   void test_shrink_twoExtraSlots()
   {  // setup
      //      0    1    2    3    4    6
      //    +----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 89 |    |    |
      //    +----+----+----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(6);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.alloc.construct(&v.data[2], Spy(67));
      v.alloc.construct(&v.data[3], Spy(89));
      v.numElements = 4;
      v.numCapacity = 6;
      Spy::reset();
      // exercise
      std::vector<Spy> vS{Spy(26),Spy(49),Spy(67),Spy(89)};
      vS.reserve(6);
      Spy::reset();
      vS.shrink_to_fit();
      Spy::reset();
      v.shrink_to_fit();
      // verify
      assertUnit(Spy::numCopy() == 4);      // copy [26,49,67,89] to new buffer
      assertUnit(Spy::numAlloc() == 4);     // allocate [26,49,67,89]
      assertUnit(Spy::numDestructor() == 4);// destroy [26,49,67,89] from old buffer
      assertUnit(Spy::numDelete() == 4);    // delete [26,49,67,89]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   /***************************************
    * SIZE EMPTY CAPACITY
    ***************************************/

   // size of an empty vector
   void test_size_empty()
   {  // setup
      custom::vector<Spy> v;
      // exercise
      size_t size = v.size();
      // verify
      assertUnit(0 == size);
      assertEmptyFixture(v);
   }  // teardown
   
   // size of a full vector
   void test_size_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // exercise
      size_t size = v.size();
      // verify
      assertUnit(4 == size);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // empty vector empty?
   void test_empty_empty()
   {  // setup
      custom::vector<Spy> v;
      // exercise
      bool empty = v.empty();
      // verify
      assertUnit(true == empty);
      assertEmptyFixture(v);
   }  // teardown
   
   // full vector empty?
   void test_empty_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // exercise
      bool empty = v.empty();
      // verify
      assertUnit(false == empty);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // size of an empty vector
   void test_capacity_empty()
   {  // setup
      custom::vector<Spy> v;
      // exercise
      size_t capacity = v.capacity();
      // verify
      assertUnit(0 == capacity);
      assertEmptyFixture(v);
   }  // teardown
   
   // size of a full vector
   void test_capacity_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      v.numElements = 3;
      // exercise
      size_t capacity = v.capacity();
      // verify
      assertUnit(4 == capacity);
      v.numElements = 4;
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   /***************************************
    * SWAP
    ***************************************/

    // swap empty vectors
   void test_swap_empty()
   {  // setup
      custom::vector<Spy> vSrc;
      custom::vector<Spy> vDest;
      Spy::reset();
      // exercise
      vDest.swap(vSrc);
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
      assertEmptyFixture(vSrc);
      assertEmptyFixture(vDest);
   }  // teardown

   // swap when source and destination are same size
   void test_swap_sameSize()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      vDest.data[0] = Spy(99);
      vDest.data[1] = Spy(99);
      vDest.data[2] = Spy(99);
      vDest.data[3] = Spy(99);
      Spy::reset();
      // exercise
      vDest.swap(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numAssign() == 0);   
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      assertUnit(vSrc.numCapacity == 4);
      assertUnit(vSrc.numElements == 4);
      assertUnit(vSrc.data != nullptr);
      if (vSrc.data)
      {
         assertUnit(vSrc.data[0] == Spy(99));
         assertUnit(vSrc.data[1] == Spy(99));
         assertUnit(vSrc.data[2] == Spy(99));
         assertUnit(vSrc.data[3] == Spy(99));
      }
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // swap when the destination is smaller than the source
   void test_swap_rightBigger()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vDest;
      vDest.data = vDest.alloc.allocate(2);
      vDest.alloc.construct(&vDest.data[0], Spy(99));
      vDest.alloc.construct(&vDest.data[1], Spy(99));
      vDest.numElements = 2;
      vDest.numCapacity = 2;
      Spy::reset();
      // exercise
      vDest.swap(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numCopy() == 0);       
      assertUnit(Spy::numAlloc() == 0);      
      assertUnit(Spy::numDestructor() == 0); 
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      assertUnit(vSrc.numCapacity == 2);
      assertUnit(vSrc.numElements == 2);
      assertUnit(vSrc.data != nullptr);
      if (vSrc.data)
      {
         assertUnit(vSrc.data[0] == Spy(99));
         assertUnit(vSrc.data[1] == Spy(99));
      }
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // swap when the destination is bigger than the source
   void test_swap_leftBigger()
   {  // setup
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vSrc;
      vSrc.data = vSrc.alloc.allocate(2);
      vSrc.alloc.construct(&vSrc.data[0], Spy(99));
      vSrc.alloc.construct(&vSrc.data[1], Spy(99));
      vSrc.numElements = 2;
      vSrc.numCapacity = 2;
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      Spy::reset();
      // exercise
      vDest.swap(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numAssign() == 0); 
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);    
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vSrc);
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      assertUnit(vDest.numCapacity == 2);
      assertUnit(vDest.numElements == 2);
      assertUnit(vDest.data != nullptr);
      if (vDest.data)
      {
         assertUnit(vDest.data[0] == Spy(99));
         assertUnit(vDest.data[1] == Spy(99));
      }
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }


   /***************************************
    * ASSIGN 
    ***************************************/
   
   // assignment when there is nothing to copy
   void test_assign_empty()
   {  // setup
      custom::vector<Spy> vSrc;
      custom::vector<Spy> vDest;
      Spy::reset();
      // exercise
      vDest = vSrc;
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
      assertEmptyFixture(vSrc);
      assertEmptyFixture(vDest);
   }  // teardown
   
   // assignment when source and destination are same size
   void test_assign_sameSize()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      vDest.data[0] = Spy(99);
      vDest.data[1] = Spy(99);
      vDest.data[2] = Spy(99);
      vDest.data[3] = Spy(99);
      Spy::reset();
      // exercise
      vDest = vSrc;
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numAssign() == 4);      // assign [26,49,67,89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   // assignment when the destination is smaller than the source
   void test_assign_rightBigger()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vDest;
      vDest.data = vDest.alloc.allocate(2);
      vDest.alloc.construct(&vDest.data[0], Spy(99));
      vDest.alloc.construct(&vDest.data[1], Spy(99));
      vDest.numElements = 2;
      vDest.numCapacity = 2;
      Spy::reset();
      // exercise
      vDest = vSrc;
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numCopy() == 4);       // copy [26,49,67,89] from src to dest
      assertUnit(Spy::numAlloc() == 4);      // the copy involves a new buffer for [26,49,67,89]
      assertUnit(Spy::numDestructor() == 2); // destroy [99,99]
      assertUnit(Spy::numDelete() == 2);     // delete [99,99]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   // assignment when the destination is bigger than the source
   void test_assign_leftBigger()
   {  // setup
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vSrc;
      vSrc.data = vSrc.alloc.allocate(2);
      vSrc.alloc.construct(&vSrc.data[0], Spy(99));
      vSrc.alloc.construct(&vSrc.data[1], Spy(99));
      vSrc.numElements = 2;
      vSrc.numCapacity = 2;
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      Spy::reset();
      // exercise
      vDest = vSrc;
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numAssign() == 2);     // assign [99,99] into dest
      assertUnit(Spy::numDestructor() == 2); // destroy [67,89]
      assertUnit(Spy::numDelete() == 2);     // delete [67,89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      assertUnit(vSrc.numCapacity == 2);
      assertUnit(vSrc.numElements == 2);
      assertUnit(vSrc.data != nullptr);
      if (vSrc.data)
      {
         assertUnit(vSrc.data[0] == Spy(99));
         assertUnit(vSrc.data[1] == Spy(99));
      }
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 |    |    |
      //    +----+----+----+----+
      assertUnit(vDest.numCapacity == 4);
      assertUnit(vDest.numElements == 2);
      assertUnit(vDest.data != nullptr);
      if (vDest.data)
      {
         assertUnit(vDest.data[0] == Spy(99));
         assertUnit(vDest.data[1] == Spy(99));
      }
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // assignment when there is nothing to copy
   void test_assignMove_empty()
   {  // setup
      custom::vector<Spy> vSrc;
      custom::vector<Spy> vDest;
      Spy::reset();
      // exercise
      vDest = std::move(vSrc);
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
      assertEmptyFixture(vSrc);
      assertEmptyFixture(vDest);
   }  // teardown

   // assignment when source and destination are same size
   void test_assignMove_sameSize()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      vDest.data[0] = Spy(99);
      vDest.data[1] = Spy(99);
      vDest.data[2] = Spy(99);
      vDest.data[3] = Spy(99);
      Spy::reset();
      // exercise
      vDest = std::move(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numDestructor() == 4);  // destroy [99,99,99,99]
      assertUnit(Spy::numDelete() == 4);      // delete  [99,99,99,99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // assignment when the destination is smaller than the source
   void test_assignMove_rightBigger()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vDest;
      vDest.data = vDest.alloc.allocate(2);
      vDest.alloc.construct(&vDest.data[0], Spy(99));
      vDest.alloc.construct(&vDest.data[1], Spy(99));
      vDest.numElements = 2;
      vDest.numCapacity = 2;
      Spy::reset();
      // exercise
      vDest = std::move(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numDestructor() == 2); // destroy [99,99]
      assertUnit(Spy::numDelete() == 2);     // delete [99,99]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);     
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(vSrc);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // assignment when the destination is bigger than the source
   void test_assignMove_leftBigger()
   {  // setup
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vSrc;
      vSrc.data = vSrc.alloc.allocate(2);
      vSrc.alloc.construct(&vSrc.data[0], Spy(99));
      vSrc.alloc.construct(&vSrc.data[1], Spy(99));
      vSrc.numElements = 2;
      vSrc.numCapacity = 2;
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      Spy::reset();
      // exercise
      vDest = std::move(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numDestructor() == 4); // destroy [26,49,67,89]
      assertUnit(Spy::numDelete() == 4);     // delete [26,49,67,89]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      assertUnit(vDest.numCapacity == 2);
      assertUnit(vDest.numElements == 2);
      assertUnit(vDest.data != nullptr);
      if (vDest.data)
      {
         assertUnit(vDest.data[0] == Spy(99));
         assertUnit(vDest.data[1] == Spy(99));
      }
      assertEmptyFixture(vSrc);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // assignment when destination has same capacity
   void test_assign_fullToFull()
   {  // Setup
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      //    +----+----+----+----+
      //    | 11 | 99 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> vDes;
      try
      {
         vDes.data = vDes.alloc.allocate(4);
         vDes.alloc.construct(&vDes.data[0], Spy(11));
         vDes.alloc.construct(&vDes.data[1], Spy(99));
         vDes.numElements = 2;
         vDes.numCapacity = 4;
      }
      catch (...)
      {
         assert(false);
      }
      Spy::reset();
      // exercise
      vDes = vSrc;
      // verify
      assertUnit(Spy::numAssign() == 2); // 26->11 49->99
      assertUnit(Spy::numCopy() == 2);  // [67,89]
      assertUnit(Spy::numAlloc() == 2); // [67,89]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertStandardFixture(vSrc);
      assertStandardFixture(vDes);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDes);
   }

   // assignment when destination has same capacity
   void test_assignMove_fullToFull()
   {  // setup
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      //    +----+----+----+----+
      //    | 11 | 99 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> vDes;
      try
      {
         vDes.data = vDes.alloc.allocate(4);
         vDes.alloc.construct(&vDes.data[0], Spy(11));
         vDes.alloc.construct(&vDes.data[1], Spy(99));
         vDes.numElements = 2;
         vDes.numCapacity = 4;
      }
      catch (...)
      {
         assert(false);
      }
      Spy::reset();
      // exercise
      vDes = std::move(vSrc);
      // verify
      assertUnit(Spy::numDestructor() == 2); // [11,99]
      assertUnit(Spy::numDelete() == 2); // [11,99]
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(vSrc);
      assertStandardFixture(vDes);
      // teardown
      teardownStandardFixture(vDes);
   }

   /***************************************
    * SUBSCRIPT
    ***************************************/

   // read one element using square bracket
   void test_subscript_read()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      const custom::vector<Spy> v(vSrc);
      Spy value(99);
      Spy::reset();
      // exercise
      value = v[1];
      // verify
      assertUnit(Spy::numAssign() == 1);    // from assigning [49] into value
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(value == Spy(49));
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(vSrc);
   }


   // write one element using square brackets
   void test_subscript_write()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy value(99);
      Spy::reset();
      // exercise
      v[1] = value;
      // verify
      assertUnit(Spy::numAssign() == 1);  // 99 is set to v[1]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 99 | 67 | 89 |
      //    +----+----+----+----+
      assertUnit(v.data[1] == Spy(99));
      v.data[1] = Spy(49);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // verify that we can look at the front of a fector
   void test_front_read()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      const custom::vector<Spy> v(vSrc);
      Spy value(99);
      Spy::reset();
      // exercise
      value = v.front();
      // verify
      assertUnit(Spy::numAssign() == 1);    // from assigning [26] into value
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(value == Spy(26));
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(vSrc);
   }

   // write to the front
   void test_front_write()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy value(99);
      Spy::reset();
      // exercise
      v.front() = value;
      // verify
      assertUnit(Spy::numAssign() == 1);  // 99 is set to v[1]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data)
      { 
         assertUnit(v.data[0] == Spy(99));
         v.data[0] = Spy(26);
      }
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // verify we can look at the back of a filled vector
   void test_back_read()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      const custom::vector<Spy> v(vSrc);
      Spy value(99);
      Spy::reset();
      // exercise
      value = v.back();
      // verify
      assertUnit(Spy::numAssign() == 1);    // from assigning [26] into value
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(value == Spy(89));
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(vSrc);
   }
   // verify we can look at the back of a filled vector
   void test_back_write()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy value(99);
      Spy::reset();
      // exercise
      v.back() = value;
      // verify
      assertUnit(Spy::numAssign() == 1);  // 99 is set to v[1]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 99 |
      //    +----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data)
      {
         assertUnit(v.data[3] == Spy(99));
         v.data[3] = Spy(89);
      }
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // verify we can look at the back of a filled vector
   void test_back_partiallyfilled()
   {
      // setup
      //    +----+----+----+----+
      //    | 11 | 22 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      try
      {
         v.data = v.alloc.allocate(4);
         v.alloc.construct(&v.data[0], Spy(11));
         v.alloc.construct(&v.data[1], Spy(22));
         v.numElements = 2;
         v.numCapacity = 4;
      }
      catch (...)
      {
         assert(false);
      }
      Spy value(99);
      Spy::reset();
      // exercise
      value = v.back();
      // verify
      assertUnit(Spy::numAssign() == 1);    // from assigning [22] into value
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(value == Spy(22));
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 2);
      assertUnit(v.data != nullptr);
      if (v.data)
      {
         assertUnit(v.data[0] == Spy(11));
         assertUnit(v.data[1] == Spy(22));
      }
      // teardown
      teardownStandardFixture(v);
   }


   /***************************************
    * POP BACK
    ***************************************/

   // remove an element from an empty vector
   void test_popback_empty()
   {  // setup
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.pop_back();
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
      assertEmptyFixture(v);
   }  // teardown

   // popback when there are elements
   void test_popback_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.pop_back();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [89]
      assertUnit(Spy::numDelete() == 1);       // delete [89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 3);
      assertUnit(v.data != nullptr);
      if (v.data != nullptr)
      {
         assertUnit(v.data[0] == Spy(26));
         assertUnit(v.data[1] == Spy(49));
         assertUnit(v.data[2] == Spy(67));
      }
      // teardown
      teardownStandardFixture(v);
   }

   // pop-back when there are elements
   void test_popback_partiallyFilled()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.numElements = 2;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.pop_back();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [49]
      assertUnit(Spy::numDelete() == 1);       // delete [49]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 1);
      assertUnit(v.data != nullptr);
      if (v.data != nullptr)
      {
         assertUnit(v.data[0] == Spy(26));
      }      // teardown
      teardownStandardFixture(v);
   }

   /***************************************
    * CLEAR
    ***************************************/
   
   // clear an empty collection
   void test_clear_empty()
   {  // setup
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.clear();
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
      assertEmptyFixture(v);
   }  // teardown

   // clear when there are elements
   void test_clear_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.clear();
      // verify
      assertUnit(Spy::numDestructor() == 4);   // destroy [26,49,67,89]
      assertUnit(Spy::numDelete() == 4);       // delete [26,49,67,89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 0);
      assertUnit(v.data != nullptr);
      // teardown
      teardownStandardFixture(v);
   }
   
   // clear when there are elements
   void test_clear_partiallyFilled()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.numElements = 2;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.clear();
      // verify
      assertUnit(Spy::numDestructor() == 2);   // destroy [26,49]
      assertUnit(Spy::numDelete() == 2);       // delete [26,49]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 0);
      assertUnit(v.data != nullptr);
      // teardown
      teardownStandardFixture(v);
   }

   
   /***************************************
    * PUSH BACK
    ***************************************/
   
   // add an element to the back when empty
   void test_pushback_empty()
   {  // setup
      custom::vector<Spy> v;
      Spy s(99);
      Spy::reset();
      // exercise
      v.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);           // copy [99]
      assertUnit(Spy::numAlloc() == 1);          // allocate [99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    
      //    +----+
      //    | 99 |
      //    +----+
      assertUnit(v.data != nullptr);
      if (v.data)
         assertUnit(v.data[0] == Spy(99));
      assertUnit(v.numCapacity == 1);
      assertUnit(v.numElements == 1);
      // teardown
      teardownStandardFixture(v);
   }

   // add an element to the back when there is room. No reallocation
   void test_pushback_excessCapacity()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.alloc.construct(&v.data[2], Spy(67));
      v.numElements = 3;
      v.numCapacity = 4;
      Spy s(89);
      Spy::reset();
      // exercise
      v.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);           // copy [99]
      assertUnit(Spy::numAlloc() == 1);          // allocate [99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 99 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // add an element to the back when there is not room. Capacity should double
   void test_pushback_requireReallocate()
   {  // setup
      //      0    1    2  
      //    +----+----+----+
      //    | 26 | 49 | 67 |
      //    +----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(3);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.alloc.construct(&v.data[2], Spy(67));
      v.numElements = 3;
      v.numCapacity = 3;
      Spy s(99);
      Spy::reset();
      // exercise
      v.push_back(s);
      // verify
      assertUnit(Spy::numCopyMove() == 3);       // move [26,49,67] 
      assertUnit(Spy::numDestructor() == 3);     // destroy empty [26,49,67]
      assertUnit(Spy::numCopy() == 1);           // copy [99]
      assertUnit(Spy::numAlloc() == 1);          // allocate [99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3    4    5   
      //    +----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 99 |    |    |
      //    +----+----+----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data && v.numElements >= 4)
      {
         assertUnit(v.data[0] == Spy(26));
         assertUnit(v.data[1] == Spy(49));
         assertUnit(v.data[2] == Spy(67));
         assertUnit(v.data[3] == Spy(99));
      }
      assertUnit(v.numCapacity == 6);
      assertUnit(v.numElements == 4);
      // teardown
      teardownStandardFixture(v);
   }

   // add an element to the back when empty
   void test_pushback_moveEmpty()
   {  // setup
      custom::vector<Spy> v;
      Spy s(99);
      Spy::reset();
      // exercise
      v.push_back(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);       // copy-move [99]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);       
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    
      //    +----+
      //    | 99 |
      //    +----+
      assertUnit(v.data != nullptr);
      if (v.data)
         assertUnit(v.data[0] == Spy(99));
      assertUnit(v.numCapacity == 1);
      assertUnit(v.numElements == 1);
      assertUnit(s == Spy());
      // teardown
      teardownStandardFixture(v);
   }

   // add an element to the back when there is room. No reallocation
   void test_pushback_moveExcessCapacity()
   {  // setup
     //      0    1    2    3
     //    +----+----+----+----+
     //    | 26 | 49 | 67 |    |
     //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.alloc.construct(&v.data[2], Spy(67));
      v.numElements = 3;
      v.numCapacity = 4;
      Spy s(89);
      Spy::reset();
      // exercise
      v.push_back(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);   // copy-move [89] 
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);         
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 99 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // add an element to the back when there is not room. Capacity should double
   void test_pushback_moveRequireReallocate()
   {  // setup
      //      0    1    2  
      //    +----+----+----+
      //    | 26 | 49 | 67 |
      //    +----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(3);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.alloc.construct(&v.data[2], Spy(67));
      v.numElements = 3;
      v.numCapacity = 3;
      Spy s(99);
      Spy::reset();
      // exercise
      v.push_back(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 4);       // move [26,49,67] and [99]
      assertUnit(Spy::numDestructor() == 3);     // destroy empty [26,49,67]
      assertUnit(Spy::numCopy() == 0);           
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3    4    5   
      //    +----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 99 |    |    |
      //    +----+----+----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data && v.numElements >= 4)
      {
         assertUnit(v.data[0] == Spy(26));
         assertUnit(v.data[1] == Spy(49));
         assertUnit(v.data[2] == Spy(67));
         assertUnit(v.data[3] == Spy(99));
      }
      assertUnit(v.numCapacity == 6);
      assertUnit(v.numElements == 4);
      // teardown
      teardownStandardFixture(v);
   }


   /***************************************
    * ITERATOR
    ***************************************/
   
   // empty iterator
   void test_iterator_beginEmpty()
   {  // setup
      custom::vector<Spy> v;
      // exercise
      custom::vector<Spy>::iterator it = v.begin();
      // verify
      assertUnit(it.p == nullptr);
      assertEmptyFixture(v);
   }  // teardown
  
   // iterator the first element
   void test_iterator_beginFull()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // exercise
      custom::vector<Spy>::iterator it = v.begin();
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //      it
      assertUnit(v.data != nullptr);
      assertUnit(it.p != nullptr);
      if (v.data && it.p)
      {
         assertUnit(it.p == &(v.data[0]));
         assertUnit(*(it.p) == 26);
      }
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // iterator the end() method
   void test_iterator_endFull()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // exercise
      custom::vector<Spy>::iterator it = v.end();
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                           it
      assertUnit(it.p == &(v.data[4]));
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // iterator use the decrement operator
   void test_iterator_incrementFull()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //           it
      custom::vector<Spy> v;
      setupStandardFixture(v);
      custom::vector<Spy>::iterator it;
      it.p = &(v.data[1]);
      // exercise
      ++it;
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                it
      assertUnit(*(it.p) == Spy(67));
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // iterator use the decrement operator
   void test_iterator_dereferenceReadFull()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //           it
      custom::vector<Spy> v;
      setupStandardFixture(v);
      custom::vector<Spy>::iterator it;
      it.p = &(v.data[1]);
      // exercise
      Spy value = *it;
      // verify
      assertUnit(value == Spy(49));
      assertUnit(it.p == &(v.data[1]));
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // iterator change a value
   void test_iterator_dereferenceUpdate()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //           it
      custom::vector<Spy> v;
      setupStandardFixture(v);
      custom::vector<Spy>::iterator it;
      it.p = &(v.data[1]);
      // exercise
      *it = Spy(99);
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 99 | 67 | 89 |
      //    +----+----+----+----+
      //           it
      assertUnit(v.data != nullptr);
      if (v.data)
      {
         assertUnit(v.data[0] == Spy(26));
         assertUnit(v.data[1] == Spy(99));
         assertUnit(v.data[2] == Spy(67));
         assertUnit(v.data[3] == Spy(89));
         assertUnit(it.p == &(v.data[1]));
      }
      assertUnit(v.numElements == 4);
      assertUnit(v.numCapacity == 4);
      // teardown
      teardownStandardFixture(v);
   }

   // create a default iterator
   void test_iterator_construct_default()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy>::iterator it;
      // verify
      assertUnit(it.p == nullptr);
      assertUnit(Spy::numCopyMove() == 0); 
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
   }  // teardown

   // create an iterator at a specific location in the vector
   void test_iterator_construct_pointer()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                it
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      custom::vector<Spy>::iterator it(v.data + 2);
      // verify
      assertUnit(it.p == v.data + 2);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // create an iterator based on a vector and an index
   void test_iterator_construct_index()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                it
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      custom::vector<Spy>::iterator it(2, v);
      // verify
      assertUnit(it.p == v.data + 2);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   /*************************************************************
    * SETUP STANDARD FIXTURE
    *      0    1    2    3
    *    +----+----+----+----+
    *    | 26 | 49 | 67 | 89 |
    *    +----+----+----+----+
    *************************************************************/
   void setupStandardFixture(custom::vector<Spy>& v)
   {
      try
      {
         v.data = v.alloc.allocate(4);
         v.alloc.construct(&v.data[0], Spy(26));
         v.alloc.construct(&v.data[1], Spy(49));
         v.alloc.construct(&v.data[2], Spy(67));
         v.alloc.construct(&v.data[3], Spy(89));
         v.numElements = 4;
         v.numCapacity = 4;
      }
      catch (...)
      {
         assert(false);
      }
   }

   /*************************************************************
    * VERIFY STANDARD FIXTURE PARAMETERS
    *      0    1    2    3
    *    +----+----+----+----+
    *    | 26 | 49 | 67 | 89 |
    *    +----+----+----+----+
    *************************************************************/
   void assertStandardFixtureParameters(const custom::vector<Spy>& v, int line, const char* function)
   {
      assertIndirect(v.data != nullptr);
      assertIndirect(v.numCapacity == 4);
      assertIndirect(v.numElements == 4);

      if (v.data != nullptr && v.numElements >= 4)
      {
         assertIndirect(v.data[0] == Spy(26));
         assertIndirect(v.data[1] == Spy(49));
         assertIndirect(v.data[2] == Spy(67));
         assertIndirect(v.data[3] == Spy(89));
      }
   }
   
   /*************************************************************
    * VERIFY EMPTY FIXTURE PARAMETERS
    *************************************************************/
   void assertEmptyFixtureParameters(const custom::vector<Spy>& v, int line, const char* function)
   {
      assertIndirect(v.data == nullptr);
      assertIndirect(v.numCapacity == 0);
      assertIndirect(v.numElements == 0);
   }
   
   /*************************************************************
    * TEARDOWN STANDARD FIXTURE
    *************************************************************/
   void teardownStandardFixture(custom::vector<Spy>&v)
   {
      if (v.data != nullptr && false)
      {
         for (size_t i = 0; i < v.numElements; i++)
            v.alloc.destroy(&v.data[i]);
         v.alloc.deallocate(v.data, v.numCapacity);

      }
         v.data = nullptr;
         v.numElements = v.numCapacity = 0;
   }



};

#endif // DEBUG
