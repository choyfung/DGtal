#pragma once

/**
 * @file HyperRectDomain_Iterator.h
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2010/05/31
 *
 * Header file for module HyperRectDomain_Iterator.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(HyperRectDomain_Iterator_RECURSES)
#error Recursive header files inclusion detected in HyperRectDomain_Iterator.h
#else // defined(HyperRectDomain_Iterator_RECURSES)
/** Prevents recursive inclusion of headers. */
#define HyperRectDomain_Iterator_RECURSES

#if !defined HyperRectDomain_Iterator_h
/** Prevents repeated inclusion of headers. */
#define HyperRectDomain_Iterator_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

/////////////////////////////////////////////////////////////////////////////
// class HyperRectDomain_Iterator
/**
 * Description of class 'HyperRectDomain_Iterator' <p>
 * Aim:
 */
template<typename TPointType>
class HyperRectDomain_Iterator
{

public:

    typedef std::bidirectional_iterator_tag iterator_category; ///\todo construct a RANDOM-ACCESS iterator
    typedef TPointType value_type;
    typedef ptrdiff_t difference_type;
    typedef TPointType* pointer;
    typedef TPointType& reference;


    HyperRectDomain_Iterator ( const TPointType & p, const TPointType& lower,const TPointType &upper )
            : myPoint ( p ),  myCurrentPos ( 0 ), mylower ( lower ), myupper ( upper )
    {
    }

    const TPointType & operator*() const
    {
        return myPoint;
    }

    /**
    * Operator ==
    *
    */
    bool operator== ( const HyperRectDomain_Iterator<TPointType> &it ) const
    {
        return ( myPoint == ( *it ) );
    }

    /**
    * Operator !=
    *
    */
    bool operator!= ( const HyperRectDomain_Iterator<TPointType> &aIt ) const
    {
        return ( myPoint != ( *aIt ) );
    }

    /**
    * Implements the next() method to scan the domain points dimension by dimension
    * (lexicographic order).
    *
    **/
    void next()
    {
        if ( myPoint.at ( myCurrentPos )  < myupper.at ( myCurrentPos ) )
            myPoint.at ( myCurrentPos ) ++;
        else
        {
            while ( ( myCurrentPos < myPoint.dimension() ) &&
                    ( myPoint.at ( myCurrentPos )  >=  myupper.at ( myCurrentPos ) ) )
            {
                myPoint.at ( myCurrentPos ) = mylower.at ( myCurrentPos );
                myCurrentPos++;
            }

            if ( myCurrentPos < myPoint.dimension() )
            {
                myPoint.at ( myCurrentPos ) ++;
                myCurrentPos = 0;
            }
            else
            {
                myPoint = myupper;
            }
        }
    }

    /**
    * Operator ++ (++it)
    *
    */
    HyperRectDomain_Iterator<TPointType> &operator++()
    {
        this->next();
        return *this;
    }

    /**
    * Operator ++ (it++)
    *
    */
    HyperRectDomain_Iterator<TPointType> &operator++ ( int )
    {
        HyperRectDomain_Iterator<TPointType> tmp = *this;
        ++*this;
        return tmp;
    }


    /**
    * Implements the prev() method to scan the domain points dimension by dimension
    * (lexicographic order).
    *
    **/
    void prev()
    {
        if ( myPoint.at ( myCurrentPos )  > mylower.at ( myCurrentPos ) )
            myPoint.at ( myCurrentPos ) --;
        else
        {
            while ( ( myCurrentPos >= 0 ) &&
                    ( myPoint.at ( myCurrentPos )  <=  mylower.at ( myCurrentPos ) ) )
            {
                myPoint.at ( myCurrentPos ) = myupper.at ( myCurrentPos );
                myCurrentPos++;
            }

            if ( myCurrentPos >= 0 )
            {
                myPoint.at ( myCurrentPos ) --;
                myCurrentPos = 0;
            }
            else
            {
                myPoint = mylower;
            }
        }

    }

    /**
    * Operator ++ (++it)
    *
    */
    HyperRectDomain_Iterator<TPointType> &operator--()
    {
        this->prev();
        return *this;
    }

    /**
    * Operator ++ (it++)
    *
    */
    HyperRectDomain_Iterator<TPointType> &operator-- ( int )
    {
        HyperRectDomain_Iterator<TPointType> tmp = *this;
        --*this;
        return tmp;
    }


private:
    ///Current Point in the domain
    TPointType myPoint;
    ///Copies of the Domain limits
    TPointType mylower, myupper;
    ///Second index of the iterator position
    std::size_t myCurrentPos;
};


} //namespace
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined HyperRectDomain_Iterator_h

#undef HyperRectDomain_Iterator_RECURSES
#endif // else defined(HyperRectDomain_Iterator_RECURSES)