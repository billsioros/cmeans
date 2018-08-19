
#include "cmeans.hpp"
#include "vector2.hpp"
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cstdio>
#include <string>

template <typename T>
T str2num(const char *);

int main(int argc, char * argv[])
{
    std::size_t NUMBER_OF_POINTS = 5UL, CAPACITY = 3UL, SEED = 123456789UL;
    double MIN = 0.0, MAX = 50.0;

    if (argc >= 5)
    {
        NUMBER_OF_POINTS = str2num<std::size_t>(argv[1]);
        CAPACITY = str2num<std::size_t>(argv[2]);

        SEED = static_cast<unsigned>(std::time(nullptr));

        MIN = str2num<double>(argv[3]);
        MAX = str2num<double>(argv[4]);
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

template <typename T>
T str2num(const char * str)
{
    std::stringstream ss(str);

    T num;
    if (ss >> num)
    {
        return num;
    }
    else
    {
        std::cerr << "<ERR>: Malformed arguement (" << str << ")" << std::endl;
        std::exit(EXIT_FAILURE);                                
    }
}
