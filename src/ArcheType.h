//
// Created by deni on 05.08.22.
//

#pragma once
#include <vector>
#include <map>
#include <optional>
#include <queue>
#include <memory>
#include "IComponentArray.h"
#include "types.h"

class ArcheType;

struct Record{
    ArcheType* archeType;
    size_t row;
};


class ArcheType {
public:
    ArcheType(Signature sig): sig(sig){}

    template<typename... T>
    static ArcheType create();

    template<typename... T>
    [[nodiscard]] std::tuple<T&...> getComponents(entKey entity);

    template<typename... T>
    [[nodiscard]] std::tuple<const T&...> getComponents(entKey entity) const;

    void transferEntity(entKey entity, ArcheType& other);

    template<typename... T>
    void addEntity(entKey entity, T&&... components);

    void removeEntity(entKey entity);

    [[nodiscard]] const Signature& getSig() const;
private:
    Signature sig;
    std::map<componentId, std::unique_ptr<IComponentArray>> arrays;
    std::vector<entKey> entities;
    std::map<entKey, size_t> indices;
};