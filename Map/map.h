/***********************************************************************
 * Header:
 *    MAP
 * Summary:
 *    Our custom implementation of a std::map 
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    Peter Benson, Jarom Diaz, Isaac Radford
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap;

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend class ::TestMap;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs); 
public:
   using Pairs = custom::pair<K, V>;

   // 
   // Construct
   //
   map()
   {
   }
   map(const map &  rhs) : bst(rhs.bst)
   { 
   }
   map(map && rhs) : bst(std::move(rhs.bst))
   { 
   }
   template <class Iterator>
   map(Iterator first, Iterator last) 
   {
      insert(first, last);
   }
   map(const std::initializer_list <Pairs>& il) 
   {
      insert(il); // using built in init insert to not duplicate code.
   }
  ~map()         
   {
     clear();
   }

   //
   // Assign
   //
   map & operator = (const map & rhs) 
   {
      bst = rhs.bst;
      return *this;
      
   }
   map & operator = (map && rhs)
   {
      clear();
      swap(*this, rhs);
      return *this;
   }
   map & operator = (const std::initializer_list <Pairs> & il)
   {
      clear();
      insert(il); //using built in init insert to not duplicate code.
      return *this;
   }
   
   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(bst.begin());
   }
   iterator end() 
   { 
      return iterator(bst.end());    
   }

   // 
   // Access
   //
   const V & operator [] (const K & k) const;
         V & operator [] (const K & k);
   const V & at (const K& k) const;
         V & at (const K& k);
   iterator find(const K & k)
   {
      return iterator(bst.find(k));
   }

   //
   // Insert
   //
   custom::pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
      auto BSTPair = bst.insert(move(rhs));
      bool wasNewNodeCreated = BSTPair.second;
      return make_pair(iterator(BSTPair.first), wasNewNodeCreated);
   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
      auto BSTPair = bst.insert(rhs);
      bool wasNewNodeCreated = BSTPair.second;
      return make_pair(iterator(BSTPair.first), wasNewNodeCreated);
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
      for (auto it = first; it != last; it++)
         bst.insert(*it, true);
   }
   void insert(const std::initializer_list <Pairs>& il)
   {
      for (auto&& element : il)
         bst.insert(element);
   }

   //
   // Remove
   //
   void clear() noexcept
   {
      bst.clear();
   }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept 
   { 
      return bst.empty();
   }
   size_t size() const noexcept 
   { 
      return bst.size();
   }


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map <K, V> :: iterator
{
   friend class ::TestMap;
   template <class KK, class VV>
   friend class custom::map; 
public:
   //
   // Construct
   //
   iterator()
   {
      it = nullptr;
   }
   iterator(const typename BST < pair <K, V> > :: iterator & rhs)
   { 
      it = rhs;
   }
   iterator(const iterator & rhs) 
   { 
      it = rhs.it;
   }

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
      it = rhs.it;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const 
   { 
      return it == rhs.it;
   }
   bool operator != (const iterator & rhs) const 
   { 
      return it != rhs.it;
   }

   // 
   // Access
   //
   const pair <K, V> & operator * () const
   {
      return *it;
   }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
      iterator temp(*this);
      ++it;
      return temp;
   }
   iterator operator ++ (int postfix)
   {
      iterator temp(*this);
      ++it;
      return temp;
   }
   iterator & operator -- ()
   {
      iterator temp(*this);
      --it;
      return temp;
   }
   iterator  operator -- (int postfix)
   {
      iterator temp(*this);
      --it;
      return temp;
   }

private:

   // Member variable
   typename BST < pair <K, V >>  :: iterator it;   
};


/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> :: operator [] (const K& key)
{
   // create a pair, then insert if not in bst.
   Pairs pair(key);
   auto returnValue = bst.insert(pair, true); //keepUnique = true.
   auto itBST = returnValue.first;

   // return the value of the pair.
   return itBST.pNode->data.second;
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> :: operator [] (const K& key) const
{
   // create a pair, then find it.
   Pairs pair(key);
   auto itBST = bst.find(pair);

   // If a pair is found, return it.
   if (itBST == bst.end())
      return itBST.pNode->data.second;

   // else, return the default value.
   return pair.second;
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> ::at(const K& key)
{
   // create a pair, then find it in bst.
   Pairs pair(key);
   auto it = bst.find(pair);

   // checking if the pair exists, if so the return it.
   if (it != nullptr)
      return it.pNode->data.second;

   // throw an exception if not found in bst.
   else
      throw std::out_of_range("invalid map<K, T> key");
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> ::at(const K& key) const
{
   // create a pair, then find it in bst.
   Pairs pair(key);
   auto it = bst.find(pair);

   // checking if the pair exists, if so then return it.
   if (it != nullptr)
      return it.pNode->data.second;

   // throw an exception if not found in bst.
   else
      throw std::out_of_range("invalid map<K, T> key");
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
   lhs.bst.swap(rhs.bst);
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
   // create a pair, then find the pair in the bst.
   Pairs pair(k);
   auto it = bst.find(pair);

   // if the pair was not found, return 0 deletions.
   if (it == bst.end())
      return 0;

   // if the pair is found, erase it then return num of deletions.
   bst.erase(it);
   return 1;
}

/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   while (first != last)
      first = erase(first);
   return last;
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
   return map<K, V>::iterator(bst.erase(it.it));
}

}; //  namespace custom

