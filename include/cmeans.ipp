
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
inline const std::unordered_set<const T *>& Cluster<T>::elements() const
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

    // Initialize the binary matrix with zeros
    std::unordered_map<const T *, Cluster<T> *> assignedTo;
    for (const auto& request : requests)
        assignedTo[&request] = nullptr;

    auto assign = [&assignedTo](
        typename std::unordered_map<const T *, Cluster<T> *>::iterator urgent,
        Cluster<T> * cluster
    )
    {
        if (urgent->second)
            urgent->second->_elements.erase(urgent->first);

        (assignedTo[urgent->first] = cluster)->_elements.insert(urgent->first);
    };

    bool converged = false;
    while (!converged)
    {
        converged = true;

        for (const auto& request : requests)
        {
            // Calculate the "cost" to each
            // of the k clusters and arrange it in sorted order
            heap<Cluster<T> *> candidates(
                clusters->size(),
                [&](const Cluster<T> * A, const Cluster<T> * B)
                {
                    return cost(A->_centroid, request) < cost(B->_centroid, request);
                }
            );

            for (auto& cluster : *clusters)
                candidates.push(&cluster);

            Cluster<T> * nearest;
            while (candidates.pop(nearest))
            {
                if (nearest->_elements.size() >= capacity)
                    if (assignedTo[&request] == nearest)
                        break;
                    else
                        continue;

                // Group all unassigned requesters as G with m
                // as their nearest centroid
                // Calculate the priority value for r i ∈ G
                auto priority = [&](
                    const std::pair<const T *, Cluster<T> *>& A,
                    const std::pair<const T *, Cluster<T> *>& B
                )
                {
                    double pa = std::numeric_limits<double>().max();
                    double pb = std::numeric_limits<double>().max();
                    
                    if (!A.second)
                        pa = cost(*(A.first), nearest->_centroid) / demand(*(A.first));

                    if (!B.second)
                        pb = cost(*(B.first), nearest->_centroid) / demand(*(B.first));

                    return pa < pb;
                };

                // Assign r i ∈ G to their nearest centroid based
                // on the priority value without violating the constraint
                // Update xij
                assign(
                    std::min_element(assignedTo.begin(), assignedTo.end(), priority),
                    nearest
                );

                // if r i is not assigned then
                // choose the next nearest centroid
                // end if
                converged = false; break;
            }
        }

        // Calculate the new centroid from the formed clusters
        for (auto& cluster : *clusters)
        {
            cluster._centroid = std::accumulate(
                cluster._elements.begin(),
                cluster._elements.end(),
                T(),
                [](const T& currentCentroid, const T * B)
                {
                    return currentCentroid + *B;
                }
            )
            / static_cast<double>(cluster._elements.size());
        }
    }

    return clusters;
}
