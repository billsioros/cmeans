
#include "cmeans.hpp"
#include "vector2.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#define NUMBER_OF_POINTS (50UL)
#define MIN (-0.0)
#define MAX (+50.0)
#define CAPACITY (7UL)
#define SEED (123456789UL)

int main()
{
    auto frand = [](double min, double max)
    {
        const double fraction = static_cast<double>(std::rand())
                              / static_cast<double>(RAND_MAX);
                              
        return min + fraction * (max - min);
    };

    #ifndef SEED
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    #else
    std::srand(SEED);
    #endif

    std::vector<Vector2> points;
    for (std::size_t count = 0UL; count < NUMBER_OF_POINTS; count++)
        points.emplace_back(frand(MIN, MAX), frand(MIN, MAX));

    const std::vector<Cluster<Vector2>> * clusters = Cluster<Vector2>::cmeans(
        points,
        CAPACITY,
        [](const Vector2& A, const Vector2& B)
        {
            const double xdiff = A.x() - B.x();
            const double ydiff = A.y() - B.y();

            return xdiff * xdiff + ydiff * ydiff;
        },
        [](const Vector2& v) { return 1UL; }
    );

    for (const auto& cluster : *clusters)
    {
        for (const auto& element : cluster.elements())
            std::cout << *element << " ";

        std::cout << std::endl;
    }

    delete clusters;

    return 0;
}
