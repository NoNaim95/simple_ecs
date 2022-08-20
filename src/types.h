//
// Created by deni on 05.08.22.
//

#pragma once

#include <bitset>

using entKey = uint64_t;
using Signature = std::bitset<64>;
using componentId = size_t;

namespace detail{
    static int counter = 0;
}

template<typename T>
struct TypeId{
    inline static componentId id = detail::counter++;
};

namespace Sig{
    template<typename... T>
    static Signature createSig(){
        Signature sig;
        (sig.set(TypeId<T>::id), ...);
        return sig;
    }
    struct Comparer {
        bool operator() (const Signature &b1, const Signature &b2) const {
            return b1.to_ulong() < b2.to_ulong();
        }
    };
}
