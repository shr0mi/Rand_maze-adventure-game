#ifndef BSP_ALGORITHM_H
#define BSP_ALGORITHM_H

#include<iostream>
#include<vector>
#include<random>
#include<set>

using namespace std;

class BSP_algorithm {
    bool firstRoom = true;
    public:
        vector<vector<int>> generate_bsp_map(int width, int height);

    private:
        void bsp(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size);
        void horizontal_split(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size);
        void vertical_split(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size);
};

#endif