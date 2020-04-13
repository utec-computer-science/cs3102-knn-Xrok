#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <stdio.h>
#include <unistd.h>
#include <set>
#include <math.h>

using namespace std;
template <typename T>
class CartesianCoord
{
public:
    typedef T value_t;
    value_t x;
    value_t y;

public:
    CartesianCoord(value_t _x = 0, value_t _y = 0) : x(_x), y(_y)
    {
    }
    ~CartesianCoord(void) {}

    template <typename _T>
    friend ostream &operator<<(ostream &out, const CartesianCoord<_T> c)
    {
        out << "(" << c.x << "," << c.y << ")";
        return out;
    }
};

typedef CartesianCoord<int> coord_t;

vector<coord_t> knn(int k, vector<coord_t> &points, const coord_t &q)
{ 
  struct distances {
        bool operator()(pair<coord_t, double> pair1, pair<coord_t, double> pair2) {
            return pair1.second < pair2.second;
        }
    };

    set<pair<coord_t,double>, distances> list_ordered;
    
    vector<coord_t>::iterator it = points.begin();

    for(; it != points.end(); it++) {
        if((*it) != q) {
            double distance_between = eucledean_distance(it,q);
            pair<coord_t, double> point_distance = make_pair(*it,distance_between);
            list_ordered.insert(point_distance);
        }
    }

    set<pair<coord_t,double>, distances>::iterator it_list_ord = list_ordered.begin();

    vector<coord_t> k_first_points;

    for (int i = 0; i < k; i++)
    {
        k_first_points.push_back((*it_list_ord).first);
        it_list_ord++;
    }

    return k_first_points;  
}

double eucledean_distance (vector<coord_t>::iterator it , const coord_t &q){
    double x = (*it).x - q.x;
    double y = (*it).y - q.y;
    return pow(x,2) + pow(y,2);
}

int main()
{
    srand(time(NULL));
    vector<coord_t> points;
    for (int i = 0; i < 1000; i++)
        points.push_back(coord_t(rand() % 1000, rand() % 1000));
    vector<coord_t>::iterator it = points.begin();
    for (; it != points.end(); it++)
    {
        fflush(stdout);
        cout << "\r" << *it;
        usleep(2000);
    }
    cout << endl;
    vector<coord_t> knns;
    auto start = chrono::high_resolution_clock::now();
    knns = knn(3, points, coord_t(100, 200));
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time: " << endl;
    cout << duration.count() << endl;
    cout << "knns" << endl;
    vector<coord_t>::iterator kit = knns.begin();
    for (; kit != knns.end(); kit++)
        cout << *kit << endl;
}