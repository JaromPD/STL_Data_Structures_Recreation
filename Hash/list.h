/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Jarom Diaz, Peter Benson, Isaac Radford
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList; // forward declaration for unit tests
class TestHash; // forward declaration for hash used later

namespace custom
{

   /**************************************************
    * LIST
    * Just like std::list
    **************************************************/
   template <typename T, typename A = std::allocator<T>>
   class list
   {
      friend class ::TestList; // give unit tests access to the privates
      friend class ::TestHash;
      friend void swap(list& lhs, list& rhs);
   public:

      //
      // Construct
      //

      list(const A& a = A()) : alloc(a), numElements(0),
         pHead(nullptr), pTail(nullptr) {}

      list(list <T, A>& rhs, const A& a = A()) : pHead(nullptr), pTail(nullptr), numElements(0), alloc(a)
      {
         *this = rhs;
         /*for (auto it = rhs.begin(); it != rhs.end(); ++it)
         {
            push_back(*it);
         }*/
      }

      list(list <T, A>&& rhs, const A& a = A());
      list(size_t num, const T& t, const A& a = A());
      list(size_t num, const A& a = A());
      list(const std::initializer_list<T>& il, const A& a = A())
      {
         for (auto it = il.begin(); it != il.end(); ++it)
         {
            push_back(*it);
         }
      }
      template <class Iterator>
      list(Iterator first, Iterator last, const A& a = A())
      {
         for (auto it = first; it != last; ++it)
         {
            push_back(*it);
         }
      }

      ~list()
      {
         /*while (pHead != nullptr)
         {
            Node* temp = pHead;
            pHead = temp->pNext;
            delete temp;
         }

         pHead = pTail = nullptr;
         numElements = 0;*/
         clear();
      }

      //
      // Assign
      //

      list <T, A>& operator = (list <T, A>& rhs);
      list <T, A>& operator = (list <T, A>&& rhs);
      list <T, A>& operator = (const std::initializer_list<T>& il);
      void swap(list <T, A>& rhs)
      {
         auto tempHead = rhs.pHead;
         rhs.pHead = pHead;
         pHead = tempHead;

         auto tempTail = rhs.pTail;
         rhs.pTail = pTail;
         pTail = tempTail;

         auto tempElements = rhs.numElements;
         rhs.numElements = numElements;
         numElements = tempElements;
      }

      //
      // Iterator
      //

      class iterator;
      iterator begin() { return iterator(pHead); }
      iterator rbegin() { return iterator(pTail); }
      iterator end() { return iterator(nullptr); }
      
      iterator find(const T& t)
      {
         for (auto it = begin(); it != end(); ++it) {
            if (*it == t)
               return it;
         }
         return end();
      }

      //
      // Access
      //

      T& front();
      T& back();

      //
      // Insert
      //

      void push_front(const T& data);
      void push_front(T&& data);
      void push_back(const T& data);
      void push_back(T&& data);
      iterator insert(iterator it, const T& data);
      iterator insert(iterator it, T&& data);

      //
      // Remove
      //

      void pop_back();
      void pop_front();
      void clear();
      iterator erase(const iterator& it);

      //
      // Status
      //

      bool empty()  const { return numElements == 0 ? true : false; }
      size_t size() const { return numElements; }

   private:
      // nested linked list class
      class Node;

      // member variables
      A    alloc;         // use alloacator for memory allocation
      size_t numElements; // though we could count, it is faster to keep a variable
      Node* pHead;       // pointer to the beginning of the list
      Node* pTail;       // pointer to the ending of the list
   };

   /*************************************************
    * NODE
    * the node class.  Since we do not validate any
    * of the setters, there is no point in making them
    * private.  This is the case because only the
    * List class can make validation decisions
    *************************************************/
   template <typename T, typename A>
   class list <T, A> ::Node
   {
   public:
      //
      // Construct

      // Standard constructor
      Node() : pNext(nullptr), pPrev(nullptr) {};
      // Copy constructor
      Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr) {};
      // Move constructor
      Node(T&& data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) {};

      //
      // Member Variables
      //

      T data;             // user data
      Node* pNext;       // pointer to next node
      Node* pPrev;       // pointer to previous node


   };

   /*************************************************
    * LIST ITERATOR
    * Iterate through a List, non-constant version
    ************************************************/
   template <typename T, typename A>
   class list <T, A> ::iterator
   {
      friend class ::TestList; // give unit tests access to the privates
      friend class ::TestHash;
      template <typename TT, typename AA>
      friend class custom::list;

   public:
      // constructors, destructors, and assignment operator
      iterator()
      {
         p = nullptr;
         /*p = new list <T, A> ::Node;*/
      }
      iterator(Node* pRHS)
      {
         p = pRHS;
         /*p = new list <T, A> ::Node;*/
      }
      iterator(const iterator& rhs)
      {
         p = rhs.p;
         /*p = new list <T, A> ::Node;*/
      }
      iterator& operator = (const iterator& rhs)
      {
         if (this != &rhs)
         {
            p = rhs.p;
         }
         return *this;
      }

      // equals, not equals operator
      bool operator == (const iterator& rhs) const { return (p == rhs.p); }
      bool operator != (const iterator& rhs) const { return (p != rhs.p); }

      // dereference operator, fetch a node
      T& operator * ()
      {
         return p->data;
      }

      // postfix increment
      iterator operator ++ (int postfix)
      {
         iterator original = *this;

         if (p->pNext != nullptr)
            p = p->pNext;
         return original;
      }

      // prefix increment
      iterator& operator ++ ()
      {
         if (p->pNext != nullptr)
            p = p->pNext;
         else
            p = nullptr;

         return *this;
      }

      // postfix decrement
      iterator operator -- (int postfix)
      {
         iterator original = *this;

         if (p->pPrev != nullptr)
            p = p->pPrev;

         return original;
      }

      // prefix decrement
      iterator& operator -- ()
      {
         if (p->pPrev != nullptr)
            p = p->pPrev;

         else
            p = nullptr;

         return *this;
      }

      // two friends who need to access p directly
      friend iterator list <T, A> ::insert(iterator it, const T& data);
      friend iterator list <T, A> ::insert(iterator it, T&& data);
      friend iterator list <T, A> ::erase(const iterator& it);

   private:

      typename list <T, A> ::Node* p;
   };

   /*****************************************
    * LIST :: NON-DEFAULT constructors
    * Create a list initialized to a value
    ****************************************/
   template <typename T, typename A>
   list <T, A>::list(size_t num, const T& t, const A& a) : alloc(a), numElements(num), pHead(nullptr), pTail(nullptr)
   {

      for (size_t i = 0; i < numElements; ++i)
      {
         Node* pNew = new list <T, A> ::Node(t);

         if (!pHead)
         {
            pHead = pTail = pNew;
            pNew->pNext = nullptr;
            pNew->pPrev = nullptr;
         }
         else
         {
            pTail->pNext = pNew;
            pNew->pPrev = pTail;
            pTail = pNew;
            pTail->pNext = nullptr;
         }
      }
   }

   /*****************************************
    * LIST :: NON-DEFAULT constructors
    * Create a list initialized to a value
    ****************************************/
   template <typename T, typename A>
   list<T, A>::list(size_t num, const A& a) : alloc(a), numElements(num), pHead(nullptr), pTail(nullptr)
   {
      for (size_t i = 0; i < numElements; ++i)
      {
         Node* pNew = new list <T, A> ::Node();

         if (!pHead)
         {
            pHead = pTail = pNew;
            pNew->pNext = nullptr;
            pNew->pPrev = nullptr;
         }
         else
         {
            pTail->pNext = pNew;
            pNew->pPrev = pTail;
            pTail = pNew;
            pTail->pNext = nullptr;
         }
      }
   }

   /*****************************************
    * LIST :: MOVE constructors
    * Steal the values from the RHS
    ****************************************/
   template <typename T, typename A>
   list <T, A> ::list(list <T, A>&& rhs, const A& a) :
      numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail), alloc(a)
   {
      rhs.pHead = rhs.pTail = nullptr;
      rhs.numElements = 0;
   }

   /**********************************************
    * LIST :: assignment operator - MOVE
    * Copy one list onto another
    *     INPUT  : a list to be moved
    *     OUTPUT :
    *     COST   : O(n) with respect to the size of the LHS
    *********************************************/
   template <typename T, typename A>
   list <T, A>& list <T, A> :: operator = (list <T, A>&& rhs)
   {
      clear();
      swap(rhs);
      return *this;
   }

   /**********************************************
    * LIST :: assignment operator
    * Copy one list onto another
    *     INPUT  : a list to be copied
    *     OUTPUT :
    *     COST   : O(n) with respect to the number of nodes
    *********************************************/
   template <typename T, typename A>
   list <T, A>& list <T, A> :: operator = (list <T, A>& rhs)
   {
      iterator itRHS = rhs.begin();
      iterator itLHS = begin();

      // Fill exhisting nodes.
      while (itRHS != rhs.end() && itLHS != end())
      {
         *itLHS = *itRHS;
         ++itRHS;
         ++itLHS;
      }

      // Addd new nodes.
      if (itRHS != rhs.end())
      {
         while (itRHS != rhs.end())
         {
            push_back(*itRHS);
            ++itRHS;
         }
      }

      else if (rhs.empty())
         clear();

      //Remove the extra nodes.
      else if (itLHS != end())
      {
         list <T, A> ::Node* p = itLHS.p;
         assert(p);
         pTail = p->pPrev;
         list <T, A> ::Node* pNext = p->pNext;

         for (p = itLHS.p; p; p = pNext)
         {
            pNext = p->pNext;
            delete p;
            numElements--;
         }
         pTail->pNext = nullptr;

         /*Node* p = itLHS.p;
         pTail = p->pPrev;
         Node* pNext = p->pNext;

         while (p != nullptr)
         {
            pNext = p->pNext;
            delete p;
            p = pNext;
            numElements--;
         }

         pTail->pNext = nullptr;*/
      }

      return *this;

   }

   /**********************************************
    * LIST :: assignment operator
    * Copy one list onto another
    *     INPUT  : a list to be copied
    *     OUTPUT :
    *     COST   : O(n) with respect to the number of nodes
    *********************************************/
   template <typename T, typename A>
   list <T, A>& list <T, A> :: operator = (const std::initializer_list<T>& rhs)
   {
      // Iterator for existing nodes
      iterator itLHS = begin();
      auto itRHS = rhs.begin();

      // Fill exhisting nodes.
      while (itRHS != rhs.end() && itLHS != end())
      {
         *itLHS = *itRHS;
         ++itRHS;
         ++itLHS;
      }

      // Addd new nodes.
      if (itRHS != rhs.end())
      {
         while (itRHS != rhs.end())
         {
            push_back(*itRHS);
            ++itRHS;
         }
      }

      else if (empty())
         clear();

      // Remove extra nodes.
      else if (itLHS != end())
      {
         Node* p = itLHS.p;
         pTail = p->pPrev;
         Node* pNext = p->pNext;

         while (p != nullptr)
         {
            pNext = p->pNext;
            delete p;
            p = pNext;
            numElements--;
         }

         pTail->pNext = nullptr;
      }

      return *this;
   }

   /**********************************************
    * LIST :: CLEAR
    * Remove all the items currently in the linked list
    *     INPUT  :
    *     OUTPUT :
    *     COST   : O(n) with respect to the number of nodes
    *********************************************/
   template <typename T, typename A>
   void list <T, A> ::clear()
   {
      list <T, A> ::Node* pNext;
      for (list<T, A>::Node* p = pHead; p; p = pNext)
      {
         pNext = p->pNext;
         delete p;
      }

      pHead = pTail = nullptr;
      numElements = 0;
      /*while (pHead != nullptr)
      {
         Node* pDelete = pHead;
         pHead = pHead->pNext;
         pTail = nullptr;
         delete pDelete;
         numElements = 0;
      }*/
   }

   /*********************************************
    * LIST :: PUSH BACK
    * add an item to the end of the list
    *    INPUT  : data to be added to the list
    *    OUTPUT :
    *    COST   : O(1)
    *********************************************/
   template <typename T, typename A>
   void list <T, A> ::push_back(const T& data)
   {
      // create new node.
      list<T, A> ::Node* pNew = new list <T, A> ::Node(data);
      
      // point it to the old tail.
      pNew->pPrev = pTail;

      // now point tail to the new guy.
      if (pTail != nullptr)
      {
         assert(pTail != nullptr);
         pTail->pNext = pNew;
      }

      else
         pHead = pNew; // there is no tail so there is no head.

      // Finally this is the new tail.
      pTail = pNew;
      numElements++;
      //Node* pNew = new Node(data); // create a new node

      //pNew->pNext = pTail; // set the new node's next pointer to the tail

      //if (pHead) // if the list is not empty
      //{
      //   pNew->pNext = nullptr; // set the new node's next pointer to null
      //   pNew->pPrev = pTail; // set the new node's previous pointer to the tail
      //   pTail->pNext = pNew; // set the tail's next pointer to the new node
      //}
      //else // if the list is empty set the head to the new node
      //{
      //   pHead = pNew;
      //}

      //pTail = pNew; // update pTail
      //numElements++; // increment the number of elements
   }

   template <typename T, typename A>
   void list <T, A> ::push_back(T&& data)
   {
      Node* pNew = new Node(std::move(data));
      pNew->pNext = pTail;

      if (pHead)
      {
         pNew->pNext = nullptr;
         pNew->pPrev = pTail;
         pTail->pNext = pNew;
      }
      else
      {
         pHead = pNew;
      }
      pTail = pNew;
      numElements++;
   }

   /*********************************************
    * LIST :: PUSH FRONT
    * add an item to the head of the list
    *     INPUT  : data to be added to the list
    *     OUTPUT :
    *     COST   : O(1)
    *********************************************/
   template <typename T, typename A>
   void list <T, A> ::push_front(const T& data)
   {
      Node* pNew = new Node(data); // create a new node

      pNew->pNext = pHead; // set the new node's next pointer to the head
      if (pHead) // if the list is not empty set the head's previous pointer to the new node
         pHead->pPrev = pNew;
      else // if the list is empty set the tail to the new node
         pTail = pNew;

      pHead = pNew; // update pHead
      numElements++; // increment the number of elements
   }

   template <typename T, typename A>
   void list <T, A> ::push_front(T&& data)
   {
      Node* pNew = new Node(std::move(data));

      pNew->pNext = pHead;
      if (pHead)
         pHead->pPrev = pNew;
      else
         pTail = pNew;
      pHead = pNew;
      numElements++;
   }


   /*********************************************
    * LIST :: POP BACK
    * remove an item from the end of the list
    *    INPUT  :
    *    OUTPUT :
    *    COST   : O(1)
    *********************************************/
   template <typename T, typename A>
   void list <T, A> ::pop_back()
   {
      erase(pTail);
   }

   /*********************************************
    * LIST :: POP FRONT
    * remove an item from the front of the list
    *    INPUT  :
    *    OUTPUT :
    *    COST   : O(1)
    *********************************************/
   template <typename T, typename A>
   void list <T, A> ::pop_front()
   {
      erase(pHead);
   }

   /*********************************************
   * LIST :: FRONT
   * retrieves the first element in the list
   *     INPUT  :
   *     OUTPUT : data to be displayed
   *     COST   : O(1)
   *********************************************/
   template <typename T, typename A>
   T& list <T, A> ::front()
   {
      if (pHead != nullptr)
         return pHead->data;
      else
         throw "ERROR: unable to access data from an empty list";
   }

   /*********************************************
    * LIST :: BACK
    * retrieves the last element in the list
    *     INPUT  :
    *     OUTPUT : data to be displayed
    *     COST   : O(1)
    *********************************************/
   template <typename T, typename A>
   T& list <T, A> ::back()
   {
      if (pTail != nullptr)
         return pTail->data;
      else
         throw "ERROR: unable to access data from an empty list";
   }


   /******************************************
    * LIST :: REMOVE
    * remove an item from the middle of the list
    *     INPUT  : an iterator to the item being removed
    *     OUTPUT : iterator to the new location
    *     COST   : O(1)
    ******************************************/
   template <typename T, typename A>
   typename list <T, A> ::iterator  list <T, A> ::erase(const list <T, A> ::iterator& it)
   {
      iterator itNext = end();

      if (it.p)
      {
         // take care of any nodes after it.
         if (it.p->pNext != nullptr)
         {
            it.p->pNext->pPrev = it.p->pPrev;
            itNext = it.p->pNext;
         }

         else
            pTail = pTail->pPrev;

         // Take care of any nodes before it.
         if (it.p->pPrev)
            it.p->pPrev->pNext = it.p->pNext;

         else
            pHead = pHead->pNext;

         // Delete the node.
         delete it.p;
         numElements--;
         return itNext;
      }

      return itNext;


   }

   /******************************************
    * LIST :: INSERT
    * add an item to the middle of the list
    *     INPUT  : data to be added to the list
    *              an iterator to the location where it is to be inserted
    *     OUTPUT : iterator to the new item
    *     COST   : O(1)
    ******************************************/
   template <typename T, typename A>
   typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
      const T& data)
   {
      if (empty()) {
         pHead = pTail = new Node(data);
         numElements++;
         return begin();
      }

      if (it == end())
      {
         Node* pNew = new Node(data);
         pTail->pNext = pNew;
         pNew->pPrev = pTail;
         pTail = pNew;
         numElements++;
         return iterator(pNew);
      }

      if (it != end())
      {
         Node* pNew = new Node(data);
         pNew->pPrev = it.p->pPrev;
         pNew->pNext = it.p;

         if (pNew->pPrev)
            pNew->pPrev->pNext = pNew;
         else
            pHead = pNew;

         if (pNew->pNext)
            pNew->pNext->pPrev = pNew;
         else
            pTail = pNew;

         numElements++;
         return iterator(pNew);
      }
      return end();
   }


   /******************************************
    * LIST :: INSERT
    * add several items into the middle of the list
    *     INPUT  : data to be added to the list
    *              an iterator to the location where it is to be inserted
    *     OUTPUT : iterator to the new item
    *     COST   : O(1)
    ******************************************/
   template <typename T, typename A>
   typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
      T&& data)
   {
      if (empty()) {
         pHead = pTail = new Node(std::move(data));
         numElements++;
         return begin();
      }

      if (it == end())
      {
         Node* pNew = new Node(std::move(data));
         pTail->pNext = pNew;
         pNew->pPrev = pTail;
         pTail = pNew;
         numElements++;
         return iterator(pNew);
      }

      if (it != end())
      {
         Node* pNew = new Node(std::move(data));
         pNew->pPrev = it.p->pPrev;
         pNew->pNext = it.p;

         if (pNew->pPrev)
            pNew->pPrev->pNext = pNew;
         else
            pHead = pNew;

         if (pNew->pNext)
            pNew->pNext->pPrev = pNew;
         else
            pTail = pNew;

         numElements++;
         return iterator(pNew);
      }
      return end();
   }

   /**********************************************
    * LIST :: assignment operator - MOVE
    * Copy one list onto another
    *     INPUT  : a list to be moved
    *     OUTPUT :
    *     COST   : O(n) with respect to the size of the LHS
    *********************************************/
   template <typename T, typename A>
   void swap(list <T, A>& lhs, list <T, A>& rhs)
   {
      lhs.clear();
      lhs.swap(rhs);
   }

}; // namespace custom


