#include <iostream>
#include <memory>

using namespace std;

template <typename Derived>
class Geometry
{
public:
    int area()
    {
        return static_cast<Derived*>(this)->area();
    }
};

class Rectangle : public Geometry<Rectangle>
{
    int width, height;

public:
    Rectangle(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    int area()
    {
        return width * height;
    }
};

template<typename T>
void print_area(shared_ptr<Geometry<T>> geometry)
{
    cout << "Area is " << geometry->area() << endl;
}

int main(int argc, char *argv[])
{
    cout << " Test is getting started ..." << endl;
    shared_ptr<Geometry<Rectangle>> g = make_shared<Rectangle>(10, 5);

    print_area(g);

    return 0;
}