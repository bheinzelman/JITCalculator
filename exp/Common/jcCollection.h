//  jcCollection.h

#pragma once

#include "jcVariable.hpp"

#include <algorithm>

/**
 Interface to describe methods that all collections should have.
 */
class jcCollection
{
public:
    virtual ~jcCollection() {}
    /**
     Return the number of elements in the collection
     */
    virtual size_t size() const = 0;

    /**
     Returns if the collection is empty or not
     */
    virtual bool isEmpty() const
    {
        return size() == 0;
    }

    /**
     Returns the first element in the collection
     */
    virtual jcVariablePtr head() const = 0;

    /**
     Returns the collection with all elements but the first
     */
    virtual jcCollection* tail() const = 0;

    /**
     Returns the current collection and the given collection concatenated
     */
    virtual jcCollection* concat(const jcCollection &other) const = 0;

    /**
     Iterates over the array calling the given callback for every element
     */
    virtual void forEach(std::function<void(jcVariablePtr)> callback) const = 0;

    /**
     Returns a slice of the collection
     */
    virtual jcCollection* slice(int startIdx, int endIdx) const = 0;

    /**
     Returns the jcVariable type of the object
     */

    virtual jcVariable::Type getType() const = 0;
};

