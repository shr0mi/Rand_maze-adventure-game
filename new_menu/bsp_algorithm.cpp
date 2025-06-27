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

void bsp(vector<vector<int>>& map, int x, int y, int W, int H);

void horizontal_split(vector<vector<int>>& map, int x, int y, int W, int H){
    //Split the map horizontally
    int x1 = randNum(x+30, x+W-30);
    bsp(map, x, y, x1-x, H);
    bsp(map, x1, y, W+x-x1, H);
    
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

void vertical_split(vector<vector<int>>& map, int x, int y, int W, int H){
    //Split the map vertically
    int y1 = randNum(y+30, y+H-30);
    bsp(map, x, y, W, y1-y);
    bsp(map, x, y1, W, H+y-y1);

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

void bsp(vector<vector<int>>& map, int x, int y, int W, int H){
    if(W>=60 && H>=60){
        int choice = randNum(0, 1); // Randomly choose to split horizontally or vertically
        if(choice == 0){
            horizontal_split(map, x, y, W, H);
        }else{
            vertical_split(map, x, y, W, H);
        }
    }else if(W>=60){
        // Horizontal split only
        horizontal_split(map, x, y, W, H);

    }else if(H>=60){
        // Vertical split only
        vertical_split(map, x, y, W, H);

    }else{
        // Leaf Codes
        //cout << x << " " << y << " " << W << " " << H << "\n";
        //cout << "------------------\n";

        // Draw rooms
        int rand_room_shrink_y = randNum(3, 7);
        int rand_room_shrink_x = randNum(3, 7);
        for(int i=y + rand_room_shrink_y; i< y + H - rand_room_shrink_y;i++){
            for(int j=x + rand_room_shrink_x;j< x + W - rand_room_shrink_x;j++){
                map[i][j] = 1;
            }
        }

        map[randNum(y + rand_room_shrink_y + 1, y + H - rand_room_shrink_y - 2)][randNum(x + rand_room_shrink_x + 1, x + W - rand_room_shrink_x - 2)] = -1;
        map[randNum(y + rand_room_shrink_y + 1, y + H - rand_room_shrink_y - 2)][randNum(x + rand_room_shrink_x + 1, x + W - rand_room_shrink_x - 2)] = -2;
        map[randNum(y + rand_room_shrink_y + 1, y + H - rand_room_shrink_y - 2)][randNum(x + rand_room_shrink_x + 1, x + W - rand_room_shrink_x - 2)] = -3; 

    }
}

//Draw Floors and Paths
void draw_floors_and_paths(vector<vector<int>>& map){
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

vector<vector<int>> BSP_algorithm::generate_bsp_map(int width, int height){
    vector<vector<int>> map(height, vector<int>(width, 0));

    bsp(map, 0, 0, width, height);
    draw_walls(map);
    draw_floors_and_paths(map);
    draw_trees(map);

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
