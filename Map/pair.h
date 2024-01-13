/***********************************************************************
 * Module:
 *    PAIR
 * Author:
 *    Br. Helfrich
 * Summary:
 *    This program will implement a pair: two values
 ************************************************************************/

#pragma once

#include <iostream>  // for ISTREAM and OSTREAM

namespace custom
{

/**********************************************
 * PAIR
 * This class couples together a pair of values, which may be of
 * different types (T1 and T2). The individual values can be
 * accessed through its public members first and second.
 *
 * Additionally, when compairing two pairs, only T1 is compared. This
 * is a key in a name-value pair.
 ***********************************************/
template <class T1, class T2, typename C = std::less<T1>>
class pair
{
public:
   //
   // Constructors
   //
   
   // Default Constructor: call the T1, T2 default constructors
   pair(const C& c = C())
       : first(     ), second(      ), compare(c) {}
   // Non-Default Constructor: call the T1, T2 copy constructors
   pair(const T1 & first, const T2 & second, const C& c = C())
       : first(first), second(second), compare(c) {}
   pair(const T1& first, T2 && second, const C& c = C())
      : first(first), second(std::move(second)), compare(c) {}
   pair(const T1& first, const C& c = C())
      : first(first), second(), compare(c) {}
   // Copy Constructor: call the T1, T2 copy constructors
   pair(const pair <T1, T2> & rhs, const C& c = C())
       : first(rhs.first), second(rhs.second), compare(c) {}
   // Non-Default Move Constructor: call the T1, T2 move constructors
   pair(T1 && first, T2 && second, const C& c = C())
       : first(std::move(first)), second(std::move(second)), compare(c) {}
   // Move Constructor: call the T1, T2 move constructors
   pair(pair <T1, T2> && rhs, const C& c = C())
       : first(std::move(rhs.first)), second(std::move(rhs.second)), compare(c) {}

   //
   // Assignment Operators
   //
   
   // Standard assignment operator: call the T1, T2 assignment operator
   pair <T1, T2> & operator = (const pair <T1, T2> & rhs)
   {
      first  = rhs.first;
      second = rhs.second;
      return *this;
   }
   // Move assignment operator: call the T1, T2 move assignment operators
   pair <T1, T2> & operator = (pair <T1, T2> && rhs)
   {
      first  = std::move(rhs.first);
      second = std::move(rhs.second);
      return *this;
   }
   
   //
   // Equivalence: only the first will be compared
   //

   bool operator == (const pair & rhs) const { return first == rhs.first; }
   bool operator != (const pair & rhs) const { return !(*this == rhs);    }

   //
   // Relative: only the first will be compared
   //

   bool operator <  (const pair & rhs) const { return compare(first, rhs.first); }
   bool operator >  (const pair & rhs) const { return compare(rhs.first, first);        }
   bool operator >= (const pair & rhs) const { return !(compare(first, rhs.first));     }
   bool operator <= (const pair & rhs) const { return !(compare(rhs.first, first));     }
   
   //
   // Swap: swap the places
   //
   
   void swap(pair & rhs)
   {
      pair temp(std::move(rhs));  // move constructor
      rhs = std::move(*this);     // move assignment
      *this = std::move(temp);    // move assignment
   }
   
   //
   // Member Variables: direct access to the two member variables
   //
   
   //
   // Get: retrieve a value
   //
   
   C    compare;              // comparision operator

   
   // these are public. We cannot validate because we know nothing about T
   T1 first;
   T2 second;
};


/*****************************************************
 * PAIR SWAP
 * Stand-alone swap function
 ****************************************************/
template <class T1, class T2, typename C = std::less<T1>>
inline void swap(pair <T1, T2, C> & lhs, pair <T1, T2, C> & rhs)
{
   lhs.swap(rhs);
}

/*****************************************************
 * MAKE PAIR
 * Much like the non-default constructor
 ****************************************************/
template <class T1, class T2, typename C = std::less<T1>>
inline pair <T1, T2, C> make_pair(const T1 & t1, const T2 & t2)
{
   return pair<T1, T2, C> (t1, t2);
}


/*****************************************************
 * PAIR INSERTION
 * Display a pair for debug purposes
 ****************************************************/
template <class T1, class T2, typename C = std::less<T1>>
inline std::ostream & operator << (std::ostream & out,
                                   const pair <T1, T2, C> & rhs)
{
   out << '(' << rhs.first << ", " << rhs.second << ')';
   return out;
}

/*****************************************************
 * PAIR EXTRACTION
 * input a pair
 ****************************************************/
template <class T1, class T2, typename C = std::less<T1>>
inline std::istream & operator >> (std::istream & in,
                                   pair <T1, T2, C> & rhs)
{
   in >> rhs.first >> rhs.second;
   return in;
}

}
