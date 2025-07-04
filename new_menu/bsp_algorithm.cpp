#include<iostream>
#include<vector>
#include<random>
#include "bsp_algorithm.h"

// Generates a random number between min and max (inclusive)
int randNum(int min, int max){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);

    int randomNum = dist(gen);
    return randomNum;
}

// void bsp(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size);

void BSP_algorithm::horizontal_split(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size){
    //Split the map horizontally
    int x1 = randNum(x+max_room_size, x+W-max_room_size);
    bsp(map, x, y, x1-x, H, max_room_size);
    bsp(map, x1, y, W+x-x1, H, max_room_size);
    
    //Join the centers of two rooms
    int cx1 = x + (x1 - x)/2;
    int cx2 = x1 + (W+x - x1)/2;
    int cy = y + H/2;
    // Draw a line between the two centers
    for(int i=cx1;i<=cx2;i++){
        if(map[cy][i] != 1)
            map[cy][i] = 2;
        else{
            map[cy][i] = 1;
        }
        
    }

}

void BSP_algorithm::vertical_split(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size) {
    //Split the map vertically
    int y1 = randNum(y+max_room_size, y+H-max_room_size);
    bsp(map, x, y, W, y1-y, max_room_size);
    bsp(map, x, y1, W, H+y-y1, max_room_size);

    //Join the centers of two rooms
    int cy1 = y + (y1 - y)/2;
    int cy2 = y1 + (H+y - y1)/2;
    int cx = x + W/2;
    // Draw a line between the two centers
    for(int i=cy1;i<=cy2;i++){
        if(map[i][cx] != 1)
            map[i][cx] = 2;
    }
}


pair<int, int> player_pos;
vector<pair<int, int>> room_centers;

void BSP_algorithm::bsp(vector<vector<int>>& map, int x, int y, int W, int H, int max_room_size){
    if(W>=max_room_size*2 && H>=max_room_size*2){
        int choice = randNum(0, 1); // Randomly choose to split horizontally or vertically
        if(choice == 0){
            horizontal_split(map, x, y, W, H, max_room_size);
        }else{
            vertical_split(map, x, y, W, H, max_room_size);
        }
    }else if(W>=max_room_size*2){
        // Horizontal split only
        horizontal_split(map, x, y, W, H, max_room_size);

    }else if(H>=max_room_size*2){
        // Vertical split only
        vertical_split(map, x, y, W, H, max_room_size);

    }else{
        // Leaf Codes
        //cout << x << " " << y << " " << W << " " << H << "\n";
        //cout << "------------------\n";

        // Draw rooms
        int rand_room_shrink_y = randNum(3, 5);
        int rand_room_shrink_x = randNum(3, 5);
        for(int i=y + rand_room_shrink_y; i< y + H - rand_room_shrink_y;i++){
            for(int j=x + rand_room_shrink_x;j< x + W - rand_room_shrink_x;j++){
                map[i][j] = 1;
            }
        }

        //Room centers
        int room_center_y = (y + rand_room_shrink_y + y + H - rand_room_shrink_y)/2;
        int room_center_x = (x + rand_room_shrink_x + x + W - rand_room_shrink_x)/2;

        if(firstRoom){
            // Set Player Position -> -4 is player position
            cout << room_center_x << " " << room_center_y << endl;
            player_pos.first = room_center_x; player_pos.second = room_center_y;
            firstRoom = false;
        }else{

            //Enemy Generation -> -1 = shooter enemy, -2 = Turrent Enemy, -3 = Exploding Enemy
            if(randNum(1, 2) == 1)
                map[randNum(y + rand_room_shrink_y + 1, y + H - rand_room_shrink_y - 2)][randNum(x + rand_room_shrink_x + 1, x + W - rand_room_shrink_x - 2)] = -1;
            if(randNum(1, 2) == 1)
                map[randNum(y + rand_room_shrink_y + 1, y + H - rand_room_shrink_y - 2)][randNum(x + rand_room_shrink_x + 1, x + W - rand_room_shrink_x - 2)] = -2;
            if(randNum(1, 2) == 1)
                map[randNum(y + rand_room_shrink_y + 1, y + H - rand_room_shrink_y - 2)][randNum(x + rand_room_shrink_x + 1, x + W - rand_room_shrink_x - 2)] = -3;

            room_centers.push_back({room_center_x, room_center_y});
        }

    }
}

//Draw Floors and Paths
void draw_floors_and_paths(vector<vector<int>>& map) {
    int floors[10] = {1, 1, 1, 1, 1, 1, 2, 3, 4, 5};
    for(int i=0;i<map.size();i++){
        for(int j=0;j<map[i].size();j++){
            if(map[i][j] == 1){
                // Draw floor
                map[i][j] = floors[randNum(0, 9)];
            }else if(map[i][j] == 2){
                // Draw path
                map[i][j] = 17; // Path tile
            }
        }
    }
}

// Draw room walls and path walls and trees
void draw_walls(vector<vector<int>>& map){
    for(int i=1;i<map.size()-1;i++){
        for(int j=1;j<map[i].size()-1;j++){
            if(map[i][j] == 0){
                // Draw wall
                if(map[i-1][j] == 1 || map[i+1][j] == 1 || map[i][j-1] == 1 || map[i][j+1] == 1){
                    map[i][j] = 12; // Wall tile
                }
                //Draw path wall
                if(map[i-1][j] == 2 || map[i+1][j] == 2 || map[i][j-1] == 2 || map[i][j+1] == 2){
                    map[i][j] = 638; // Wall tile
                }
            }
        }
    }
}

void draw_trees(vector<vector<int>>& map){
    for(int i=0;i<map.size();i++){
        for(int j=0;j<map[i].size();j++){
            if(map[i][j] == 0){
                // Draw tree
                int trees[21] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 50, 51, 52, 53, 54, 55};
                map[i][j] = trees[randNum(0, 20)];
            }
        }
    }
}

//Position Keys and Chest in the map
void draw_keys_and_chest(vector<vector<int>>& map){
    set<int> s;
    while(s.size() < 4){
        s.insert(randNum(0, room_centers.size()-1));
    }
    // place keys and Chest in Map -> -5=keys and -6=chest
    int k=1;
    for(auto i: s){
        if(k<=3){
            map[room_centers[i].second][room_centers[i].first] = -5;
            k++;
        }else{
            map[room_centers[i].second][room_centers[i].first] = -6;
        }
    }
}

vector<vector<int>> BSP_algorithm::generate_bsp_map(int width, int height){
    vector<vector<int>> map(height, vector<int>(width, 0));

    bsp(map, 0, 0, width, height, 17);
    draw_walls(map);
    draw_floors_and_paths(map);
    draw_trees(map);
    draw_keys_and_chest(map);
    map[player_pos.second][player_pos.first] = -4;
    for(int i=0;i<map.size();i++){
        for(int j=0;j<map[i].size();j++){
            if(map[i][j] == -4){
                cout << "found it" << endl;
                break;
            }
        }
    }

    return map;
}



/*
int main(){
    
    int width = 200, height = 200;
    vector<vector<int>> map(height, vector<int>(width, 0));

    bsp(map, 0, 0, width, height);
    //draw_walls(map);
    //draw_floors_and_paths(map);
    
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
    

}
*/
