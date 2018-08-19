
#pragma once

#include "heap.hpp"
#include <algorithm>        // std::min_element
#include <limits>           // std::numeric_limits
#include <numeric>          // std::accumulate
#include <unordered_set>    // std::unordered_set
#include <unordered_map>    // std::unordered_map
#include <vector>           // std::vector
#include <functional>       // std::function
#include <cmath>            // std::ceil
#include <utility>          // std::pair

// Acknowledgements:
// Improved K-Means Algorithm for Capacitated Clustering Problem by
// S.GEETHA
// G.POONTHALIR
// P.T.VANATHI

template <typename T>
inline Cluster<T>::Cluster(const T& _centroid)
:
_centroid(_centroid)
{
}

template <typename T>
inline Cluster<T>::Cluster(const Cluster& other)
:
_centroid(other._centroid), _elements(other._elements)
{
}

template <typename T>
inline const T& Cluster<T>::centroid() const
{
    return _centroid;
}

template <typename T>
inline const Set<T>& Cluster<T>::elements() const
{
    return _elements;
}

template <typename T>
const std::vector<Cluster<T>> * Cluster<T>::cmeans
(
    const std::vector<T>& requests,
    std::size_t capacity,
    const std::function<double(const T&, const T&)>& cost,
    const std::function<std::size_t(const T&)>& demand
)
{
    auto priority = [&cost, &demand](const T& request, const Cluster<T>& cluster)
    {
        return demand(request) / cost(request, cluster._centroid);
    };

    // Calculate k
    const std::size_t k = static_cast<std::size_t>(
        std::ceil(
            static_cast<double>(
                std::accumulate(
                    requests.begin(),
                    requests.end(),
                    0UL,
                    [&](std::size_t currentSum, const T& request)
                    {
                        return currentSum + demand(request);
                    }
                )
            )
            / static_cast<double>(capacity)
        )
    );

    // Select k first centroids
    std::vector<Cluster<T>> * clusters = new std::vector<Cluster<T>>;
    for (std::size_t i = 0; i < k; i++)
        clusters->emplace_back(requests[i]);

    auto assign = [&clusters](const T * request, Cluster<T> * dst)
    {
        for (auto& cluster : *clusters)
        {
            typename Set<T>::iterator it;
            if ((it = cluster._elements.find(request)) != cluster._elements.end())
                cluster._elements.erase(it);
        }

        dst->_elements.insert(request);
    };

    bool converged = false;
    while (!converged)
    {
        converged = true;

        // Initialize the binary matrix with zeros
        Set<T> unassigned;
        for (const auto& request : requests)
            unassigned.insert(&request);

        for (const auto& request : requests)
        {
            // Calculate the "cost" to each
            // of the k clusters and arrange it in sorted order
            std::sort(
                clusters->begin(),
                clusters->end(),
                [&cost, &request](const Cluster<T>& A, const Cluster<T>& B)
                {
                    return cost(A._centroid, request) < cost(B._centroid, request);
                }
            );

            for (auto& cluster : *clusters)
            {
                // Group all unassigned requesters as G with m
                // as their nearest centroid
                // Calculate the priority value for r i ∈ G
                typename Set<T>::iterator urgent = std::max_element(
                    unassigned.begin(),
                    unassigned.end(),
                    [&priority, &cluster](const T * A, const T * B)
                    {
                        const double pa = priority(*A, cluster._centroid);
                        const double pb = priority(*B, cluster._centroid);

                        return pa > pb;
                    }
                );

                // Assign r i ∈ G to their nearest centroid based
                // on the priority value without violating the constraint
                // Update xij
                if (cluster._elements.find(*urgent) != cluster._elements.end())
                {
                    unassigned.erase(*urgent);
                }
                else
                {
                    if (cluster._elements.size() < capacity)
                    {
                        unassigned.erase(*urgent);

                        assign(*urgent, &cluster);
                        
                        converged = false;
                    }
                }
                
                // if r i is not unassigned then
                // choose the next nearest centroid
                // end if
                if (unassigned.find(&request) == unassigned.end())
                    break;
            }
        }

        // Calculate the new centroid from the formed clusters
        for (auto& cluster : *clusters)
        {
            cluster._centroid = std::accumulate(
                cluster._elements.begin(),
                cluster._elements.end(),
                T(),
                [&cluster](const T& currentCentroid, const T * B)
                {
                    return currentCentroid + (*B / static_cast<double>(cluster._elements.size()));
                }
            );
        }
    }

    return clusters;
}
