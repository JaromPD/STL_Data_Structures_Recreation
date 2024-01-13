/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Peter Benson, Jarom Diaz, Isaac Radford
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return (root == nullptr); }
   size_t size()  const noexcept { return numElements;   }
   
private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree

   void _clear(BNode*& pThis)
   {
      if (!pThis)
         return;

      _clear(pThis->pLeft);   // L
      _clear(pThis->pRight);  // R
      delete pThis;           // V
      pThis = nullptr;
   }

   void _assign(BNode*& pDest, const BNode* pSrc)
   {
      // Source is empty test case.
      if (!pSrc)
         _clear(pDest);

      else
      {
         // Destination is empty test case.
         if (!pDest)
            pDest = new BNode(pSrc->data);

         // Neither the source nor destination are empty.
         else
            pDest->data = pSrc->data;

         // need to set each Bnode to red or black.
         pDest->isRed = pSrc->isRed;

         //assign the right nodes.
         _assign(pDest->pRight, pSrc->pRight);
         if (pDest->pRight)
            pDest->pRight->pParent = pDest;

         // assign the left nodes.
         _assign(pDest->pLeft, pSrc->pLeft);
         if (pDest->pLeft)
            pDest->pLeft->pParent = pDest;
      }

   }

   std::pair<typename BST<T>::iterator, bool>_insert(BNode*& pNode, const T& t, bool keepUnique)
   {
      // empty tree.
      if (!pNode)
      {
         pNode = new BNode(t);
         return std::make_pair(iterator(pNode), true);
      }

      if (keepUnique && t == pNode->data)
      {
         // Duplicate values are not allowed, return false
         return std::make_pair(iterator(pNode), false);
      }

      // If the value is less than the node.
      if (t < pNode->data)
      {
         if (pNode->pLeft)
            return _insert(pNode->pLeft, t, keepUnique);

         pNode->addLeft(t);
         pNode->pLeft->balance();
         return std::make_pair(iterator(root->pLeft), true);
      }

      // if the value is greater than the node.
      if (pNode->pRight)
         return _insert(pNode->pRight, t, keepUnique);

      pNode->addRight(t);
      pNode->pRight->balance();
      return std::make_pair(iterator(root->pRight), true);
      
   }

   std::pair<typename BST<T>::iterator, bool>_insertMove(BNode*& pNode, T && t, bool keepUnique)
   {
      // empty tree.
      if (!pNode)
      {
         pNode = new BNode(t);
         return std::make_pair(iterator(pNode), true);
      }

      if (keepUnique && t == pNode->data)
      {
         // Duplicate values are not allowed, return false
         return std::make_pair(iterator(pNode), false);
      }

      // If the value is less than the node.
      if (t < pNode->data)
      {
         // if pLeft exists, move to the next pLeft node.
         if (pNode->pLeft)
            return _insertMove(pNode->pLeft, std::move(t), keepUnique);

         // we can now insert the node to the left and balance the tree.
         pNode->addLeft(std::move(t));
         pNode->pLeft->balance();
         return std::make_pair(iterator(root->pLeft), true);
      }

      // if the value is greater than the node.
      if (pNode->pRight)
         return _insertMove(pNode->pRight, std::move(t), keepUnique);

      pNode->addRight(std::move(t));
      pNode->pRight->balance();
      return std::make_pair(iterator(root->pRight), true);
      
   }
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}
   BNode(const T& t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}
   BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode* pNode) const { return (pParent && pParent->pRight == this); }
   bool isLeftChild( BNode * pNode) const { return (pParent && pParent->pLeft == this); }

   // balance the tree
   void balance();

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed = true;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode * p = nullptr) : pNode(p) {}

   iterator(const iterator & rhs)         
   { 
       *this = rhs;
   }
   iterator & operator = (const iterator & rhs)
   {
       pNode = rhs.pNode;
	   return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
       return pNode == rhs.pNode;
   }
   bool operator != (const iterator& rhs) const
   {
       return pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
       return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix) 
   {
       auto itCopy = *this;
       ++(*this);
       return itCopy;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
       auto itCopy = *this;
       --(*this);
       return itCopy;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST() : numElements(0), root(nullptr){}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) : root(nullptr), numElements(rhs.numElements)
{
   *this = rhs;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) : root(rhs.root), numElements(rhs.numElements)
{
   rhs.root = nullptr;
   rhs.numElements = 0;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il) : root(nullptr), numElements(il.size())
{
   *this = il;
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   _assign(root, rhs.root);
   numElements = rhs.numElements;
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
    clear(); // Clear the existing tree

    for (auto& it : il)
        insert(it);

    return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   clear();
   swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
   // swap the root pointers of the two trees.
   std::swap(root, rhs.root);

   // swap the number of elements.
   std::swap(numElements, rhs.numElements);
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   // if there is a node in the root then populate the tree.
   if (root)
   {
      auto result = _insert(root, t, keepUnique);
      if (result.second)
         numElements++;

      // hook up my root.
      while (root->pParent)
         root = root->pParent;

      return result;
   }

   // empty tree.
   else
   {
      root = new BNode(t);
      root->isRed = false;  // Root is always black in a red-black tree
      numElements++;
      return std::make_pair(iterator(root), true);
   }
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   // if there is a node in the root then populate the tree.
   if (root)
   {
      auto result = _insertMove(root, std::move(t), keepUnique);
      if (result.second)
         numElements++;

      //hook up my root.
      while (root->pParent)
         root = root->pParent;

      return result;
   }

   // empty tree.
   else
   {
      root = new BNode(t);
      root->isRed = false;  // Root is always black in a red-black tree.
      numElements++;
      return std::make_pair(iterator(root), true);
   }

}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{  
    if (!it.pNode)
        return it;

    // CASE ONE: Handle if node has no children
    if (!it.pNode->pRight && !it.pNode->pLeft)
    {
        auto itNext = it;
        ++itNext;

        // Adjust parent's right or left to nullptr.
        (it.pNode->pParent && it.pNode->isRightChild(it.pNode->pParent)) ?
           it.pNode->pParent->pRight = nullptr :
           it.pNode->pParent->pLeft = nullptr;

        delete it.pNode;
        --numElements;
        return itNext;
    }

    // CASE TWO: Handle if node has one child, left or right
    if (!it.pNode->pRight && it.pNode->pLeft)
    {
        auto itNext = it;
        ++itNext;

        it.pNode->pLeft->pParent = it.pNode->pParent;

        // Adjust parent's left or right to pNode's left.
        (it.pNode->pParent && it.pNode->isRightChild(it.pNode->pParent)) ?
           it.pNode->pParent->pRight = it.pNode->pLeft :
           it.pNode->pParent->pLeft = it.pNode->pLeft;

        delete it.pNode;
        --numElements;
        return itNext;
    }

    if (!it.pNode->pLeft && it.pNode->pRight)
    {
        auto itNext = it;
        ++itNext;

        it.pNode->pRight->pParent = it.pNode->pParent;

        // Adjust parent's left or right to pNode's right.
        (it.pNode->pParent && it.pNode->isRightChild(it.pNode->pParent)) ?
           it.pNode->pParent->pRight = it.pNode->pRight :
           it.pNode->pParent->pLeft = it.pNode->pRight;

        delete it.pNode;
        --numElements;
        return itNext;
    }

    // CASE THREE: Node has two children
    if (it.pNode->pRight != nullptr && it.pNode->pLeft != nullptr)
    {
        auto pSuccessor = it;
        ++pSuccessor;

        // Adjust parent's pointer to be the successors right child
        (pSuccessor.pNode->isLeftChild(pSuccessor.pNode->pParent)) ?
           pSuccessor.pNode->pParent->pLeft = pSuccessor.pNode->pRight :
           pSuccessor.pNode->pParent->pRight = pSuccessor.pNode->pRight;

        // If it has a right child, adjust child's parent pointer
        if (pSuccessor.pNode->pRight)
            pSuccessor.pNode->pRight->pParent = pSuccessor.pNode->pParent;


        // Connect successor to the erased node's parent, unless it's the root, where you just make the root the successor.
        if (it.pNode->pParent)
           (it.pNode->isLeftChild(it.pNode->pParent)) ?
              it.pNode->pParent->pLeft = pSuccessor.pNode :
              it.pNode->pParent->pRight = pSuccessor.pNode;
        else
            root = pSuccessor.pNode;

        // Update pointers for new successor
        pSuccessor.pNode->pParent = it.pNode->pParent;
        pSuccessor.pNode->pLeft = it.pNode->pLeft;
        pSuccessor.pNode->pRight = it.pNode->pRight;

        if (it.pNode->pLeft)
            it.pNode->pLeft->pParent = pSuccessor.pNode;
        if (it.pNode->pRight)
            it.pNode->pRight->pParent = pSuccessor.pNode;

        // Delete the erased node and update the numElements
        delete it.pNode;
        --numElements;
        return pSuccessor;
    }

    return it;
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   numElements = 0;
   _clear(root);
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   // if it's empty, return end.
   if (empty())
      return end();

   BST<T>::BNode* pNode = root;

   while (pNode->pLeft)
      pNode = pNode->pLeft;

   return iterator(pNode);
}

/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   auto p = root;

   while (p)
   {
      // If the current Bnode is the value, return it.
      if (p->data == t)
         return iterator(p);

      // if t is less than the Bnode then go left.
      else if (t < p->data)
         p = p->pLeft;

      // else go right.
      else
         p = p->pRight;
   }

   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
    // If pNode is not null...
    if (pNode)
       // Make pNode it's parent
       pNode->pParent = this;

    // Now pNode's new left is the added node.
    this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
    // If pNode is not null...
    if (pNode)
       // Make pNode it's parent
       pNode->pParent = this;

    // Now pNode's new right is the added node.
    this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
    // Create the new node.
   BST<T>:: BNode* pAdd = new BST<T>::BNode(t);

    // Make it's parent the Node we're adding it from.
    pAdd->pParent = this;

    // Attatch it to it's parents pLeft.
    this->pLeft = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
    // Create the new node.
    BST<T>::BNode* pAdd = new BST<T>::BNode(std::move(t));

    // Make it's parent the Node we're adding it from.
    pAdd->pParent = this;

    // Attatch it to it's parents pLeft.
    this->pLeft = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
    // Create the new node.
    BST<T>::BNode* pAdd = new BST<T>::BNode(t);

    // Make it's parent the Node we're adding it from.
    pAdd->pParent = this;

    // Attatch it to it's parents pRight.
    this->pRight = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
    // Create the new node.
    BST<T>::BNode* pAdd = new BST<T>::BNode(std::move(t));

    // Make it's parent the Node we're adding it from.
    pAdd->pParent = this;

    // Attatch it to it's parents pRight.
    this->pRight = pAdd;
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: balance()
{
   // Case 1: if we are the root, then color ourselves black and call it a day.
   if (!pParent)
   {
      isRed = false;
      return;
   }

   // Case 2: if the parent is black, then there is nothing left to do
   else
   {
      if (!pParent->isRed)
         return;

      BNode* pGrandparent = pParent->pParent;

      BNode* pGreatGranny = pGrandparent->pParent;

      BNode* pAunt = (pParent == pGrandparent->pLeft) ? pGrandparent->pRight :
         pGrandparent->pLeft;

      BNode* pSibling = (this->isLeftChild(pParent)) ? pParent->pRight :
         pParent->pLeft;

      // Case 3: if the aunt is red, then just recolor
      if (pAunt && pAunt->isRed)
      {
         // Recolor the grandparent, then balance off of granparent node.
         pGrandparent->isRed = true;
         pGrandparent->balance();

         // Now that we have balanced off of granparent, recolor parent and aunt.
         pParent->isRed = false;
         pAunt->isRed = false;

         return;
      }

      // Case 4: if the aunt is black or non-existant, then we need to rotate
      if((pAunt && !pAunt->isRed) || !pAunt)
      {

         // Case 4a: We are mom's left and mom is granny's left
         if (this->isLeftChild(pParent) && pParent->isLeftChild(pGrandparent) &&
            pParent->isRed && !pGrandparent->isRed)
         {
            // left rotate.
            pParent->addRight(pGrandparent); // puts the original parent to the right.
            pGrandparent->addLeft(pSibling); // moves the sibling if it has one to the left.

            //hook the parent to the greatGranny.
            pParent->pParent = pGreatGranny;

            // Update parent references
            if (pGreatGranny)
               (pGrandparent->isRightChild(pGreatGranny)) ?
               pGreatGranny->pLeft = pParent :
               pGreatGranny->pRight = pParent;

            // recolor the nodes.
            pParent->isRed = false;
            pGrandparent->isRed = true;
         }

         // case 4b: We are mom's right and mom is granny's right
         if (this->isRightChild(pParent) && pParent->isRightChild(pGrandparent) &&
            pParent->isRed && !pGrandparent->isRed)
         {
            // right rotate.
            pParent->addLeft(pGrandparent); // puts the original parent to the right.
            pGrandparent->addRight(pSibling); // moves the sibling if it has one to the left.

            //hook the parent to the greatGranny.
            pParent->pParent = pGreatGranny;

            // Update parent references
            if (pGreatGranny)
               (pGrandparent->isRightChild(pGreatGranny)) ?
               pGreatGranny->pLeft = pParent :
               pGreatGranny->pRight = pParent;

            // recolor the nodes.
            pParent->isRed = false;
            pGrandparent->isRed = true;
         }

         // Case 4c: We are mom's right and mom is granny's left
         if (this->isRightChild(pParent) && pParent->isLeftChild(pGrandparent) &&
            pParent->isRed && !pGrandparent->isRed)
         {
            // distribute any children.
            pGrandparent->addLeft(this->pRight);
            pParent->addRight(this->pLeft);

            // make this the parent.
            this->addRight(pGrandparent); // put grandparent to the right.
            this->addLeft(pParent); // put parent to the left.

            // Update parent references
            if (!pGreatGranny)
               pParent = pGreatGranny;

            else if (pGrandparent->isRightChild(pGreatGranny))
               pGreatGranny->pRight = pParent;
            else
               pGreatGranny->pLeft = pParent;

            // change the colors.
            pGrandparent->isRed = true;
            isRed = false;
         }

         // case 4d: we are mom's left and mom is granny's right
         if (this->isLeftChild(pParent) && pParent->isRightChild(pGrandparent) &&
            pParent->isRed && !pGrandparent->isRed)
         {
            // distribute any children.
            pGrandparent->addRight(this->pLeft);
            pParent->addLeft(this->pRight);

            // make this the parent.
            this->addLeft(pGrandparent); // put grandparent to the right.
            this->addRight(pParent); // put parent to the left.

            // Update parent references
            if (!pGreatGranny)
               pParent = pGreatGranny;

            else if (pGrandparent->isRightChild(pGreatGranny))
               pGreatGranny->pRight = pParent;
            else
               pGreatGranny->pLeft = pParent;

            // change the colors.
            pGrandparent->isRed = true;
            isRed = false;
         }
      } 
   }
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
    if (pNode)
    {
        // If there is a right child...
        if (pNode->pRight)
        {
            // Go right.
            pNode = pNode->pRight;
            // Then as far left as we can.
            while (pNode->pLeft)
                pNode = pNode->pLeft;
            return *this;
        }

        // If there is no right child and we are parent's left.
        if(!pNode->pRight && pNode->isLeftChild(pNode->pParent))
        {
            // Go up to parent.
            pNode = pNode->pParent;
            return *this;
        }

        // No right child and we are parent's right.
        if (!pNode->pRight && pNode->isRightChild(pNode->pParent))
        {
            //Find parent successor
            while (pNode->pParent && pNode->isRightChild(pNode->pParent))
                pNode = pNode->pParent;

            pNode = pNode->pParent;
            return *this;
        }
    }
    return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T>::iterator& BST <T>::iterator::operator--()
{
    if (pNode != nullptr)
    {
        // If there is a left child...
        if (pNode->pLeft)
        {
            // Go left.
            pNode = pNode->pLeft;
            // Then as far right as we can.
            while (pNode->pRight)
            {
                pNode = pNode->pRight;
            }
        }
        else
        {
            while (pNode->pParent)
            {
                // If the current node is the right child
                if (pNode->isRightChild(pNode->pParent))
                {
                    // Move up to the parent
                    pNode = pNode->pParent;
                    return *this;
                }
                // Move up to the parent
                pNode = pNode->pParent;

                if (!pNode->pParent)
                {
                    // If we've reached the root, increment to the end. (nullptr)
                    pNode = pNode->pParent;
                    return *this;
                }
            }
        }
    }
    return *this;
}



} // namespace custom


