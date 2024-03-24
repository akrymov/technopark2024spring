#include <iostream>
#include <vector>
#include <string>

struct Point;

struct Point {
    int x;
    int y;

    // bool operator<(const Point &r) const {
    //     return x < r.x;
    // }
};

bool operator<(const Point& l, const Point& r) {
    return l.x > r.x;
}




template<typename T>
struct DefaultComparator {
    bool operator()(const T& l, const T& r) const {
        return l < r;
    }
};

template <typename T, typename Comparator=DefaultComparator<T>>
class Heap {
public:
    Heap(Comparator comp = Comparator());
    explicit Heap(size_t size, Comparator comp = Comparator());

    ~Heap();

    Heap(const Heap&) = delete;
    Heap(Heap&&) = delete;

    Heap& operator=(const Heap&) = delete;
    Heap& operator=(Heap &&) = delete;

    const T& top() const;
    void pop();

    void push(const T&);

private:
    void grow();
    void sift_up();
    void sift_down();

    T* buf;
    size_t size;
    size_t capacity;

    Comparator comp;
};


//Heap<int> h;
//h.top() = -1;



struct DefaultComparatorsdfdsfsdfsdf
{
    bool operator()(const Point &l, const Point &r) const
    {
        return l < r;
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
void sort(T *arr, size_t size, Comparator comp = Comparator())
{
    for (size_t i = 0 ; i < size; ++i) {
        for (size_t j = 0; j < size - 1; ++j) {
            if (comp(arr[j + 1], arr[j])) {
                std::swap(arr[j + 1], arr[j]);
            }
        }
    }
}

bool int_comp_less(const int& l, const int& r) {
    return l < r;
}

bool int_comp_more(const int &l, const int &r)
{
    return !(l < r);
}

struct PointFunctor {
    Point base_point;

    PointFunctor(const Point& bp) : base_point(bp) {}

    bool operator()(const Point& l, const Point& r) const {
        return abs(l.x - base_point.x) < abs(r.x - base_point.x);
    }
};




int main() {
    std::vector<int> arr = {1,2,3,4,5,10,9,8,7,6};
    int comp_count = 0;
    sort(&arr[0], arr.size(), [&comp_count](const int&l, const int& r) {
        comp_count++;
        return l < r;
    });

    for (auto c : arr) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    std::cout << "comp_count: " << comp_count << std::endl;

    std::vector<std::string> s_arr = {"1", "2", "3", "10", "9", "8"};
    sort<std::string>(&s_arr[0], s_arr.size());
    for (auto c : s_arr)
    {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    std::vector<Point> p_arr = {{0,0}, {10,10}, {2,2}};
    //sort(&p_arr[0], p_arr.size(), [](const Point& l, const Point& r) {
    //     return l.x < r.x;
    // });
    // PointFunctor fp({-1,-1});
    // sort(&p_arr[0], p_arr.size(), fp);
    Point base_point = {-1, -1};
    sort(&p_arr[0], p_arr.size(), [&base_point](const Point &l, const Point &r)
    { return abs(l.x - base_point.x) < abs(r.x - base_point.x); });

    for (auto c : p_arr)
    {
        std::cout << c.x << " ";
    }
    std::cout << std::endl;

    Point* arr2 = new Point[10];
    return 0;
}
