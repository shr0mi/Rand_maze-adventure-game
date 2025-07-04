#ifndef BSP_ALGORITHM_H
#define BSP_ALGORITHM_H

#include<iostream>
#include<vector>
#include<random>
#include<set>

using namespace std;

class BSP_algorithm {
    public:
        vector<vector<int>> generate_bsp_map(int width, int height);
};

#endif