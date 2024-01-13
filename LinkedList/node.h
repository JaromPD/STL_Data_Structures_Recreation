/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:
   //
   // Construct
   //

    // Standard constructor
    Node() : pNext(nullptr), pPrev(nullptr) {};
    // Copy constructor
    Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr) {};
    // Move constructor
    Node(T&& data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) {};

   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource) 
{
    Node<T>* pDestination = nullptr;
	Node<T>* pDes = nullptr;

	// iterate through the list and copy the data.
    for (const Node <T>* p = pSource; p; p = p->pNext)
    {
        // Check if pDestination has nodes.
        if (pDestination == nullptr)
        {
            // create a new node and assign the data.
			pDestination = new Node<T>(p->data);
			pDes = pDestination;
		}

        else
		    pDes = insert(pDes, p->data, true);
	}

	return pDestination;    
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   // Step 1: Copy the values from the source linked list into the destination.
    const Node<T> * pSrc = pSource;
    Node<T> * pDes = pDestination;
    Node<T>* pDesPrevious = pDes;

    while (pSrc != nullptr && pDes != nullptr)
    {
       pDesPrevious = pDes;
       pDes->data = pSrc->data;
       pSrc = pSrc->pNext;
       pDes = pDes->pNext;
    }

    // Step 2: See if pSource is longer.
    if (pSrc != nullptr)
    {
       pDes = pDesPrevious;

       while (pSrc != nullptr)
       {
          pDes = insert(pDes, pSrc->data, true);

          if (pDestination == nullptr)
             pDestination = pDes;

          pSrc = pSrc->pNext;
       }
    }

    // Step 3: See if pDestination list is longer. 
    else if (pSrc == nullptr && pDes != nullptr)
    {
       bool setToNull = false;

       if (pDes->pPrev != nullptr)
          pDes->pPrev->pNext = nullptr;

       else
          setToNull = true;

       while (pDes != nullptr)
       {
          Node<T>* temp = pDes->pNext;
          delete pDes;
          pDes = temp;
                 
       }

       if (setToNull)
          pDestination = nullptr;


    }

}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
   //  Set temporary Node to use later
   Node<T>* temp = pLHS;

   //  Change Left Hand Side to equal RHS
   pLHS = pRHS;

   //  Set Right Hand Side to equal temp
   pRHS = temp;
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{  
    Node<T>* pReturn = nullptr;

    if (pRemove != nullptr)
    {
        if (pRemove->pPrev)
            pRemove->pPrev->pNext = pRemove->pNext;

        if (pRemove->pNext)
			pRemove->pNext->pPrev = pRemove->pPrev;

        pRemove->pPrev ? pReturn = pRemove->pPrev : pReturn = pRemove->pNext;

        delete pRemove;

    }
    return pReturn;
}


/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
   // Step 1: allocate the new node
   Node<T>* pNew = new Node<T>(t);

   // Checking if it is the tail node.
   if (pCurrent != nullptr && after == false)
   {
       // Step 2: Direct the new node to the destination linked list.
       pNew->pNext = pCurrent;
       pNew->pPrev = pCurrent->pPrev;

       // Step 3: Direct the destination linked list to the new node.
       pCurrent->pPrev = pNew;

       if (pNew->pPrev)
           pNew->pPrev->pNext = pNew;

   }

   // If it is 't the tail node.
   if (pCurrent != nullptr && after == true)
   {

       // Step 2: Direct the new node to the destination linked list.
       pNew->pPrev = pCurrent;
       pNew->pNext = pCurrent->pNext;

       // Step 3: Direct the destination linked list to the new node.
       pCurrent->pNext = pNew;

       if (pNew->pNext)
           pNew->pNext->pPrev = pNew;
   }

   return pNew;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
    int nodes = 0;
    for (const Node <T> * p = pHead; p; p = p->pNext)
		nodes++;

    return nodes;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
    for (const Node <T> * p = pHead; p; p = p->pNext)
		out << (p == pHead ? "" : ", ") << p->data;
   return out;
}

/*****************************************************
 * FREE DATA
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
    while (pHead != nullptr)
    {
        Node<T>* pDelete = pHead;
        pHead = pHead->pNext;
        delete pDelete;
    }
}


