/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Peter Benson, Jarom Diaz, Isaac Radford
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

   /*****************************************
    * VECTOR
    * Just like the std :: vector <T> class
    ****************************************/
   template <typename T, typename A = std::allocator<T>>
   class vector
   {
      friend class ::TestVector; // give unit tests access to the privates
      friend class ::TestStack;
      friend class ::TestPQueue;
      friend class ::TestHash;
   public:

      //
      // Construct
      //
      vector(const A& a = A());
      vector(size_t numElements, const A& a = A());
      vector(size_t numElements, const T& t, const A& a = A());
      vector(const std::initializer_list<T>& l, const A& a = A());
      vector(const vector& rhs);
      vector(vector&& rhs);
      ~vector();

      //
      // Assign
      //
      void swap(vector& rhs)
      {
         std::swap(data, rhs.data);
         std::swap(numElements, rhs.numElements);
         std::swap(numCapacity, rhs.numCapacity);
      }
      vector& operator = (const vector& rhs);
      vector& operator = (vector&& rhs);

      //
      // Iterator
      //
      class iterator;
      iterator begin()
      {
         return iterator(data);
      }
      iterator end()
      {
         return iterator(data + numElements);
      }

      //
      // Access
      //
      T& operator [] (size_t index);
      const T& operator [] (size_t index) const;
      T& front();
      const T& front() const;
      T& back();
      const T& back() const;

      //
      // Insert
      //
      void push_back(const T& t);
      void push_back(T&& t);
      void reserve(size_t newCapacity);
      void resize(size_t newElements);
      void resize(size_t newElements, const T& t);

      //
      // Remove
      //
      void clear()
      {
         for (size_t i = 0; i < numElements; ++i)
            alloc.destroy(&data[i]);

         numElements = 0;
      }
      void pop_back()
      {
         if (numElements)
            alloc.destroy(&data[--numElements]);
      }
      void shrink_to_fit();

      //
      // Status
      //
      size_t  size()          const { return numElements; }
      size_t  capacity()      const { return numCapacity; }
      bool empty()            const { return numElements == 0; }

   private:

      A    alloc;                // use allocator for memory allocation
      T* data;                 // user data, a dynamically-allocated array
      size_t  numCapacity;       // the capacity of the array
      size_t  numElements;       // the number of items currently used
   };

   /**************************************************
    * VECTOR ITERATOR
    * An iterator through vector.  You only need to
    * support the following:
    *   1. Constructors (default and copy)
    *   2. Not equals operator
    *   3. Increment (prefix and postfix)
    *   4. Dereference
    * This particular iterator is a bi-directional meaning
    * that ++ and -- both work.  Not all iterators are that way.
    *************************************************/
   template <typename T, typename A>
   class vector <T, A> ::iterator
   {
      friend class ::TestVector; // give unit tests access to the privates
      friend class ::TestStack;
      friend class ::TestPQueue;
      friend class ::TestHash;
   public:
      // constructors, destructors, and assignment operator
      iterator() { this->p = nullptr; }
      iterator(T* p) { this->p = p; }
      iterator(const iterator& rhs) { *this = rhs; }
      iterator(size_t index, vector<T>& v) { this->p = v.data + index; }
      iterator& operator = (const iterator& rhs)
      {
        /* p = std::move(rhs.p)*/
         this->p = rhs.p;
         return *this;
      }

      // Legacy Random Access Iterator
      iterator operator+(size_t n) const {
         return iterator(p + n);
      }

      // equals, not equals operator
      bool operator != (const iterator& rhs) const { return p != rhs.p; }
      bool operator == (const iterator& rhs) const { return p == rhs.p; }

      // dereference operator
      T& operator * ()
      {
         return *p;
      }

      // prefix increment
      iterator& operator ++ ()
      {
         ++p;
         return *this;
      }

      // postfix increment
      iterator operator ++ (int postfix)
      {
         iterator returnCopy(*this);
         ++p;
         return returnCopy;
      }

      // prefix decrement
      iterator& operator -- ()
      {
         --p;
         return *this;
      }

      // postfix decrement
      iterator operator -- (int postfix)
      {
         iterator returnCopy(this);
         --p;
         return returnCopy;
      }

   private:
      T* p;
   };


   /*****************************************
    * VECTOR :: NON-DEFAULT constructors
    * non-default constructor: set the number of elements,
    * construct each element, and copy the values over
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const A& a) : alloc(a)
   {
      data = alloc.allocate(0);
      numElements = 0;
      numCapacity = 0;
   }


   /*****************************************
    * VECTOR :: NON-DEFAULT constructors
    * non-default constructor: set the number of elements,
    * construct each element, and copy the values over
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(size_t num, const T& t, const A& a) : alloc(a)
   {
      // Use the allocator to allocate memory for 'num' elements
      data = alloc.allocate(num);

      // Initialize 'numElements' to the given 'num'
      numElements = num;

      // Set 'numCapacity' to match 'num' since we have exactly 'num' elements allocated
      numCapacity = num;

      // Initialize the elements with the provided value 't'
      std::uninitialized_fill_n(data, num, t);
      /*for (size_t i = 0; i < num; ++i)
         alloc.construct(data + i, t);*/
   }

   /*****************************************
    * VECTOR :: INITIALIZATION LIST constructors
    * Create a vector with an initialization list.
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const std::initializer_list<T>& l, const A& a) : alloc(a)
   {
      data = alloc.allocate(l.size());
      numElements = l.size();
      numCapacity = l.size();

      // Initialize the elements using values from the initializer list.

      for (size_t i = 0; i < numElements; ++i)
         alloc.construct(data + i, *(l.begin() + i));
   }

   /*****************************************
    * VECTOR :: NON-DEFAULT constructors
    * non-default constructor: set the number of elements,
    * construct each element, and copy the values over
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(size_t num, const A& a) : alloc(a)
   {
      data = alloc.allocate(num);
      numElements = num;
      numCapacity = num;

      for (size_t i = 0; i < num; ++i)
         //alloc.construct(data + i);
         new (&data[i]) T;
   }

   /*****************************************
    * VECTOR :: COPY CONSTRUCTOR
    * Allocate the space for numElements and
    * call the copy constructor on each element
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const vector& rhs) : numCapacity(rhs.numElements), numElements(rhs.numElements), alloc(rhs.alloc)
   {
      data = alloc.allocate(numElements);

      for (size_t i = 0; i < numElements; ++i)
         alloc.construct(&data[i], rhs.data[i]);
   }

   /*****************************************
    * VECTOR :: MOVE CONSTRUCTOR
    * Steal the values from the RHS and set it to zero.
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(vector&& rhs)
   {
      /*std::swap(data, rhs.data);
      std::swap(numCapacity, rhs.numCapacity);
      std::swap(numElements, rhs.numElements);*/
      data = rhs.data;
      rhs.data = nullptr;

      numElements = rhs.numElements;
      rhs.numElements = 0;

      numCapacity = rhs.numCapacity;
      rhs.numCapacity = 0;
   }

   /*****************************************
    * VECTOR :: DESTRUCTOR
    * Call the destructor for each element from 0..numElements
    * and then free the memory
    ****************************************/
   template <typename T, typename A>
   vector <T, A> :: ~vector()
   {
      for (size_t i = 0; i < numElements; ++i)
         alloc.destroy(&data[i]);

      alloc.deallocate(data, numCapacity);

      //data = nullptr;
      //numElements = 0;
      //numCapacity = 0;
   }

   /***************************************
    * VECTOR :: RESIZE
    * This method will adjust the size to newElements.
    * This will either grow or shrink newElements.
    *     INPUT  : newCapacity the size of the new buffer
    *     OUTPUT :
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::resize(size_t newElements)
   {
      if (newElements > numElements)
      {
         reserve(newElements);
         for (size_t i = numElements; i < newElements; ++i)
            alloc.construct(&data[i]);

         numElements = newElements;
      }

      else if (newElements < numElements)
      {
         //destroy extra elements
         for (size_t i = newElements; i < numElements; ++i)
            alloc.destroy(&data[i]);

         numElements = newElements;
      }
   }

   template <typename T, typename A>
   void vector <T, A> ::resize(size_t newElements, const T& t)
   {
      if (newElements > numElements)
      {
         reserve(newElements);
         for (size_t i = numElements; i < newElements; ++i)
            alloc.construct(&data[i], t);

         numElements = newElements;
      }

      else if (newElements < numElements)
      {
         // destroy extra elements
         for (size_t i = newElements; i < numElements; ++i)
            alloc.destroy(&data[i]);

         numElements = newElements;
      }
   }

   /***************************************
    * VECTOR :: RESERVE
    * This method will grow the current buffer
    * to newCapacity.  It will also copy all
    * the data from the old buffer into the new
    *     INPUT  : newCapacity the size of the new buffer
    *     OUTPUT :
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::reserve(size_t newCapacity)
   {
      if (newCapacity > numCapacity)
      {
         T* newBuffer = alloc.allocate(newCapacity);
         for (std::size_t i = 0; i < numElements; ++i)
         {
            new(newBuffer + i) T(std::move(data[i]));
            data[i].~T();
         }

         alloc.deallocate(data, numCapacity);
         data = newBuffer;
         numCapacity = newCapacity;
      }
   }

   /***************************************
    * VECTOR :: SHRINK TO FIT
    * Get rid of any extra capacity
    *     INPUT  :
    *     OUTPUT :
    **************************************/
   template <typename T, typename A>
   void vector<T, A>::shrink_to_fit()
   {
      if (numCapacity == numElements)
         return;

      // allocate the new array.
      T* pNew;
      if (numElements != 0)
      {
         pNew = alloc.allocate(numElements);
         std::uninitialized_copy(data, data + numElements, pNew);
      }

      else
         pNew = nullptr;

      // Delete the old and assign the new.
      if (nullptr != data)
      {
         // call the destructor on elements being destroyed.
         for (size_t i = 0; i < numElements; i++)
            alloc.destroy(&data[i]);
         // actually delete the memory.
         alloc.deallocate(data, numCapacity);
      }

      data = pNew;
      numCapacity = numElements;
      /*if (numElements < numCapacity)
      {
         T* dataNew = nullptr;

         if (numElements > 0)
         {
            dataNew = alloc.allocate(numElements);
            for (size_t i = 0; i < numElements; ++i)
               alloc.construct(dataNew + i, data[i]);
         }

         for (size_t i = 0; i < numElements; ++i)
            alloc.destroy(data + i);

         if (data != nullptr)
            alloc.deallocate(data, numCapacity);

         data = dataNew;
         numCapacity = numElements;
      }*/
   }

   /*****************************************
    * VECTOR :: SUBSCRIPT
    * Read-Write access
    ****************************************/
   template <typename T, typename A>
   T& vector <T, A> :: operator [] (size_t index)
   {
      return data[index];
   }

   /******************************************
    * VECTOR :: SUBSCRIPT
    * Read-Write access
    *****************************************/
   template <typename T, typename A>
   const T& vector <T, A> :: operator [] (size_t index) const
   {
      return data[index];
   }

   /*****************************************
    * VECTOR :: FRONT
    * Read-Write access
    ****************************************/
   template <typename T, typename A>
   T& vector <T, A> ::front()
   {
      return data[0];
   }

   /******************************************
    * VECTOR :: FRONT
    * Read-Write access
    *****************************************/
   template <typename T, typename A>
   const T& vector <T, A> ::front() const
   {
      return data[0];
   }

   /*****************************************
    * VECTOR :: FRONT
    * Read-Write access
    ****************************************/
   template <typename T, typename A>
   T& vector <T, A> ::back()
   {
      return data[numElements - 1];
   }

   /******************************************
    * VECTOR :: FRONT
    * Read-Write access
    *****************************************/
   template <typename T, typename A>
   const T& vector <T, A> ::back() const
   {
      return data[numElements - 1];
   }

   /***************************************
    * VECTOR :: PUSH BACK
    * This method will add the element 't' to the
    * end of the current buffer.  It will also grow
    * the buffer as needed to accomodate the new element
    *     INPUT  : 't' the new element to be added
    *     OUTPUT : *this
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::push_back(const T& t)
   {
      if (numElements == numCapacity)
         reserve(numCapacity == 0 ? 1 : numCapacity * 2);

      new(data + numElements++) T(t);
      //++numElements;
   }

   template <typename T, typename A>
   void vector <T, A> ::push_back(T&& t)
   {
      if (numElements == numCapacity)
         reserve(numCapacity == 0 ? 1 : numCapacity * 2);

      new(data + numElements++) T(std::move(t));
     // ++numElements;
   }

   /***************************************
    * VECTOR :: ASSIGNMENT
    * This operator will copy the contents of the
    * rhs onto *this, growing the buffer as needed
    *     INPUT  : rhs the vector to copy from
    *     OUTPUT : *this
    **************************************/
   template <typename T, typename A>
   vector <T, A>& vector <T, A> :: operator = (const vector& rhs)
   {
      if (rhs.numElements == numElements)
      {
         for (size_t i = 0; i < rhs.numElements; i++)
            data[i] = rhs.data[i];
      }

      else if (rhs.numElements > numElements)
      {
         if (rhs.numElements <= numCapacity)
         {
            // copy into the already filled slots.
            for (size_t i = 0; i < numElements; i++)
               data[i] = rhs.data[i];

            // copy construct the rest.
            for (size_t i = numElements; i < rhs.numElements; i++)
               alloc.construct(&data[i], rhs.data[i]);
            numElements = rhs.numElements;
         }

         else
         {
            T* pNew = alloc.allocate(rhs.numElements);

            for (size_t i = 0; i < rhs.numElements; i++)
               alloc.construct(&pNew[i], rhs.data[i]);

            clear();

            alloc.deallocate(data, numCapacity);

            data = pNew;
            numElements = rhs.numElements;
            numCapacity = rhs.numCapacity;
         }

      
      }
      else
      {
         // copy over the elements.
         for (size_t i = 0; i < rhs.numElements; i++)
            data[i] = rhs.data[i];

         for (size_t i = rhs.numElements; i < numElements; i++)
            alloc.destroy(&data[i]);

         numElements = rhs.numElements;
      }

      /*if (rhs.size() == size())
         for (size_t i = 0; i < size(); ++i)
         {
            data[i] = rhs.data[i];
         }

      else if (rhs.size() > size())
      {
         if (rhs.size() <= capacity())
         {
            for (size_t i = 0; i < size(); ++i)
               data[i] = rhs.data[i];

            for (size_t i = numElements; i < rhs.size(); ++i)
               alloc.construct(data + i, rhs.data[i]);
         }

         else
         {
            T* dataNew = alloc.allocate(rhs.size());

            for (size_t i = 0; i < rhs.size(); ++i)
               alloc.construct(dataNew + i, rhs.data[i]);

            clear();
            alloc.deallocate(data, numCapacity);
            data = dataNew;
            numCapacity = rhs.size();
         }
      }

      else
      {
         for (size_t i = 0; i < rhs.size(); ++i)
            data[i] = rhs.data[i];

         for (size_t i = rhs.size(); i < size(); ++i)
            alloc.destroy(data + i);
      }

      numElements = rhs.size();*/

      return *this;
   }
   template <typename T, typename A>
   vector <T, A>& vector <T, A> :: operator = (vector&& rhs)
   {
      clear();
      shrink_to_fit();
      swap(rhs);

      //rhs.data = nullptr;
      //rhs.numCapacity = 0;

      return *this;
   }

} // namespace custom

