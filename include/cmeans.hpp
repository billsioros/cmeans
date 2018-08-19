
#pragma once

#include <unordered_set>    // std::unordered_set
#include <vector>           // std::vector
#include <functional>       // std::function

template <typename T>
using Set = std::unordered_set<const T *>;

template <typename T>
class Cluster
{
    T _centroid;
    
    Set<T> _elements;

public:

    Cluster(const T&);
    Cluster(const Cluster&);

    const T& centroid() const;
    const Set<T>& elements() const;

    static const std::vector<Cluster<T>> * cmeans
    (
        const std::vector<T>&,
        std::size_t,
        const std::function<double(const T&, const T&)>&,
        const std::function<std::size_t(const T&)>&
    );
};

#include "cmeans.ipp"
