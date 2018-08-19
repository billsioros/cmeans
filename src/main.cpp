
#include "cmeans.hpp"
#include "vector2.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cstdio>
#include <string>

#define SAFE_SSCANF(src, frmt, dst)                         \
do                                                          \
{                                                           \
    if (std::sscanf(src, frmt, dst) == EOF)                 \
    {                                                       \
        std::cerr << "<ERR>: Malformed input" << std::endl; \
        return EXIT_FAILURE;                                \
    }                                                       \
                                                            \
} while (0);                                                \

int main(int argc, char * argv[])
{
    std::size_t NUMBER_OF_POINTS = 10UL, CAPACITY = 3UL, SEED = 123456789UL;
    double MIN = 0.0, MAX = 50.0;

    if (argc >= 5)
    {
        SAFE_SSCANF(argv[1], "%lu", &NUMBER_OF_POINTS);
        SAFE_SSCANF(argv[2], "%lu", &CAPACITY);

        SEED = static_cast<unsigned>(std::time(nullptr));

        SAFE_SSCANF(argv[3], "%lf",  &MIN);
        SAFE_SSCANF(argv[4], "%lf",  &MAX);
    }

    auto frand = [](double min, double max)
    {
        const double fraction = static_cast<double>(std::rand())
                              / static_cast<double>(RAND_MAX);
                              
        return min + fraction * (max - min);
    };

    std::srand(SEED);

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
        std::string xs, ys;

        for (const auto& element : cluster.elements())
        {
            xs += std::to_string(element->x()) + " ";
            ys += std::to_string(element->y()) + " ";
        }

        std::cout << xs << std::endl << ys << std::endl;
    }

    delete clusters;

    return 0;
}
