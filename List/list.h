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

        list(list <T, A>& rhs, const A& a = A()) : alloc(a), numElements(0),
        pHead(nullptr), pTail(nullptr)
        {
            // Iterate through rhs and push_back each element
            for (auto it = rhs.begin(); it != rhs.end(); ++it)
            {
                push_back(*it);
            }
        }

        list(list <T, A>&& rhs, const A& a = A());
        list(size_t num, const T& t, const A& a = A());
        list(size_t num, const A& a = A());
        list(const std::initializer_list<T>& il, const A& a = A()) : alloc(a), numElements(0),
        pHead(nullptr), pTail(nullptr)
        {
            // Iterate through initializer list and push_back each element
            for (auto it = il.begin(); it != il.end(); ++it)
            {
                push_back(*it);
            }
        }
        template <class Iterator>
        list(Iterator first, Iterator last, const A& a = A()) : alloc(a), numElements(0),
        pHead(nullptr), pTail(nullptr)
        {
            // Iterate through range and push_back each element
            for (auto it = first; it != last; ++it)
            {
                push_back(*it);
            }
        }

        ~list()
        {
            // Iterate through list and delete all nodes
            while (pHead != nullptr)
            {
                Node* temp = pHead;
                pHead = temp->pNext;
                delete temp;
            }

            // Reset member variables
            pHead = pTail = nullptr;
            numElements = 0;
        }

        //
        // Assign
        //

        list <T, A>& operator = (list <T, A>& rhs);
        list <T, A>& operator = (list <T, A>&& rhs);
        list <T, A>& operator = (const std::initializer_list<T>& il);
        void swap(list <T, A>& rhs) 
        {
            // Swap heads
            auto tempHead = rhs.pHead;
            rhs.pHead = pHead;
            pHead = tempHead;

            // Swap tails
            auto tempTail = rhs.pTail;
            rhs.pTail = pTail;
            pTail = tempTail;

            // Swap numElements
            auto tempElements = rhs.numElements;
            rhs.numElements = numElements;
            numElements = tempElements;
        }

        //
        // Iterator
        //

        class iterator;
        iterator begin() { return iterator(pHead); }  // pHead is start of list. Return an iterator of it.
        iterator rbegin() { return iterator(pTail); } // pTail is last node of list. Return an iterator of it.
        iterator end() { return iterator(nullptr); }  // nullptr is end of list. Return an iterator of it.

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

        bool empty()  const { return size() == 0; }
        size_t size() const { return numElements; }

    private:
        // nested linked list class
        class Node;

        // member variables
        A    alloc;         // use alloacator for memory allocation
        size_t numElements; // though we could count, it is faster to keep a variable
        Node* pHead;        // pointer to the beginning of the list
        Node* pTail;        // pointer to the ending of the list
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
        Node* pNext;        // pointer to next node
        Node* pPrev;        // pointer to previous node

        
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
        }
        iterator(Node* pRHS)
        {
           p = pRHS;
        }
        iterator(const iterator& rhs)
        {
           p = rhs.p;
        }
        iterator& operator = (const iterator& rhs)
        {
           if (this != &rhs) // Make sure we aren't copying ourselves
           {
              p = rhs.p;     // Copy the pointer
           }
           return *this;
        }

        // equals, not equals operator
        bool operator == (const iterator& rhs) const { return (p == rhs.p); } // Compare p's
        bool operator != (const iterator& rhs) const { return (p != rhs.p); }

        // dereference operator, fetch a node
        T& operator * ()
        {
            return p->data;
        }

        // postfix increment
        iterator operator ++ (int postfix)
        {
           iterator original = *this; // save the original value.

           if (p != nullptr)          // Increment p if it isn't null.
              p = p->pNext;

            return original;          // Return the original iterator.
        }

        // prefix increment
        iterator& operator ++ ()
        {
           if (p != nullptr) // Increment p if it isn't null.
              p = p->pNext;

           return *this;     // Return the iterator.
        }

        // postfix decrement
        iterator operator -- (int postfix)
        {
           iterator original = *this; // save the original value.

           if (p != nullptr)          // Increment p if it isn't null.
              p = p->pPrev;

           return original;          // Return the original iterator.
        }

        // prefix decrement
        iterator& operator -- ()
        {
           if (p != nullptr)
              p = p->pPrev;

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
       for (size_t i = 0; i < numElements; ++i)   // Loop through the number of elements.
       {
          Node* pNew = new list <T, A> ::Node(t); // Create a new node with the value every loop.

          if (!pHead)                             // If the head is null, set the head and tail to the new node.
          {
             pHead = pTail = pNew;
             pNew->pNext = nullptr;
             pNew->pPrev = nullptr;
          }
          else                                    // Otherwise, set the tail's next to the new node, and the new node's previous to the tail.
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
        for (size_t i = 0; i < numElements; ++i)   // Loop through the number of elements.
        {
            Node* pNew = new list <T, A> ::Node(); // Create a new node with the value every loop.

            if (!pHead)                            // If the head is null, set the head and tail to the new node.
            {
                pHead = pTail = pNew;
                pNew->pNext = nullptr;
                pNew->pPrev = nullptr;
            }
            else                                   // Otherwise, set the tail's next to the new node, and the new node's previous to the tail.
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
        rhs.pHead = rhs.pTail = nullptr; // Reset RHS.
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
       clear();      // Clear the LHS.
       swap(rhs);    // Swap the LHS and RHS.
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

       // Add new nodes.
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
       while (pHead != nullptr)  // Iterate through all values of list.
       {
          Node* pDelete = pHead; // Delete each node, one at a time.
          pHead = pHead->pNext;
          pTail = nullptr;
          delete pDelete;
          numElements = 0;
       }
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
       Node* pNew = new Node(data); // create a new node
       
       pNew->pNext = pTail;

       if (pHead)                   // The old tail must be updated or ...
       {
           pNew->pNext = nullptr; 
           pNew->pPrev = pTail;
           pTail->pNext = pNew;
       }
       else                         // ...the new node is the tail and head.
       {
           pHead = pNew; 
       }

       pTail = pNew;
       numElements++;              // increment the number of elements.
    }

    template <typename T, typename A>
    void list <T, A> ::push_back(T&& data)
    {
        Node* pNew = new Node(std::move(data)); // Create a new node.
        pNew->pNext = pTail;

        if (pHead)                              // The old tail must be updated or...
        {
            pNew->pNext = nullptr;
            pNew->pPrev = pTail;
            pTail->pNext = pNew;
        }
        else                                    // ...the new node is the tail and head.
        {
            pHead = pNew;
        }
        pTail = pNew;
        numElements++;                          // increment the number of elements.
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
        Node* pNew = new Node(data); // Create a new node

        pNew->pNext = pHead;
        if (pHead)                   // The old head must be updated or...
            pHead->pPrev = pNew;
        else                         // ...the new node is the tail and head.
            pTail = pNew;

        pHead = pNew;
        numElements++;               // increment the number of elements
    }

    template <typename T, typename A>
    void list <T, A> ::push_front(T&& data)
    {
        Node* pNew = new Node(std::move(data)); // Create a new node.
        
        pNew->pNext = pHead;
        if (pHead)                              // The old head must be updated or...
            pHead->pPrev = pNew;
        else
            pTail = pNew;

        pHead = pNew;
        numElements++;                         // increment the number of elements.
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
            pHead = pTail = new Node(data); // New element is both head and tail.
            numElements = 1;                // Just one element now.
            return begin();
        }
        
        if (it == end())
        {
            Node* pNew = new Node(data); // Create the new node.
            pTail->pNext = pNew;         // Hook up the pointers
            pNew->pPrev = pTail;
            pTail = pNew;
            numElements++;              // Increment elements.
            return iterator(pNew);
        }
        
        if ( it != end())
        {
            // Step 1: A new node is created.
            Node* pNew = new Node(data);

            // Step 2: The new node's pPrev and pNext are hooked up.
            pNew->pPrev = it.p->pPrev;
            pNew->pNext = it.p;
            
            // Step 3: The old list must be made aware of pNew.
            if (pNew->pPrev)
                pNew->pPrev->pNext = pNew;
            else
                pHead = pNew;
            
            if (pNew->pNext)
                pNew->pNext->pPrev = pNew;
            else
                pTail = pNew;
            
            // Step 4: Increment the number of elements and return.
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
            pHead = pTail = new Node(std::move(data)); // New element is both head and tail.
            numElements = 1;                           // Just one element now.
            return begin();
        }

        if (it == end())
        {
            Node* pNew = new Node(std::move(data));  // Create the new node.
            pTail->pNext = pNew;                     // Hook up the pointers
            pNew->pPrev = pTail;
            pTail = pNew;
            numElements++;                           // Increment elements.
            return iterator(pNew);
        }

        if (it != end())
        {
            // Step 1: A new node is created.
            Node* pNew = new Node(std::move(data));

            // Step 2: The new node's pPrev and pNext are hooked up.
            pNew->pPrev = it.p->pPrev;
            pNew->pNext = it.p;

            // Step 3: The old list must be made aware of pNew.
            if (pNew->pPrev)
                pNew->pPrev->pNext = pNew;
            else
                pHead = pNew;

            if (pNew->pNext)
                pNew->pNext->pPrev = pNew;
            else
                pTail = pNew;

            // Step 4: Increment the number of elements and return.
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
       lhs.clear();   // Clear the LHS list.
       lhs.swap(rhs); // Swap LHS and RHS.
    }

}; // namespace custom


