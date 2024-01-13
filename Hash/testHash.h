/***********************************************************************
 * Header:
 *    TEST HASH
 * Summary:
 *    Unit tests for hash
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include "hash.h"
#include "unitTest.h"
#include "spy.h"

#include <cassert>
#include <memory>
#include <unordered_set>
#include <functional>
#include <vector>

using std::cout;
using std::endl;

template <class T>
class Hash1
{
   public:
      std::size_t operator() (const T & t) const { return 1; }
};

template <class T>
size_t hash1(const T & t) { return 1; }

class TestHash : public UnitTest
{

public:
   void run()
   {
      reset();

      //// Construct
      test_construct_default();
      test_construct_nonDefault11();
      test_construct_nonDefaultIterator();
      test_construct_copyEmpty();
      test_construct_copyStandard();
      test_construct_nonDefaultHash();

      //// Assign
      test_assign_emptyEmpty();
      test_assign_emptyStandard();
      test_assign_standardEmpty();
      test_assignMove_emptyEmpty();
      test_assignMove_emptyStandard();
      test_assignMove_standardEmpty();
      test_swapMember_emptyEmpty();
      test_swapMember_standardEmpty();
      test_swapMember_standardOther();
      test_swapNonMember_emptyEmpty();
      test_swapNonMember_standardEmpty();
      test_swapNonMember_standardOther();

      //// Iterator
      test_iterator_begin_empty();
      test_iterator_begin_standard();
      test_iterator_end_empty();
      test_iterator_end_standard();
      test_iterator_increment_empty();
      test_iterator_increment_moreInBucket();
      test_iterator_increment_nextBucket();
      test_iterator_increment_toEnd();
      test_iterator_dereference();
      test_localIterator_begin_single();
      test_localIterator_begin_multiple();
      test_localIterator_begin_empty();
      test_localIterator_increment_single();
      test_localIterator_increment_multiple();

      //// Access
      test_bucket_empty0();
      test_bucket_empty7();
      test_bucket_empty58();
      test_bucket_standard0();
      test_bucket_standard7();
      test_bucket_standard58();
      test_bucket_custom0();
      test_bucket_custom3();
      test_find_empty();
      test_find_standardFront();
      test_find_standardBack();
      test_find_standardMissingEmptyList();
      test_find_standardMissingFilledList();

      // Insert
      test_rehash_emptySmaller();
      test_rehash_emptyBigger();
      test_rehash_standard6();
      test_rehash_standard8();
      test_reserve_empty10();
      test_reserve_empty12();
      test_reserve_standard6();
      test_reserve_standard8();
      test_insert_empty0();
      test_insert_empty58();
      test_insert_standard3();
      test_insert_standard44();
      test_insert_standardDuplicate();
      test_insert_standardRehash();

      //// Remove
      test_clear_empty();
      test_clear_standard();
      test_erase_empty();
      test_erase_standardMissing();
      test_erase_standardAlone();
      test_erase_standardFront();
      test_erase_standardBack();
      test_erase_standardLast();

      //// Status
      test_size_empty();
      test_size_standard();
      test_empty_empty();
      test_empty_standard();
      test_bucketSize_empty();
      test_bucketSize_standardEmpty();
      test_bucketSize_standardOne();
      test_bucketSize_standardTwo();
      test_bucketCount_empty();
      test_bucketCount_standard();
      test_loadFactor_empty();
      test_loadFactor_standard();
      test_loadFactor_default();
      test_loadFactor_two();
      test_setLoadFactor_five();
      
      report("Hash");
   }

   /***************************************
    * CONSTRUCTOR
    ***************************************/
   
   // create a spy unordered set
   void test_construct_default()
   {  // setup
      std::allocator<custom::unordered_set<Spy>> alloc;
      custom::unordered_set<Spy> us;
      us.numElements = 99;
      us.buckets = 88;
      us.maxLoadFactor = (float)77.7;
      Spy::reset();
      // exercise
      alloc.construct(&us);
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
      assertEmptyFixture(us);
   }  // teardown

   // create a spy unordered set with 11 buckets
   void test_construct_nonDefault11()
   {  // setup
      std::allocator<custom::unordered_set<Spy>> alloc;
      custom::unordered_set<Spy> us;
      us.numElements = 99;
      us.buckets = 88;
      us.maxLoadFactor = (float)77.7;
      Spy::reset();
      // exercise
      alloc.construct(&us, 11);
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

      assertUnit(us.numElements == 0);
      assertUnit(us.buckets.size() == 11);
      assertUnit(us.maxLoadFactor == (float)1.0);
      if (us.buckets.size() == 11)
      {
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 0);
         assertUnit(us.buckets[2].size() == 0);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[4].size() == 0);
         assertUnit(us.buckets[5].size() == 0);
         assertUnit(us.buckets[6].size() == 0);
         assertUnit(us.buckets[7].size() == 0);
         assertUnit(us.buckets[8].size() == 0);
         assertUnit(us.buckets[9].size() == 0);
         assertUnit(us.buckets[10].size() == 0);
      }
   }  // teardown

   // create a spy unordered set from a vector iterator
   void test_construct_nonDefaultIterator()
   {  // setup
      std::vector<Spy> v{Spy(31), Spy(49), Spy(67), Spy(59)};
      std::allocator<custom::unordered_set<Spy>> alloc;
      custom::unordered_set<Spy> us;
      us.numElements = 99;
      us.buckets = 88;
      us.maxLoadFactor = (float)77.7;
      Spy::reset();
      // exercise
      alloc.construct(&us, v.begin(), v.end());
      // verify
      assertUnit(Spy::numAlloc() == 4);   // 31, 49, 67, 59
      assertUnit(Spy::numCopy() == 4);    // 31, 49, 67, 59
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] -->
      assertUnit(us.maxLoadFactor == (float)1.0);
      us.maxLoadFactor = (float)1.3;
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   } 
   
   // copy an empty set
   void test_construct_copyEmpty()
   {  // setup
      custom::unordered_set<Spy> usSrc;
      Spy::reset();
      // exercise
      custom::unordered_set<Spy> usDes(usSrc);
      // verify
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(usSrc);
      assertEmptyFixture(usDes);
   }  // teardown

   // copy a standard set
   void test_construct_copyStandard()
   {  // setup
      custom::unordered_set<Spy> usSrc;
      setupStandardFixture(usSrc);
      Spy::reset();
      // exercise
      custom::unordered_set<Spy> usDes(usSrc);
      // verify
      assertUnit(Spy::numAlloc() == 4);    // 31, 49, 67, 59
      assertUnit(Spy::numCopy() == 4);     // 31, 49, 67, 59
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertStandardFixture(usSrc);
      assertStandardFixture(usDes);
      // teardown
      teardownStandardFixture(usSrc);
      teardownStandardFixture(usDes);
   } 

   // empty 4-element with custom hash function 
   void test_construct_nonDefaultHash()
   {  // setup
      Spy::reset();
      // exercise
      custom::unordered_set<Spy, Hash1<Spy>> us(4);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);

      assertUnit(us.numElements == 0);
      assertUnit(us.buckets.size() == 4);
      assertUnit(us.maxLoadFactor == (float)1.0);

      if (us.buckets.size() == 4)
      {
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 0);
         assertUnit(us.buckets[2].size() == 0);
         assertUnit(us.buckets[3].size() == 0);
      }
   }  // teardown

   /***************************************
    * FIND
    ***************************************/

    // find the bucket that the value 0 will go in the empty hash
    void test_bucket_empty0()
    {  // setup
       custom::unordered_set<Spy> us;
       Spy s(0); // (0 % 8 = 0)
       size_t iBucket = 99;
       Spy::reset();
       // exercise
       iBucket = us.bucket(s);
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
       assertUnit(iBucket == 0);
       assertEmptyFixture(us);
    }  // teardown

    // find the bucket that the value 7 will go in the empty hash
    void test_bucket_empty7()
    {  // setup
       custom::unordered_set<Spy> us;
       Spy s(7); // (7 % 8 = 7)
       size_t iBucket = 99;
       Spy::reset();
       // exercise
       iBucket = us.bucket(s);
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
       assertUnit(iBucket == 7);
       assertEmptyFixture(us);
    }  // teardown

        // find the bucket that the value 58 will go in the empty hash
    void test_bucket_empty58()
    {  // setup
       custom::unordered_set<Spy> us;
       Spy s(58);  // (5+8 % 8 = 5)
       size_t iBucket = 99;
       Spy::reset();
       // exercise
       iBucket = us.bucket(s);
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
       assertUnit(iBucket == 5);
       assertEmptyFixture(us);
    }  // teardown

    // find the bucket that the value 0 will go in the standard hash
    void test_bucket_standard0()
    {  // setup
       custom::unordered_set<Spy> us;
       setupStandardFixture(us);
       Spy s(0); // (0 % 4 = 0)
       size_t iBucket = 99;
       Spy::reset();
       // exercise
       iBucket = us.bucket(s);
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
       assertStandardFixture(us);
       assertUnit(iBucket == 0);
       // teardown
       teardownStandardFixture(us);
    }

    // find the bucket that the value 7 will go in the standard hash
    void test_bucket_standard7()
    {  // setup
       custom::unordered_set<Spy> us;
       setupStandardFixture(us);
       Spy s(7); // (7 % 4 = 3)
       size_t iBucket = 99;
       Spy::reset();
       // exercise
       iBucket = us.bucket(s);
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
       assertStandardFixture(us);
       assertUnit(iBucket == 3);
       // teardown
       teardownStandardFixture(us);
    }

        // find the bucket that the value 58 will go in the standard hash
    void test_bucket_standard58()
    {  // setup
       custom::unordered_set<Spy> us;
       setupStandardFixture(us);
       Spy s(58);  // (5+8 % 4 = 1)
       size_t iBucket = 99;
       Spy::reset();
       // exercise
       iBucket = us.bucket(s);
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
       assertStandardFixture(us);
       assertUnit(iBucket == 1);
       // teardown
       teardownStandardFixture(us);
    }

    // find the bucket that the value 0 will go in the a custom hash
    void test_bucket_custom0()
    {  // setup
       custom::unordered_set<Spy, Hash1<Spy> > us;
       Spy s(0); 
       size_t iBucket = 99;
       Spy::reset();
       // exercise
       iBucket = us.bucket(s);
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
       assertUnit(iBucket == 1);
    }  // teardown

    // find the bucket that the value 3 will go in the a custom hash
    void test_bucket_custom3()
    {  // setup
       custom::unordered_set<Spy, Hash1<Spy> > us;
       Spy s(3); 
       size_t iBucket = 99;
       Spy::reset();
       // exercise
       iBucket = us.bucket(s);
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
       assertUnit(iBucket == 1);
    }  // teardown

    // find something from an empty hash
   void test_find_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      custom::unordered_set<Spy>::iterator it = us.begin();
      Spy s(99);
      Spy::reset();
      // exercise
      it = us.find(s);
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
      assertUnit(it.itVectorEnd == us.buckets.end());
      if (it.itList.p != nullptr)
         assertUnit(it.itList == us.buckets[0].end());
      assertUnit(it.itVector == us.buckets.end());
      assertEmptyFixture(us);
   }  // teardown

   // find something at the front of the list of a hash
   void test_find_standardFront()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      custom::unordered_set<Spy>::iterator it = us.begin();
      setupStandardFixture(us);
      Spy s(59);
      Spy::reset();
      // exercise
      it = us.find(s);
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
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itList == us.buckets[2].begin());
      auto itVector = us.buckets.begin();
      ++itVector;
      ++itVector;
      assertUnit(it.itVector == itVector);
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(59));
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }  

   // find something at the front of the list of a hash
   void test_find_standardBack()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      custom::unordered_set<Spy>::iterator it = us.begin();
      setupStandardFixture(us);
      Spy s(67);
      Spy::reset();
      // exercise
      it = us.find(s);
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
      assertUnit(it.itVectorEnd == us.buckets.end());
      auto itList = us.buckets[1].begin();
      ++itList;
      assertUnit(it.itList == itList);
      auto itVector = us.buckets.begin();
      ++itVector;
      assertUnit(it.itVector == itVector);
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(67));
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // find something missing from an empty list
   void test_find_standardMissingEmptyList()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      custom::unordered_set<Spy>::iterator it = us.begin();
      setupStandardFixture(us);
      Spy s(12);  // hashes to bucket 3
      Spy::reset();
      // exercise
      it = us.find(s);
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
      assertUnit(it.itVectorEnd == us.buckets.end());
      if (it.itList.p != nullptr)
         assertUnit(it.itList == us.buckets[0].end());
      assertUnit(it.itVector == us.buckets.end());
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // find something missing from an empty list
   void test_find_standardMissingFilledList()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      custom::unordered_set<Spy>::iterator it = us.begin();
      setupStandardFixture(us);
      Spy s(10);  // hashes to bucket 1
      Spy::reset();
      // exercise
      it = us.find(s);
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
      assertUnit(it.itVectorEnd == us.buckets.end());
      if (it.itList.p != nullptr)
         assertUnit(it.itList == us.buckets[0].end());
      assertUnit(it.itVector == us.buckets.end());
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }


   /***************************************
    * SIZE EMPTY 
    ***************************************/

    // size of an empty hash
   void test_size_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      Spy::reset();
      // exercise
      size_t size = us.size();
      // verify
      assertUnit(0 == size);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(us);
   }  // teardown

   // size of the standard hash
   void test_size_standard()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy::reset();
      // exercise
      size_t size = us.size();
      // verify
      assertUnit(4 == size);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }  


   // empty hash empty?
   void test_empty_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      Spy::reset();
      // exercise
      bool empty = us.empty();
      // verify
      assertUnit(true == empty);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(us);
   }  // teardown

   // standard hash empty?
   void test_empty_standard()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy::reset();
      // exercise
      bool empty = us.empty();
      // verify
      assertUnit(false == empty);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // verify the bucket size of an empty hash
   void test_bucketSize_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      size_t i = 0;
      size_t num = 0;
      Spy::reset();
      // exercise
      num = us.bucket_size(i);
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
      assertUnit(num == 0);
      assertEmptyFixture(us);
   }  // teardown

   // verify the bucket size of standard hash, empty bucket
   void test_bucketSize_standardEmpty()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      size_t i = 3;
      size_t num = 0;
      Spy::reset();
      // exercise
      num = us.bucket_size(i);
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
      assertUnit(num == 0);
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // verify the bucket size of standard hash, bucket has one element
   void test_bucketSize_standardOne()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      size_t i = 2;
      size_t num = 0;
      Spy::reset();
      // exercise
      num = us.bucket_size(i);
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
      assertUnit(num == 1);
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // verify the bucket size of standard hash, bucket has two elements
   void test_bucketSize_standardTwo()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      size_t i = 1;
      size_t num = 0;
      Spy::reset();
      // exercise
      num = us.bucket_size(i);
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
      assertUnit(num == 2);
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // verify the number of buckets in the empty fixture
   void test_bucketCount_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      size_t num = 0;
      Spy::reset();
      // exercise
      num = us.bucket_count();
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
      assertUnit(num == 8);
      assertEmptyFixture(us);
   }  // teardown

   // verify the number of buckets in the standard fixture
   void test_bucketCount_standard()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      size_t num = 0;
      Spy::reset();
      // exercise
      num = us.bucket_count();
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
      assertUnit(num == 4);
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // verify the load factor in the empty fixture
   void test_loadFactor_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      float lf = float(-99.9);
      Spy::reset();
      // exercise
      lf = us.load_factor();
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
      assertUnit(lf == 0.0);
      assertEmptyFixture(us);
   }  // teardown

   // verify the load factor in the standard fixture
   void test_loadFactor_standard()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      float lf = float(-99.9);
      Spy::reset();
      // exercise
      lf = us.load_factor();
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
      assertUnit(lf == 1.0);
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // verify the maximum load factor is 1.0
   void test_loadFactor_default()
   {  // setup
      custom::unordered_set<Spy> us;
      float max = float(-99.9);
      Spy::reset();
      // exercise
      max = us.max_load_factor();
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
      assertUnit(max == (float)1.0);
      assertEmptyFixture(us);
   }  // teardown

   // verify the maximum load factor is 2.0
   void test_loadFactor_two()
   {  // setup
      custom::unordered_set<Spy> us;
      us.maxLoadFactor = (float)2.0;
      float max = float(-99.9);
      Spy::reset();
      // exercise
      max = us.max_load_factor();
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
      assertUnit(max == (float)2.0);
      us.maxLoadFactor = (float)1.0;
      assertEmptyFixture(us);
   }  // teardown

   // verify the maximum load factor is 1.0
   void test_setLoadFactor_five()
   {  // setup
      custom::unordered_set<Spy> us;
      float max = float(5.0);
      Spy::reset();
      // exercise
      us.max_load_factor(max);
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
      assertUnit(us.maxLoadFactor == (float)5.0);
      us.maxLoadFactor = (float)1.0;
      assertEmptyFixture(us);
   }  // teardown

   /***************************************
    * ASSIGN
    ***************************************/

    // assign an empty set to an empty set
    void test_assign_emptyEmpty()
    {  // setup
       custom::unordered_set<Spy> usSrc;
       custom::unordered_set<Spy> usDes;
       Spy::reset();
       // exercise
       usDes = usSrc;
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
       assertEmptyFixture(usSrc);
       assertEmptyFixture(usDes);
    }  // teardown

    // assign an empty set to a standard set
    void test_assign_emptyStandard()
    {  // setup
       custom::unordered_set<Spy> usSrc;
       custom::unordered_set<Spy> usDes;
       setupStandardFixture(usDes);
       Spy::reset();
       // exercise
       usDes = usSrc;
       // verify
       assertUnit(Spy::numDestructor() == 4);   // [31, 49, 67, 59]
       assertUnit(Spy::numDelete() == 4);       // [31, 49, 67, 59]
       assertUnit(Spy::numAssign() == 0);
       assertUnit(Spy::numAlloc() == 0);
       assertUnit(Spy::numDefault() == 0);
       assertUnit(Spy::numNondefault() == 0);
       assertUnit(Spy::numCopy() == 0);
       assertUnit(Spy::numCopyMove() == 0);
       assertUnit(Spy::numAssignMove() == 0);
       assertEmptyFixture(usSrc);
       assertEmptyFixture(usDes);
    }  // teardown

    // assign an standard set to an empty set
    void test_assign_standardEmpty()
    {  // setup
       custom::unordered_set<Spy> usSrc;
       custom::unordered_set<Spy> usDes;
       setupStandardFixture(usSrc);
       Spy::reset();
       // exercise
       usDes = usSrc;
       // verify
       assertUnit(Spy::numAlloc() == 4);   // [31, 49, 67, 59]
       assertUnit(Spy::numCopy() == 4);    // [31, 49, 67, 59]
       assertUnit(Spy::numDestructor() == 0);
       assertUnit(Spy::numDelete() == 0);       
       assertUnit(Spy::numAssign() == 0);
       assertUnit(Spy::numDefault() == 0);
       assertUnit(Spy::numNondefault() == 0);
       assertUnit(Spy::numCopyMove() == 0);
       assertUnit(Spy::numAssignMove() == 0);
       assertStandardFixture(usSrc);
       assertStandardFixture(usDes);
       // teardown
       teardownStandardFixture(usSrc);
       teardownStandardFixture(usDes);
    }  

   // move-assign an empty set to an empty set
   void test_assignMove_emptyEmpty()
   {  // setup
      custom::unordered_set<Spy> usSrc;
      custom::unordered_set<Spy> usDes;
      Spy::reset();
      // exercise
      usDes = std::move(usSrc);
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
      assertEmptyFixture(usSrc);
      assertEmptyFixture(usDes);
   }  // teardown
   
   // move-assign an empty set to a standard set
   void test_assignMove_emptyStandard()
   {  // setup
      custom::unordered_set<Spy> usSrc;
      custom::unordered_set<Spy> usDes;
      setupStandardFixture(usDes);
      Spy::reset();
      // exercise
      usDes = std::move(usSrc);
      // verify
      assertUnit(Spy::numDestructor() == 4);   // [31, 49, 67, 59]
      assertUnit(Spy::numDelete() == 4);       // [31, 49, 67, 59]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(usSrc);
      assertEmptyFixture(usDes);
   }  // teardown
   
   // move-assign an standard set to an empty set
   void test_assignMove_standardEmpty()
   {  // setup
      custom::unordered_set<Spy> usSrc;
      custom::unordered_set<Spy> usDes;
      setupStandardFixture(usSrc);
      Spy::reset();
      // exercise
      usDes = std::move(usSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertEmptyFixture(usSrc);
      assertStandardFixture(usDes);
      // teardown
      teardownStandardFixture(usSrc);
      teardownStandardFixture(usDes);
   }
   
   
   // swap empty hashes use member swap
   void test_swapMember_emptyEmpty()
   {  // setup
      custom::unordered_set<Spy> us1;
      custom::unordered_set<Spy> us2;
      Spy::reset();
      // exercise
      us1.swap(us2);
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
      assertEmptyFixture(us1);
      assertEmptyFixture(us2);
   }  // teardown
   
   // swap standard hash with empty hash using memger functions
   void test_swapMember_standardEmpty()
   {  // setup
      custom::unordered_set<Spy> us1;
      custom::unordered_set<Spy> us2;
      setupStandardFixture(us1);
      Spy::reset();
      // exercise
      us1.swap(us2);
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
      assertEmptyFixture(us1);
      assertStandardFixture(us2);
      // teardown
      teardownStandardFixture(us2);
   }  // teardown
   
   // swap standard hash with single-element hash
   void test_swapMember_standardOther()
   {  // setup
      custom::unordered_set<Spy> us1;
      custom::unordered_set<Spy> us2;
      setupStandardFixture(us1);
      us2.buckets.clear();
      us2.buckets.resize(2);
      std::hash<Spy> pHash;
      assert(pHash(Spy(99)) % size_t(2) == 0);
      us2.buckets[0].push_back(Spy(99));
      us2.numElements = 1;
      us2.maxLoadFactor = (float)3.14;
      Spy::reset();
      // exercise
      us1.swap(us2);
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
      assertStandardFixture(us2);
      assertUnit(us1.maxLoadFactor == (float)3.14);
      assertUnit(us1.numElements == 1);
      assertUnit(us1.buckets.size() == 2);
      assertUnit(us1.buckets[0].size() == 1);
      if (!us1.buckets[0].empty())
         assertUnit(us1.buckets[0].front() == Spy(99));
      assertUnit(us1.buckets[1].size() == 0);
      // teardown
      teardownStandardFixture(us1);
      teardownStandardFixture(us2);
   }  // teardown
   
   // swap empty hashs using non-member swap
   void test_swapNonMember_emptyEmpty()
   {  // setup
      custom::unordered_set<Spy> us1;
      custom::unordered_set<Spy> us2;
      Spy::reset();
      // exercise
      swap(us1, us2);
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
      assertEmptyFixture(us1);
      assertEmptyFixture(us2);
   }  // teardown
   
   // swap standard hash with empty hash using non-member functions
   void test_swapNonMember_standardEmpty()
   {  // setup
      custom::unordered_set<Spy> us1;
      custom::unordered_set<Spy> us2;
      setupStandardFixture(us1);
      Spy::reset();
      // exercise
      swap(us1, us2);
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
      assertEmptyFixture(us1);
      assertStandardFixture(us2);
      // teardown
      teardownStandardFixture(us2);
   }  // teardown
   
   // swap standard hash with single-element hash
   void test_swapNonMember_standardOther()
   {  // setup
      custom::unordered_set<Spy> us1;
      custom::unordered_set<Spy> us2;
      setupStandardFixture(us1);
      us2.buckets.clear();
      us2.buckets.resize(2);
      std::hash<Spy> pHash;
      assert(pHash(Spy(99)) % size_t(2) == 0);
      us2.buckets[0].push_back(Spy(99));
      us2.numElements = 1;
      us2.maxLoadFactor = (float)3.14;
      Spy::reset();
      // exercise
      swap(us1, us2);
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
      assertStandardFixture(us2);
      assertUnit(us1.maxLoadFactor == (float)3.14);
      assertUnit(us1.numElements == 1);
      assertUnit(us1.buckets.size() == 2);
      assertUnit(us1.buckets[0].size() == 1);
      if (!us1.buckets[0].empty())
         assertUnit(us1.buckets[0].front() == Spy(99));
      assertUnit(us1.buckets[1].size() == 0);
      // teardown
      teardownStandardFixture(us1);
      teardownStandardFixture(us2);
   }  // teardown
   
   /***************************************
    * INSERT
    ***************************************/
   
    // rehash an empty hash... nothing is moved, right?
   void test_rehash_emptySmaller()
   {  // setup
      custom::unordered_set<Spy> us;
      Spy::reset();
      // exercise
      us.rehash(5);
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
      assertEmptyFixture(us);
   }  // teardown

   // rehash an empty hash... nothing is moved, right?
   void test_rehash_emptyBigger()
   {  // setup
      custom::unordered_set<Spy> us;
      Spy::reset();
      // exercise
      us.rehash(10);
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

      assertUnit(us.numElements == 0);
      assertUnit(us.buckets.size() == 10);
      assertUnit(us.maxLoadFactor == (float)1.0);
      if (us.buckets.size() == 10)
      {
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 0);
         assertUnit(us.buckets[2].size() == 0);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[4].size() == 0);
         assertUnit(us.buckets[5].size() == 0);
         assertUnit(us.buckets[6].size() == 0);
         assertUnit(us.buckets[7].size() == 0);
         assertUnit(us.buckets[8].size() == 0);
         assertUnit(us.buckets[9].size() == 0);
      }
   }  // teardown

   // rehash the standard hash to size 6
   void test_rehash_standard6()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy::reset();
      // exercise
      us.rehash(6);
      // verify
      assertUnit(Spy::numCopyMove() == 4);   // [31, 49, 67, 59]
      assertUnit(Spy::numDestructor() == 4); // [31, 49, 67, 59]
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      // h[0] --> 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      // h[4] --> 31
      // h[5] --> 
      assertUnit(us.numElements == 4);
      assertUnit(us.buckets.size() == 6);
      assertUnit(us.maxLoadFactor == (float)1.3);
      if (us.buckets.size() == 10)
      {
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 2);
         assertUnit(us.buckets[2].size() == 1);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[4].size() == 1);
         assertUnit(us.buckets[5].size() == 0);
         if (!us.buckets[1].empty())
         {
            assertUnit(us.buckets[1].front() == Spy(49)); // (4 + 9) % 6 = 1
            assertUnit(us.buckets[1].back() == Spy(67)); // (6 + 7) % 6 = 1
         }
         if (!us.buckets[2].empty())
            assertUnit(us.buckets[2].front() == Spy(59)); // (5 + 9) % 6 = 2
         if (!us.buckets[4].empty())
            assertUnit(us.buckets[4].front() == Spy(31)); // (3 + 1) % 6 = 4
      }
     // teardown
      teardownStandardFixture(us);
   }

   // rehash the standard hash to size 8
   void test_rehash_standard8()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy::reset();
      // exercise
      us.rehash(8);
      // verify
      assertUnit(Spy::numCopyMove() == 4);   // [31, 49, 67, 59]
      assertUnit(Spy::numDestructor() == 4); // [31, 49, 67, 59]
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      // h[0] --> 
      // h[1] --> 
      // h[2] -->  
      // h[3] --> 
      // h[4] --> 31
      // h[5] --> 49 67
      // h[6] --> 59
      // h[7] --> 
      assertUnit(us.numElements == 4);
      assertUnit(us.buckets.size() == 8);
      assertUnit(us.maxLoadFactor == (float)1.3);
      if (us.buckets.size() == 10)
      {
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 0);
         assertUnit(us.buckets[2].size() == 0);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[4].size() == 1);
         assertUnit(us.buckets[5].size() == 2);
         assertUnit(us.buckets[6].size() == 1);
         assertUnit(us.buckets[7].size() == 0);
         if (!us.buckets[4].empty())
            assertUnit(us.buckets[4].front() == Spy(31)); // (3 + 1) % 8 = 4
         if (!us.buckets[5].empty())
         {
            assertUnit(us.buckets[5].front() == Spy(49)); // (4 + 9) % 8 = 5
            assertUnit(us.buckets[5].back() == Spy(67));  // (6 + 7) % 8 = 5
         }
         if (!us.buckets[6].empty())
            assertUnit(us.buckets[6].front() == Spy(59)); // (5 + 9) % 8 = 6
      }
     // teardown
      teardownStandardFixture(us);
   }

   // reserve enough buckets to fit 10 elements with the load factor at default 1.0
   void test_reserve_empty10()
   {  // setup
      custom::unordered_set<Spy> us;
      Spy::reset();
      // exercise
      us.reserve(10);
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

      assertUnit(us.numElements == 0);
      assertUnit(us.buckets.size() == 10);
      assertUnit(us.maxLoadFactor == (float)1.0);
      if (us.buckets.size() == 10)
      {
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 0);
         assertUnit(us.buckets[2].size() == 0);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[4].size() == 0);
         assertUnit(us.buckets[5].size() == 0);
         assertUnit(us.buckets[6].size() == 0);
         assertUnit(us.buckets[7].size() == 0);
         assertUnit(us.buckets[8].size() == 0);
         assertUnit(us.buckets[9].size() == 0);
      }
   }  // teardown

   // reserve enough buckets to fit 12 elements with the load factor at 0.5
   void test_reserve_empty12()
   {  // setup
      custom::unordered_set<Spy> us;
      us.maxLoadFactor = (float)0.5;
      Spy::reset();
      // exercise
      us.reserve(12);
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

      assertUnit(us.numElements == 0);
      assertUnit(us.buckets.size() == 24);
      assertUnit(us.maxLoadFactor == (float)0.5);
      if (us.buckets.size() == 24)
      {
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 0);
         assertUnit(us.buckets[2].size() == 0);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[4].size() == 0);
         assertUnit(us.buckets[5].size() == 0);
         assertUnit(us.buckets[6].size() == 0);
         assertUnit(us.buckets[7].size() == 0);
         assertUnit(us.buckets[8].size() == 0);
         assertUnit(us.buckets[9].size() == 0);
         assertUnit(us.buckets[10].size() == 0);
         assertUnit(us.buckets[11].size() == 0);
         assertUnit(us.buckets[12].size() == 0);
         assertUnit(us.buckets[13].size() == 0);
         assertUnit(us.buckets[14].size() == 0);
         assertUnit(us.buckets[15].size() == 0);
         assertUnit(us.buckets[16].size() == 0);
         assertUnit(us.buckets[17].size() == 0);
         assertUnit(us.buckets[18].size() == 0);
         assertUnit(us.buckets[19].size() == 0);
         assertUnit(us.buckets[20].size() == 0);
         assertUnit(us.buckets[21].size() == 0);
         assertUnit(us.buckets[22].size() == 0);
         assertUnit(us.buckets[23].size() == 0);
      }
   }  // teardown

   // reserve enough for 6 with load factor set to 1.0
   void test_reserve_standard6()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      us.maxLoadFactor = (float)1.0;
      Spy::reset();
      // exercise
      us.reserve(6);
      // verify
      assertUnit(Spy::numCopyMove() == 4);   // [31, 49, 67, 59]
      assertUnit(Spy::numDestructor() == 4); // [31, 49, 67, 59]
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      // h[0] --> 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      // h[4] --> 31
      // h[5] --> 
      assertUnit(us.numElements == 4);
      assertUnit(us.buckets.size() == 6);
      assertUnit(us.maxLoadFactor == (float)1.0);
      if (us.buckets.size() == 6)
      {
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 2);
         assertUnit(us.buckets[2].size() == 1);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[4].size() == 1);
         assertUnit(us.buckets[5].size() == 0);
         if (!us.buckets[1].empty())
         {
            assertUnit(us.buckets[1].front() == Spy(49)); // (4 + 9) % 6 = 1
            assertUnit(us.buckets[1].back() == Spy(67)); // (6 + 7) % 6 = 1
         }
         if (!us.buckets[2].empty())
            assertUnit(us.buckets[2].front() == Spy(59)); // (5 + 9) % 6 = 2
         if (!us.buckets[4].empty())
            assertUnit(us.buckets[4].front() == Spy(31)); // (3 + 1) % 6 = 4
      }
     // teardown
      teardownStandardFixture(us);
   }

   //  reserve enough for 8 with load factor set to 0.5
   void test_reserve_standard8()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      us.maxLoadFactor = (float)0.5;
      Spy::reset();
      // exercise
      us.reserve(8);
      // verify
      assertUnit(Spy::numCopyMove() == 4);   // [31, 49, 67, 59]
      assertUnit(Spy::numDestructor() == 4); // [31, 49, 67, 59]
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      // h[0] --> 
      // h[1] --> 
      // h[2] -->  
      // h[3] --> 
      // h[4] --> 31
      // h[5] --> 
      // h[6] --> 
      // h[7] --> 
      // h[8] --> 
      // h[9] --> 
      // h[10] --> 
      // h[11] --> 
      // h[12] --> 
      // h[13] --> 49 67
      // h[14] --> 59
      // h[15] --> 
      assertUnit(us.numElements == 4);
      assertUnit(us.buckets.size() == 16);
      assertUnit(us.maxLoadFactor == (float)0.5);
      if (us.buckets.size() == 16)
      {
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 0);
         assertUnit(us.buckets[2].size() == 0);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[4].size() == 1);
         assertUnit(us.buckets[5].size() == 0);
         assertUnit(us.buckets[6].size() == 0);
         assertUnit(us.buckets[7].size() == 0);
         assertUnit(us.buckets[8].size() == 0);
         assertUnit(us.buckets[9].size() == 0);
         assertUnit(us.buckets[10].size() == 0);
         assertUnit(us.buckets[11].size() == 0);
         assertUnit(us.buckets[12].size() == 0);
         assertUnit(us.buckets[13].size() == 2);
         assertUnit(us.buckets[14].size() == 1);
         assertUnit(us.buckets[15].size() == 0);
         if (!us.buckets[4].empty())
            assertUnit(us.buckets[4].front() == Spy(31)); // (3 + 1) % 16 = 4
         if (!us.buckets[13].empty())
         {
            assertUnit(us.buckets[13].front() == Spy(49)); // (4 + 9) % 16 = 13
            assertUnit(us.buckets[13].back() == Spy(67));  // (6 + 7) % 16 = 13
         }
         if (!us.buckets[14].empty())
            assertUnit(us.buckets[14].front() == Spy(59)); // (5 + 9) % 16 = 14
     }
     // teardown
      teardownStandardFixture(us);
   }


    // test that we can insert 0 into an empty hash
   void test_insert_empty0()
   {  // setup
      custom::unordered_set<Spy> us;
      Spy s(0);
      custom::pair<custom::unordered_set<Spy>::iterator, bool> p;
      Spy::reset();
      // exercise
      p = us.insert(s);
      // verify
      assertUnit(Spy::numAlloc() == 1);    // allocate [0]
      assertUnit(Spy::numCopy() == 1);     // copy     [0]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(p.first != us.end());
      if (p.first != us.end())
         assertUnit(*(p.first) == Spy(0));
      assertUnit(p.second == true);
      assertUnit(us.numElements == 1);
      if (us.numElements == 1)
      {
         assertUnit(us.buckets[0].size() == 1);
         if (us.buckets[0].size() == 1)
         {
            assertUnit(us.buckets[0].front() == Spy(0));
            us.buckets[0].pop_front();
            us.numElements = 0;
         }
      }
      assertEmptyFixture(us);
   }  // teardown

    // test that we can insert 58 into an empty hash
   void test_insert_empty58()
   {  // setup
      custom::unordered_set<Spy> us;
      Spy s(58);   // into slot (5+8)%8 = 5
      custom::pair<custom::unordered_set<Spy>::iterator, bool> p;
      Spy::reset();
      // exercise
      p = us.insert(s);
      // verify
      assertUnit(Spy::numAlloc() == 1);    // allocate [58]
      assertUnit(Spy::numCopy() == 1);     // copy     [58]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(p.first != us.end());
      assertUnit(us.numElements == 1);
      if (us.numElements > 0)
      { 
         if (p.first != us.end())
            assertUnit(*(p.first) == Spy(58));
         assertUnit(p.second == true);
         assertUnit(us.buckets[5].size() == 1);
         if (us.buckets[5].size() == 1)
         {
            assertUnit(us.buckets[5].front() == Spy(58));
            us.buckets[5].pop_front();
            us.numElements = 0;
         }
      }
      assertEmptyFixture(us);
   }  // teardown

    // test that we can insert 3 into the standard hash
    void test_insert_standard3()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::pair<custom::unordered_set<Spy>::iterator, bool> p;
      Spy s(3);   // into slot (3)%4 = 3
      Spy::reset();
      // exercise
      p = us.insert(s);
      // verify
      assertUnit(Spy::numAlloc() == 1);    // allocate [3]
      assertUnit(Spy::numCopy() == 1);     // copy     [3]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(p.first != us.end());
      if (p.first != us.end())
         assertUnit(*(p.first) == Spy(3));
      assertUnit(p.second == true);
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> [3]
      assertUnit(us.numElements == 5);
      if (us.numElements == 5)
      {
         assertUnit(us.buckets[3].size() == 1);
         if (us.buckets[3].size() == 1)
         {
            assertUnit(us.buckets[3].front() == Spy(3));
            us.buckets[3].pop_front();
            us.numElements = 4;
         }
      }
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

    // test that we can insert 44 into the standard hash
    void test_insert_standard44()
    {  // setup
       // h[0] --> 31 
       // h[1] --> 49 67
       // h[2] --> 59 
       // h[3] --> 
       custom::unordered_set<Spy> us;
       setupStandardFixture(us);
       Spy s(44);   // into slot (4+4)%4 = 0
       custom::pair<custom::unordered_set<Spy>::iterator, bool> p;
       Spy::reset();
       // exercise
       p = us.insert(s);
       // verify
       assertUnit(Spy::numAlloc() == 1);    // allocate [44]
       assertUnit(Spy::numCopy() == 1);     // copy     [44]
       assertUnit(Spy::numAssign() == 0);
       assertUnit(Spy::numDelete() == 0);
       assertUnit(Spy::numDefault() == 0);
       assertUnit(Spy::numNondefault() == 0);
       assertUnit(Spy::numCopyMove() == 0);
       assertUnit(Spy::numAssignMove() == 0);
       assertUnit(Spy::numDestructor() == 0);
       assertUnit(p.first != us.end());
       if (p.first != us.end())
          assertUnit(*(p.first) == Spy(44));
       assertUnit(p.second == true);
       // h[0] --> 31 [44]
       // h[1] --> 49 67
       // h[2] --> 59 
       // h[3] --> 
       assertUnit(us.numElements == 5);
       if (us.numElements == 5)
       {
          assertUnit(us.buckets[0].size() == 2);
          if (us.buckets[0].size() == 2)
          {
             assertUnit(us.buckets[0].back() == Spy(44));
             us.buckets[0].pop_back();
             us.numElements = 4;
          }
       }
       assertStandardFixture(us);
       // teardown
       teardownStandardFixture(us);
    }

    // test that we can insert 67 into the empty hash
    void test_insert_standardDuplicate()
    {  // setup
       // h[0] --> 31 
       // h[1] --> 49 67
       // h[2] --> 59 
       // h[3] --> 
       custom::unordered_set<Spy> us;
       setupStandardFixture(us);
       Spy s(67);   // into slot (6+7)%4 = 1
       custom::pair<custom::unordered_set<Spy>::iterator, bool> p;
       Spy::reset();
       // exercise
       p = us.insert(s);
       // verify
       assertUnit(Spy::numAlloc() == 0);
       assertUnit(Spy::numCopy() == 0); 
       assertUnit(Spy::numAssign() == 0);
       assertUnit(Spy::numDelete() == 0);
       assertUnit(Spy::numDefault() == 0);
       assertUnit(Spy::numNondefault() == 0);
       assertUnit(Spy::numCopyMove() == 0);
       assertUnit(Spy::numAssignMove() == 0);
       assertUnit(Spy::numDestructor() == 0);
       assertUnit(p.first != us.end());
       if (p.first != us.end())
          assertUnit(*(p.first) == Spy(67));
       assertUnit(p.second == false);
       // h[0] --> 31 
       // h[1] --> 49 67
       // h[2] --> 59 
       // h[3] --> 
       assertStandardFixture(us);
       // teardown
       teardownStandardFixture(us);
    }

    // test that we can insert 44 into the standard hash causing a rehash
    void test_insert_standardRehash()
    {  // setup
       // h[0] --> 31 
       // h[1] --> 49 67
       // h[2] --> 59 
       // h[3] --> 
       custom::unordered_set<Spy> us;
       setupStandardFixture(us);
       us.maxLoadFactor = 1.0;
       Spy s(44);   // into slot (4+4)%8 = 0
       custom::pair<custom::unordered_set<Spy>::iterator, bool> p;
       Spy::reset();
       // exercise
       p = us.insert(s);
       // verify
       assertUnit(Spy::numAlloc() == 1);      // allocate [44]
       assertUnit(Spy::numCopy() == 1);       // copy     [44]
       assertUnit(Spy::numCopyMove() == 4);   // [31, 49, 67, 59]
       assertUnit(Spy::numDestructor() == 4); // [31, 49, 67, 59]
       assertUnit(Spy::numAssign() == 0);
       assertUnit(Spy::numDelete() == 0);
       assertUnit(Spy::numDefault() == 0);
       assertUnit(Spy::numNondefault() == 0);
       assertUnit(Spy::numAssignMove() == 0);
       // h[0] --> [44]
       // h[1] --> 
       // h[2] --> 
       // h[3] --> 
       // h[4] --> 31 
       // h[5] --> 49 67
       // h[6] --> 59 
       // h[7] --> 
       assertUnit(p.first != us.end());
       if (p.first != us.end())
          assertUnit(*(p.first) == Spy(44));
       assertUnit(p.second == true);
       assertUnit(us.numElements == 5);
       assertUnit(us.buckets.size() == 8);
       assertUnit(us.maxLoadFactor == (float)1.0);
       if (us.buckets.size() == 8)
       {
          assertUnit(us.buckets[0].size() == 1);  // 44
          assertUnit(us.buckets[1].size() == 0);
          assertUnit(us.buckets[2].size() == 0);
          assertUnit(us.buckets[3].size() == 0);
          assertUnit(us.buckets[4].size() == 1); // 31
          assertUnit(us.buckets[5].size() == 2); // 49 67
          assertUnit(us.buckets[6].size() == 1); // 59
          assertUnit(us.buckets[7].size() == 0);
          if (!us.buckets[0].empty())
             assertUnit(us.buckets[0].front() == Spy(44)); // (4 + 4) % 8 = 0
          if (!us.buckets[4].empty())
             assertUnit(us.buckets[4].front() == Spy(31)); // (3 + 1) % 8 = 4
          if (!us.buckets[5].empty())
          {
             assertUnit(us.buckets[5].front() == Spy(49)); // (4 + 9) % 8 = 5
             assertUnit(us.buckets[5].back() == Spy(67));  // (6 + 7) % 8 = 5
          }
          if (!us.buckets[6].empty())
             assertUnit(us.buckets[6].front() == Spy(59)); // (5 + 9) % 8 = 6
       }
       // teardown
       teardownStandardFixture(us);
    }


   /***************************************
    * ITERATOR
    ***************************************/

   // beginning of empty hash
   void test_iterator_begin_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      custom::unordered_set<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = us.begin();
      // verify
      assertUnit(it.itVector == us.buckets.end());
      if (us.buckets.size() > 0)
        assertUnit(it.itList == us.buckets[0].end());
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(us);
   }  // teardown

   // beginning of standard hash
   void test_iterator_begin_standard()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = us.begin();
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
      assertUnit(it.itVector == us.buckets.begin());
      if (it.itVector != us.buckets.end())
      {
         assertUnit(it.itList == us.buckets[0].begin());
         if (it.itList != us.buckets[0].end())
            assertUnit(*(it.itList) == Spy(31));
      }
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // end of empty hash
   void test_iterator_end_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      custom::unordered_set<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = us.end();
      // verify
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == us.buckets.end());
      if (us.buckets.size() > 0)
         assertUnit(it.itList == us.buckets[0].end());
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(us);
   }  // teardown

   // end of standard hash
   void test_iterator_end_standard()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = us.end();
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
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == us.buckets.end());
      assertUnit(it.itList == us.buckets[0].end());
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // increment of empty hash
   void test_iterator_increment_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      custom::unordered_set<Spy>::iterator it = us.end();
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == us.buckets.end());
      if (!us.buckets.empty())
         assertUnit(it.itList == us.buckets[0].end());
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(us);
   }  // teardown

   // increment of standard hash where there is more than one element
   void test_iterator_increment_moreInBucket()
   {  // setup
      // h[0] --> 31 
      // h[1] --> [49] 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::iterator it = us.begin();
      if (it.itList.p != nullptr && it.itVector.p != nullptr)
      {
         ++(it.itVector);
         it.itList = (*it.itVector).begin();
         assert(*(it.itList) == Spy(49));
      }
      Spy::reset();
      // exercise
      ++it;
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
      // h[0] --> 31 
      // h[1] --> 49 [67]
      // h[2] --> 59 
      // h[3] --> 
      auto itVectorCheck = us.buckets.begin();
      ++itVectorCheck;
      if (!us.buckets.empty() && it.itList.p != nullptr)
      {
         auto itListCheck = us.buckets[1].begin();
         ++itListCheck;
         assertUnit(it.itVectorEnd == us.buckets.end());
         assertUnit(it.itVector == itVectorCheck);
         assertUnit(it.itList == itListCheck);
         assertUnit(*(it.itList) == Spy(67));
      }
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // increment of standard hash where we need to go to the next bucket
   void test_iterator_increment_nextBucket()
   {  // setup
      // h[0] --> [31] 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::iterator it = us.begin();
      if (it.itList.p != nullptr)
         assert(*(it.itList) == Spy(31));
      Spy::reset();
      // exercise
      ++it;
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
      // h[0] --> 31 
      // h[1] --> [49] 67
      // h[2] --> 59 
      // h[3] --> 
      auto itVectorCheck = us.buckets.begin();
      ++itVectorCheck;
      auto itListCheck = us.buckets[1].begin();
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == itVectorCheck);
      assertUnit(it.itList == itListCheck);
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(49));
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // increment of standard hash where we were at the last element
   void test_iterator_increment_toEnd()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> [59] 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::iterator it = us.begin();
      if (it.itList.p != nullptr)
      {
         ++(it.itVector);
         ++(it.itVector);
         it.itList = (*it.itVector).begin();
         assert(*(it.itList) == Spy(59));
      }
      Spy::reset();
      // exercise
      ++it;
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
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == us.buckets.end());
      assertUnit(it.itList == us.buckets[0].end());
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // dereference the iterator
   void test_iterator_dereference()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> [59] 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::iterator it = us.begin();
      if (it.itList.p != nullptr)
      {
         ++(it.itVector);
         ++(it.itVector);
         it.itList = (*it.itVector).begin();
         assert(*(it.itList) == Spy(59));
      }
      Spy s(99);
      Spy::reset();
      // exercise
      s = *it;
      // verify
      assertUnit(Spy::numAssign() == 1);  // assign 59 --> s
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> [59] 
      // h[3] --> 
      assertUnit(s == Spy(59));
      auto itVectorCheck = us.buckets.begin();
      ++itVectorCheck;
      ++itVectorCheck;
      auto itListCheck = us.buckets[2].begin();
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == itVectorCheck);
      assertUnit(it.itList == itListCheck);
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(59));
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // local iterator begin for single element bucket
   void test_localIterator_begin_single()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::local_iterator it;
      Spy::reset();
      // exercise
      it = us.begin(0);
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
      // h[0] --> [31] 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      assertUnit(it.itList == us.buckets[0].begin());
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(31));
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // local iterator begin for double element bucket
   void test_localIterator_begin_multiple()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::local_iterator it;
      Spy::reset();
      // exercise
      it = us.begin(1);
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
      // h[0] --> 31 
      // h[1] --> [49] 67
      // h[2] --> 59 
      // h[3] --> 
      assertUnit(it.itList == us.buckets[1].begin());
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(49));
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // local iterator begin for empty element bucket
   void test_localIterator_begin_empty()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::local_iterator it;
      Spy::reset();
      // exercise
      it = us.begin(3);
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
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      assertUnit(it.itList == us.buckets[3].begin());
      if (it.itList.p != nullptr)
         assertUnit(it == us.buckets[3].end());
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // local iterator increment for single element bucket
   void test_localIterator_increment_single()
   {  // setup
      // h[0] --> [31] 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::local_iterator it;
      it.itList = us.buckets[0].begin();
      Spy::reset();
      // exercise
      ++it;
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
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      assertUnit(it.itList == us.buckets[0].end());
      if (it.itList.p != nullptr)
         assertUnit(it == us.end(0));
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // local iterator increment for multiple element bucket
   void test_localIterator_increment_multiple()
   {  // setup
      // h[0] --> 31 
      // h[1] --> [49] 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      custom::unordered_set<Spy>::local_iterator it;
      it.itList = us.buckets[1].begin();
      Spy::reset();
      // exercise
      ++it;
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
      // h[0] --> 31 
      // h[1] --> 49 [67]
      // h[2] --> 59 
      // h[3] --> 
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(67));
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }



   /***************************************
    * REMOVE
    ***************************************/

   // clear an empty hash
   void test_clear_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      Spy::reset();
      // exercise
      us.clear();
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
      assertEmptyFixture(us);
   }  // teardown

   // clear all the elements from the standard fixture
   void test_clear_standard()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy::reset();
      // exercise
      us.clear();
      // verify
      assertUnit(Spy::numDelete() == 4);     // delete  [31,49,67,59]
      assertUnit(Spy::numDestructor() == 4); // destroy [31,49,67,59]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // h[0] --> 
      // h[1] --> 
      // h[2] -->  
      // h[3] --> 
      assertUnit(us.numElements == 0);
      assertUnit(us.buckets.size() == 4);
      assertUnit(us.buckets[0].size() == 0);
      assertUnit(us.buckets[1].size() == 0);
      assertUnit(us.buckets[2].size() == 0);
      assertUnit(us.buckets[3].size() == 0);
      // teardown
      teardownStandardFixture(us);
   }

   // erase an empty hash
   void test_erase_empty()
   {  // setup
      custom::unordered_set<Spy> us;
      Spy s(99);
      custom::unordered_set<Spy>::iterator it = us.end();
      Spy::reset();
      // exercise
      it = us.erase(s);
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
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == us.buckets.end());
      if (it.itList.p != nullptr)
         assertUnit(it.itList == us.buckets[0].end());
      assertEmptyFixture(us);
   }  // teardown

   // clear all the elements from the standard fixture
   void test_erase_standardMissing()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy s(99);
      custom::unordered_set<Spy>::iterator it = us.end();
      Spy::reset();
      // exercise
      it = us.erase(s);
      // verify
      assertUnit(Spy::numDelete() == 0);     
      assertUnit(Spy::numDestructor() == 0); 
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == us.buckets.end());
      assertUnit(it.itList == us.buckets[0].end());
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      assertStandardFixture(us);
      // teardown
      teardownStandardFixture(us);
   }

   // clear all the elements from the standard fixture
   void test_erase_standardAlone()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy s(31);
      custom::unordered_set<Spy>::iterator it = us.end();
      Spy::reset();
      // exercise
      it = us.erase(s);
      // verify
      assertUnit(Spy::numDelete() == 1);      // delete  [31]
      assertUnit(Spy::numDestructor() == 1);  // destroy [31]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // h[0] --> 
      // h[1] --> [49] 67
      // h[2] --> 59 
      // h[3] --> 
      auto itVectorCheck = us.buckets.begin();
      ++itVectorCheck;
      auto itListCheck = us.buckets[1].begin();      
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == itVectorCheck);
      assertUnit(it.itList == itListCheck);
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(49));
      assertUnit(us.numElements == 3);
      if (us.numElements == 3)
      {
         assertUnit(us.buckets.size() == 4);
         assertUnit(us.buckets[0].size() == 0);
         assertUnit(us.buckets[1].size() == 2);
         assertUnit(us.buckets[2].size() == 1);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[1].front() == Spy(49));
         assertUnit(us.buckets[1].back()  == Spy(67));
         assertUnit(us.buckets[2].front() == Spy(59));      
      }
      // teardown
      teardownStandardFixture(us);
   }

   // erase the first element on the list
   void test_erase_standardFront()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy s(49);
      custom::unordered_set<Spy>::iterator it = us.end();
      Spy::reset();
      // exercise
      it = us.erase(s);
      // verify
      assertUnit(Spy::numDelete() == 1);      // delete  [49]
      assertUnit(Spy::numDestructor() == 1);  // destroy [49]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // h[0] --> 31
      // h[1] --> [67]
      // h[2] --> 59 
      // h[3] --> 
      auto itVectorCheck = us.buckets.begin();
      ++itVectorCheck;
      auto itListCheck = us.buckets[1].begin();
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == itVectorCheck);
      assertUnit(it.itList == itListCheck);
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(67));
      assertUnit(us.numElements == 3);
      if (us.numElements == 3)
      {
         assertUnit(us.buckets.size() == 4);
         assertUnit(us.buckets[0].size() == 1);
         assertUnit(us.buckets[1].size() == 1);
         assertUnit(us.buckets[2].size() == 1);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[0].front() == Spy(31));
         assertUnit(us.buckets[1].front() == Spy(67));
         assertUnit(us.buckets[2].front() == Spy(59));
      }
      // teardown
      teardownStandardFixture(us);
   }

   // erase the last element on the list
   void test_erase_standardBack()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy s(67);
      custom::unordered_set<Spy>::iterator it = us.end();
      Spy::reset();
      // exercise
      it = us.erase(s);
      // verify
      assertUnit(Spy::numDelete() == 1);      // delete  [67]
      assertUnit(Spy::numDestructor() == 1);  // destroy [67]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // h[0] --> 31
      // h[1] --> 49
      // h[2] --> [59] 
      // h[3] --> 
      auto itVectorCheck = us.buckets.begin();
      ++itVectorCheck;
      ++itVectorCheck;
      auto itListCheck = us.buckets[2].begin();
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == itVectorCheck);
      assertUnit(it.itList == itListCheck);
      if (it.itList.p != nullptr)
         assertUnit(*(it.itList) == Spy(59));
      assertUnit(us.numElements == 3);
      if (us.numElements == 3)
      {
         assertUnit(us.buckets.size() == 4);
         assertUnit(us.buckets[0].size() == 1);
         assertUnit(us.buckets[1].size() == 1);
         assertUnit(us.buckets[2].size() == 1);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[0].front() == Spy(31));
         assertUnit(us.buckets[1].front() == Spy(49));
         assertUnit(us.buckets[2].front() == Spy(59));
      }
      // teardown
      teardownStandardFixture(us);
   }

   // erase the last element on the entire hash
   void test_erase_standardLast()
   {  // setup
      // h[0] --> 31 
      // h[1] --> 49 67
      // h[2] --> 59 
      // h[3] --> 
      custom::unordered_set<Spy> us;
      setupStandardFixture(us);
      Spy s(59);
      custom::unordered_set<Spy>::iterator it = us.end();
      Spy::reset();
      // exercise
      it = us.erase(s);
      // verify
      assertUnit(Spy::numDelete() == 1);      // delete  [59]
      assertUnit(Spy::numDestructor() == 1);  // destroy [59]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // h[0] --> 31
      // h[1] --> 49 67
      // h[2] --> 
      // h[3] --> 
      assertUnit(it.itVectorEnd == us.buckets.end());
      assertUnit(it.itVector == us.buckets.end());
      if (it.itList.p != nullptr)
         assertUnit(it.itList == us.buckets[0].end());      
      assertUnit(us.numElements == 3);
      if (us.numElements == 3)
      {
         assertUnit(us.buckets.size() == 4);
         assertUnit(us.buckets[0].size() == 1);
         assertUnit(us.buckets[1].size() == 2);
         assertUnit(us.buckets[2].size() == 0);
         assertUnit(us.buckets[3].size() == 0);
         assertUnit(us.buckets[0].front() == Spy(31));
         assertUnit(us.buckets[1].front() == Spy(49));
         assertUnit(us.buckets[1].back() == Spy(67));
      }
      // teardown
      teardownStandardFixture(us);
   }

   /*************************************************************
    * SETUP STANDARD FIXTURE
    *      h[0] --> 31 
    *      h[1] --> 49 67
    *      h[2] --> 59 
    *      h[3] --> 
    *************************************************************/
   void setupStandardFixture(custom::unordered_set<Spy> & us)
   {
      // clear out whatever the default constructor created
      us.buckets.clear();

      // allocate 4 buckets
      us.buckets.resize(4);
      assert(us.buckets.size() == 4);
      
      // place in four elements
      std::hash<Spy> pHash;
      assert(pHash(Spy(31)) % size_t(4) == 0);
      us.buckets[0].push_back(Spy(31));
      assert(pHash(Spy(49)) % size_t(4) == 1);
      us.buckets[1].push_back(Spy(49));
      assert(pHash(Spy(59)) % size_t(4) == 2);
      us.buckets[2].push_back(Spy(59));
      assert(pHash(Spy(67)) % size_t(4) == 1);
      us.buckets[1].push_back(Spy(67));
      

      // set the number of elements
      us.numElements = 4;

      // set the max load factor: 1.3 for now
      us.maxLoadFactor = (float)1.3;
   }



   /*************************************************************
    * VERIFY STANDARD FIXTURE
    *      h[0] --> 31
    *      h[1] --> 49 67
    *      h[2] --> 59
    *      h[3] -->
    *************************************************************/
   void assertStandardFixtureParameters(custom::unordered_set<Spy>& us, int line, const char * function)
   {
      assertIndirect(us.numElements == 4);
      assertIndirect(us.buckets.size() == 4);
      assertIndirect(us.maxLoadFactor == (float)1.3);

      if (us.buckets.size() == 4)
      {
         assertIndirect(us.buckets[0].size() == 1);
         assertIndirect(us.buckets[1].size() == 2);
         assertIndirect(us.buckets[2].size() == 1);
         assertIndirect(us.buckets[3].size() == 0);

         assertIndirect(us.buckets[0].front() == Spy(31));

         assertIndirect(us.buckets[1].front() == Spy(49));
         assertIndirect(us.buckets[1].back()  == Spy(67));

         assertIndirect(us.buckets[2].front() == Spy(59));
      }
   }
   /*************************************************************
    * VERIFY EMPTY FIXTURE
    *      h[0] --> 31
    *      h[1] --> 49 67
    *      h[2] --> 59
    *      h[3] -->
    *************************************************************/
   void assertEmptyFixtureParameters(custom::unordered_set<Spy>& us, int line, const char* function)
   {
      assertIndirect(us.numElements == 0);
      assertIndirect(us.buckets.size() == 8);
      assertIndirect(us.maxLoadFactor == (float)1.0);
      if (us.buckets.size() == 8)
      {
         assertIndirect(us.buckets[0].size() == 0);
         assertIndirect(us.buckets[1].size() == 0);
         assertIndirect(us.buckets[2].size() == 0);
         assertIndirect(us.buckets[3].size() == 0);
         assertIndirect(us.buckets[4].size() == 0);
         assertIndirect(us.buckets[5].size() == 0);
         assertIndirect(us.buckets[6].size() == 0);
         assertIndirect(us.buckets[7].size() == 0);
      }
   }

   /*************************************************************
    * TEARDOWN STANDARD FIXTURE
    *************************************************************/
   void teardownStandardFixture(custom::unordered_set<Spy>& us)
   {
      // explicitly clear each bucket
      for (auto & bucket : us.buckets)
         bucket.clear();

      // empty the vector itself
      us.buckets.clear();

      // reset the member variable
      us.numElements = 0;
   }

};

#endif // DEBUG
