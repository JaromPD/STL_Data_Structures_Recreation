/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Peter Benson, Isaac Radford, Jarom Diaz
 ************************************************************************/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

/*****************************************************************
 * BNODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <class T>
class BNode
{
public:
   // 
   // Construct
   //
   BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}
   BNode(const T &  t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}
   BNode(T && t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}

   //
   // Data
   //
   BNode <T> * pLeft;
   BNode <T> * pRight;
   BNode <T> * pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T> * p)
{
	// If there's no node, return 0.
	if (p == nullptr)
		return 0;
	// If there is a node add 1 and look at the left and right size.
	else
		return size(p->pLeft) + 1 + size(p->pRight);
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{
	// If pAdd is not null...
	if (pAdd != nullptr)
		// Make pNode it's parent
		pAdd->pParent = pNode;
	// Now pNode's new left is the added node.
	pNode->pLeft = pAdd;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight (BNode <T> * pNode, BNode <T> * pAdd)
{
	// If pAdd is not null...
	if (pAdd != nullptr)
		// Make pNode it's parent
		pAdd->pParent = pNode;
	// Now pNode's new right is the added node.
	pNode->pRight = pAdd;
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft (BNode <T> * pNode, const T & t) 
{
	// Create the new node.
	BNode<T>* pAdd = new BNode<T>(t);
	// Make it's parent the Node we're adding it from.
	pAdd->pParent = pNode;
	// Attatch it to it's parents pLeft.
	pNode->pLeft = pAdd;
}

template <class T>
inline void addLeft(BNode <T>* pNode, T && t)
{
	// Create the new node.
	BNode<T>* pAdd = new BNode<T>(std::move(t));
	// Make it's parent the Node we're adding it from.
	pAdd->pParent = pNode;
	// Attatch it to it's parents pLeft.
	pNode->pLeft = pAdd;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight (BNode <T> * pNode, const T & t)
{
	// Create the new node.
	BNode<T>* pAdd = new BNode<T>(t);
	// Make it's parent the Node we're adding it from.
	pAdd->pParent = pNode;
	// Attatch it to it's parents pRight.
	pNode->pRight = pAdd;
}

template <class T>
void addRight(BNode <T>* pNode, T && t)
{
	// Create the new node.
	BNode<T>* pAdd = new BNode<T>(std::move(t));
	// Make it's parent the Node we're adding it from.
	pAdd->pParent = pNode;
	// Attatch it to it's parents pRight.
	pNode->pRight = pAdd;
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void clear(BNode <T> * & pThis)
{
   if (pThis != nullptr)
   {
      // clear left node.
      clear(pThis->pLeft);

      // clear right node.
      clear(pThis->pRight);

      // delete V node.
      delete pThis;
      pThis = nullptr;
   }
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
   // It swaps the nodes. 
   std::swap(pLHS, pRHS);
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T> * copy(const BNode <T> * pSrc) 
{
	// If there is nothing to copy just return nullptr.
	if (pSrc == nullptr)
	{
		return nullptr;
	}

	// Create the new node, and and hook up the copy of pLeft.
	BNode<T>* destination = new BNode<T>(pSrc->data);
	destination->pLeft = copy(pSrc->pLeft);

	// If it's a nullptr...
	if (destination->pLeft != nullptr)
	{
		// Hookup the new left node as dest's child.
		destination->pLeft->pParent = destination;
	}
	// Hookup the copy of pRight.
	destination->pRight = copy(pSrc->pRight);

	// If it's a nullpt...
	if (destination->pRight != nullptr)
	{
		// Hookup the new right node as dest's child.
		destination->pRight->pParent = destination;
	}

	// Return the dest node.
	return destination;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T> * & pDest, const BNode <T>* pSrc)
{
   // Source is empty test case.
   if (pSrc == nullptr)
      clear(pDest);

   else
   {
      // Destination is empty test case.
      if (pDest == nullptr)
         pDest = new BNode<T>(pSrc->data);

      // Neither the source nor destination are empty.
      else
         pDest->data = pSrc->data;


      //assign the right nodes.
      assign(pDest->pRight, pSrc->pRight);
      if (pDest->pRight) 
         pDest->pRight->pParent = pDest;

      // assign the left nodes.
      assign(pDest->pLeft, pSrc->pLeft);
      if (pDest->pLeft)
         pDest->pLeft->pParent = pDest;
   }

}