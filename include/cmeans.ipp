
#pragma once

#include <algorithm>        // std::min_element, std::sort
#include <limits>           // std::numeric_limits
#include <numeric>          // std::accumulate
#include <unordered_set>    // std::unordered_set
#include <vector>           // std::vector
#include <functional>       // std::function
#include <cmath>            // std::ceil

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
    Set<T> unassigned;
    for (const auto& request : requests)
        unassigned.insert(&request);

    bool converged = false;
    while (!converged)
    {
        converged = true;

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
                // if r i is not unassigned then
                // choose the next nearest centroid
                // end if
                if (unassigned.find(&request) == unassigned.end())
                    break;

                if (cluster._elements.size() >= capacity)
                    continue;

                // Group all unassigned requesters as G with m
                // as their nearest centroid
                // Calculate the priority value for r i ∈ G
                typename Set<T>::iterator urgent = std::min_element(
                    unassigned.begin(),
                    unassigned.end(),
                    [&](const T * A, const T * B)
                    {
                        const double pa = cost(*A, cluster._centroid) / demand(*A);
                        const double pb = cost(*B, cluster._centroid) / demand(*B);

                        return pa < pb;
                    }
                );

                // Assign r i ∈ G to their nearest centroid based
                // on the priority value without violating the constraint
                // Update xij
                unassigned.erase(*urgent);

                cluster._elements.insert(*urgent); converged = false;
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
