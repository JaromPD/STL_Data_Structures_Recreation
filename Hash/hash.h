/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Isaac Radford, Jarom Diaz, Peter Benson
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include "vector.h"   // because this->buckets is a vector
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T,
          typename Hash = std::hash<T>,
          typename EqPred = std::equal_to<T>,
          typename A = std::allocator<T> >
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
public:
   //
   // Construct
   //
   unordered_set() : maxLoadFactor(1.0), numElements(0), buckets(8)
   {
   }
   unordered_set(size_t numBuckets) : maxLoadFactor(1.0), numElements(0), buckets(numBuckets)
   {
   }
   unordered_set(const unordered_set& rhs) : maxLoadFactor(rhs.maxLoadFactor), numElements(rhs.numElements), buckets(rhs.buckets)
   {
   }
   unordered_set(unordered_set&& rhs) : maxLoadFactor(rhs.maxLoadFactor), numElements(rhs.numElements), buckets(rhs.buckets)
   {
   }
   template <class Iterator>
   unordered_set(Iterator first, Iterator last) : maxLoadFactor(1.0), numElements(0)
   {
      // First we need to reserve the number of buckets.
      reserve(last - first);

      // Now we can insert.
      for (auto it = first; it != last; ++it)
         insert(*it);
   }

   //
   // Assign
   //
   unordered_set& operator=(const unordered_set& rhs)
   {
      numElements = rhs.numElements;
      maxLoadFactor = rhs.maxLoadFactor;
      buckets = rhs.buckets;

      return *this;
   }
   unordered_set& operator=(unordered_set&& rhs)
   {
      // move the elements from rhs to lhs.
      numElements = rhs.numElements;
      maxLoadFactor = rhs.maxLoadFactor;
      buckets = std::move(rhs.buckets);

      // return rhs to default state.
      rhs.numElements = 0;
      rhs.maxLoadFactor = 1.0;
      rhs.buckets.resize(8);

      return *this;
   }
   unordered_set& operator=(const std::initializer_list<T>& il)
   {
      return *this;
   }
   void swap(unordered_set& rhs)
   {
      std::swap(numElements, rhs.numElements);
      std::swap(maxLoadFactor, rhs.maxLoadFactor);
      buckets.swap(rhs.buckets);
   }

   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
      for (auto itBucket = buckets.begin(); itBucket != buckets.end(); itBucket++)
         if (!((*itBucket).empty()))
            return iterator(buckets.end(), itBucket, (*itBucket).begin());
      return end();
   }
   iterator end()
   {
      return iterator(buckets.end(), buckets.end(), buckets[0].end());
   }
   local_iterator begin(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].end());
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
      Hash hashFunc;
      size_t hashValue = hashFunc(t);
      return hashValue % bucket_count();
   }
   iterator find(const T& t);

   //   
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T> & il);
   void rehash(size_t numBuckets);
   void reserve(size_t num)
   {
      rehash(num / maxLoadFactor);
   }

   // 
   // Remove
   //
   void clear() noexcept
   {
      for (auto itBucket = buckets.begin(); itBucket != buckets.end(); itBucket++)
			(*itBucket).clear();
		numElements = 0;
   }
   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const
   {
      return (size_t)numElements;
   }
   bool empty() const
   {
      return numElements == 0;
   }
   size_t bucket_count() const
   {
      return buckets.size();
   }
   size_t bucket_size(size_t i) const
   {
      return buckets[i].size();
   }
   float load_factor() const noexcept
   {
      return numElements / buckets.size();
   }
   float max_load_factor() const noexcept
   {
      return maxLoadFactor;
   }
   void  max_load_factor(float m)
   {
      maxLoadFactor = m;
   }

private:

   size_t min_buckets_required(size_t num) const
   {
      float loadFactor = num / buckets.size();
      return (size_t) loadFactor;
   }

   custom::vector<custom::list<T,A>> buckets;  // each bucket in the hash
   int numElements;                            // number of elements in the Hash
   float maxLoadFactor;                        // the ratio of elements to buckets signifying a rehash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   iterator() 
   {
   }
   iterator(const typename custom::vector<custom::list<T> >::iterator& itVectorEnd,
            const typename custom::vector<custom::list<T> >::iterator& itVector,
            const typename custom::list<T>::iterator &itList)
   {
      this->itVectorEnd = itVectorEnd;
      this->itVector = itVector;
      this->itList = itList;
   }
   iterator(const iterator& rhs) 
   { 
      this->itVectorEnd = std::move(rhs.itVectorEnd);
      this->itVector = std::move(rhs.itVector);
      this->itList = std::move(rhs.itList);
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      itVectorEnd = std::move(rhs.itVectorEnd);
      itVector = std::move(rhs.itVector);
      itList = std::move(rhs.itList);
      return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   { 
      return (itVectorEnd != rhs.itVectorEnd || itVector != rhs.itVector || itList != rhs.itList);
   }
   bool operator == (const iterator& rhs) const 
   { 
      return (itVectorEnd == rhs.itVectorEnd && itVector == rhs.itVector && itList == rhs.itList);
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      iterator temp(*this);
      ++temp;
      return temp;
   }

private:
   typename vector<list<T>>::iterator itVectorEnd;
   typename list<T>::iterator itList;
   typename vector<list<T>>::iterator itVector;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   local_iterator()  
   {
   }
   local_iterator(const typename custom::list<T>::iterator& itList)
   {
      this->itList = itList;
   }
   local_iterator(const local_iterator& rhs) 
   { 
      this->itList = std::move(rhs.itList);
   }

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
      this->itList = std::move(rhs.itList);
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
      return (*this != rhs);
   }
   bool operator == (const local_iterator& rhs) const
   {
      return (*this == rhs);
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *this;
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
      ++itList;
      return *this;
   }
   local_iterator operator ++ (int postfix)
   {
      iterator returnCopy(*this);
      ++itList;
      return returnCopy;
   }

private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
typename unordered_set <T, Hash, E, A> ::iterator unordered_set<T, Hash, E, A>::erase(const T& t)
{
   // 1. Find element to be erased. Return end() if the element is not present.
   auto itErase = find(t);
   if (itErase == end())
      return itErase;
   // 2. Determine the return value.
   auto itReturn = itErase;
   ++itReturn;
   // 3. Erase the element from the bucket.
   (*itErase.itVector).erase(itErase.itList);
   if (numElements != 0)
      numElements--;
   // 4. Returns iterator to the next element.
   return itReturn;
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T, typename H, typename E, typename A>
custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool> unordered_set<T, H, E, A>::insert(const T& t)
{
   // 1. Find the bucket where the new element is to reside.
   auto iBucket = bucket(t);

   // 2. See if the element is already there. If so, then return out.
   for (auto it = buckets[iBucket].begin(); it != buckets[iBucket].end(); ++it)
      if (*it == t)
      {
         iterator itHash(buckets.end(), buckets.begin(), it);
         return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(itHash, false);
      }

   // load factor needs to be a float to take in decimals.
   float loadFactor = ((float)numElements + 1) / (float)bucket_count();

   // 3. Reserve more space if we are already at the limit.
   if (loadFactor > max_load_factor())
   {
      reserve(numElements * 2);
      iBucket = bucket(t);
   }

   // 4. Actually insert the new element on the back of the bucket.
   buckets[iBucket].push_back(t);
   numElements++;

   // 5. Return the results.
   iterator itHash(buckets.end(), buckets.begin(), buckets[iBucket].rbegin());
   return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(itHash, true);
}
template <typename T, typename H, typename E, typename A>
void unordered_set<T, H, E, A>::insert(const std::initializer_list<T> & il)
{
   //loop through the initializer list, then insert each element.
   for (const auto&& t : il)
   {
      insert(t);
   }
}

/*****************************************
 * UNORDERED SET :: REHASH
 * Re-Hash the unordered set by numBuckets
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
void unordered_set<T, Hash, E, A>::rehash(size_t numBuckets)
{
   using std::swap;
   Hash hash;        // so we can use the hash function.

   // if the current bucket is sufficient, then do nothing.
   if (numBuckets <= bucket_count())
      return;

   // create new hash bucket.
   custom::vector<custom::list<T, A>> bucketsNew(numBuckets);

   // insert the elements into the new hash table, one at a time.
   for (auto it = begin(); it != end(); ++it)
   {
      assert(it != end());
      bucketsNew[hash(*it) % numBuckets].push_back(std::move(*it));
	}

   // Swap the old bucket for the new.
   swap(buckets, bucketsNew);
}


/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator unordered_set<T, H, E, A>::find(const T& t)
{
   // Identify the bucket corresponding to the element
   size_t iBucket = bucket(t);

   // Need a vector iterator.
   typename vector<list<T>>::iterator iVector(iBucket, buckets);

   // Get a list iterator to the element using the lists find() method
   auto itList = buckets[iBucket].find(t);

   if (itList != buckets[iBucket].end())
      return iterator(buckets.end(), iVector , itList);

   else
      return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator & unordered_set<T, H, E, A>::iterator::operator ++ ()
{
   // 1. Only advance if we are not already at the end.
   if (itVector == itVectorEnd)
      return *this;

   // 2. Advance the list iterator. If we are not at the end, then we are done.
   assert(itVector != itVectorEnd);
   ++itList;
   if (itList != (*itVector).end())
      return *this;

   // 3. We are at the ed of the list. Find next bucket.
   ++itVector;
   while (itVector != itVectorEnd && (*itVector).empty())
      ++itVector;

   if (itVector != itVectorEnd)
      itList = (*itVector).begin();

   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T, typename H, typename E, typename A>
void swap(unordered_set<T,H,E,A>& lhs, unordered_set<T,H,E,A>& rhs)
{
   lhs.swap(rhs);
}

}
