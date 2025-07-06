#ifndef BSP_ALGORITHM_H
#define BSP_ALGORITHM_H

#include<iostream>
#include<vector>
#include<random>
#include<set>

using namespace std;

class BSP_algorithm {
    bool firstRoom = true;
    pair<int, int> player_pos;
    vector<pair<int, int>> room_centers;
    public:
        vector<vector<int>> generate_bsp_map(int width, int height);

    private:
        int randNum(int min, int max);
        void bsp(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size);
        void horizontal_split(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size);
        void vertical_split(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size);
        void draw_keys_and_chest(vector<vector<int>>& map);
        void draw_floors_and_paths(vector<vector<int>>& map);
        void draw_walls(vector<vector<int>>& map);
        void draw_trees(vector<vector<int>>& map);
};

#endif