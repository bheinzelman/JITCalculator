//  jcString.hpp

#pragma once

#include "jc.h"
#include "jcCollection.h"

#include <string>
#include <memory>

class jcString : public jcCollection {
public:

    enum Context
    {
        StringContextValue,
        StringContextId
    };

    jcString(const std::string &value, Context ctx);

    static jcStringPtr Create(const std::string &value, Context ctx);

    const std::string& asStdString() const;

    bool equal(const jcString &other) const;

    // TODO, fix this
    Context getContext() const
    {
        return mCtx;
    }

    /**
     jcCollection methods
     */
    size_t size() const override;
    jcVariablePtr head() const override;
    jcCollection* concat(const jcCollection &other) const override;
    void forEach(std::function<void(jcVariablePtr&)> callback) const override;
    jcCollection* slice(int startIdx, int endIdx) const override;
    jcVariable::Type getType() const override;

private:
    std::string mData;
    Context mCtx;
};
