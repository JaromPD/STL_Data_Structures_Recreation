/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    Peter Benson, Jarom Diaz, Isaac Radford
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h" // for default underlying container

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
template<class T, class Container = custom::vector<T>, class Compare = std::less<T>>
class priority_queue
{
   friend class ::TestPQueue; // give the unit test class access to the privates
   template <class TT, class CContainer, class CCompare>
   friend void swap(priority_queue<TT, CContainer, CCompare>& lhs, priority_queue<TT, CContainer, CCompare>& rhs);

public:

   //
   // construct
   //
   priority_queue(const Compare & c = Compare()) : container(), compare(c) {}
   priority_queue(const priority_queue &  rhs, const Compare & c = Compare()) : container(rhs.container), compare(c) {}
   priority_queue(priority_queue && rhs, const Compare & c = Compare())
        : container(std::move(rhs.container)), compare(c) {}
   
   template <class Iterator>
   priority_queue(Iterator first, Iterator last, const Compare& c = Compare()) : compare(c)
   {
       // Allocate space to hold all the elements
       container.reserve(std::distance(first, last));
       
       // Push the elements to newly allocated space
       while (first != last)
       {
           push(*first);
           ++first;
       }
   }
  
   explicit priority_queue (const Compare& c, Container && rhs) : container(rhs), compare(c) {} // These don't work yet.
   explicit priority_queue(const Compare& c, Container& rhs) : compare(c)
   { 
      // reserve space in the container.
      container.reserve(rhs.size());

      // copy and push the values on.
      for (auto it = rhs.begin(); it != rhs.end(); ++it)
         push(*it);
   }
  ~priority_queue()
   {
      while (!empty())
         pop();
   }

   //
   // Access
   //
   const T & top() const;

   //
   // Insert
   //
   void  push(const T& t);
   void  push(T&& t);     

   //
   // Remove
   //
   void  pop(); 

   //
   // Status
   //
   size_t size()  const 
   { 
      return container.size();
   }
   bool empty() const 
   { 
      return size() == size_t(0);  
   }
   
private:

   bool percolateDown(size_t indexHeap);


   Container container;       // underlying container (probably a vector)
   Compare   compare;         // comparison operator

};

/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T, class Container, class Compare>
const T & priority_queue <T, Container, Compare> :: top() const
{
   if (!container.empty())
      return container.front();
   else
      throw std::out_of_range("std:out_of_range");
}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/

template <class T, class Container, class Compare>
void priority_queue <T, Container, Compare> :: pop()
{
   using std::swap;

   // If it's not empty, swap it down and then remove it
   if (!empty())
   {
      swap(container[0], container[size() -1]);
      container.pop_back();
      percolateDown(1);
   }
}

/**********************************************
 * P QUEUE :: CLEAR
 * Delete all items from the heap.
 **********************************************/
template <class T>
void clear(priority_queue<T>& p)
{
   while(!p.empty())
      p.pop();
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T, class Container, class Compare>
void priority_queue <T, Container, Compare> :: push(const T & t)
{
   // Add in new item, find the index of the parent
   container.push_back(t);
   auto index = container.size() / 2;

   // Keep percolating until everythings in the right spot
   while (index && percolateDown(index))
      index /= 2;
}

template <class T, class Container, class Compare>
void priority_queue <T, Container, Compare> :: push(T && t)
{
   // Add in new item, find the index of the parent
   container.push_back(std::move(t));
   auto index = container.size() / 2;

   // Keep percolating until everythings in the right spot
   while (index && percolateDown(index))
      index /= 2;
}

/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/
template <class T, class Container, class Compare>
bool priority_queue <T, Container, Compare> :: percolateDown(size_t indexHeap)
{
   // Set swap so it can be overloaded.
   using std::swap;
    
   // The left child will always be 2X larger than current index, the right index will just be + 1
   size_t indexLeft = indexHeap * 2;
   size_t indexRight = indexLeft + 1;
   size_t indexBigger = 1;

   // Find the larger child and save it
   if (indexRight <= size() && compare(container[indexLeft - 1], container[indexRight - 1]))
      indexBigger = indexRight;
   else
      indexBigger = indexLeft;

   // Set the correct index to be the new parent node
   if (indexBigger <= size() && compare(container[indexHeap - 1], container[indexBigger - 1]))
   {
      swap(container[indexHeap - 1], container[indexBigger - 1]);
      percolateDown(indexBigger); 
      return true;
   }
   return false;

}

/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/
template <class T, class Container, class Compare>
inline void swap(custom::priority_queue <T, Container, Compare> & lhs,
                 custom::priority_queue <T, Container, Compare> & rhs)
{
   lhs.container.swap(rhs.container);
   std::swap(lhs.compare, rhs.compare);
}

}; 
