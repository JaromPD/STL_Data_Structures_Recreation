/***********************************************************************
 * Header:
 *    TEST MAP
 * Summary:
 *    Unit tests for Map
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once
#ifdef DEBUG

#include "map.h"        // class under test
#include "unitTest.h"   // unit test baseclass
#include "spy.h"        // spy is a mock class to monitor the class under test

#include <map>
#include <vector>

/***********************************************
 * TEST MAP
 * Unit tests for the Map class
 ***********************************************/
class TestMap : public UnitTest
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
      test_assign_standardToNotempty();
      test_assignMove_emptyToEmpty();
      test_assignMove_standardToEmpty();
      test_assignMove_emptyToStandard();
      test_assignMove_standardToNotempty();
      test_assignInit_emptyToEmpty();
      test_assignInit_standardToEmpty();
      test_assignInit_emptyToStandard();
      test_assignInit_standardToNotempty();
      test_swap_emptyToEmpty();
      test_swap_standardToEmpty();
      test_swap_emptyToStandard();
      test_swap_standardToStandard();

      // Iterator
      test_begin_empty();
      test_begin_standard();
      test_end_standard();
      test_iterator_increment_standardToChild();
      test_iterator_increment_standardToParent();
      test_iterator_dereference_standardRead();

      // Access
      test_access_standardRootRead();
      test_access_standardLeftRead();
      test_access_standardRightRead();
      test_access_standardRootWrite();
      test_access_standardLeftWrite();
      test_access_standardRightWrite();
      test_access_emptyWrite();
      test_access_standardFrontInsert();
      test_access_standardMiddleInsert();
      test_at_standardRootRead();
      test_at_standardLeftRead();
      test_at_standardRightRead();
      test_at_standardRootWrite();
      test_at_standardLeftWrite();
      test_at_standardRightWrite();
      test_at_emptyWrite();
      test_at_standardWrite();
      test_find_empty();
      test_find_standardRoot();
      test_find_standardLeft();
      test_find_standardRight();
      test_find_standardMissing();

      // Insert
      test_insertCopy_empty();
      test_insertCopy_standardFront();
      test_insertCopy_standardMiddle();
      test_insertMove_empty();
      test_insertMove_standard();

      // Remove
      test_erase_emptyKey();
      test_erase_standardKey();
      test_erase_standardKeyMissing();
      test_erase_emptyIterator();
      test_erase_standardIterator();
      test_erase_standardIteratorMissing();
      test_erase_emptyRange();
      test_erase_standardRange();
      test_clear_empty();
      test_clear_standard();

      // Status
      test_empty_empty();
      test_empty_standard();
      test_size_empty();
      test_size_standard();

      report("Map");
   }

   /***************************************
    * CONSTRUCTOR
    *    map::map()
    ***************************************/

    // default constructor, no allocations
   void test_construct_default()
   {  // setup
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> m;
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
      assertEmptyFixture(m);
   }  // teardown


   /***************************************
    * COPY
    *     map::map(const map &)
    ***************************************/

   // copy an empty map
   void test_constructCopy_empty()
   {  // setup
      custom::map<std::string, Spy> mSrc;
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> mDes(mSrc);
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
      assertEmptyFixture(mSrc);
      assertEmptyFixture(mDes);
   }  // teardown

   // copy a one-element map
   void test_constructCopy_one()
   {  // setup
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      custom::map<std::string, Spy> mSrc;
      custom::pair<std::string, Spy> p50(std::string("50"), Spy(50));
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode50;
      bnode50 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(p50);
      mSrc.bst.root = bnode50;
      mSrc.bst.numElements = 1;
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> mDes(mSrc);
      // verify
      assertUnit(Spy::numCopy() == 1);     // copy-create [50]
      assertUnit(Spy::numAlloc() == 1);    // allocate    [50]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(mSrc.bst.root != mDes.bst.root);
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      assertUnit(mSrc.bst.numElements == 1);
      assertUnit(mSrc.bst.root != nullptr);
      if (mSrc.bst.root)
      {
         assertUnit(mSrc.bst.root->data.first == std::string("50"));
         assertUnit(mSrc.bst.root->data.second == Spy(50));
         assertUnit(mSrc.bst.root->pLeft == nullptr);
         assertUnit(mSrc.bst.root->pRight == nullptr);
         assertUnit(mSrc.bst.root->pParent == nullptr);
      }
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      assertUnit(mDes.bst.numElements == 1);
      assertUnit(mDes.bst.root != nullptr);
      if (mDes.bst.root)
      {
         assertUnit(mDes.bst.root->data.first == std::string("50"));
         assertUnit(mDes.bst.root->data.second == Spy(50));
         assertUnit(mDes.bst.root->pLeft == nullptr);
         assertUnit(mDes.bst.root->pRight == nullptr);
         assertUnit(mDes.bst.root->pParent == nullptr);
      }
      // teardown
      teardownStandardFixture(mSrc);
      teardownStandardFixture(mDes);
   }

   // copy the standard fixture
   void test_constructCopy_standard()
   {  // setup
      //    "30"     "50"     "70" 
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mSrc;
      setupStandardFixture(mSrc);
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> mDes(mSrc);
      // verify
      assertUnit(Spy::numCopy() == 3);     // copy-create [50][30][70]
      assertUnit(Spy::numAlloc() == 3);    // allocate    [50][30][70]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "30"     "50"     "70" 
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertUnit(mSrc.bst.root != mDes.bst.root);
      assertStandardFixture(mSrc);
      assertStandardFixture(mDes);
      // teardown
      teardownStandardFixture(mSrc);
      teardownStandardFixture(mDes);
   }

   /***************************************
    * MOVE
    *     map::map(map &&)
    ***************************************/

    // copy an empty map
   void test_constructMove_empty()
   {  // setup
      custom::map<std::string, Spy> mSrc;
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> mDes(std::move(mSrc));
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
      assertEmptyFixture(mSrc);
      assertEmptyFixture(mDes);
   }  // teardown

   // copy a one-element map
   void test_constructMove_one()
   {  // setup
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      custom::map<std::string, Spy> mSrc;
      custom::pair<std::string, Spy> p50(std::string("50"), Spy(50));
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode50;
      bnode50 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(p50);
      mSrc.bst.root = bnode50;
      mSrc.bst.numElements = 1;
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> mDes(std::move(mSrc));
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
      assertUnit(mSrc.bst.root != mDes.bst.root);
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      assertUnit(mDes.bst.numElements == 1);
      assertUnit(mDes.bst.root != nullptr);
      if (mDes.bst.root)
      {
         assertUnit(mDes.bst.root->data.first == std::string("50"));
         assertUnit(mDes.bst.root->data.second == Spy(50));
         assertUnit(mDes.bst.root->pLeft == nullptr);
         assertUnit(mDes.bst.root->pRight == nullptr);
         assertUnit(mDes.bst.root->pParent == nullptr);
      }
      assertEmptyFixture(mSrc);
      // teardown
      teardownStandardFixture(mDes);
   }

   // copy the standard fixture
   void test_constructMove_standard()
   {  // setup
      //    "30"     "50"     "70" 
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mSrc;
      setupStandardFixture(mSrc);
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> mDes(std::move(mSrc));
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
      //    "30"     "50"     "70" 
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertUnit(mSrc.bst.root != mDes.bst.root);
      assertStandardFixture(mDes);
      assertEmptyFixture(mSrc);
      // teardown
      teardownStandardFixture(mDes);
   }

   /***************************************
    * INITIALIZER LIST CONSTRUCTOR
    *     map::map(const initializer_list &)
    ***************************************/

    // copy an empty map
   void test_constructInit_empty()
   {  // setup
      //     {  }
      std::initializer_list<custom::pair<std::string, Spy>> il{};
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> m(il);
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
      assertEmptyFixture(m);
   }  // teardown

   // copy a one-element map
   void test_constructInit_one()
   {  // setup
      //      { ("50",50) }
      custom::pair<std::string, Spy> p50(std::string("50"), Spy(50));
      std::initializer_list<custom::pair<std::string, Spy>> il{p50};
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> m(il);
      // verify
      assertUnit(Spy::numCopy() == 1);      // copy-create [50]
      assertUnit(Spy::numAlloc() == 1);     // allocate    [50]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      assertUnit(m.bst.numElements == 1);
      assertUnit(m.bst.root != nullptr);
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pLeft == nullptr);
         assertUnit(m.bst.root->pRight == nullptr);
         assertUnit(m.bst.root->pParent == nullptr);
      }
      // teardown
      teardownStandardFixture(m);
   }

   // copy the standard fixture
   void test_constructInit_standard()
   {  // setup
      //      { ("50",50)  ("30",30)  ("70",70) }
      custom::pair<std::string, Spy> p50(std::string("50"), Spy(50));
      custom::pair<std::string, Spy> p30(std::string("30"), Spy(30));
      custom::pair<std::string, Spy> p70(std::string("70"), Spy(70));
      std::initializer_list<custom::pair<std::string, Spy>> il{ p50, p30, p70 };
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> m(il);
      // verify
      assertUnit(Spy::numCopy() == 3);     // copy-create [50][30][70]
      assertUnit(Spy::numAlloc() == 3);    // allocate    [50][30][70]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "30"     "50"     "70" 
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   /***************************************
    * RANGE CONSTRUCTOR
    *     map::map(first, last)
    ***************************************/

    // copy an empty map
   void test_constructRange_empty()
   {  // setup
      //     {  }
      std::initializer_list<custom::pair<std::string, Spy>> il{};
      auto itBegin = il.begin();
      auto itEnd = il.end();
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> m(itBegin, itEnd);
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
      assertEmptyFixture(m);
   }  // teardown

   // copy a one-element map
   void test_constructRange_one()
   {  // setup
      //      { ("50",50) }
      custom::pair<std::string, Spy> p50(std::string("50"), Spy(50));
      std::initializer_list<custom::pair<std::string, Spy>> il{ p50 };
      auto itBegin = il.begin();
      auto itEnd = il.end();
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> m(itBegin, itEnd);
      // verify
      assertUnit(Spy::numCopy() == 1);      // copy-create [50]
      assertUnit(Spy::numAlloc() == 1);     // allocate    [50]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      assertUnit(m.bst.numElements == 1);
      assertUnit(m.bst.root != nullptr);
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pLeft == nullptr);
         assertUnit(m.bst.root->pRight == nullptr);
         assertUnit(m.bst.root->pParent == nullptr);
      }
      // teardown
      teardownStandardFixture(m);
   }

   // copy the standard fixture
   void test_constructRange_standard()
   {  // setup
      //      { ("50",50)  ("30",30)  ("70",70) }
      custom::pair<std::string, Spy> p50(std::string("50"), Spy(50));
      custom::pair<std::string, Spy> p30(std::string("30"), Spy(30));
      custom::pair<std::string, Spy> p70(std::string("70"), Spy(70));
      std::initializer_list<custom::pair<std::string, Spy>> il{ p50, p30, p70 };
      auto itBegin = il.begin();
      auto itEnd = il.end();
      Spy::reset();
      // exercise
      custom::map<std::string, Spy> m(itBegin, itEnd);
      // verify
      assertUnit(Spy::numCopy() == 3);     // copy-create [50][30][70]
      assertUnit(Spy::numAlloc() == 3);    // allocate    [50][30][70]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "30"     "50"     "70" 
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   /***************************************
    * DESTRUCTOR
    ***************************************/

    // destroy an empty set
   void test_destructor_empty()
   {  // setup
      {
         custom::map<std::string, Spy> m;
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
         //    "30"     "50"     "70" 
         //   +----+   +----+   +----+
         //   | 30 | - | 50 | - | 70 |
         //   +----+   +----+   +----+
         custom::map<std::string, Spy> m;
         setupStandardFixture(m);
         Spy::reset();
      }  // exercise
      // verify
      assertUnit(Spy::numDestructor() == 3); // destroy  [30][50][70]
      assertUnit(Spy::numDelete() == 3);     // delete   [30][50][70]
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
    * SIZE, EMPTY
    *    map::empty()
    *    map::size()
    ***************************************/

    // is the empty map empty?
   void test_empty_empty()
   {  // setup
      custom::map<std::string, Spy> m;
      Spy::reset();
      // exercise
      bool empty = m.empty();
      // verify
      assertUnit(Spy::numDestructor() == 0); 
      assertUnit(Spy::numDelete() == 0);     
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(empty == true);
      assertEmptyFixture(m);
   }  // teardown

   // is the standard fixture empty?
   void test_empty_standard()
   {  // setup
      //    "30"     "50"     "70" 
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy::reset();
      // exercise
      bool empty = m.empty();
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(empty == false);
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // what is the size of an empty map?
   void test_size_empty()
   {  // setup
      custom::map<std::string, Spy> m;
      Spy::reset();
      // exercise
      size_t size = m.size();
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(size == 0);
      assertEmptyFixture(m);
   }  // teardown

   // what is the size of the standard fixture?
   void test_size_standard()
   {  // setup
      //    "30"     "50"     "70" 
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy::reset();
      // exercise
      size_t size = m.size();
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(size == 3);
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   /***************************************
    * ASSIGNMENT
    *    map::operator=(const map &)
    ***************************************/

   // assign: empty = empty
   void test_assign_emptyToEmpty()
   {  // setup
      custom::map<std::string, Spy> mSrc;
      custom::map<std::string, Spy> mDes;
      Spy::reset();
      // exercise
      mDes = mSrc;
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(mSrc);
      assertEmptyFixture(mDes);
   }  // teardown

   // assign: empty = standard
   void test_assign_standardToEmpty()
   {  // setup
      //    "30"     "50"     "70"   = mSrc
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mSrc;
      setupStandardFixture(mSrc);
      custom::map<std::string, Spy> mDes;
      Spy::reset();
      // exercise
      mDes = mSrc;
      // verify
      assertUnit(Spy::numCopy() == 3);         // copy-create [50][30][70]
      assertUnit(Spy::numAlloc() == 3);        // allocate    [50][30][70]
      assertUnit(Spy::numEquals() == 0); 
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(mSrc.bst.root != mDes.bst.root);
      //    "30"     "50"     "70" 
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(mSrc);
      assertStandardFixture(mDes);
      // teardown
      teardownStandardFixture(mSrc);
      teardownStandardFixture(mDes);
   }

   // assign: standard = empty
   void test_assign_emptyToStandard()
   {  // setup
      custom::map<std::string, Spy> mSrc;
      //    "30"     "50"     "70"   = mDes
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mDes;
      setupStandardFixture(mDes);
      Spy::reset();
      // exercise
      mDes = mSrc;
      // verify
      assertUnit(Spy::numDestructor() == 3);  // destroy  [50][30][70]
      assertUnit(Spy::numDelete() == 3);      // delete   [50][30][70]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);        
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(mSrc);
      assertEmptyFixture(mDes);
   }  // teardown

   // assign: notempty = standard
   void test_assign_standardToNotempty() 
   {  // setup
      //    "30"     "50"     "70"   = mSrc
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //        (50)
      //    +-----+-----+
      //  (30)        (70)
      custom::map<std::string, Spy> mSrc;
      setupStandardFixture(mSrc);
      //    "40"     "60"  = mDes
      //   +----+   +----+ 
      //   | 40 | - | 60 | 
      //   +----+   +----+ 
      //     p40      p60
      //        (40)
      //          +-----+
      //              (60)
      custom::map<std::string, Spy> mDes;
      custom::pair<std::string, Spy> pair40(std::string("40"), Spy(40));
      custom::pair<std::string, Spy> pair60(std::string("60"), Spy(60));
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode40;
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode60;
      bnode40 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair40);
      bnode60 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair60);
      bnode40->pRight = bnode60;
      bnode60->pParent = bnode40;
      bnode40->isRed = false;
      bnode60->isRed = true;
      mDes.bst.root = bnode40;
      mDes.bst.numElements = 2;
      Spy::reset();
      // exercise
      mDes = mSrc;
      // verify
      assertUnit(Spy::numAlloc() == 1);       // allocate    [30]
      assertUnit(Spy::numCopy() == 1);        // copy-create [30]
      assertUnit(Spy::numAssign() == 2);      // assign [50][50]
      assertUnit(Spy::numDestructor() == 0); 
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(mSrc.bst.root != mDes.bst.root);
      //    "30"     "50"     "70"   
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(mSrc);
      assertStandardFixture(mDes);
      // teardown
      teardownStandardFixture(mSrc);
      teardownStandardFixture(mDes);
   }

   /***************************************
    * ASSIGNMENT - Move
    *    map::operator=(map &&)
    ***************************************/

    // assign: empty = move(empty)
   void test_assignMove_emptyToEmpty()
   {  // setup
      custom::map<std::string, Spy> mSrc;
      custom::map<std::string, Spy> mDes;
      Spy::reset();
      // exercise
      mDes = std::move(mSrc);
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(mSrc);
      assertEmptyFixture(mDes);
   }  // teardown

   // assign: empty = move(standard)
   void test_assignMove_standardToEmpty()
   {  // setup
      //    "30"     "50"     "70"   = mSrc
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mSrc;
      setupStandardFixture(mSrc);
      custom::map<std::string, Spy> mDes;
      Spy::reset();
      // exercise
      mDes = std::move(mSrc);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(mSrc.bst.root != mDes.bst.root);
      //    "30"     "50"     "70" = mDes
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(mDes);
      assertEmptyFixture(mSrc);
      // teardown
      teardownStandardFixture(mDes);
   }

   // assign: standard = move(empty)
   void test_assignMove_emptyToStandard()
   {  // setup
      custom::map<std::string, Spy> mSrc;
      //    "30"     "50"     "70"   = mDes
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mDes;
      setupStandardFixture(mDes);
      Spy::reset();
      // exercise
      mDes = std::move(mSrc);
      // verify
      assertUnit(Spy::numDestructor() == 3);  // destroy  [50][30][70]
      assertUnit(Spy::numDelete() == 3);      // delete   [50][30][70]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(mSrc);
      assertEmptyFixture(mDes);
   }  // teardown

   // assign: notempty = move(standard)
   void test_assignMove_standardToNotempty()
   {  // setup
      //    "30"     "50"     "70"   = mSrc
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mSrc;
      setupStandardFixture(mSrc);
      //    "40"     "60"  = mDes
      //   +----+   +----+ 
      //   | 40 | - | 60 | 
      //   +----+   +----+ 
      //     p40      p60
      custom::map<std::string, Spy> mDes;
      custom::pair<std::string, Spy> pair40(std::string("40"), Spy(40));
      custom::pair<std::string, Spy> pair60(std::string("60"), Spy(60));
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode40;
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode60;
      bnode40 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair40);
      bnode60 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair60);
      bnode40->pRight = bnode60;
      bnode60->pParent = bnode40;
      bnode40->isRed = false;
      bnode60->isRed = true;
      mDes.bst.root = bnode40;
      mDes.bst.numElements = 2;
      Spy::reset();
      // exercise
      mDes = std::move(mSrc);
      // verify
      assertUnit(Spy::numDestructor() == 2);  // destroy  [40][60]
      assertUnit(Spy::numDelete() == 2);      // delete   [40][60]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(mSrc.bst.root != mDes.bst.root);
      //    "30"     "50"     "70"   = mDes
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertEmptyFixture(mSrc);
      assertStandardFixture(mDes);
      // teardown
      teardownStandardFixture(mDes);
   }

   /***************************************
    * ASSIGNMENT - Initialization List
    *    map::operator={}
    ***************************************/

    // assign: empty = {}
   void test_assignInit_emptyToEmpty()
   {  // setup
      //      {  } = il
      std::initializer_list<custom::pair<std::string, Spy>> il{ };
      custom::map<std::string, Spy> m;
      Spy::reset();
      // exercise
      m = il;
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(m);
   }  // teardown

   // assign: empty = {50, 30, 70}
   void test_assignInit_standardToEmpty()
   {  // setup
      //      { ("50",50) ("30",30) ("70",70)}
      custom::pair<std::string, Spy> p50(std::string("50"), Spy(50));
      custom::pair<std::string, Spy> p30(std::string("30"), Spy(30));
      custom::pair<std::string, Spy> p70(std::string("70"), Spy(70));
      std::initializer_list<custom::pair<std::string, Spy>> il{ p50, p30, p70 };
      custom::map<std::string, Spy> m;
      Spy::reset();
      // exercise
      m = il;
      // verify
      assertUnit(Spy::numCopy() == 3);       // copy-create [50][30][70]
      assertUnit(Spy::numAlloc() == 3);      // allocate    [50][30][70]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    "30"     "50"     "70" = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // assign: standard = move(empty)
   void test_assignInit_emptyToStandard()
   {  // setup
      //      {  } = il
      std::initializer_list<custom::pair<std::string, Spy>> il{ };
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy::reset();
      // exercise
      m = il;
      // verify
      assertUnit(Spy::numDestructor() == 3);  // destroy  [50][30][70]
      assertUnit(Spy::numDelete() == 3);      // delete   [50][30][70]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(m);
   }  // teardown

   // assign: notempty = move(standard)
   void test_assignInit_standardToNotempty()
   {  // setup
      //      { ("50",50) ("30",30) ("70",70)}
      custom::pair<std::string, Spy> p50(std::string("50"), Spy(50));
      custom::pair<std::string, Spy> p30(std::string("30"), Spy(30));
      custom::pair<std::string, Spy> p70(std::string("70"), Spy(70));
      std::initializer_list<custom::pair<std::string, Spy>> il{ p50, p30, p70 };
      //    "40"     "60"  = m
      //   +----+   +----+ 
      //   | 40 | - | 60 | 
      //   +----+   +----+ 
      //     p40      p60
      custom::map<std::string, Spy> m;
      custom::pair<std::string, Spy> pair40(std::string("40"), Spy(40));
      custom::pair<std::string, Spy> pair60(std::string("60"), Spy(60));
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode40;
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode60;
      bnode40 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair40);
      bnode60 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair60);
      bnode40->pRight = bnode60;
      bnode60->pParent = bnode40;
      bnode40->isRed = false;
      bnode60->isRed = true;
      m.bst.root = bnode40;
      m.bst.numElements = 2;
      Spy::reset();
      // exercise
      m = il;
      // verify
      assertUnit(Spy::numDestructor() == 2);  // destroy  [40][60]
      assertUnit(Spy::numDelete() == 2);      // delete   [40][60]
      assertUnit(Spy::numCopy() == 3);        // copy-create [50][30][70]
      assertUnit(Spy::numAlloc() == 3);       // allocate    [50][30][70]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   /***************************************
    * SWAP
    *    swap(lhs, rhs)
    ***************************************/

    // swap(empty, empty)
   void test_swap_emptyToEmpty()
   {  // setup
      custom::map<std::string, Spy> mRHS;
      custom::map<std::string, Spy> mLHS;
      Spy::reset();
      // exercise
      swap(mLHS, mRHS);
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(mRHS);
      assertEmptyFixture(mLHS);
   }  // teardown

   // swap(standard, empty)
   void test_swap_standardToEmpty()
   {  // setup
      //    "30"     "50"     "70"   = mLHS
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mLHS;
      setupStandardFixture(mLHS);
      custom::map<std::string, Spy> mRHS;
      Spy::reset();
      // exercise
      swap(mLHS, mRHS);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(mLHS.bst.root != mRHS.bst.root);
      //    "30"     "50"     "70" = mRHS
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(mRHS);
      assertEmptyFixture(mLHS);
      // teardown
      teardownStandardFixture(mRHS);
   }

   // swap(empty, standard)
   void test_swap_emptyToStandard()
   {  // setup
      custom::map<std::string, Spy> mLHS;
      //    "30"     "50"     "70"   = mRHS
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mRHS;
      setupStandardFixture(mRHS);
      Spy::reset();
      // exercise
      swap(mLHS, mRHS);
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);  
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertStandardFixture(mLHS);
      assertEmptyFixture(mRHS);
      // teardown 
      teardownStandardFixture(mLHS);
   }

   // swap(standard, different)
   void test_swap_standardToStandard()
   {  // setup
      //    "30"     "50"     "70"   = mLHS
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> mLHS;
      setupStandardFixture(mLHS);
      //    "40"     "60"  = mRHS
      //   +----+   +----+ 
      //   | 40 | - | 60 | 
      //   +----+   +----+ 
      //     p40      p60
      custom::map<std::string, Spy> mRHS;
      custom::pair<std::string, Spy> pair40(std::string("40"), Spy(40));
      custom::pair<std::string, Spy> pair60(std::string("60"), Spy(60));
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode40;
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode60;
      bnode40 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair40);
      bnode60 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair60);
      bnode40->pRight = bnode60;
      bnode60->pParent = bnode40;
      bnode40->isRed = false;
      bnode60->isRed = true;
      mRHS.bst.root = bnode40;
      mRHS.bst.numElements = 2;
      Spy::reset();
      // exercise
      swap(mLHS, mRHS);
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(mLHS.bst.root != mRHS.bst.root);
      //    "30"     "50"     "70"   = mRHS
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(mRHS);
      //    "40"     "60"  = mLHS
      //   +----+   +----+ 
      //   | 40 | - | 60 | 
      //   +----+   +----+ 
      assertUnit(mLHS.bst.numElements == 2);
      assertUnit(mLHS.bst.root != nullptr);
      //         (40)
      //          +-----+
      //               (60)
      if (mLHS.bst.root)
      {
         assertUnit(mLHS.bst.root->data.first == std::string("40"));
         assertUnit(mLHS.bst.root->data.second == Spy(40));
         assertUnit(mLHS.bst.root->pParent == nullptr);
         assertUnit(mLHS.bst.root->pLeft == nullptr);
         assertUnit(mLHS.bst.root->pRight != nullptr);
         if (mLHS.bst.root->pRight)
         {
            assertUnit(mLHS.bst.root->pRight->data.first == std::string("60"));
            assertUnit(mLHS.bst.root->pRight->data.second == Spy(60));
            assertUnit(mLHS.bst.root->pRight->pParent == mLHS.bst.root);
            assertUnit(mLHS.bst.root->pRight->pLeft == nullptr);
            assertUnit(mLHS.bst.root->pRight->pRight == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(mLHS);
      teardownStandardFixture(mRHS);
   }


   /***************************************
    * CLEAR
    *     map::clear()
    ***************************************/

   // clear an empty map
   void test_clear_empty()
   {  // setup
      custom::map<std::string, Spy> m;
      Spy::reset();
      // exercise
      m.clear();
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(m);
   }  // teardown

   // clear the standard map
   void test_clear_standard()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy::reset();
      // exercise
      m.clear();
      // verify
      assertUnit(Spy::numDestructor() == 3);  // destroy [50][30][70]
      assertUnit(Spy::numDelete() == 3);      // delete  [50][30][70]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(m);
   }  // teardown


   /***************************************
    * ITERATOR
    *     map::begin()
    *     map::end()
    *     map::iterator::operaator++()
    *     map::iterator::operator*()
    ***************************************/

   // begin() from an empty map
   void test_begin_empty()
   {  // setup
      custom::map<std::string, Spy> m;
      custom::map<std::string, Spy>::iterator it;
      // exercise
      it = m.begin();
      // verify
      assertUnit(it.it.pNode == nullptr);
      assertEmptyFixture(m);
   }  // teardown

   // begin() from the standard fixture
   void test_begin_standard()
   {  // setup
      custom::map<std::string, Spy>::iterator it;
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy::reset();
      // exercise
      it = m.begin();
      // verify
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //     it
      assertUnit(it.it.pNode == m.bst.root->pLeft);
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // end() from the standard fixture
   void test_end_standard()
   {  // setup
      custom::map<std::string, Spy>::iterator it;
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy::reset();
      // exercise
      it = m.end();
      // verify
      assertUnit(Spy::numDestructor() == 0);  
      assertUnit(Spy::numDelete() == 0);      
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.it.pNode == nullptr);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //                               it
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // iterator increment to the parent from the standard fixture
   void test_iterator_increment_standardToParent()
   {  // setup
      custom::map<std::string, Spy>::iterator it;
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //     it
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      it.it.pNode = m.bst.root->pLeft;
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
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //              it
      assertUnit(it.it.pNode == m.bst.root);
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // iterator increment to the child from the standard fixture
   void test_iterator_increment_standardToChild()
   {  // setup
      custom::map<std::string, Spy>::iterator it;
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //              it
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      it.it.pNode = m.bst.root;
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
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.it.pNode == m.bst.root->pRight);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //                       it
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // read one element from the standard fixture
   void test_iterator_dereference_standardRead()
   {  // setup
      custom::map<std::string, Spy>::iterator it;
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //              it
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      it.it.pNode = m.bst.root;
      Spy::reset();
      // exercise
      custom::pair<std::string, Spy> value = *it;
      // verify
      assertUnit(Spy::numCopy() == 1);      // assignment operator copied [50]
      assertUnit(Spy::numAlloc() == 1);     // allocated [50]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(value.first == std::string("50"));
      assertUnit(value.second == Spy(50));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //              it
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   /***************************************
    * FIND
    *    map::find()
    ***************************************/

   // attempt to find something from an empty map
   void test_find_empty()
   {  // setup
      custom::map<std::string, Spy> m;
      custom::map<std::string, Spy>::iterator it;
      std::string s50("50");
      Spy::reset();
      // exercise
      it = m.find(s50);
      // verify
      assertUnit(it.it.pNode == nullptr);
      assertEmptyFixture(m);
   }  // teardown

   // attempt to find something at the root from the standard fixture
   void test_find_standardRoot()
   {  // setup
      custom::map<std::string, Spy>::iterator it;
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      std::string s50("50");
      Spy::reset();
      // exercise
      it = m.find(s50);
      // verify
      assertUnit(Spy::numDefault() == 1); // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);     
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //              it
      assertUnit(it.it.pNode == m.bst.root);
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // attempt to find something at the left corner of the standard fixture
   void test_find_standardLeft()
   {  // setup
      custom::map<std::string, Spy>::iterator it;
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      std::string s30("30");
      Spy::reset();
      // exercise
      it = m.find(s30);
      // verify
      assertUnit(Spy::numDefault() == 1); // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //    it
      assertUnit(it.it.pNode == m.bst.root->pLeft);
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // attempt to find something at the right corner of the standard fixture
   void test_find_standardRight()
   {  // setup
      custom::map<std::string, Spy>::iterator it;
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      std::string s70("70");
      Spy::reset();
      // exercise
      it = m.find(s70);
      // verify
      assertUnit(Spy::numDefault() == 1); // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //                        it
      assertUnit(it.it.pNode == m.bst.root->pRight);
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // attempt to find something from the standard fixture that is not there
   void test_find_standardMissing()
   {  // setup
      custom::map<std::string, Spy>::iterator it;
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      std::string s99("99");
      Spy::reset();
      // exercise
      it = m.find(s99);
      // verify
      assertUnit(Spy::numDefault() == 1); // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //                                     it
      assertUnit(it.it.pNode == nullptr);
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   /***************************************
    * INSERT
    *    map::insert(const T &)
    ***************************************/

   // insert into an empty fixture
   void test_insertCopy_empty()
   {  // setup
      custom::map<std::string, Spy> m;
      custom::pair<std::string, Spy> pair50(std::string("50"), Spy(50));
      Spy::reset();
      // exercise
      m.insert(pair50);
      // verify
      assertUnit(Spy::numCopy() == 1);      // copy-create [50]
      assertUnit(Spy::numAlloc() == 1);     // allocate    [50]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      assertUnit(m.bst.numElements == 1);
      assertUnit(m.bst.root != nullptr);
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pLeft == nullptr);
         assertUnit(m.bst.root->pRight == nullptr);
         assertUnit(m.bst.root->pParent == nullptr);
      }
      // teardown
      teardownStandardFixture(m);
   }

   // insert onto the front of the standard fixture
   void test_insertCopy_standardFront()
   {  // setup
      custom::pair<std::string, Spy> pair10(std::string("10"), Spy(10));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      custom::pair<custom::map<std::string, Spy>::iterator, bool> pReturn;
      Spy::reset();
      // exercise
      pReturn = m.insert(pair10);
      // verify
      assertUnit(Spy::numCopy() == 1);      // copy-create [10]
      assertUnit(Spy::numAlloc() == 1);     // allocate    [10]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "10"     "30"     "50"     "70"   = m
      //   +----+   +----+   +----+   +----+
      //   | 10 | - | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+   +----+
      //   pReturn
      //
      assertUnit(pReturn.first.it.pNode != nullptr);
      assertUnit(pReturn.second == true);
      if (m.bst.root && m.bst.root->pLeft)
         assertUnit(pReturn.first.it.pNode == m.bst.root->pLeft->pLeft);
      assertUnit(m.bst.numElements == 4);
      assertUnit(m.bst.root != nullptr);
      //                  (50)
      //              +-----+-----+
      //            (30)        (70)
      //          +---+
      //        (10)
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pParent == nullptr);
         assertUnit(m.bst.root->pLeft != nullptr);
         if (m.bst.root->pLeft)
         {
            assertUnit(m.bst.root->pLeft->data.first == std::string("30"));
            assertUnit(m.bst.root->pLeft->data.second == Spy(30));
            assertUnit(m.bst.root->pLeft->pParent == m.bst.root);
            assertUnit(m.bst.root->pLeft->pLeft != nullptr);
            assertUnit(m.bst.root->pLeft->pRight == nullptr);
            if (m.bst.root->pLeft->pLeft)
            {
               assertUnit(m.bst.root->pLeft->pLeft->data.first == std::string("10"));
               assertUnit(m.bst.root->pLeft->pLeft->data.second == Spy(10));
               assertUnit(m.bst.root->pLeft->pLeft->pParent == m.bst.root->pLeft);
               assertUnit(m.bst.root->pLeft->pLeft->pLeft == nullptr);
               assertUnit(m.bst.root->pLeft->pLeft->pRight == nullptr);
            }
         }
         assertUnit(m.bst.root->pRight != nullptr);
         if (m.bst.root->pRight)
         {
            assertUnit(m.bst.root->pRight->data.first == std::string("70"));
            assertUnit(m.bst.root->pRight->data.second == Spy(70));
            assertUnit(m.bst.root->pRight->pParent == m.bst.root);
            assertUnit(m.bst.root->pRight->pLeft == nullptr);
            assertUnit(m.bst.root->pRight->pRight == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(m);
   }

   // insert something into the middle of the standard fixture
   void test_insertCopy_standardMiddle()
   {  // setup
      custom::pair<std::string, Spy> pair60(std::string("60"), Spy(60));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      custom::pair<custom::map<std::string, Spy>::iterator, bool> pReturn;
      Spy::reset();
      // exercise
      pReturn = m.insert(pair60);
      // verify
      assertUnit(Spy::numCopy() == 1);      // copy-create [10]
      assertUnit(Spy::numAlloc() == 1);     // allocate    [10]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "30"     "50"     "60"     "70"   = m
      //   +----+   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 60 | - | 70 |
      //   +----+   +----+   +----+   +----+
      //   pReturn
      //
      assertUnit(pReturn.first.it.pNode != nullptr);
      assertUnit(pReturn.second == true);
      if (m.bst.root && m.bst.root->pRight)
         assertUnit(pReturn.first.it.pNode == m.bst.root->pRight->pLeft);
      assertUnit(m.bst.numElements == 4);
      assertUnit(m.bst.root != nullptr);
      //                  (50)
      //              +-----+-----+
      //            (30)        (70)
      //                       +--+
      //                     (60)
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pParent == nullptr);
         assertUnit(m.bst.root->pLeft != nullptr);
         if (m.bst.root->pLeft)
         {
            assertUnit(m.bst.root->pLeft->data.first == std::string("30"));
            assertUnit(m.bst.root->pLeft->data.second == Spy(30));
            assertUnit(m.bst.root->pLeft->pParent == m.bst.root);
            assertUnit(m.bst.root->pLeft->pLeft == nullptr);
            assertUnit(m.bst.root->pLeft->pRight == nullptr);
         }
         assertUnit(m.bst.root->pRight != nullptr);
         if (m.bst.root->pRight)
         {
            assertUnit(m.bst.root->pRight->data.first == std::string("70"));
            assertUnit(m.bst.root->pRight->data.second == Spy(70));
            assertUnit(m.bst.root->pRight->pParent == m.bst.root);
            assertUnit(m.bst.root->pRight->pLeft != nullptr);
            if (m.bst.root->pRight->pLeft)
            {
               assertUnit(m.bst.root->pRight->pLeft->data.first == std::string("60"));
               assertUnit(m.bst.root->pRight->pLeft->data.second == Spy(60));
               assertUnit(m.bst.root->pRight->pLeft->pParent == m.bst.root->pRight);
               assertUnit(m.bst.root->pRight->pLeft->pLeft == nullptr);
               assertUnit(m.bst.root->pRight->pLeft->pRight == nullptr);
            }
            assertUnit(m.bst.root->pRight->pRight == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(m);
   }

   /***************************************
    * INSERT
    *    map::insert(T &&)
    ***************************************/

    // insert into an empty fixture
   void test_insertMove_empty()
   {  // setup
      custom::map<std::string, Spy> m;
      custom::pair<std::string, Spy> pair50(std::string("50"), Spy(50));
      Spy::reset();
      // exercise
      m.insert(std::move(pair50));
      // verify
      assertUnit(Spy::numCopyMove() == 1);    // move [50]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      assertUnit(m.bst.numElements == 1);
      assertUnit(m.bst.root != nullptr);
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pLeft == nullptr);
         assertUnit(m.bst.root->pRight == nullptr);
         assertUnit(m.bst.root->pParent == nullptr);
      }
      // teardown
      teardownStandardFixture(m);
   }


   // insert something into the middle of the standard fixture
   void test_insertMove_standard()
   {  // setup
      custom::pair<std::string, Spy> pair60(std::string("60"), Spy(60));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      custom::pair<custom::map<std::string, Spy>::iterator, bool> pReturn;
      Spy::reset();
      // exercise
      pReturn = m.insert(std::move(pair60));
      // verify
      assertUnit(Spy::numCopyMove() == 1);   // move[60]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "30"     "50"     "60"     "70"   = m
      //   +----+   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 60 | - | 70 |
      //   +----+   +----+   +----+   +----+
      //   pReturn
      //
      assertUnit(pReturn.first.it.pNode != nullptr);
      assertUnit(pReturn.second == true);
      if (m.bst.root && m.bst.root->pRight)
         assertUnit(pReturn.first.it.pNode == m.bst.root->pRight->pLeft);
      assertUnit(m.bst.numElements == 4);
      assertUnit(m.bst.root != nullptr);
      //                  (50)
      //              +-----+-----+
      //            (30)        (70)
      //                       +--+
      //                     (60)
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pParent == nullptr);
         assertUnit(m.bst.root->pLeft != nullptr);
         if (m.bst.root->pLeft)
         {
            assertUnit(m.bst.root->pLeft->data.first == std::string("30"));
            assertUnit(m.bst.root->pLeft->data.second == Spy(30));
            assertUnit(m.bst.root->pLeft->pParent == m.bst.root);
            assertUnit(m.bst.root->pLeft->pLeft == nullptr);
            assertUnit(m.bst.root->pLeft->pRight == nullptr);
         }
         assertUnit(m.bst.root->pRight != nullptr);
         if (m.bst.root->pRight)
         {
            assertUnit(m.bst.root->pRight->data.first == std::string("70"));
            assertUnit(m.bst.root->pRight->data.second == Spy(70));
            assertUnit(m.bst.root->pRight->pParent == m.bst.root);
            assertUnit(m.bst.root->pRight->pLeft != nullptr);
            if (m.bst.root->pRight->pLeft)
            {
               assertUnit(m.bst.root->pRight->pLeft->data.first == std::string("60"));
               assertUnit(m.bst.root->pRight->pLeft->data.second == Spy(60));
               assertUnit(m.bst.root->pRight->pLeft->pParent == m.bst.root->pRight);
               assertUnit(m.bst.root->pRight->pLeft->pLeft == nullptr);
               assertUnit(m.bst.root->pRight->pLeft->pRight == nullptr);
            }
            assertUnit(m.bst.root->pRight->pRight == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(m);
   }


   /***************************************
    * SQUARE BRACKET
    *     map::operator[](const T &)
    ***************************************/

   // read something from the root of the standard fixture
   void test_access_standardRootRead()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(99);
      Spy::reset();
      // exercise
      s = m[std::string("50")];
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [50]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);     
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(s == Spy(50));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // read something from the left corner of the standard fixture
   void test_access_standardLeftRead()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(99);
      Spy::reset();
      // exercise
      s = m[std::string("30")];
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [30]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(s == Spy(30));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // read something from the right corner of the standard fixture
   void test_access_standardRightRead()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(99);
      Spy::reset();
      // exercise
      s = m[std::string("70")];
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [70]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(s == Spy(70));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // write something from the root of the standard fixture
   void test_access_standardRootWrite()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(55);
      Spy::reset();
      // exercise
      m[std::string("50")] = s;
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [55]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(m.bst.root != nullptr);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 55 | - | 70 |
      //   +----+   +----+   +----+
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(55));
         m.bst.root->data.second = Spy(50);
      }
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // write something from the left corner of the standard fixture
   void test_access_standardLeftWrite()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(33);
      Spy::reset();
      // exercise
      m[std::string("30")] = s;
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [33]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(m.bst.root != nullptr);
      assertUnit(m.bst.root->pLeft != nullptr);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 33 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      if (m.bst.root && m.bst.root->pLeft)
      {
         assertUnit(m.bst.root->pLeft->data.first == std::string("30"));
         assertUnit(m.bst.root->pLeft->data.second == Spy(33));
         m.bst.root->pLeft->data.second = Spy(30);
      }
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // write something from the right corner of the standard fixture
   void test_access_standardRightWrite()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(77);
      Spy::reset();
      // exercise
      m[std::string("70")] = s;
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [77]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(m.bst.root != nullptr);
      assertUnit(m.bst.root->pRight != nullptr);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 77 |
      //   +----+   +----+   +----+
      if (m.bst.root && m.bst.root->pRight)
      {
         assertUnit(m.bst.root->pRight->data.first == std::string("70"));
         assertUnit(m.bst.root->pRight->data.second == Spy(77));
         m.bst.root->pRight->data.second = Spy(70);
      }
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // insert something into an empty map using the [] operator
   void test_access_emptyWrite()
   {  // setup
      custom::map<std::string, Spy> m;
      Spy s(50);
      Spy::reset();
      // exercise
      m[std::string("50")] = s;
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy-create [50]
      assertUnit(Spy::numAlloc() == 1);      // allocate    [50]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numAssign() == 1);     // use the assignment operator for [50]
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "50"
      //   +----+
      //   | 50 |
      //   +----+
      assertUnit(m.bst.numElements == 1);
      assertUnit(m.bst.root != nullptr);
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pLeft == nullptr);
         assertUnit(m.bst.root->pRight == nullptr);
         assertUnit(m.bst.root->pParent == nullptr);
      }
      // teardown 
      teardownStandardFixture(m);
   }

   // insert something to the front of the standard fixture using the square bracket operator
   void test_access_standardFrontInsert()
   {  // setup
      Spy s(10);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy::reset();
      // exercise
      m[std::string("10")] = s;
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy-create [10]
      assertUnit(Spy::numAlloc() == 1);      // allocate    [10]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numAssign() == 1);     // use the assignment operator for [50]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "10"     "30"     "50"     "70"   = m
      //   +----+   +----+   +----+   +----+
      //   | 10 | - | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+   +----+
      assertUnit(m.bst.numElements == 4);
      assertUnit(m.bst.root != nullptr);
      //                  (50)
      //              +-----+-----+
      //            (30)        (70)
      //          +---+
      //        (10)
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pParent == nullptr);
         assertUnit(m.bst.root->pLeft != nullptr);
         if (m.bst.root->pLeft)
         {
            assertUnit(m.bst.root->pLeft->data.first == std::string("30"));
            assertUnit(m.bst.root->pLeft->data.second == Spy(30));
            assertUnit(m.bst.root->pLeft->pParent == m.bst.root);
            assertUnit(m.bst.root->pLeft->pLeft != nullptr);
            assertUnit(m.bst.root->pLeft->pRight == nullptr);
            if (m.bst.root->pLeft->pLeft)
            {
               assertUnit(m.bst.root->pLeft->pLeft->data.first == std::string("10"));
               assertUnit(m.bst.root->pLeft->pLeft->data.second == Spy(10));
               assertUnit(m.bst.root->pLeft->pLeft->pParent == m.bst.root->pLeft);
               assertUnit(m.bst.root->pLeft->pLeft->pLeft == nullptr);
               assertUnit(m.bst.root->pLeft->pLeft->pRight == nullptr);
            }
         }
         assertUnit(m.bst.root->pRight != nullptr);
         if (m.bst.root->pRight)
         {
            assertUnit(m.bst.root->pRight->data.first == std::string("70"));
            assertUnit(m.bst.root->pRight->data.second == Spy(70));
            assertUnit(m.bst.root->pRight->pParent == m.bst.root);
            assertUnit(m.bst.root->pRight->pLeft == nullptr);
            assertUnit(m.bst.root->pRight->pRight == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(m);
   }


   // insert something to the moiddle of the standard fixture using the square bracket operator
   void test_access_standardMiddleInsert()
   {  // setup
      Spy s(60);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy::reset();
      // exercise
      m[std::string("60")] = s;
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy-create [60]
      assertUnit(Spy::numAlloc() == 1);      // allocate    [60]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numAssign() == 1);     // use the assignment operator for [60]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //    "30"     "50"     "60"     "70"   = m
      //   +----+   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 60 | - | 70 |
      //   +----+   +----+   +----+   +----+
      assertUnit(m.bst.numElements == 4);
      assertUnit(m.bst.root != nullptr);
      //                  (50)
      //              +-----+-----+
      //            (30)        (70)
      //                       +--+
      //                      (60)
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pParent == nullptr);
         assertUnit(m.bst.root->pLeft != nullptr);
         if (m.bst.root->pLeft)
         {
            assertUnit(m.bst.root->pLeft->data.first == std::string("30"));
            assertUnit(m.bst.root->pLeft->data.second == Spy(30));
            assertUnit(m.bst.root->pLeft->pParent == m.bst.root);
            assertUnit(m.bst.root->pLeft->pLeft == nullptr);
            assertUnit(m.bst.root->pLeft->pRight == nullptr);
         }
         assertUnit(m.bst.root->pRight != nullptr);
         if (m.bst.root->pRight)
         {
            assertUnit(m.bst.root->pRight->data.first == std::string("70"));
            assertUnit(m.bst.root->pRight->data.second == Spy(70));
            assertUnit(m.bst.root->pRight->pParent == m.bst.root);
            assertUnit(m.bst.root->pRight->pLeft != nullptr);
            if (m.bst.root->pRight->pLeft)
            {
               assertUnit(m.bst.root->pRight->pLeft->data.first == std::string("60"));
               assertUnit(m.bst.root->pRight->pLeft->data.second == Spy(60));
               assertUnit(m.bst.root->pRight->pLeft->pParent == m.bst.root->pRight);
               assertUnit(m.bst.root->pRight->pLeft->pLeft == nullptr);
               assertUnit(m.bst.root->pRight->pLeft->pRight == nullptr);
            }
            assertUnit(m.bst.root->pRight->pRight == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(m);
   }


   /***************************************
    * AT
    *     map::at(const T &)
    ***************************************/

    // read something from the root of the standard fixture
   void test_at_standardRootRead()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(99);
      Spy::reset();
      // exercise
      s = m.at(std::string("50"));
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [50]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(s == Spy(50));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // read something from the left corner of the standard fixture
   void test_at_standardLeftRead()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(99);
      Spy::reset();
      // exercise
      s = m.at(std::string("30"));
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [30]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(s == Spy(30));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // read something from the right corner of the standard fixture
   void test_at_standardRightRead()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(99);
      Spy::reset();
      // exercise
      s = m.at(std::string("70"));
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [70]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(s == Spy(70));
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // write something from the root of the standard fixture
   void test_at_standardRootWrite()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(55);
      Spy::reset();
      // exercise
      m.at(std::string("50")) = s;
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [55]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(m.bst.root != nullptr);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 55 | - | 70 |
      //   +----+   +----+   +----+
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(55));
         m.bst.root->data.second = Spy(50);
      }
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // write something from the left corner of the standard fixture
   void test_at_standardLeftWrite()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(33);
      Spy::reset();
      // exercise
      m.at(std::string("30")) = s;
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [33]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(m.bst.root != nullptr);
      assertUnit(m.bst.root->pLeft != nullptr);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 33 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      if (m.bst.root && m.bst.root->pLeft)
      {
         assertUnit(m.bst.root->pLeft->data.first == std::string("30"));
         assertUnit(m.bst.root->pLeft->data.second == Spy(33));
         m.bst.root->pLeft->data.second = Spy(30);
      }
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // write something from the right corner of the standard fixture
   void test_at_standardRightWrite()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy s(77);
      Spy::reset();
      // exercise
      m.at(std::string("70")) = s;
      // verify
      assertUnit(Spy::numAssign() == 1);     // assign [77]
      assertUnit(Spy::numDefault() == 1);    // a blank Spy is created for comparison
      assertUnit(Spy::numDestructor() == 1); // destroy it and the move-copy when done
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(m.bst.root != nullptr);
      assertUnit(m.bst.root->pRight != nullptr);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 77 |
      //   +----+   +----+   +----+
      if (m.bst.root && m.bst.root->pRight)
      {
         assertUnit(m.bst.root->pRight->data.first == std::string("70"));
         assertUnit(m.bst.root->pRight->data.second == Spy(77));
         m.bst.root->pRight->data.second = Spy(70);
      }
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // insert something into an empty map using the [] operator
   void test_at_emptyWrite()
   {  // setup
      custom::map<std::string, Spy> m;
      Spy s(50);
      Spy::reset();
      // exercise
      try
      {
         m.at(std::string("50")) = s;
         // verify
         assertUnit(false);
      }
      catch (const std::out_of_range & e)
      {
         assertUnit(e.what() == std::string("invalid map<K, T> key"));
      }
      assertUnit(Spy::numDefault() == 1);    // create a default value during search
      assertUnit(Spy::numDestructor() == 1); // create a default value during search
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);     
      assertUnit(Spy::numAssign() == 0);    
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(m);
      // teardown 
      teardownStandardFixture(m);
   }

   // insert something to the front of the standard fixture using the square bracket operator
   void test_at_standardWrite()
   {  // setup
      Spy s(10);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      Spy::reset();
      // exercise
      try
      {
         m.at(std::string("10")) = s;
         // verify
         assertUnit(false);
      }
      catch (const std::out_of_range& e)
      {
         assertUnit(e.what() == std::string("invalid map<K, T> key"));
      }
      assertUnit(Spy::numDefault() == 1);    // create a default value during search
      assertUnit(Spy::numDestructor() == 1); // create a default value during search
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   /***************************************
    * ERASE
    *     map::erase()
    ***************************************/

   // attempt to erase from an empty map
   void test_erase_emptyKey()
   {  // setup
      custom::map<std::string, Spy> m;
      size_t size;
      std::string key("50");
      Spy::reset();
      // exercise
      size = m.erase(key);
      // verify
      assertUnit(Spy::numDefault() == 1);      // create a default value for find
      assertUnit(Spy::numDestructor() == 1);   // destroy the default value for find
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(size == 0);
      assertEmptyFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // erase an element from the standard fixture
   void test_erase_standardKey()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      size_t size;
      std::string key("50");
      Spy::reset();
      // exercise
      size = m.erase(key);
      // verify
      assertUnit(Spy::numDestructor() == 2);   // destroy [50] and default for find
      assertUnit(Spy::numDelete() == 1);       // delete  [50]  
      assertUnit(Spy::numDefault() == 1);      // create default for find
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(size == 1);
      //    "30"     "70"   = m
      //   +----+   +----+
      //   | 30 | - | 70 |
      //   +----+   +----+
      assertUnit(m.bst.size() == 2);
      assertUnit(m.bst.root != nullptr);
      //                  (70)
      //              +-----+
      //            (30)    
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("70"));
         assertUnit(m.bst.root->data.second == Spy(70));
         assertUnit(m.bst.root->pParent == nullptr);
         assertUnit(m.bst.root->pLeft != nullptr);
         if (m.bst.root->pLeft)
         {
            assertUnit(m.bst.root->pLeft->data.first == std::string("30"));
            assertUnit(m.bst.root->pLeft->data.second == Spy(30));
            assertUnit(m.bst.root->pLeft->pParent == m.bst.root);
            assertUnit(m.bst.root->pLeft->pLeft == nullptr);
            assertUnit(m.bst.root->pLeft->pRight == nullptr);
         }
         assertUnit(m.bst.root->pRight == nullptr);
      }      
      // teardown
      teardownStandardFixture(m);
   }

   // attempt to erase an element that does not exist
   void test_erase_standardKeyMissing()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      size_t size;
      std::string key("40");
      Spy::reset();
      // exercise
      size = m.erase(key);
      // verify
      assertUnit(Spy::numDestructor() == 1);   // default for find
      assertUnit(Spy::numDefault() == 1);      // create default for find
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(size == 0);
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // attempt to erase from an empty map
   void test_erase_emptyIterator()
   {  // setup
      custom::map<std::string, Spy> m;
      custom::map<std::string, Spy>::iterator itReturn;
      custom::map<std::string, Spy>::iterator itErase = m.end();
      Spy::reset();
      // exercise
      itReturn = m.erase(itErase);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // erase one element from a map with an iterator
   void test_erase_standardIterator()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      custom::map<std::string, Spy>::iterator itReturn;
      custom::map<std::string, Spy>::iterator itErase = m.begin();
      Spy::reset();
      // exercise
      itReturn = m.erase(itErase);
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [30] 
      assertUnit(Spy::numDelete() == 1);       // delete  [30]  
      assertUnit(Spy::numDefault() == 0);     
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(itReturn != m.end());
      if (itReturn != m.end())
      {
         assertUnit((*itReturn).first == std::string("50"));
         assertUnit((*itReturn).second == Spy(50));
      }
      //    "50"     "70"   = m
      //   +----+   +----+ 
      //   | 50 | - | 70 | 
      //   +----+   +----+ 
      assertUnit(m.bst.size() == 2);
      assertUnit(m.bst.root != nullptr);
      //                  (50)
      //                    +-----+
      //                        (70)    
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("50"));
         assertUnit(m.bst.root->data.second == Spy(50));
         assertUnit(m.bst.root->pParent == nullptr);
         assertUnit(m.bst.root->pLeft == nullptr);
         assertUnit(m.bst.root->pRight != nullptr);
         if (m.bst.root->pRight)
         {
            assertUnit(m.bst.root->pRight->data.first == std::string("70"));
            assertUnit(m.bst.root->pRight->data.second == Spy(70));
            assertUnit(m.bst.root->pRight->pParent == m.bst.root);
            assertUnit(m.bst.root->pRight->pLeft == nullptr);
            assertUnit(m.bst.root->pRight->pRight == nullptr);
         }
      }
      // teardown
      teardownStandardFixture(m);
   }

   // attempt to erase one element from a map that is not there
   void test_erase_standardIteratorMissing()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      custom::map<std::string, Spy>::iterator itReturn;
      custom::map<std::string, Spy>::iterator itErase = m.end();
      Spy::reset();
      // exercise
      itReturn = m.erase(itErase);
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);    
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(itReturn == m.end());
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      assertStandardFixture(m);
      // teardown
      teardownStandardFixture(m);
   }

   // attempt to erase from an empty map
   void test_erase_emptyRange()
   {  // setup
      custom::map<std::string, Spy> m;
      custom::map<std::string, Spy>::iterator itFirst = m.begin();
      custom::map<std::string, Spy>::iterator itLast = m.end();
      custom::map<std::string, Spy>::iterator itReturn;
      Spy::reset();
      // exercise
      itReturn = m.erase(itFirst, itLast);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(m);
      // teardown
      teardownStandardFixture(m);
   }   
   
   void test_erase_standardRange()
   {  // setup
      //    "30"     "50"     "70"   = m
      //   +----+   +----+   +----+
      //   | 30 | - | 50 | - | 70 |
      //   +----+   +----+   +----+
      //     itFirst           itLast
      custom::map<std::string, Spy> m;
      setupStandardFixture(m);
      custom::map<std::string, Spy>::iterator itFirst = m.begin();
      custom::map<std::string, Spy>::iterator itLast = m.begin();
      ++itLast;
      ++itLast;
      custom::map<std::string, Spy>::iterator itReturn;
      Spy::reset();
      // exercise
      itReturn = m.erase(itFirst, itLast);
      // verify
      assertUnit(Spy::numDestructor() == 2);  // erase  [30][50]
      assertUnit(Spy::numDelete() == 2);      // delete [30][50]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(itReturn == m.begin());
      //    "70"     = m
      //   +----+ 
      //   | 70 |
      //   +----+
      //   itReturn
      assertUnit(m.bst.numElements == 1);
      assertUnit(m.bst.root != nullptr);
      if (m.bst.root)
      {
         assertUnit(m.bst.root->data.first == std::string("70"));
         assertUnit(m.bst.root->data.second == Spy(70));
         assertUnit(m.bst.root->pLeft == nullptr);
         assertUnit(m.bst.root->pRight == nullptr);
         assertUnit(m.bst.root->pParent == nullptr);
      }
      // teardown
      teardownStandardFixture(m);
   }
   /****************************************************************
    * Setup Standard Fixture
    *    "30"     "50"     "70"
    *   +----+   +----+   +----+
    *   | 30 | - | 50 | - | 70 |
    *   +----+   +----+   +----+
    *   bst pointers
    *                  (50)
    *              +-----+-----+
    *            (30)        (70)
    ****************************************************************/
   void setupStandardFixture(custom::map<std::string, Spy>& m)
   {
      // set up the pairs
      custom::pair<std::string, Spy> pair30(std::string("30"), Spy(30));
      custom::pair<std::string, Spy> pair50(std::string("50"), Spy(50));
      custom::pair<std::string, Spy> pair70(std::string("70"), Spy(70));

      // create the nodes
      //               (50)b
      //           +-----+-----+
      //         (30)r        (70)r
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode30;
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode50;
      custom::BST < custom::pair<std::string, Spy> > ::BNode* bnode70;
      bnode30 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair30);
      bnode50 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair50);
      bnode70 = new custom::BST < custom::pair<std::string, Spy> > ::BNode(pair70);

      // hook up the links and stuff
      bnode50->pLeft  = bnode30;
      bnode50->pRight = bnode70;
      bnode30->pParent = bnode70->pParent = bnode50;
      bnode50->isRed = false;
      bnode30->isRed = bnode70->isRed = false;

      // place the nodes in the bst
      m.bst.root = bnode50;
      m.bst.numElements = 3;
   }

   /****************************************************************
    * Verify Empty Fixture
    ****************************************************************/
   void assertEmptyFixtureParameters(const custom::map<std::string, Spy>& m, int line, const char* function)
   {
      // bst stuff
      assertIndirect(m.bst.size() == 0);
      assertIndirect(m.bst.empty() == true);
      assertIndirect(m.bst.numElements == 0);
   }

   /****************************************************************
    * Verify Standard Fixture
    *    "30"     "50"     "70"
    *   +----+   +----+   +----+
    *   | 30 | - | 50 | - | 70 |
    *   +----+   +----+   +----+
    *   bst pointers
    *                  (50)
    *              +-----+-----+
    *            (30)        (70)
    ****************************************************************/
   void assertStandardFixtureParameters(const custom::map<std::string, Spy> & m, int line, const char* function)
   {
      assertIndirect(m.bst.numElements == 3);

      // check that we have a tree
      assertIndirect(m.bst.root != nullptr);
      if (m.bst.root == nullptr)
         return;

      // check the root
      assertIndirect(m.bst.root->data.first == std::string("50"));
      assertIndirect(m.bst.root->data.second == Spy(50));
      assertIndirect(m.bst.root->pLeft != nullptr);
      assertIndirect(m.bst.root->pRight != nullptr);
      assertIndirect(m.bst.root->pParent == nullptr);

      // check left branch
      if (m.bst.root->pLeft)
      {
         assertIndirect(m.bst.root->pLeft->data.first == std::string("30"));
         assertIndirect(m.bst.root->pLeft->data.second == Spy(30));
         assertIndirect(m.bst.root->pLeft->pLeft == nullptr);
         assertIndirect(m.bst.root->pLeft->pRight == nullptr);
         assertIndirect(m.bst.root->pLeft->pParent == m.bst.root);
      }

      // check right branch
      if (m.bst.root->pRight)
      {
         assertIndirect(m.bst.root->pRight->data.first == std::string("70"));
         assertIndirect(m.bst.root->pRight->data.second == Spy(70));
         assertIndirect(m.bst.root->pRight->pLeft == nullptr);
         assertIndirect(m.bst.root->pRight->pRight == nullptr);
         assertIndirect(m.bst.root->pRight->pParent == m.bst.root);
      }
   }


   /****************************************************************
    * Teardown Standard Fixture
    *    "30"     "50"     "70" 
    *   +----+   +----+   +----+
    *   | 30 | - | 50 | - | 70 |
    *   +----+   +----+   +----+
    *   bst pointers
    *                  (50)
    *              +-----+-----+
    *            (30)        (70)
    ****************************************************************/
   void teardownStandardFixture(custom::map<std::string, double>& m)
   {
      // delete the nodes
      if (m.bst.root)
      {
         if (m.bst.root->pLeft)
         {
            if (m.bst.root->pLeft->pLeft)
               delete m.bst.root->pLeft->pLeft;
            if (m.bst.root->pLeft->pRight)
               delete m.bst.root->pLeft->pRight;

            delete m.bst.root->pLeft;
         }

         if (m.bst.root->pRight)
         {
            if (m.bst.root->pRight->pLeft)
               delete m.bst.root->pRight->pLeft;
            if (m.bst.root->pRight->pRight)
               delete m.bst.root->pRight->pRight;

            delete m.bst.root->pRight;
         }
         delete m.bst.root;
      }
      m.bst.root = nullptr;
      m.bst.numElements = 0;
   }

   void teardownStandardFixture(custom::map<std::string, Spy>& m)
   {
      // delete the nodes
      if (m.bst.root)
      {
         if (m.bst.root->pLeft && m.bst.root->pLeft != m.bst.root)
         {
            if (m.bst.root->pLeft->pLeft && m.bst.root->pLeft->pLeft != m.bst.root->pLeft)
               delete m.bst.root->pLeft->pLeft;
            if (m.bst.root->pLeft->pRight && m.bst.root->pLeft->pRight != m.bst.root->pLeft)
               delete m.bst.root->pLeft->pRight;

            delete m.bst.root->pLeft;
         }

         if (m.bst.root->pRight && m.bst.root->pRight != m.bst.root)
         {
            if (m.bst.root->pRight->pLeft && m.bst.root->pRight->pLeft != m.bst.root->pRight)
               delete m.bst.root->pRight->pLeft;
            if (m.bst.root->pRight->pRight && m.bst.root->pRight->pRight != m.bst.root->pRight)
               delete m.bst.root->pRight->pRight;

            delete m.bst.root->pRight;
         }
         delete m.bst.root;
      }
      m.bst.root = nullptr;
      m.bst.numElements = 0;
   }
};

#endif // DEBUG
