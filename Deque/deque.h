/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque 
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    Peter Benson, Isaac Radford, Jarom Diaz
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <memory>   // for std::allocator

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *****************************************************/
template <typename T, typename A = std::allocator<T>>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque(const A& a = A()) : data(nullptr), numCells(16), numBlocks(0),
         numElements(0), iaFront(0)
   {
   }
   deque(deque & rhs);
   ~deque()
   {
   }

   //
   // Assign
   //
   deque & operator = (deque& rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin()
   {
      return iterator(0, this);
   }
    
   iterator end()
   {
      return iterator(numElements, this);
   }

   //
   // Access
   //
   T & front()
   {
      assert(numElements != 0);
      assert(data[ibFromID(0)] != nullptr);
      return data[ibFromID(0)][icFromID(0)];
   }
   const T & front() const
   {
      assert(numElements != 0);
      assert(data[ibFromID(0)] != nullptr);
      return data[ibFromID(0)][icFromID(0)];
   }
   T & back()
   {
      assert(numElements != 0);
      assert(data[ibFromID(numElements - 1)] != nullptr);
      return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
   }
   const T & back() const
   {
      assert(numElements != 0);
      assert(data[ibFromID(numElements - 1)] != nullptr);
      return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
   }
    
   T & operator[](int id)
   {
      assert(data != nullptr);
      assert(data[ibFromID(id)] != nullptr);
      return (data[ibFromID(id)][icFromID(id)]);
   }
    
   const T & operator[](int id) const
   {
      assert(data[ibFromID(id)] != nullptr);
      return (data[ibFromID(id)][icFromID(id)]);
   }

   // AllBlocks Filled
   bool isAllBlocksFilled() const;

   //
   // Insert
   //
   void push_back(const T & t);
   void push_back(T && t);
   void push_front(const T& t);
   void push_front(T&& t);

   //
   // Remove
   //
   void pop_front();
   void pop_back();
   void clear();

   //
   // Status
   //
   size_t size()  const { return numElements; }
   bool   empty() const { return numElements == 0; }
  
   
private:
   // array index from deque index
   int iaFromID(int id) const
   {
      // id should be within the bounds of 0 to numElements.
      assert(0 <= id);
     
      assert(0 <= iaFront && iaFront < (numCells * numBlocks));
      int ia = (id + iaFront) % (numCells * numBlocks);

      assert(0 <= ia && ia < (numCells * numBlocks));
      return ia;
   }

   // block index from deque index
   int ibFromID(int id) const
   {
      int ib = iaFromID(id) / numCells;
      assert(0 <= ib && ib < numBlocks);
      return ib;
   }

   // cell index from deque index
   int icFromID(int id) const
   {
      int ic = iaFromID(id) % numCells;
      assert(0 <= ic && ic < numCells);
      return ic;
   }

   // reallocate
   void reallocate(int numBlocksNew);

   A    alloc;                // use alloacator for memory allocation
   size_t numCells;           // number of cells in a block
   size_t numBlocks;          // number of blocks in the data array
   size_t numElements;        // number of elements in the deque
   int iaFront;               // array-centered index of the front of the deque
   T ** data;                 // array of arrays
};

/**************************************************
 * DEQUE ITERATOR
 * An iterator through deque.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T, typename A>
class deque <T, A> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   // 
   // Construct
   //
   iterator() : id(0), d(nullptr) {}
   iterator(int id, deque* d) : id(id), d(d) {}
   iterator(const iterator& rhs) : id(rhs.id), d(rhs.d) {}

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      id = rhs.id;
      d = rhs.d;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const iterator& rhs) const { return !(*this == rhs); }
   bool operator == (const iterator& rhs) const { return (d == rhs.d) && (id == rhs.id); }

   // 
   // Access
   //
   T& operator * ()
   {
       return (*d)[id];
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
       return id - it.id;
   }
    
   iterator& operator += (int offset)
   {
      id += offset;
      return *this;
   }
   iterator& operator ++ ()
   {
      ++id;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      ++id;
      return temp;
   }
    
   iterator& operator -- ()
   {
      --id;
      return *this;
   }
   iterator operator -- (int postfix)
   {
      iterator temp = *this;
       --id;
      return *this;
   }

private:
   int id;
   deque* d;
};

/*****************************************
 * DEQUE :: COPY CONSTRUCTOR
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> ::deque(deque& rhs) : data(nullptr), numCells(16), numBlocks(0),
numElements(0), iaFront(0)
{
   *this = rhs;
}

/*****************************************
 * DEQUE :: COPY-ASSIGN
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> & deque <T, A> :: operator = (deque & rhs)
{
   auto itLHS = begin();
   auto itRHS = rhs.begin();

   // Copy elements into existing slots in the deque.
   while (itLHS != end() && itRHS != rhs.end())
   {
      *itLHS = *itRHS;
      ++itLHS;
      ++itRHS;
   }
   
   // Remove excess slots.
   while (itLHS != end())
      pop_back();

   // Insert new elements as needed.
   while (itRHS != rhs.end())
   {
      push_back(*itRHS);
      ++itRHS;
   }

   return *this;
}

/*****************************************
 * DEQUE :: IS ALL BLOCKS FILLED?
 * return TRUE if all the blocks are filled
 ****************************************/
template <typename T, typename A>
bool deque <T, A> ::isAllBlocksFilled() const
{
   // We have no choice but to check each block looking for a NULLPTR
   for (size_t ib = 0; ib < numBlocks; ib++)

      if (nullptr == data[ib])
         return false;

   return true;
}

/*****************************************
 * DEQUE :: PUSH_BACK
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(const T& t)
{
   // 1. Reallocate the array of blocks as needed.
   size_t icTail = ((numElements == 0)) ? numCells - 1 : icFromID(numElements - 1);
   if (isAllBlocksFilled() && icTail == numCells - 1)
      reallocate((numBlocks == 0) ? 1 : numBlocks * 2);

   // 2. Allocate a new block as needed.
   size_t ib = ibFromID(numElements);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);
   
   // 3. Assign the value into the block.
   alloc.construct(&data[ib][icFromID(numElements)], t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_BACK - move
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(T && t)
{
   // 1. Reallocate the array of blocks as needed.
   size_t icTail = ((numElements == 0)) ? numCells - 1 : icFromID(numElements - 1);
   if (isAllBlocksFilled() && icTail == numCells - 1)
      reallocate((numBlocks == 0) ? 1 : numBlocks * 2);

   // 2. Allocate a new block as needed.
   size_t ib = ibFromID(numElements);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);

   // 3. Assign the value into the block.
   alloc.construct(&data[ib][icFromID(numElements)], t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(const T& t)
{
   // 1. Reallocate the array of blocks needed.
   if (isAllBlocksFilled())
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);

   // 2. Adjust the front array index, wrapping as needed.
   if (iaFront != 0)
      iaFront--;
   else
      iaFront = (numBlocks * numCells) - 1;

   // 3. Allocate a new block as needed.
   auto ib = ibFromID(0);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);

   // 4. Assign the value into the block.
   alloc.construct(&data[ib][icFromID(0)], t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT - move
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(T&& t)
{
   // 1. Reallocate the array of blocks needed.
   if (isAllBlocksFilled())
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);

   // 2. Adjust the front array index, wrapping as needed.
   if (iaFront != 0)
      iaFront--;
   else
      iaFront = (numBlocks * numCells) - 1;

   // 3. Allocate a new block as needed.
   auto ib = ibFromID(0);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);

   // 4. Assign the value into the block.
   alloc.construct(&data[ib][icFromID(0)], t);
   numElements++;
}

/*****************************************
 * DEQUE :: CLEAR
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::clear()
{
    // Delete the elements
    for (int id = 0; id < numElements; ++id) 
    {
        int ib = ibFromID(id);
        int ic = icFromID(id);
        alloc.destroy(&data[ib][ic]);
    }
    
    // Delete the blocks themselves
    for (int ib = 0; ib < numBlocks; ++ib) 
    {
        if (data[ib] != nullptr) 
        {
            alloc.deallocate(data[ib], numCells);
            data[ib] = nullptr;
        }
    }
    
    numElements = 0;
}

/*****************************************
 * DEQUE :: POP FRONT
 * Remove the front element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: pop_front()
{
   int idRemove = 0;

   // Call the destructor on the back element.
   alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);

   // Delete the block as needed.
   if (numElements == 1 || (icFromID(idRemove) == numElements - 1 && 
      ibFromID(idRemove) != ibFromID(numElements - 1))) 
   {
      alloc.deallocate(data[ibFromID(idRemove)], numCells);
      data[ibFromID(idRemove)] = nullptr;
   }
    
   numElements--;
   iaFront = (iaFront + 1) % (numCells * numBlocks);
}

/*****************************************
 * DEQUE :: POP BACK
 * Remove the back element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::pop_back()
{
   int idRemove = numElements - 1;
    
   // Call the destructor on the back element
   alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);
    
   // Delete the block as needed
   if (numElements == 1 || ((icFromID(idRemove) == 0) && 
      (icFromID(idRemove) != ibFromID(0)))) 
   {
      alloc.deallocate(data[ibFromID(idRemove)], numCells);
      data[ibFromID(idRemove)] = nullptr;
   }
    
   numElements--;
}

/*****************************************
 * DEQUE :: REALLOCATE
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: reallocate(int numBlocksNew)
{
   // 1. Allocate a new array of pointers that is the requested size.
   T** dataNew = new T * [numBlocksNew];

   // 2. Copy over the pointers, unwrapping as we go.
   int ibNew = 0;
   for (int ibOld = 0; ibOld < numElements; ibOld += numCells)
   {
      dataNew[ibNew] = data[ibFromID(ibOld)];
      ibNew++;
   }

   // 3. Set all the block pointers to null when there are no blocks to point to.
   while (ibNew < numBlocksNew)
   {
      dataNew[ibNew] = nullptr;
      ibNew++;
   }

   // 4. If the back element is in the front element's block, then move it.
   if (numElements > 0 &&
      ibFromID(0) == ibFromID(numElements - 1) &&
      icFromID(0) > icFromID(numElements - 1))
   {
      auto ibFrontOld = ibFromID(0);
      auto ibBackOld = ibFromID(numElements - 1);
      auto ibBackNew = numElements / numCells;
      dataNew[ibBackNew] = new T[numCells];
      for (size_t ic = 0; ic < icFromID(numElements - 1); ic++)
         dataNew[ibBackNew][ic] = std::move(data[ibBackOld][ic]);

   }

  // 5. Change the deques member variables with the new ones.
  if (data)
      delete[] data;

   data = dataNew;
   numBlocks = numBlocksNew;
   iaFront = iaFront % numCells;
}


} // namespace custom
