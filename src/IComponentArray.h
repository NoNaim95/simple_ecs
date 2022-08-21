//
// Created by deni on 11.08.22.
//

#pragma once
#include <cstddef>
#include <memory>

class IComponentArray {
public:
    virtual void removeComponent(size_t index) = 0;
    virtual void transerComponent(IComponentArray& other, size_t idx) = 0;
    virtual std::unique_ptr<IComponentArray> emptyClone() = 0;

    virtual ~IComponentArray() = default;
};