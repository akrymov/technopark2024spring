#include <iostream>
#include <algorithm>
#include <string>

template <typename T>
struct DefaultComparator
{
    bool operator()(const T &l, const T &r) const
    {
        return l < r;
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
int partition(T *arr, int l, int r, Comparator comp = Comparator())
{
    int pivot_idx = l + (r - l) / 2;

    std::swap(arr[pivot_idx], arr[r - 1]);
    T& pivot = arr[r - 1];

    int i = l;
    int j = r - 1;
    while (i < j) {
        for (; comp(arr[i], pivot); ++i);
        // arr[i] >= pivot

        while( !comp(arr[j], pivot) ) j--;
        //arr[j] < pivot

        if (i < j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    std::swap(arr[i], arr[r -  1]);
    return i;
}

template <typename T, typename Comparator = DefaultComparator<T>>
void kth_element(T *arr, int size, int k, Comparator comp = Comparator())
{
    int l = 0;
    int r = size;

    while(l <= r) {
        int m = partition(arr, l, r);
        if (k == m) {
            return;
        } else if (k < m) {
            r = m;
        } else { // k > m
            l = m + 1;
        }
    }
}

void unstable_count_sort(int* arr, int size, int R) {
    int* counters = new int[R + 1];
    for (int i = 0; i < R + 1; ++i) {
        counters[i] = 0;
    }
    for (int i = 0; i < size; ++i) {
        counters[ arr[i] ]++;
    }
    int arr_idx = 0;
    for (int counter_idx = 0; counter_idx <= R; counter_idx++) {
        int counter = counters[counter_idx];
        while (counter > 0) {
            arr[arr_idx] = counter_idx;

            counter--;
            arr_idx++;
        }
    }
    delete[] counters;
}

void stable_count_sort(int *arr, int size, int R)
{
    int counters_size = R + 1;
    int *counters = new int[counters_size];
    for (int i = 0; i < counters_size; ++i)
    {
        counters[i] = 0;
    }
    for (int i = 0; i < size; ++i)
    {
        counters[arr[i]]++;
    }
    for (int i = 0; i < counters_size - 1; ++ i) {
        counters[i + 1] += counters[i];
    }

    int* temp = new int[size];
    for (int i = size - 1; i >= 0; --i) {
        temp[--counters[arr[i]]] = arr[i];
    }
    for (int i = 0; i < size; i++) {
        arr[i] = temp[i];
    }
    delete[] temp;
    delete[] counters;
}

struct Person {
    std::string name;
    int year;

    // Person& operator=(const Person& p) {
    //     name = p.name;
    //     year = p.year;
    //     return *this;
    // }
};

template<typename T>
void count_sort(T *arr, int size, int R, int (*key)(const T&))
{
    int counters_size = R + 1;
    int *counters = new int[counters_size];
    for (int i = 0; i < counters_size; ++i)
    {
        counters[i] = 0;
    }
    for (int i = 0; i < size; ++i)
    {
        counters[key(arr[i])]++;
    }
    for (int i = 0; i < counters_size - 1; ++i)
    {
        counters[i + 1] += counters[i];
    }

    T *temp = new T[size];
    for (int i = size - 1; i >= 0; --i)
    {
        temp[--counters[key(arr[i])]] = arr[i];
    }
    for (int i = 0; i < size; i++)
    {
        arr[i] = temp[i];
    }
    delete[] temp;
    delete[] counters;
}

void testFunc(std::vector<int>& arr, int k) {
    kth_element(&arr[0], arr.size(), k);
    int k_value = arr[k];

    std::sort(arr.begin(), arr.end());
    int sort_value = arr[k];
    assert(sort_value == k_value);

    std::cout << arr[k] << std::endl;
}

int main()
{
    {
        std::vector<int> arr = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        testFunc(arr, 0);
    }

    {
        std::vector<int> arr = {1};
        testFunc(arr, 0);
    }
    {
        std::vector<int> arr = {1,1,1,1,1};
        testFunc(arr, 0);
    }
    {
        std::vector<int> arr = {3,1,6,6};
        testFunc(arr, 3);
    }

    {
        std::vector<int> arr = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        unstable_count_sort(&arr[0], arr.size(), 9);

        for (auto c : arr) {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    {
        std::vector<int> arr = {0, 0, 0, 0, 100000, 5, 6};
        unstable_count_sort(&arr[0], arr.size(), 100000);

        for (auto c : arr)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    {
        std::vector<int> arr = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        stable_count_sort(&arr[0], arr.size(), 9);

        for (auto c : arr)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    {
        std::vector<int> arr = {0, 0, 0, 0, 100000, 5, 6};
        stable_count_sort(&arr[0], arr.size(), 100000);

        for (auto c : arr)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    {
        std::vector<int> arr = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        count_sort<int>(&arr[0], arr.size(), 9, [](const int& v) { return v; });

        for (auto c : arr)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    {
        std::vector<int> arr = {0, 0, 0, 0, 100000, 5, 6};
        count_sort<int>(&arr[0], arr.size(), 100000, [](const int &v){ return v; });

        for (auto c : arr)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    {
        std::vector<Person> arr = {
            {"Masha", 1900},
            {"Ivan", 2020},
            {"Olga", 860},
            {"Oleg", 1900},
        };
        count_sort<Person>(&arr[0], arr.size(), 2020, [](const Person &v)
        {
            return v.year;
        });

        for (auto c : arr)
        {
            std::cout << c.year << " : " << c.name << " | ";
        }
        std::cout << std::endl;
    }

    std::cout << "!!" << std::endl;


    std::vector<int> arr;
    int input = 0;
    // while(std::cin >> input) {
    //     arr.push_back(input);
    // }

    while (true) {
        std::cin >> input;
        if (std::cin.fail()) {
            break;
        }
        arr.push_back(input);
    }



    std::cout << "done" << std::endl;
}
