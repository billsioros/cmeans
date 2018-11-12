
#pragma once

#include <unordered_set>    // std::unordered_set
#include <vector>           // std::vector
#include <functional>       // std::function

template <typename T>
class Cluster
{
    T _centroid;
    
    std::unordered_set<const T *> _elements;

public:

    Cluster(const T&);
    Cluster(const Cluster&);
    Cluster(Cluster&&) noexcept;

    Cluster& operator=(const Cluster&);
    Cluster& operator=(Cluster&&) noexcept;

    const T& centroid() const;
    const std::unordered_set<const T *>& elements() const;

    static std::vector<Cluster<T>> cmeans
    (
        const std::vector<T>&,
        std::size_t,
        const std::function<double(const T&, const T&)>&,
        const std::function<std::size_t(const T&)>&
    );
};

#include "cmeans.ipp"
