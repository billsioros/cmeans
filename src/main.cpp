
#include "cmeans.hpp"
#include "vector2.hpp"
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

template <typename T>
T str2num(const char *);

int main(int argc, char * argv[])
{
    std::size_t NUMBER_OF_POINTS = 10UL, CAPACITY = 6UL, SEED = 1534795927UL;
    double MIN = -50.0, MAX = +50.0;

    if (argc >= 5)
    {
        NUMBER_OF_POINTS = str2num<std::size_t>(argv[1]);
        CAPACITY = str2num<std::size_t>(argv[2]);

        #ifdef __RANDOM__
        SEED = static_cast<unsigned>(std::time(nullptr));
        #endif

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

    auto cost = [](const Vector2& A, const Vector2& B)
    {
        const double xdiff = A.x() - B.x();
        const double ydiff = A.y() - B.y();

        return xdiff * xdiff + ydiff * ydiff;
    };

    auto demand = [](const Vector2& v)
    {
        return 1UL;
    };

    auto clusters = Cluster<Vector2>::cmeans(points, CAPACITY, cost, demand);

    for (const auto& cluster : clusters)
    {
        std::string points;

        points += "(" + std::to_string(cluster.centroid().x());
        points += ", ";
        points += std::to_string(cluster.centroid().y()) + ")";

        for (const auto& element : cluster.elements())
        {
            points += ", ";
            points += "(" + std::to_string(element->x());
            points += ", ";
            points += std::to_string(element->y()) + ")";
        }

        std::cout << "[" + points + "]" << std::endl;
    }

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
