#pragma once

#include "base/types.h"
#include "base/debug.h"

namespace base
{

template<class Type>
class Singleton
{
public:
    template<typename... Args>
    static void init(Args... args) {
        ASSERT( hasInstance() == false );
        instance_ = new Type(args...);
    }
    static void shutdown() {
        ASSERT( hasInstance() == true );
        delete instance_;
        instance_ = nullptr;
    }
    static bool hasInstance() {
        return instance_ != nullptr;
    }
    static Type& instance() {
        ASSERT(hasInstance() == true);
        return *instance_;
    }
protected:
    Singleton() {}

    static Type* instance_;
};

} // namespace base
