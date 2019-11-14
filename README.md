# CMeans

## Example

### **Creating random points**

```cpp
std::size_t NUMBER_OF_POINTS = 10UL, CAPACITY = 6UL, SEED = 1534795927UL;

double MIN = -50.0, MAX = +50.0;

std::srand(SEED);

auto frand = [](double min, double max)
{
    const double fraction = static_cast<double>(std::rand())
                            / static_cast<double>(RAND_MAX);

    return min + fraction * (max - min);
};

std::vector<Vector2> points;

for (std::size_t count = 0UL; count < NUMBER_OF_POINTS; count++)
    points.emplace_back(frand(MIN, MAX), frand(MIN, MAX));
```

### **Clustering the previously generated random points**

```
std::vector<Cluster<Vector2>> clusters = Cluster<Vector2>::cmeans(
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
```

[!alt text](./data/)

## References

[Improved K-Means Algorithm for Capacitated Clustering Problem](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.414.2123&rep=rep1&type=pdf)

