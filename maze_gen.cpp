#include <cmath>
#include <ctime>
#include <iostream>

#define MAZE_SIZE 10
#define CELL_SIZE 20

class Maze;
class Cell;

enum directions {right = 0, left, up, down};
class Cell {
    public:
        bool left_edge = false;
        bool right_edge = false;
        bool up_edge = false;
        bool down_edge = false;

        bool visited = false;
        bool tracked = false;

        bool left_open = false;
        bool right_open = false;
        bool up_open = false;
        bool down_open = false;
        
        void init(int x, int y){
            if (x == MAZE_SIZE)
                right_edge = true;
            if (x == 0)
                left_edge = true;
            if (y == MAZE_SIZE)
                down_edge = true;
            if (y == 0)
                up_edge = true;
        }
        void draw(int,int);
};

class Maze {
    private:
        int start_x;
        int start_y;
        Cell maze_map[MAZE_SIZE][MAZE_SIZE];
    public:

        Cell * adjacent(int x, int y,directions d){
            switch (d){
                case right: 
                    if (x < MAZE_SIZE){
                        return &maze_map[x + 1][y];
                    }
                    else {
                        return &maze_map[MAZE_SIZE][y];
                    }
                case left:
                    if (x > 0){
                        return &maze_map[x - 1][y];
                    }
                    else {
                        return &maze_map[0][y];
                    }
                case up:
                    if (y > 0){
                        return &maze_map[x][y - 1];
                    }
                    else {
                        return &maze_map[x][0];
                    }
                case down:
                    if (y < MAZE_SIZE){
                        return &maze_map[x][y + 1];
                    }
                    else {
                        return &maze_map[x][MAZE_SIZE];
                    }
            }
        }

        void visit(int x, int y, directions direction){
            Cell * cell = &maze_map[x][y];
            switch(direction){
                case right:
                    cell->right_open = true;
                    break;
                case left:
                    cell->left_open = true;
                    break;
                case down:
                    cell->down_open = true;
                    break;
                case up:
                    cell->up_open = true;
                    break;
            }

            Cell * next_cell =  adjacent(x,y,direction);
            next_cell->visited = true;
            switch(direction){
                case left:
                    next_cell->right_open = true;
                    break;
                case right:
                    next_cell->left_open = true;
                    break;
                case up:
                    next_cell->down_open = true;
                    break;
                case down:
                    next_cell->up_open = true;
                    break;
            }

        }

        void backtrack(int x, int y, directions d){
            Cell * cell = &maze_map[x][y];
            cell->tracked = true;
            switch(d){
                case right:
                    cell->right_open = true;
                    break;
                case left:
                    cell->left_open = true;
                    break;
                case down:
                    cell->down_open = true;
                    break;
                case up:
                    cell->up_open = true;
                    break;
            }
            Cell * next_cell = adjacent(x,y,d);
            next_cell->tracked = true;
            switch(d){
                case left:
                    next_cell->right_open = true;
                    break;
                case right:
                    next_cell->left_open = true;
                    break;
                case up:
                    next_cell->down_open = true;
                    break;
                case down:
                    next_cell->up_open = true;
                    break;
            }
        }


        void rand_gen(int x,int y){
            maze_map[x][y].visited = true;
            int unvisited_ways [4] {-1,-1,-1,-1};
            for (int i = 0; i < 4; i++){
                Cell * next_cell = adjacent(x,y,(directions)i);
                if(!next_cell->visited){
                    unvisited_ways[i] = (directions)i;
                }
                switch (x) {
                    case MAZE_SIZE:
                        unvisited_ways[right] = -1;
                        break;
                    case 0:
                        unvisited_ways[left] = -1;
                        break;
                }
                switch (y) {
                    case MAZE_SIZE:
                        unvisited_ways[down] = -1;
                        break;
                    case 0:
                        unvisited_ways[up] = -1;
                        break;
                }
            }
            bool no_unvisited = true;
            for (int i : unvisited_ways){
                if ( i != -1) no_unvisited = false;
            }

            if (!no_unvisited){
                int way_index = rand() % 4;
                while(unvisited_ways[way_index] == -1){
                way_index = rand() % 4;
                }
                directions direction = (directions) unvisited_ways[way_index];
                visit(x,y,direction);
                switch (direction){
                    case right:
                        rand_gen(x + 1, y);
                        break;
                    case left:
                        rand_gen(x - 1, y);
                        break;
                    case up:
                        rand_gen(x, y - 1);
                        break;
                    case down:
                        rand_gen(x, y + 1);
                        break;
                }

            }
            else {
                bool no_untracked = true;
                int untracked_ways [4] {-1,-1,-1,-1};
                for (int d = 0; d < 4; d++){
                    Cell * next_cell = adjacent(x,y, (directions)d);
                    if (!next_cell->tracked){
                        untracked_ways[d] = (directions)d;
                        std::cout << "There is some untracked path, i am tagging it"
                            << untracked_ways[d]<< std::endl;
                    }
                }
                /*
                switch (x){
                    case MAZE_SIZE:
                        untracked_ways[right] = -1;
                        break;
                    case 0:
                        untracked_ways[left] = -1;
                        break;
                }
                switch (y){
                    case MAZE_SIZE:
                        untracked_ways[down] = -1;
                        break;
                    case 0:
                        untracked_ways[up] = -1;
                        break;
                }
                */
                
                for (int i : untracked_ways){
                    if (i != -1) no_untracked = false;
                }
                if (!no_untracked){
                    std::cout << "There is some untracked path and i am going one of them" << std::endl;
                    int way_index = rand() % 4;
                    while (untracked_ways[way_index] == -1){
                        way_index = rand() % 4;
                    }
                    directions direction = (directions) untracked_ways[way_index];
                    backtrack(x,y,direction);
                    switch (direction){
                        case right:
                            rand_gen(x + 1, y);
                            break;
                        case left:
                            rand_gen(x - 1, y);
                            break;
                        case up:
                            rand_gen(x, y - 1);
                            break;
                        case down:
                            rand_gen(x, y + 1);
                            break;
                    }
                }
                else {
                    std::cout << "There is no untracked path and i am going returning" << std::endl;
                    return;
                }
                
            }


        }

        Maze(){
            for (int i = 0; i < MAZE_SIZE; i++){
                for (int j = 0; j < MAZE_SIZE; j++){
                    maze_map[i][j].init(i,j);
                }
            }
            srand(time(NULL));
            start_x = rand() % MAZE_SIZE;            
            start_y = rand() % MAZE_SIZE;            
            switch (rand() % 2){
                case 0:
                    start_y = rand() % (MAZE_SIZE - 1) + 1;
                    switch (rand() % 2){
                        case 0:
                            start_x = 0;
                            break;
                        case 1:
                            start_x = MAZE_SIZE;
                            break;
                    }
                    break;
                case 1:
                    start_x = rand() % (MAZE_SIZE - 1) + 1;
                    switch (rand() % 2){
                        case 0:
                            start_y = 0;
                            break;
                        case 1:
                            start_y = MAZE_SIZE;
                            break;
                    }
                    break;
            }

            rand_gen(start_x,start_y);

        }



        void draw(int,int);
};

