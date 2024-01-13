/***********************************************************************
 * Header:
 *    TEST SET
 * Summary:
 *    Unit tests for set
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once


#ifdef DEBUG

#include "set.h"
#include "unitTest.h"
#include "spy.h"
#include <set>
#include <vector>


#include <iostream>
#include <cassert>
#include <memory>

class TestSet : public UnitTest
{
public:
   void run()
   {
      reset();

      // Construct
      test_construct_default();
      test_constructCopy_empty();
      test_constructCopy_one();
      test_constructCopy_standard();
      test_constructMove_empty();
      test_constructMove_one();
      test_constructMove_standard();
      test_constructInit_empty();
      test_constructInit_one();
      test_constructInit_standard();
      test_constructRange_empty();
      test_constructRange_one();
      test_constructRange_standard();
      test_destructor_empty();
      test_destructor_standard();

      // Assign
      test_assign_emptyToEmpty();
      test_assign_standardToEmpty();
      test_assign_emptyToStandard();
      test_assign_oneToStandard();
      test_assign_standardToOne();
      test_assign_standardToStandard();
      test_assignMove_emptyToEmpty();
      test_assignMove_standardToEmpty();
      test_assignMove_emptyToStandard();
      test_assignMove_oneToStandard();
      test_assignMove_standardToOne();
      test_assignMove_standardToStandard();
      test_assignInit_emptyToEmpty();
      test_assignInit_standardToEmpty();
      test_assignInit_emptyToStandard();
      test_assignInit_oneToStandard();
      test_assignInit_standardToOne();
      test_assignInit_standardToStandard();
      test_swap_emptyToEmpty();
      test_swap_standardToEmpty();
      test_swap_emptyToStandard();
      test_swap_standardToStandard();

      // Iterator
      test_begin_empty();
      test_begin_standard();
      test_end_standard();
      test_iterator_increment_standardToParent();
      test_iterator_increment_standardToChild();
      test_iterator_increment_standardToGrandma();
      test_iterator_increment_standardToGrandchild();
      test_iterator_increment_standardToDone();
      test_iterator_increment_standardEnd();
      test_iterator_dereference_standardRead();

      // Access
      test_find_empty();
      test_find_standardBegin();
      test_find_standardLast();
      test_find_standardMissing();

      // Insert
      test_insert_empty();
      test_insert_standardEnd();
      test_insert_standardFront();
      test_insert_standardMiddle();
      test_insert_standardDuplicate();
      test_insertMove_empty();
      test_insertMove_standardEnd();
      test_insertMove_standardFront();
      test_insertMove_standardMiddle();
      test_insertMove_standardDuplicate();
      test_insertInit_emptyInsertNone();
      test_insertInit_emptyInsertMany();
      test_insertInit_standardInsertNone();
      test_insertInit_standardInsertDuplicates();
      test_insertInit_manyInsertMany();

      // Remove
      test_clear_empty();
      test_clear_standard();
      test_eraseRange_several();
      test_eraseIterator_empty();
      test_eraseIterator_standardMissing();
      test_eraseIterator_noChildren();
      test_eraseIterator_oneChild();
      test_eraseIterator_twoChildren();
      test_eraseValue_empty();
      test_eraseValue_standardMissing();
      test_eraseValue_noChildren();
      test_eraseValue_oneChild();
      test_eraseValue_twoChildren();
      test_eraseRange_standardNone();
      test_eraseRange_standardOne();
      test_eraseRange_standardMany();
      test_eraseRange_oneChild();
      test_eraseRange_twoChildren();

      // Status
      test_empty_empty();
      test_empty_standard();
      test_size_empty();
      test_size_standard();

      report("Set");
   }
   
   /***************************************
    * CONSTRUCTORS
    ***************************************/
   // default constructor, no allocaations
   void test_construct_default() 
   {  // setup
      Spy::reset();
      // exercise
      custom::set<Spy> s;
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(s);
   }  // teardown


   /***************************************
    * COPY
    ***************************************/

    // copy an empty set
   void test_constructCopy_empty()
   {  // setup
      custom::set<Spy> sSrc;
      Spy::reset();
      // exercise
      custom::set<Spy> sDest(sSrc);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(sSrc);
      assertEmptyFixture(sDest);
   }  // teardown

   // copy a set that has one element
   void test_constructCopy_one()
   {  // setup
      //            (50b)
      custom::set<Spy> sSrc;
      sSrc.bst.root = new custom::BST<Spy>::BNode(Spy(50));
      sSrc.bst.root->isRed = false;
      sSrc.bst.numElements = 1;
      Spy::reset();
      // exercise
      custom::set<Spy> sDest(sSrc);
      // verify
      assertUnit(Spy::numCopy() == 1);      // copy [50]
      assertUnit(Spy::numAlloc() == 1);     // allocate [50]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(sSrc.bst.root != sDest.bst.root);
      //            (50b)
      assertUnit(sSrc.bst.numElements == 1);
      assertUnit(sSrc.bst.root != nullptr);
      if (sSrc.bst.root)
      {
         assertUnit(sSrc.bst.root->data == Spy(50));
         assertUnit(sSrc.bst.root->isRed == false);
         assertUnit(sSrc.bst.root->pLeft == nullptr);
         assertUnit(sSrc.bst.root->pRight == nullptr);
         assertUnit(sSrc.bst.root->pParent == nullptr);
      }
      //            (50b)
      assertUnit(sDest.bst.numElements == 1);
      assertUnit(sDest.bst.root != nullptr);
      if (sDest.bst.root)
      {
         assertUnit(sDest.bst.root->data == Spy(50));
         assertUnit(sDest.bst.root->isRed == false);
         assertUnit(sDest.bst.root->pLeft == nullptr);
         assertUnit(sDest.bst.root->pRight == nullptr);
         assertUnit(sDest.bst.root->pParent == nullptr);
      }
      // teardown
      if (sSrc.bst.root)
         delete sSrc.bst.root;
      sSrc.bst.root = nullptr;
      sSrc.bst.numElements = 0;
      if (sDest.bst.root)
         delete sDest.bst.root;
      sDest.bst.root = nullptr;
      sDest.bst.numElements = 0;
   }

   // copy a standard set
   void test_constructCopy_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sSrc;
      setupStandardFixture(sSrc);
      Spy::reset();
      // exercise
      custom::set <Spy> sDest(sSrc);
      // verify
      assertUnit(Spy::numCopy() == 7);      // copy     [20][30][40][50][60][70][80]
      assertUnit(Spy::numAlloc() == 7);     // allocate [20][30][40][50][60][70][80]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(sSrc.bst.root != sDest.bst.root);
      if (sSrc.bst.root && sDest.bst.root)
      {
         assertUnit(sSrc.bst.root->pLeft != sDest.bst.root->pLeft);
         assertUnit(sSrc.bst.root->pRight != sDest.bst.root->pRight);
      }
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(sSrc);
      assertStandardFixture(sDest);
      // teardown
      teardownStandardFixture(sSrc);
      teardownStandardFixture(sDest);
   }


   /***************************************
    * MOVE CONSTRUCTOR
    ***************************************/

    // move an empty set
   void test_constructMove_empty()
   {  // setup
      custom::set<Spy> sSrc;
      Spy::reset();
      // exercise
      custom::set<Spy> sDest(std::move(sSrc));
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(sSrc);
      assertEmptyFixture(sDest);
   }  // teardown

   // move a set with a single node
   void test_constructMove_one()
   {
      // setup
      //            (50b)
      custom::set <Spy> sSrc;
      sSrc.bst.root = new custom::BST<Spy>::BNode(Spy(50));
      sSrc.bst.root->isRed = false;
      sSrc.bst.numElements = 1;
      Spy::reset();
      // exercise
      custom::set <Spy> sDest(std::move(sSrc));
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(sSrc.bst.root != sDest.bst.root);
      //           
      assertEmptyFixture(sSrc);
      //            (50b)
      assertUnit(sDest.bst.numElements == 1);
      assertUnit(sDest.bst.root != nullptr);
      assertUnit(sDest.bst.root != nullptr);
      if (sDest.bst.root)
      {
         assertUnit(sDest.bst.root->data == Spy(50));
         assertUnit(sDest.bst.root->isRed == false);
         assertUnit(sDest.bst.root->pLeft == nullptr);
         assertUnit(sDest.bst.root->pRight == nullptr);
         assertUnit(sDest.bst.root->pParent == nullptr);
      }
      // teardown
      if (sDest.bst.root)
         delete sDest.bst.root;
      sDest.bst.root = nullptr;
      sDest.bst.numElements = 0;
   }

   // move the standard fixture
   void test_constructMove_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sSrc;
      setupStandardFixture(sSrc);
      Spy::reset();
      // exercise
      custom::set <Spy> sDest(std::move(sSrc));
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(sSrc);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(sDest);
      // teardown
      teardownStandardFixture(sDest);
   }

     /***************************************
      * RANGE CONSTRUCTOR
      ***************************************/

      // create a new set using an empty range
   void test_constructRange_empty()
   {  // setup
      std::initializer_list<Spy> il{ Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80) };
      auto itBegin = il.begin();
      auto itEnd = il.end();
      Spy::reset();
      // exercise
      custom::set <Spy> s(itBegin, itEnd);
      // verify
      assertUnit(Spy::numCopy() == 7);     // copy-create [50][30][70][20][40][60][80]
      assertUnit(Spy::numAlloc() == 7);    // allocate    [50][30][70][20][40][60][80]
      assertUnit(Spy::numLessthan() == 10);// compare 50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numEquals() == 10);  // compare 50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // create a new set using a range of one element
   void test_constructRange_one()
   {  // setup
      std::initializer_list<Spy> il{ Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80) };
      auto itBegin = il.begin();
      auto itEnd = il.begin();
      itEnd++;
      Spy::reset();
      // exercise
      custom::set <Spy> s(itBegin, itEnd);
      // verify
      assertUnit(Spy::numCopy() == 1);     // copy-construct [99] 
      assertUnit(Spy::numAlloc() == 1);    // allocate [99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //            (99b)
      assertUnit(s.bst.numElements == 1);
      assertUnit(s.bst.root != nullptr);
      if (s.bst.root)
      {
         assertUnit(s.bst.root->data == Spy(50));
         assertUnit(s.bst.root->isRed == false);
         assertUnit(s.bst.root->pLeft == nullptr);
         assertUnit(s.bst.root->pRight == nullptr);
         assertUnit(s.bst.root->pParent == nullptr);
      }
      // teardown
      if (s.bst.root)
         delete s.bst.root;
      s.bst.root = nullptr;
      s.bst.numElements = 0;
   }

   // create a new set using a range created in order
   void test_constructRange_standard()
   {  // setup
      std::initializer_list<Spy> il{ Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80) };
      auto itBegin = il.begin();
      auto itEnd = il.end();
      Spy::reset();
      // exercise
      custom::set <Spy> s(itBegin, itEnd);
      // verify
      assertUnit(Spy::numCopy() == 7);      // copy-construct [50,30,70,20,40,60,80] 
      assertUnit(Spy::numAlloc() == 7);     // allocate [50,30,70,20,40,60,80]
      assertUnit(Spy::numLessthan() == 10); // compare 50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numEquals() == 10);   // equal   50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numEquals() == 10);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   /***************************************
    * CONSTRUCTOR INITIALIZE LIST
    ***************************************/

   // create a new set using an empty initialize list
   void test_constructInit_empty()
   {  // setup
      Spy::reset();
      // exercise
      custom::set <Spy> s{};
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
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertEmptyFixture(s);
   }  // teardown

   // create a new set using an initialize list of one element
   void test_constructInit_one()
   {  // setup
      std::initializer_list<Spy> il{ Spy(99) };
      Spy::reset();
      // exercise
      custom::set <Spy> s(il);
      // verify
      assertUnit(Spy::numCopy() == 1);     // copy-construct [99] 
      assertUnit(Spy::numAlloc() == 1);    // allocate [99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //            (99b)
      assertUnit(s.bst.numElements == 1);
      assertUnit(s.bst.root != nullptr);
      if (s.bst.root)
      {
         assertUnit(s.bst.root->data == Spy(99));
         assertUnit(s.bst.root->isRed == false);
         assertUnit(s.bst.root->pLeft == nullptr);
         assertUnit(s.bst.root->pRight == nullptr);
         assertUnit(s.bst.root->pParent == nullptr);
      }
      // teardown
      if (s.bst.root)
         delete s.bst.root;
      s.bst.root = nullptr;
      s.bst.numElements = 0;
   }

   // create a new set using an initialize list created in order
   void test_constructInit_standard()
   {  // setup
      std::initializer_list<Spy> il{ Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80) };
      Spy::reset();
      // exercise
      custom::set <Spy> s(il);
      // verify
      assertUnit(Spy::numCopy() == 7);      // copy-construct [50,30,70,20,40,60,80] 
      assertUnit(Spy::numAlloc() == 7);     // allocate [50,30,70,20,40,60,80]
      assertUnit(Spy::numLessthan() == 10); // compare 50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numEquals() == 10);   // equal   50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numEquals() == 10);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   /***************************************
    * DESTRUCTOR
    ***************************************/

   // destroy an empty set
   void test_destructor_empty()
   {  // setup
      {
         custom::set<Spy> s;
         Spy::reset();
      }  // exercise
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
   }  // teardown

   // destroy the standard fixture
   void test_destructor_standard()
   {  // setup
      {
         //                (50b)
         //          +-------+-------+
         //        (30b)           (70b)
         //     +----+----+     +----+----+
         //   (20r)     (40r) (60r)     (80r)
         custom::set <Spy> s;
         setupStandardFixture(s);
         Spy::reset();
      }  // exercise
      // verify
      assertUnit(Spy::numDestructor() == 7); // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);     // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
   }  // teardown


   /***************************************
    * EMPTY and SIZE
    ***************************************/

    // is the empty BST empty?
   void test_empty_empty()
   {  // setup
      custom::set<Spy> s;
      Spy::reset();
      // exercise
      bool empty = s.empty();
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(empty == true);
      assertEmptyFixture(s);
   }  // teardown

   // is the standard fixture empty?
   void test_empty_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy::reset();
      // exercise
      bool empty = s.empty();
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(empty == false);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // what is the size of an empty BST?
   void test_size_empty()
   {  // setup
      custom::set<Spy> s;
      Spy::reset();
      // exercise
      size_t size = s.size();
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(size == 0);
      assertEmptyFixture(s);
   }  // teardown

   // what is the size of the standard fixture?
   void test_size_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy::reset();
      // exercise
      size_t size = s.size();
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(size == 7);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }


   /***************************************
    * Assignment
    *    set::operator=(const set &)
    ***************************************/

    // assignment operator: empty = empty
   void test_assign_emptyToEmpty()
   {  // setup
      custom::set <Spy> sSrc;
      custom::set <Spy> sDest;
      Spy::reset();
      // exercise
      sDest = sSrc;
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(sSrc);
      assertEmptyFixture(sDest);
   }  // teardown

   // assignment operator: empty = standard
   void test_assign_standardToEmpty()
   {  // setup
      //                (50b) = sSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sSrc;
      setupStandardFixture(sSrc);
      custom::set <Spy> sDest;
      Spy::reset();
      // exercise
      sDest = sSrc;
      // verify
      assertUnit(Spy::numCopy() == 7);      // copy     [20][30][40][50][60][70][80]
      assertUnit(Spy::numAlloc() == 7);     // allocate [20][30][40][50][60][70][80]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(sSrc.bst.root != sDest.bst.root);
      if (sSrc.bst.root && sDest.bst.root)
      {
         assertUnit(sSrc.bst.root->pLeft != sDest.bst.root->pLeft);
         assertUnit(sSrc.bst.root->pRight != sDest.bst.root->pRight);
      }
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(sSrc);
      assertStandardFixture(sDest);
      // teardown
      teardownStandardFixture(sSrc);
      teardownStandardFixture(sDest);
   }

   // assignment operator: standard = empty
   void test_assign_emptyToStandard()
   {  // setup
      custom::set <Spy> sSrc;
      //                (50b) = sDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sDest;
      setupStandardFixture(sDest);
      Spy::reset();
      // exercise
      sDest = sSrc;
      // verify
      assertUnit(Spy::numDestructor() == 7); // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);     // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(sSrc);
      assertEmptyFixture(sDest);
   }  // teardown

   // assignment operator : one = standard
   void test_assign_oneToStandard()
   {  // setup
      //                (50b) = sSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sSrc;
      setupStandardFixture(sSrc);
      //                (99) = sDest
      custom::set <Spy> sDest;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      sDest.bst.root = p99;
      sDest.bst.numElements = 1;
      Spy::reset();
      // exercise
      sDest = sSrc;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign   [50] onto [99]
      assertUnit(Spy::numCopy() == 6);        // copy     [20][30][40]    [60][70][80]
      assertUnit(Spy::numAlloc() == 6);       // allocate [20][30][40]    [60][70][80]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(sDest.bst.root != sSrc.bst.root);
      if (sSrc.bst.root && sDest.bst.root)
      {
         assertUnit(sSrc.bst.root->pLeft != sDest.bst.root->pLeft);
         assertUnit(sSrc.bst.root->pRight != sDest.bst.root->pRight);
      }
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(sSrc);
      assertStandardFixture(sDest);
      // teardown
      teardownStandardFixture(sSrc);
      teardownStandardFixture(sDest);
   }

   // assignment operator : standard = one
   void test_assign_standardToOne()
   {  // setup
      //                (99) = sSrc
      custom::set <Spy> sSrc;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      sSrc.bst.root = p99;
      sSrc.bst.numElements = 1;
      //                (50b) = sDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sDest;
      setupStandardFixture(sDest);
      Spy::reset();
      // exercise
      sDest = sSrc;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign   [99] onto [0]
      assertUnit(Spy::numDestructor() == 6);  // destroy  [20][30][40]    [60][70][80]
      assertUnit(Spy::numDelete() == 6);      // delete   [20][30][40]    [60][70][80]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(sDest.bst.root != sSrc.bst.root);
      //                (99) = sSrc.bst
      assertUnit(sSrc.bst.root != nullptr);
      if (sSrc.bst.root)
      {
         assertUnit(sSrc.bst.root->data == Spy(99));
         assertUnit(sSrc.bst.root->isRed == false);
         assertUnit(sSrc.bst.root->pParent == nullptr);
         assertUnit(sSrc.bst.root->pLeft == nullptr);
         assertUnit(sSrc.bst.root->pRight == nullptr);
      }
      //                (99) = sDest.bst
      assertUnit(sDest.bst.root != nullptr);
      if (sDest.bst.root)
      {
         assertUnit(sDest.bst.root->data == Spy(99));
         assertUnit(sDest.bst.root->isRed == false);
         assertUnit(sDest.bst.root->pParent == nullptr);
         assertUnit(sDest.bst.root->pLeft == nullptr);
         assertUnit(sDest.bst.root->pRight == nullptr);
      }
      // teardown
      teardownStandardFixture(sSrc);
      teardownStandardFixture(sDest);
   }

   // assignment operator : standard = standard
   void test_assign_standardToStandard()
   {  // setup
      //                (50b) = bstSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sSrc;
      setupStandardFixture(sSrc);
      //                (50b) = bstDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sDest;
      setupStandardFixture(sDest);
      Spy::reset();
      // exercise
      sDest = sSrc;
      // verify
      assertUnit(Spy::numAssign() == 7);      // assign [2][30][40][50][60][70][80]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(sDest.bst.root != sSrc.bst.root);
      if (sSrc.bst.root && sDest.bst.root)
      {
         assertUnit(sSrc.bst.root->pLeft != sDest.bst.root->pLeft);
         assertUnit(sSrc.bst.root->pRight != sDest.bst.root->pRight);
      }
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(sSrc);
      assertStandardFixture(sDest);
      // teardown
      teardownStandardFixture(sSrc);
      teardownStandardFixture(sDest);
   }

   /***************************************
    * Assignment-Move
    *    set::operator=(set &&)
    ***************************************/

 // assignment-move operator: empty = std::move(empty)
   void test_assignMove_emptyToEmpty()
   {  // setup
      custom::set <Spy> bstSrc;
      custom::set <Spy> bstDest;
      Spy::reset();
      // exercise
      bstDest = std::move(bstSrc);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstSrc);
      assertEmptyFixture(bstDest);
   }  // teardown

   // assignment-move operator: empty = move(standard)
   void test_assignMove_standardToEmpty()
   {  // setup
      //                (50b) = sSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> bstSrc;
      setupStandardFixture(bstSrc);
      custom::set <Spy> bstDest;
      Spy::reset();
      // exercise
      bstDest = std::move(bstSrc);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertStandardFixture(bstDest);
      assertEmptyFixture(bstSrc);
      // teardown
      teardownStandardFixture(bstDest);
   }

   // assignment-move operator: standard = move(empty)
   void test_assignMove_emptyToStandard()
   {  // setup
      custom::set <Spy> sSrc;
      //                (50b) = sDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sDest;
      setupStandardFixture(sDest);
      Spy::reset();
      // exercise
      sDest = std::move(sSrc);
      // verify
      assertUnit(Spy::numDestructor() == 7); // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);     // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(sSrc);
      assertEmptyFixture(sDest);
   }  // teardown

   // assignment-move operator : one = standard
   void test_assignMove_oneToStandard()
   {  // setup
      //                (50b) = sSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sSrc;
      setupStandardFixture(sSrc);
      //                (99) = bstDest
      custom::set <Spy> sDest;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      sDest.bst.root = p99;
      sDest.bst.numElements = 1;
      Spy::reset();
      // exercise
      sDest = std::move(sSrc);
      // verify
      assertUnit(Spy::numDestructor() == 1);  // destroy [99]
      assertUnit(Spy::numDelete() == 1);      // delete  [99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30r)           (70r)
      //     +----+----+     +----+----+
      //   (20b)     (40b) (60b)     (80b)
      assertStandardFixture(sDest);
      assertEmptyFixture(sSrc);
      // teardown
      teardownStandardFixture(sDest);
   }

   // assignment operator : standard = one
   void test_assignMove_standardToOne()
   {  // setup
      //                (99) = sSrc
      custom::set <Spy> sSrc;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      sSrc.bst.root = p99;
      sSrc.bst.numElements = 1;
      //                (50b) = sDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sDest;
      setupStandardFixture(sDest);
      Spy::reset();
      // exercise
      sDest = std::move(sSrc);
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(sSrc);
      //                (99) = bstDest
      assertUnit(sDest.bst.root != nullptr);
      if (sDest.bst.root)
      {
         assertUnit(sDest.bst.root->data == Spy(99));
         assertUnit(sDest.bst.root->isRed == false);
         assertUnit(sDest.bst.root->pParent == nullptr);
         assertUnit(sDest.bst.root->pLeft == nullptr);
         assertUnit(sDest.bst.root->pRight == nullptr);
      }
      // teardown
      teardownStandardFixture(sDest);
   }

   // assignment operator : standard = standard
   void test_assignMove_standardToStandard()
   {  // setup
      //                (50b) = sSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sSrc;
      setupStandardFixture(sSrc);
      //                (50b) = sDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> sDest;
      setupStandardFixture(sDest);
      Spy::reset();
      // exercise
      sDest = std::move(sSrc);
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(sDest);
      assertEmptyFixture(sSrc);
      // teardown
      teardownStandardFixture(sDest);
   }

   /***************************************
    * Assignment
    *    set::operator=(const std::initializer_list<T>& il)
    ***************************************/

    // assignment operator: empty = {}
   void test_assignInit_emptyToEmpty()
   {  // setup
      custom::set <Spy> s;
      std::initializer_list<Spy> il{ };
      Spy::reset();
      // exercise
      s = il;
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(s);
   }  // teardown

   // assignment operator: empty = {50, 30, 70, 20, 40, 60, 80}
   void test_assignInit_standardToEmpty()
   {  // setup
      custom::set <Spy> s;
      std::initializer_list<Spy> il{ Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80) };
      Spy::reset();
      // exercise
      s = il;
      // verify
      assertUnit(Spy::numCopy() == 7);      // copy     [20][30][40][50][60][70][80]
      assertUnit(Spy::numAlloc() == 7);     // allocate [20][30][40][50][60][70][80]
      assertUnit(Spy::numLessthan() == 10); // compare 50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numEquals() == 10);   // equal   50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // assignment operator: standard = {}
   void test_assignInit_emptyToStandard()
   {  // setup
      //                (50b) = sDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s;
      setupStandardFixture(s);
      std::initializer_list<Spy> il{ };
      Spy::reset();
      Spy::reset();
      // exercise
      s = il;
      // verify
      assertUnit(Spy::numDestructor() == 7); // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);     // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(s);
   }  // teardown

   // assignment operator : one = {50, 30, 70, 20, 40, 60, 80}
   void test_assignInit_oneToStandard()
   {  // setup
      std::initializer_list<Spy> il{ Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80) };
      //                (99) = s
      custom::set <Spy> s;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      s.bst.root = p99;
      s.bst.numElements = 1;
      Spy::reset();
      // exercise
      s = il;
      // verify
      assertUnit(Spy::numDestructor() == 1);  // destroy [99]
      assertUnit(Spy::numDelete() == 1);      // delete [99]
      assertUnit(Spy::numCopy() == 7);        // copy     [20][30][40][50][60][70][80]
      assertUnit(Spy::numAlloc() == 7);       // allocate [20][30][40][50][60][70][80]
      assertUnit(Spy::numLessthan() == 10);   // compare 50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numEquals() == 10);     // equal   50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // assignment operator : standard = {99}
   void test_assignInit_standardToOne()
   {  // setup
      std::initializer_list<Spy> il{ Spy(99) };
      //                (50b) = s
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy::reset();
      // exercise
      s = il;
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numCopy() == 1);        // copy [99]
      assertUnit(Spy::numAlloc() == 1);       // allocate [99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //                (99) = s.bst
      assertUnit(s.bst.root != nullptr);
      if (s.bst.root)
      {
         assertUnit(s.bst.root->data == Spy(99));
         assertUnit(s.bst.root->isRed == false);
         assertUnit(s.bst.root->pParent == nullptr);
         assertUnit(s.bst.root->pLeft == nullptr);
         assertUnit(s.bst.root->pRight == nullptr);
      }
      // teardown
      teardownStandardFixture(s);
   }

   // assignment operator : standard = {50, 30, 70, 20, 40, 60, 80}
   void test_assignInit_standardToStandard()
   {  // setup
      std::initializer_list<Spy> il{ Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80) };
      //                (50b) = s
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy::reset();
      // exercise
      s = il;
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numCopy() == 7);        // copy     [20][30][40][50][60][70][80]
      assertUnit(Spy::numAlloc() == 7);       // allocate [20][30][40][50][60][70][80]
      assertUnit(Spy::numLessthan() == 10);   // compare 50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numEquals() == 10);     // equal   50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }


   /***************************************
    * Swap
    *    set::swap(set &)
    ***************************************/

    // empty.swap(empty)
   void test_swap_emptyToEmpty()
   {  // setup
      custom::set <Spy> s1;
      custom::set <Spy> s2;
      Spy::reset();
      // exercise
      s1.swap(s2);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(s1);
      assertEmptyFixture(s2);
   }  // teardown

   // swap: standard.swap(empty)
   void test_swap_standardToEmpty()
   {  // setup
      //                (50b) = bst1
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s1;
      setupStandardFixture(s1);
      custom::set <Spy> s2;
      Spy::reset();
      // exercise
      s1.swap(s2);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertStandardFixture(s2);
      assertEmptyFixture(s1);
      // teardown
      teardownStandardFixture(s2);
   }

   // swap: empty.swap(standard)
   void test_swap_emptyToStandard()
   {  // setup
      custom::set <Spy> s1;
      //                (50b) = bst2
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s2;
      setupStandardFixture(s2);
      Spy::reset();
      // exercise
      s1.swap(s2);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertStandardFixture(s1);
      assertEmptyFixture(s2);
      // teardown
      teardownStandardFixture(s1);
   }

   // swap : standard.swap(standard)
   void test_swap_standardToStandard()
   {  // setup
      //                (50b) = s1
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s1;
      setupStandardFixture(s1);
      //                (50b) = s2
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s2;
      setupStandardFixture(s2);
      Spy::reset();
      // exercise
      s1.swap(s2);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(s1);
      assertStandardFixture(s2);
      // teardown
      teardownStandardFixture(s1);
      teardownStandardFixture(s2);
   }


   /***************************************
    * CLEAR
    *    BST::clear()
    ***************************************/

    // clear an already empty fixture
   void test_clear_empty()
   {  // setup
      custom::set<Spy> s;
      Spy::reset();
      // exercise
      s.clear();
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(s);
   }  // teardown

   // clear the standard fixture
   void test_clear_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy::reset();
      // exercise
      s.clear();
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(s);
   }  // teardown


   /***************************************
    * Iterator
    *     set::begin()
    *     set::end()
    *     set::iterator::operator++()
    *     set::iterator::operator*()
    ***************************************/

    // begin() from an empty BST
   void test_begin_empty()
   {  // setup
      custom::set<Spy> s;
      custom::set<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = s.begin();
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.it.pNode == nullptr);
      assertUnit(it == s.bst.end());
      assertEmptyFixture(s);
   }  // teardown

   // begin() from the standard fixture
   void test_begin_standard()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = s.begin();
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.it.pNode != nullptr);
      assertUnit(s.bst.root != nullptr && s.bst.root->pLeft != nullptr);
      if (it.it.pNode && s.bst.root && s.bst.root->pLeft)
         assertUnit(it.it.pNode == s.bst.root->pLeft->pLeft);
      if (it.it.pNode)
         assertUnit(it.it.pNode->data == Spy(20));
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // end() from the standard fixture.
   void test_end_standard()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = s.end();
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.it.pNode == nullptr);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // increment where the next node is the parent
   void test_iterator_increment_standardToParent()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //  [[20]]      40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      it.it.pNode = s.bst.root->pLeft->pLeft;
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.it.pNode != nullptr);
      assertUnit(s.bst.root != nullptr);
      if (s.bst.root)
         assertUnit(it.it.pNode == s.bst.root->pLeft);
      if (it.it.pNode)
         assertUnit(it.it.pNode->data == Spy(30));
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }


   // increment where the next node is the right child
   void test_iterator_increment_standardToChild()
   {  // setup
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      it.it.pNode = s.bst.root->pLeft; // 30
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20      [[40]]  60        80  
      assertUnit(it.it.pNode != nullptr);
      assertUnit(s.bst.root != nullptr && s.bst.root->pLeft != nullptr);
      if (s.bst.root && s.bst.root->pLeft)
         assertUnit(it.it.pNode == s.bst.root->pLeft->pRight);
      if (it.it.pNode)
         assertUnit(it.it.pNode->data == Spy(40));
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // increment where the next node is grandma
   void test_iterator_increment_standardToGrandma()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20      [[40]]  60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      it.it.pNode = s.bst.root->pLeft->pRight; // 40
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //               [[50]] 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.it.pNode == s.bst.root);
      if (it.it.pNode)
         assertUnit(it.it.pNode->data == Spy(50));
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // increment where the next node is the right grandchild
   void test_iterator_increment_standardToGrandchild()
   {  // setup
      //               [[50]] 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      it.it.pNode = s.bst.root; // 50
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40  [[60]]      80  
      assertUnit(it.it.pNode != nullptr);
      assertUnit(s.bst.root != nullptr && s.bst.root->pRight != nullptr);
      if (s.bst.root && s.bst.root->pRight)
         assertUnit(it.it.pNode == s.bst.root->pRight->pLeft);
      if (it.it.pNode)
         assertUnit(it.it.pNode->data == Spy(60));
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // increment where we are already at the last node
   void test_iterator_increment_standardToDone()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60      [[80]]  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      it.it.pNode = s.bst.root->pRight->pRight; // 80
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.it.pNode == nullptr);
      assertUnit(it == s.end());
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // increment where we are already at the end
   void test_iterator_increment_standardEnd()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      it.it.pNode = nullptr;
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.it.pNode == nullptr);
      assertUnit(it == s.end());
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // itereator dereference were we just read
   void test_iterator_dereference_standardRead()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20      [[40]]  60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      it.it.pNode = s.bst.root->pLeft->pRight;
      Spy spy(99);
      Spy::reset();
      // exercise
      spy = *it;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign the results to s
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(spy == Spy(40));
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   /***************************************
    * Find
    *    set::find(const T &)
    ***************************************/

 // attempt to find something in an empty BST
   void test_find_empty()
   {  // setup
      custom::set<Spy> s;
      custom::set<Spy>::iterator it;
      Spy spy(50);
      Spy::reset();
      // exercise
      it = s.find(spy);
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it == s.end());
      assertEmptyFixture(s);
   }  // teardown

   // attemp to find something where it is at the beginning
   void test_find_standardBegin()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      Spy spy(20);
      Spy::reset();
      // exercise
      it = s.find(spy);
      // verify
      assertUnit(Spy::numEquals() == 3);      // check [50][30][20]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][30]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //  [[20]]      40    60        80  
      assertUnit(it.it.pNode != nullptr);
      if (it.it.pNode)
         assertUnit(*it == Spy(20));
      assertUnit(s.bst.root != nullptr && s.bst.root->pLeft != nullptr);
      if (s.bst.root && s.bst.root->pLeft)
         assertUnit(it.it.pNode == s.bst.root->pLeft->pLeft);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // attempt to find something where it is the last element
   void test_find_standardLast()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      Spy spy(80);
      Spy::reset();
      // exercise
      it = s.find(spy);
      // verify
      assertUnit(Spy::numEquals() == 3);      // check [50][70][80]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60      [[80]]
      assertUnit(it.it.pNode != nullptr);
      if (it.it.pNode)
         assertUnit(*it == Spy(80));
      assertUnit(s.bst.root != nullptr && s.bst.root->pRight != nullptr);
      if (s.bst.root && s.bst.root->pRight)
         assertUnit(it.it.pNode == s.bst.root->pRight->pRight);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // attempt to find something where it is not there
   void test_find_standardMissing()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set<Spy>::iterator it;
      Spy spy(42);
      Spy::reset();
      // exercise
      it = s.find(spy);
      // verify
      assertUnit(Spy::numEquals() == 3);      // check [50][30][40]
      assertUnit(Spy::numLessthan() == 3);    // compare [50][30][40]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it == s.end());
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }


   /***************************************
    * INSERT
    *  set::insert(const T &)
    ***************************************/
   
   // insert an element when empty
   void test_insert_empty() 
   {  // setup
      custom::set<Spy> s;
      Spy spy(50);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(spy);
      // verify
      assertUnit(Spy::numCopy() == 1);        // copy-create [50]
      assertUnit(Spy::numAlloc() == 1);       // allocate [50]
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == true);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(50));
      //            (50b)
      assertUnit(s.bst.root != nullptr);
      assertUnit(s.bst.numElements == 1);

      if (s.bst.root)
      {
         assertUnit(s.bst.root->data == Spy(50));
         assertUnit(s.bst.root->isRed == false);
         assertUnit(s.bst.root->pLeft == nullptr);
         assertUnit(s.bst.root->pRight == nullptr);
         assertUnit(s.bst.root->pParent == nullptr);
      }
      // teardown
      delete s.bst.root;
      s.bst.root = nullptr;
      s.bst.numElements = 0;
   }

   // insert onto the end when there is excess capacity
   void test_insert_standardEnd()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+
      //    20        40    60     
      custom::set <Spy> s;
      setupStandardFixture(s);
      delete s.bst.root->pRight->pRight;
      s.bst.root->pRight->pRight = nullptr;
      s.bst.numElements = 6;
      Spy spy(80);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(spy);
      // verify
      assertUnit(Spy::numCopy() == 1);        // copy-create [80]
      assertUnit(Spy::numAlloc() == 1);       // allocate [80]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numEquals() == 2);      // compare [50][70]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == true);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(80));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // insert to the beginning when we need to reallocat
   void test_insert_standardFront()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //          +----+     +----+----+
      //              40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      delete s.bst.root->pLeft->pLeft;
      s.bst.root->pLeft->pLeft = nullptr;
      s.bst.numElements = 6;
      Spy spy(20);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(spy);
      // verify
      assertUnit(Spy::numCopy() == 1);        // copy-create [20]
      assertUnit(Spy::numAlloc() == 1);       // allocate [20]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][30]
      assertUnit(Spy::numEquals() == 2);      // compare [50][30]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == true);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(20));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // insert into the middle when there is excess capacity
   void test_insert_standardMiddle() 
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+          +----+
      //    20        40              80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      delete s.bst.root->pRight->pLeft;
      s.bst.root->pRight->pLeft = nullptr;
      s.bst.numElements = 6;
      Spy spy(60);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(spy);
      // verify
      assertUnit(Spy::numCopy() == 1);        // copy-create [60]
      assertUnit(Spy::numAlloc() == 1);       // allocate [60]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numEquals() == 2);      // compare [50][70]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == true);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(60));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // insert when the elements are already there. Nothing is added
   void test_insert_standardDuplicate() 
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy spy(60);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(spy);
      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numEquals() == 3);      // compare [50][70][60]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == false);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(60));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   /***************************************
    * INSERT MOVE
    *  set::insert(T &&)
    ***************************************/

 // insert an element when empty
   void test_insertMove_empty()
   {  // setup
      custom::set<Spy> s;
      Spy spy(50);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(std::move(spy));
      // verify
      assertUnit(Spy::numCopyMove() == 1);  // copy-move [50]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(spy.empty() == true);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == true);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(50));
      //            (50b)
      assertUnit(s.bst.root != nullptr);
      assertUnit(s.bst.numElements == 1);

      if (s.bst.root)
      {
         assertUnit(s.bst.root->data == Spy(50));
         assertUnit(s.bst.root->isRed == false);
         assertUnit(s.bst.root->pLeft == nullptr);
         assertUnit(s.bst.root->pRight == nullptr);
         assertUnit(s.bst.root->pParent == nullptr);
      }
      // teardown
      delete s.bst.root;
      s.bst.root = nullptr;
      s.bst.numElements = 0;
   }

   // insert onto the end when there is excess capacity
   void test_insertMove_standardEnd()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+
      //    20        40    60     
      custom::set <Spy> s;
      setupStandardFixture(s);
      delete s.bst.root->pRight->pRight;
      s.bst.root->pRight->pRight = nullptr;
      s.bst.numElements = 6;
      Spy spy(80);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(std::move(spy));
      // verify
      assertUnit(Spy::numCopyMove() == 1);    // copy-move [80]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numEquals() == 2);      // compare [50][70]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(spy.empty() == true);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == true);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(80));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // insert to the beginning when we need to reallocat
   void test_insertMove_standardFront()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //          +----+     +----+----+
      //              40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      delete s.bst.root->pLeft->pLeft;
      s.bst.root->pLeft->pLeft = nullptr;
      s.bst.numElements = 6;
      Spy spy(20);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(std::move(spy));
      // verify
      assertUnit(Spy::numCopyMove() == 1);    // copy-move [20]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][30]
      assertUnit(Spy::numEquals() == 2);      // compare [50][30]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(spy.empty() == true);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == true);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(20));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // insert into the middle when there is excess capacity
   void test_insertMove_standardMiddle()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+          +----+
      //    20        40              80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      delete s.bst.root->pRight->pLeft;
      s.bst.root->pRight->pLeft = nullptr;
      s.bst.numElements = 6;
      Spy spy(60);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(std::move(spy));
      // verify
      assertUnit(Spy::numCopyMove() == 1);  // copy-move [60]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numEquals() == 2);      // compare [50][70]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(spy.empty() == true);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == true);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(60));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // insert when the elements are already there. Nothing is added
   void test_insertMove_standardDuplicate()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy spy(60);
      Spy::reset();
      // exercise
      auto pairSet = s.insert(std::move(spy));
      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numEquals() == 3);      // compare [50][70][60]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairSet.first != s.end());
      assertUnit(pairSet.second == false);
      if (pairSet.first != s.end())
         assertUnit(*(pairSet.first) == Spy(60));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   /***************************************
    * Insert Initializer
    *    set::insert(const std::initializer_list<T>& il)
    ***************************************/

   // insert: empty.insert({})
   void test_insertInit_emptyInsertNone()
   {  // setup
      custom::set <Spy> s;
      std::initializer_list<Spy> il{ };
      Spy::reset();
      // exercise
      s.insert(il);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(s);
   }  // teardown

   // insert: empty.insert({50, 30, 70, 20, 40, 60, 80})
   void test_insertInit_emptyInsertMany()
   {  // setup
      custom::set <Spy> s;
      std::initializer_list<Spy> il{ Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80) };
      Spy::reset();
      // exercise
      s.insert(il);
      // verify
      assertUnit(Spy::numCopy() == 7);      // copy     [20][30][40][50][60][70][80]
      assertUnit(Spy::numAlloc() == 7);     // allocate [20][30][40][50][60][70][80]
      assertUnit(Spy::numLessthan() == 10); // compare 50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numEquals() == 10);   // equal   50: 30:[50] 70:[50] 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // insert: standard.insert({})
   void test_insertInit_standardInsertNone()
   {  // setup
      //                (50b) = s
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s;
      setupStandardFixture(s);
      std::initializer_list<Spy> il{ };
      Spy::reset();
      // exercise
      s.insert(il);
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }  

   // insert : standard.insert({50, 30})
   void test_insertInit_standardInsertDuplicates()
   {  // setup
      std::initializer_list<Spy> il{ Spy(50), Spy(40) };
      //                (50b) = s
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy::reset();
      // exercise
      s.insert(il);
      // verify
      assertUnit(Spy::numLessthan() == 2);   // compare 50: 40:[50][30] 
      assertUnit(Spy::numEquals() == 4);     // equal   50:[50] 40:[50][30][40]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);    
      assertUnit(Spy::numCopy() == 0);      
      assertUnit(Spy::numAlloc() == 0);     
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // assignment operator : standard = {99}
   void test_insertInit_manyInsertMany()
   {  // setup
      //                (50b) = s
      //          +-------+-------+
      //        (30b)           (70b)
      custom::set <Spy> s;
      custom::BST <Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST <Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST <Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));
      p50->isRed = false;
      p30->isRed = p70->isRed = true;
      s.bst.root = p30->pParent = p70->pParent = p50;
      p50->pRight = p70;
      p50->pLeft  = p30;
      s.bst.numElements = 3;
      std::initializer_list<Spy> il{ Spy(20), Spy(40), Spy(60), Spy(80) };
      Spy::reset();
      // exercise
      s.insert(il);
      // verify
      assertUnit(Spy::numLessthan() == 8);   // compare 20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]  
      assertUnit(Spy::numEquals() == 8);     // equal   20:[50][30] 40:[50][30] 60:[50][70] 80:[50][70]
      assertUnit(Spy::numCopy() == 4);       // create   [20][40][60][80]
      assertUnit(Spy::numAlloc() == 4);      // allocate [20][40][60][80]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopyMove() == 0); 
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                (50b) = s
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   /***************************************
    * Erase Range
    *    set::erase(itBegin, itBEnd)
    ***************************************/

 // erase from an empty BST
   void test_eraseRange_several()
   {  // setup
      //                (50b) = s
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      //           itBegin itEnd
      custom::set <Spy> s;
      setupStandardFixture(s);
      custom::set <Spy>::iterator itBegin(custom::BST<Spy>::iterator(s.bst.root->pLeft->pRight));
      custom::set <Spy>::iterator itEnd(custom::BST<Spy>::iterator(s.bst.root->pRight->pLeft));
      custom::set <Spy>::iterator itDone;
      Spy::reset();
      // exercise
      itDone = s.erase(itBegin, itEnd);
      // verify
      //                itDone
      //                (60b) = s
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+               +----+
      //   (20r)                       (80r)
      assertUnit(itDone.it.pNode == s.bst.root);
      assertUnit(s.bst.root != nullptr);
      if (s.bst.root != nullptr)
      {
         assertUnit(s.bst.root->data == Spy(60));
         assertUnit(s.bst.root->pLeft != nullptr);
         if (s.bst.root->pLeft != nullptr)
         {
            assertUnit(s.bst.root->pLeft->data == Spy(30));
            assertUnit(s.bst.root->pLeft->pLeft != nullptr);
            if (s.bst.root->pLeft->pLeft != nullptr)
            {
               assertUnit(s.bst.root->pLeft->pLeft->data == Spy(20));
               assertUnit(s.bst.root->pLeft->pLeft->pLeft == nullptr);
               assertUnit(s.bst.root->pLeft->pLeft->pRight == nullptr);
            }
            assertUnit(s.bst.root->pLeft->pRight == nullptr);
         }
         assertUnit(s.bst.root->pRight != nullptr);
         if (s.bst.root->pRight != nullptr)
         {
            assertUnit(s.bst.root->pRight->data == Spy(70));
            assertUnit(s.bst.root->pRight->pLeft == nullptr);
            assertUnit(s.bst.root->pRight->pRight != nullptr);
            if (s.bst.root->pRight->pRight != nullptr)
            {
               assertUnit(s.bst.root->pRight->pRight->data == Spy(80));
               assertUnit(s.bst.root->pRight->pRight->pLeft == nullptr);
               assertUnit(s.bst.root->pRight->pRight->pRight == nullptr);
            }
         }
      }
      // teardown
   }

   /***************************************
    * Erase Iterator
    *    set::erase(it)
    ***************************************/

    // erase from an empty BST
   void test_eraseIterator_empty()
   {  // setup
      custom::set <Spy> s;
      auto it = s.end();
      Spy::reset();
      // exercise
      auto itReturn = s.erase(it);
      // verify
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(itReturn == s.end());
      assertEmptyFixture(s);
   }  // teardown

   // erase with an empty iterator
   void test_eraseIterator_standardMissing()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      auto it = s.end();
      Spy::reset();
      // exercise
      auto itReturn = s.erase(it);
      // verify
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(itReturn == s.end());
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }  // teardown

   // remove a leaf node from the standard fixture
   void test_eraseIterator_noChildren()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40  [[60]]      80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      auto itBST = custom::BST <Spy> ::iterator(s.bst.root->pRight->pLeft);
      auto it = custom::set <Spy> ::iterator(itBST);
      Spy::reset();
      // exercise
      auto itReturn = s.erase(it);
      // verify
      assertUnit(Spy::numDestructor() == 1);  // destroy [60]
      assertUnit(Spy::numDelete() == 1);      // delete [60]
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30            [[70]]
      //     +----+----+          +----+
      //    20        40              80  
      itBST = custom::BST <Spy> ::iterator(s.bst.root->pRight);
      it = custom::set <Spy> ::iterator(itBST);
      assertUnit(itReturn == it);
      assertUnit(s.bst.root->pRight->pLeft == nullptr);
      assertUnit(s.bst.numElements == 6);
      s.bst.root->pRight->pLeft = new custom::BST<Spy>::BNode(Spy(60));
      s.bst.root->pRight->pLeft->pParent = s.bst.root->pRight;
      s.bst.numElements = 7;
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }  // teardown

   void test_eraseIterator_oneChild()
   {  // setup
      //                 50 
      //          +-------+-------+
      //       [[10]]            60
      //          +----+     
      //              30          
      //            +--+--+
      //           20    40
      custom::set <int> s;
      auto p10 = new custom::BST<int>::BNode(10);
      auto p20 = new custom::BST<int>::BNode(20);
      auto p30 = new custom::BST<int>::BNode(30);
      auto p40 = new custom::BST<int>::BNode(40);
      auto p60 = new custom::BST<int>::BNode(60);
      auto p50 = new custom::BST<int>::BNode(50);
      s.bst.root = p10->pParent = p60->pParent = p50;
      p50->pLeft = p30->pParent = p10;
      p50->pRight = p60;
      p10->pRight = p20->pParent = p40->pParent = p30;
      p30->pLeft = p20;
      p30->pRight = p40;
      s.bst.numElements = 6;
      auto itBST = custom::BST <int> ::iterator(p10);
      auto it = custom::set <int> ::iterator(itBST);
      // exercise
      auto itReturn = s.erase(it);
      // verify
      //                 50
      //          +-------+-------+
      //         30              60
      //     +----+----+
      //  [[20]]      40
      itBST = custom::BST <int> ::iterator(p20);
      it = custom::set <int> ::iterator(itBST);
      assertUnit(itReturn == it);
      assertUnit(s.bst.numElements == 5);
      assertUnit(s.bst.root == p50);
      assertUnit(p50->pLeft == p30);
      assertUnit(p50->pRight == p60);
      assertUnit(p30->pLeft == p20);
      assertUnit(p30->pRight = p40);
      assertUnit(p30->pParent == p50);
      assertUnit(p20->pParent == p30);
      assertUnit(p40->pParent == p30);
      assertUnit(p60->pParent == p50);
      assertUnit(p20->data == 20);
      assertUnit(p30->data == 30);
      assertUnit(p40->data == 40);
      assertUnit(p50->data == 50);
      assertUnit(p60->data == 60);
      // teardown
      delete p20;
      delete p30;
      delete p40;
      delete p50;
      delete p60;
      s.bst.numElements = 0;
      s.bst.root = nullptr;
   }

   void test_eraseIterator_twoChildren()
   {  // setup
      //                 70 
      //          +-------+-------+
      //       [[20]]            80
      //     +----+----+     
      //    10        50          
      //            +--+--+
      //           30    60
      //            +-+
      //              40 
      custom::set <int> s;
      auto p10 = new custom::BST<int>::BNode(10);
      auto p20 = new custom::BST<int>::BNode(20);
      auto p30 = new custom::BST<int>::BNode(30);
      auto p40 = new custom::BST<int>::BNode(40);
      auto p50 = new custom::BST<int>::BNode(50);
      auto p60 = new custom::BST<int>::BNode(60);
      auto p70 = new custom::BST<int>::BNode(70);
      auto p80 = new custom::BST<int>::BNode(80);
      s.bst.root = p20->pParent = p80->pParent = p70;
      p10->pParent = p50->pParent = p70->pLeft = p20;
      p70->pRight = p80;
      p20->pLeft = p10;
      p20->pRight = p30->pParent = p60->pParent = p50;
      p50->pLeft = p40->pParent = p30;
      p50->pRight = p60;
      p30->pRight = p40;
      s.bst.numElements = 8;
      auto itBST = custom::BST <int> ::iterator(p20);
      auto it = custom::set <int> ::iterator(itBST);
      // exercise
      auto itReturn = s.erase(it);
      // verify
      //                 70
      //          +-------+-------+
      //       [[30]]            80
      //     +----+----+     
      //    10        50    
      //            +--+--+
      //           40    60
      itBST = custom::BST <int> ::iterator(p30);
      it = custom::set <int> ::iterator(itBST);
      assertUnit(itReturn == it);
      assertUnit(s.bst.root == p70);
      assertUnit(s.bst.numElements == 7);
      assertUnit(p70->pLeft == p30);
      assertUnit(p70->pRight == p80);
      assertUnit(p30->pLeft == p10);
      assertUnit(p30->pRight = p50);
      assertUnit(p50->pLeft == p40);
      assertUnit(p50->pRight = p60);
      assertUnit(p30->pParent == p70);
      assertUnit(p80->pParent == p70);
      assertUnit(p10->pParent == p30);
      assertUnit(p50->pParent == p30);
      assertUnit(p40->pParent == p50);
      assertUnit(p60->pParent == p50);
      assertUnit(p10->data == 10);
      assertUnit(p30->data == 30);
      assertUnit(p40->data == 40);
      assertUnit(p50->data == 50);
      assertUnit(p60->data == 60);
      assertUnit(p70->data == 70);
      assertUnit(p80->data == 80);
      // teardown
      delete p10;
      delete p30;
      delete p40;
      delete p50;
      delete p60;
      delete p70;
      delete p80;
      s.bst.numElements = 0;
      s.bst.root = nullptr;
   }


   /***************************************
    * Erase Value
    *    set::erase(const T &)
    ***************************************/

    // erase from an empty BST
   void test_eraseValue_empty()
   {  // setup
      custom::set <Spy> s;
      Spy spy(99);
      Spy::reset();
      // exercise
      size_t num = s.erase(spy);
      // verify
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(num == 0);
      assertEmptyFixture(s);
   }  // teardown

   // erase with an empty iterator
   void test_eraseValue_standardMissing()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy spy(65);
      Spy::reset();
      // exercise
      size_t num = s.erase(spy);
      // verify
      assertUnit(Spy::numLessthan() == 3);  // compare [50][70][60]
      assertUnit(Spy::numEquals() == 3);    // check [50][70][60]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(num == 0);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }  // teardown

   // remove a leaf node from the standard fixture
   void test_eraseValue_noChildren()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      Spy spy(60);
      Spy::reset();
      // exercise
      size_t num = s.erase(spy);
      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numEquals() == 3);      // check [50][70][60]
      assertUnit(Spy::numDestructor() == 1);  // destroy [60]
      assertUnit(Spy::numDelete() == 1);      // delete [60]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30            [[70]]
      //     +----+----+          +----+
      assertUnit(num == 1);
      assertUnit(s.bst.root->pRight->pLeft == nullptr);
      assertUnit(s.bst.numElements == 6);
      s.bst.root->pRight->pLeft = new custom::BST<Spy>::BNode(Spy(60));
      s.bst.root->pRight->pLeft->pParent = s.bst.root->pRight;
      s.bst.numElements = 7;
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }  // teardown

   void test_eraseValue_oneChild()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         10              60
      //          +----+     
      //              30          
      //            +--+--+
      //           20    40
      custom::set <int> s;
      auto p10 = new custom::BST<int>::BNode(10);
      auto p20 = new custom::BST<int>::BNode(20);
      auto p30 = new custom::BST<int>::BNode(30);
      auto p40 = new custom::BST<int>::BNode(40);
      auto p50 = new custom::BST<int>::BNode(50);
      auto p60 = new custom::BST<int>::BNode(60);
      s.bst.root = p10->pParent = p60->pParent = p50;
      p50->pLeft = p30->pParent = p10;
      p50->pRight = p60;
      p10->pRight = p20->pParent = p40->pParent = p30;
      p30->pLeft = p20;
      p30->pRight = p40;
      s.bst.numElements = 6;
      // exercise
      size_t num = s.erase(10);
      // verify
      //                 50
      //          +-------+-------+
      //         30              60
      //     +----+----+
      //  [[20]]      40
      assertUnit(num == 1);
      assertUnit(s.bst.numElements == 5);
      assertUnit(s.bst.root == p50);
      assertUnit(p50->pLeft == p30);
      assertUnit(p50->pRight == p60);
      assertUnit(p30->pLeft == p20);
      assertUnit(p30->pRight = p40);
      assertUnit(p30->pParent == p50);
      assertUnit(p20->pParent == p30);
      assertUnit(p40->pParent == p30);
      assertUnit(p60->pParent == p50);
      assertUnit(p20->data == 20);
      assertUnit(p30->data == 30);
      assertUnit(p40->data == 40);
      assertUnit(p50->data == 50);
      assertUnit(p60->data == 60);
      // teardown
      delete p20;
      delete p30;
      delete p40;
      delete p50;
      delete p60;
      s.bst.numElements = 0;
      s.bst.root = nullptr;
   }

   void test_eraseValue_twoChildren()
   {  // setup
      //                 70 
      //          +-------+-------+
      //         20              80
      //     +----+----+     
      //    10        50          
      //            +--+--+
      //           30    60
      //            +-+
      //              40 
      custom::set <int> s;
      auto p10 = new custom::BST<int>::BNode(10);
      auto p20 = new custom::BST<int>::BNode(20);
      auto p30 = new custom::BST<int>::BNode(30);
      auto p40 = new custom::BST<int>::BNode(40);
      auto p50 = new custom::BST<int>::BNode(50);
      auto p60 = new custom::BST<int>::BNode(60);
      auto p70 = new custom::BST<int>::BNode(70);
      auto p80 = new custom::BST<int>::BNode(80);
      s.bst.root = p20->pParent = p80->pParent = p70;
      p10->pParent = p50->pParent = p70->pLeft = p20;
      p70->pRight = p80;
      p20->pLeft = p10;
      p20->pRight = p30->pParent = p60->pParent = p50;
      p50->pLeft = p40->pParent = p30;
      p50->pRight = p60;
      p30->pRight = p40;
      s.bst.numElements = 8;
      // exercise
      size_t num = s.erase(20);
      // verify
      //                 70
      //          +-------+-------+
      //       [[30]]            80
      //     +----+----+     
      //    10        50    
      //            +--+--+
      //           40    60
      assertUnit(num == 1);
      assertUnit(s.bst.root == p70);
      assertUnit(s.bst.numElements == 7);
      assertUnit(p70->pLeft == p30);
      assertUnit(p70->pRight == p80);
      assertUnit(p30->pLeft == p10);
      assertUnit(p30->pRight = p50);
      assertUnit(p50->pLeft == p40);
      assertUnit(p50->pRight = p60);
      assertUnit(p30->pParent == p70);
      assertUnit(p80->pParent == p70);
      assertUnit(p10->pParent == p30);
      assertUnit(p50->pParent == p30);
      assertUnit(p40->pParent == p50);
      assertUnit(p60->pParent == p50);
      assertUnit(p10->data == 10);
      assertUnit(p30->data == 30);
      assertUnit(p40->data == 40);
      assertUnit(p50->data == 50);
      assertUnit(p60->data == 60);
      assertUnit(p70->data == 70);
      assertUnit(p80->data == 80);
      // teardown
      delete p10;
      delete p30;
      delete p40;
      delete p50;
      delete p60;
      delete p70;
      delete p80;
      s.bst.numElements = 0;
      s.bst.root = nullptr;
   }


   /***************************************
    * Erase Range
    *    set::erase(iterator itBegin, iterator itEnd)
    ***************************************/
    // erase from an empty BST
   void test_eraseRange_standardNone()
   {  // setup
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      auto itBegin = custom::set <Spy> :: iterator(s.bst.root->pLeft);
      auto itEnd = itBegin;
      Spy::reset();
      // exercise
      auto itReturn = s.erase(itBegin, itEnd);
      // validate
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);   
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(itReturn == itEnd);
      assertStandardFixture(s);
      // teardown
      teardownStandardFixture(s);
   }

   // erase with an empty iterator
   void test_eraseRange_standardOne()
   {  // setup
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20      [[40]]  60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      auto itBegin = custom::set <Spy> ::iterator(s.bst.root->pLeft);
      auto itEnd = custom::set <Spy> ::iterator(s.bst.root->pLeft->pRight);
      Spy::reset();
      // exercise
      auto itReturn = s.erase(itBegin, itEnd);
      // validate
      assertUnit(Spy::numDestructor() == 1);  // destroy [30]
      assertUnit(Spy::numDelete() == 1);      // delete  [30]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //       [[40]]            70  
      //     +----+          +----+----+
      //    20              60        80  
      assertUnit(itReturn == itEnd);
      assertUnit(s.bst.numElements == 6);
      assertUnit(s.bst.root != nullptr);
      if (s.bst.root)
      {
         assertUnit(s.bst.root->data == Spy(50));
         assertUnit(s.bst.root->pParent == nullptr);
         assertUnit(s.bst.root->pLeft != nullptr);
         if (s.bst.root->pLeft)
         {
            assertUnit(s.bst.root->pLeft->data == Spy(40));
            assertUnit(s.bst.root->pLeft->pParent == s.bst.root);
            assertUnit(s.bst.root->pLeft->pLeft != nullptr);
            if (s.bst.root->pLeft->pLeft)
            {
               assertUnit(s.bst.root->pLeft->pLeft->data == Spy(20));
               assertUnit(s.bst.root->pLeft->pLeft->pLeft == nullptr);
               assertUnit(s.bst.root->pLeft->pLeft->pRight == nullptr);
            }
            assertUnit(s.bst.root->pLeft->pRight == nullptr);
         }
         assertUnit(s.bst.root->pRight != nullptr);
         if (s.bst.root->pRight)
         {
            assertUnit(s.bst.root->pRight->data == Spy(70));
            assertUnit(s.bst.root->pRight->pParent == s.bst.root);
            assertUnit(s.bst.root->pRight->pLeft != nullptr);
            if (s.bst.root->pRight->pLeft)
            {
               assertUnit(s.bst.root->pRight->pLeft->data == Spy(60));
               assertUnit(s.bst.root->pRight->pLeft->pParent == s.bst.root->pRight);
               assertUnit(s.bst.root->pRight->pLeft->pLeft == nullptr);
               assertUnit(s.bst.root->pRight->pLeft->pRight == nullptr);
            }
            assertUnit(s.bst.root->pRight->pRight != nullptr);
            if (s.bst.root->pRight->pRight)
            {
               assertUnit(s.bst.root->pRight->pRight->data == Spy(80));
               assertUnit(s.bst.root->pRight->pRight->pParent == s.bst.root->pRight);
               assertUnit(s.bst.root->pRight->pRight->pLeft == nullptr);
               assertUnit(s.bst.root->pRight->pRight->pRight == nullptr);
            }
         }
      }
      // teardown
      teardownStandardFixture(s);
   }
   // remove a leaf node from the standard fixture
   void test_eraseRange_standardMany()
   {  // setup
      //                 50 
      //          +-------+-------+
      //       [[30]]          [[70]]
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::set <Spy> s;
      setupStandardFixture(s);
      auto itBegin = custom::set <Spy> ::iterator(s.bst.root->pLeft);
      auto itEnd = custom::set <Spy> ::iterator(s.bst.root->pRight);
      Spy::reset();
      // exercise
      auto itReturn = s.erase(itBegin, itEnd);
      // validate
      assertUnit(Spy::numDestructor() == 4);  // destroy [30][40][50][60]
      assertUnit(Spy::numDelete() == 4);      // delete  [30][40][50][60]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 70 
      //          +-------+-------+
      //         20            [[80]]
      assertUnit(itReturn == itEnd);
      assertUnit(s.bst.numElements == 3);
      assertUnit(s.bst.root != nullptr);
      if (s.bst.root)
      {
         assertUnit(s.bst.root->data == Spy(70));
         assertUnit(s.bst.root->pParent == nullptr);
         assertUnit(s.bst.root->pLeft != nullptr);
         if (s.bst.root->pLeft)
         {
            assertUnit(s.bst.root->pLeft->data == Spy(20));
            assertUnit(s.bst.root->pLeft->pParent == s.bst.root);
            assertUnit(s.bst.root->pLeft->pLeft == nullptr);
            assertUnit(s.bst.root->pLeft->pRight == nullptr);
         }
         assertUnit(s.bst.root->pRight != nullptr);
         if (s.bst.root->pRight)
         {
            assertUnit(s.bst.root->pRight->data == Spy(80));
            assertUnit(s.bst.root->pRight->pParent == s.bst.root);
            assertUnit(s.bst.root->pRight->pLeft == nullptr);
            assertUnit(s.bst.root->pRight->pRight == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(s);
   }
   void test_eraseRange_oneChild()
   {  // setup

   }

   void test_eraseRange_twoChildren()
   {  // setup

   }

   /*************************************************************
    * SETUP STANDARD FIXTURE
    *                (50b)
    *          +-------+-------+
    *        (30b)           (70b)
    *     +----+----+     +----+----+
    *   (20r)     (40r) (60r)     (80r)
    *************************************************************/
   void setupStandardFixture(custom::set<Spy>& s)
   {
      // make sure that bst is clean
      assertUnit(s.bst.numElements == 0);
      assertUnit(s.bst.root == nullptr);

      // allocate
      custom::BST<Spy>::BNode* p20 = new custom::BST<Spy>::BNode(Spy(20));
      custom::BST<Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST<Spy>::BNode* p40 = new custom::BST<Spy>::BNode(Spy(40));
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST<Spy>::BNode* p60 = new custom::BST<Spy>::BNode(Spy(60));
      custom::BST<Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));
      custom::BST<Spy>::BNode* p80 = new custom::BST<Spy>::BNode(Spy(80));

      // hook up the pointers down
      p30->pLeft = p20;
      p30->pRight = p40;
      p50->pLeft = p30;
      p50->pRight = p70;
      p70->pLeft = p60;
      p70->pRight = p80;

      // hook up the pointers up
      p20->pParent = p40->pParent = p30;
      p30->pParent = p70->pParent = p50;
      p60->pParent = p80->pParent = p70;

      // color everything
      p50->isRed = p30->isRed = p70->isRed = false;
      p20->isRed = p40->isRed = p60->isRed = p80->isRed = true;

      // now assign everything to the bst
      s.bst.root = p50;
      s.bst.numElements = 7;
   }

   /*************************************************************
    * TEARDOWN STANDARD FIXTURE
    *************************************************************/
   void teardownStandardFixture(custom::set<Spy>& s)
   {
      s.bst.clear();
   }

   /*************************************************************
    * VERIFY EMPTY FIXTURE
    *************************************************************/
   void assertEmptyFixtureParameters(const custom::set<Spy> &s, int line, const char* function)
   {
      assertIndirect(s.bst.root == nullptr);
      assertIndirect(s.bst.numElements == 0);
   }


   /*************************************************************
    * VERIFY STANDARD FIXTURE
    *                (50b)
    *          +-------+-------+
    *        (30b)           (70b)
    *     +----+----+     +----+----+
    *   (20r)     (40r) (60r)     (80r)
    *************************************************************/
   void assertStandardFixtureParameters(const custom::set<Spy>& s, int line, const char* function)
   {
      // verify the member variables
      assertIndirect(s.bst.numElements == 7);
      assertIndirect(s.bst.root != nullptr);

      // verify the pointers down
      assertIndirect(s.bst.root != nullptr);
      if (s.bst.root)
      {
         assertIndirect(s.bst.root->data == Spy(50));
         assertIndirect(s.bst.root->isRed == false);
         assertIndirect(s.bst.root->pParent == nullptr);
         assertIndirect(s.bst.root->pLeft != nullptr);
         if (s.bst.root->pLeft)
         {
            assertIndirect(s.bst.root->pLeft->data == Spy(30));
            assertIndirect(s.bst.root->pLeft->isRed == false);
            assertIndirect(s.bst.root->pLeft->pParent == s.bst.root);
            assertIndirect(s.bst.root->pLeft->pLeft != nullptr);
            if (s.bst.root->pLeft->pLeft)
            {
               assertIndirect(s.bst.root->pLeft->pLeft->data == Spy(20));
               assertIndirect(s.bst.root->pLeft->pLeft->isRed == true);
               assertIndirect(s.bst.root->pLeft->pLeft->pParent == s.bst.root->pLeft);
               assertIndirect(s.bst.root->pLeft->pLeft->pLeft == nullptr);
               assertIndirect(s.bst.root->pLeft->pLeft->pRight == nullptr);
            }
            assertIndirect(s.bst.root->pLeft->pRight != nullptr);
            if (s.bst.root->pLeft->pRight)
            {
               assertIndirect(s.bst.root->pLeft->pRight->data == Spy(40));
               assertIndirect(s.bst.root->pLeft->pRight->isRed == true);
               assertIndirect(s.bst.root->pLeft->pRight->pParent == s.bst.root->pLeft);
               assertIndirect(s.bst.root->pLeft->pRight->pLeft == nullptr);
               assertIndirect(s.bst.root->pLeft->pRight->pRight == nullptr);
            }
         }
         assertIndirect(s.bst.root->pRight != nullptr);
         if (s.bst.root->pRight)
         {
            assertIndirect(s.bst.root->pRight->data == Spy(70));
            assertIndirect(s.bst.root->pRight->isRed == false);
            assertIndirect(s.bst.root->pRight->pParent == s.bst.root);
            assertIndirect(s.bst.root->pRight->pLeft != nullptr);
            if (s.bst.root->pRight->pLeft)
            {
               assertIndirect(s.bst.root->pRight->pLeft->data == Spy(60));
               assertIndirect(s.bst.root->pRight->pLeft->isRed == true);
               assertIndirect(s.bst.root->pRight->pLeft->pParent == s.bst.root->pRight);
               assertIndirect(s.bst.root->pRight->pLeft->pLeft == nullptr);
               assertIndirect(s.bst.root->pRight->pLeft->pRight == nullptr);
            }
            assertIndirect(s.bst.root->pRight->pRight != nullptr);
            if (s.bst.root->pRight->pRight)
            {
               assertIndirect(s.bst.root->pRight->pRight->data == Spy(80));
               assertIndirect(s.bst.root->pRight->pRight->isRed == true);
               assertIndirect(s.bst.root->pRight->pRight->pParent == s.bst.root->pRight);
               assertIndirect(s.bst.root->pRight->pRight->pLeft == nullptr);
               assertIndirect(s.bst.root->pRight->pRight->pRight == nullptr);
            }
         }
      }
   }


};

#endif // DEBUG
