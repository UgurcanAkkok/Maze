#include <cmath>
#include <ctime>
#include <iostream>
#include <stack>

#define MAZE_SIZE 20
#define CELL_SIZE 20
#define DEBUG false

class Maze;
class Cell;

enum directions {right = 0, left, up, down};
class Cell {
    public:

        bool visited = false;

        bool left_open = false;
        bool right_open = false;
        bool up_open = false;
        bool down_open = false;
        
        int x,y;
        
        void init(int x, int y){
            this->x = x;
            this->y = y;
        }
        void draw(int,int);
};

class Maze {
    private:
        int start_x;
        int start_y;
        int end_x;
        int end_y;
        Cell maze_map[MAZE_SIZE][MAZE_SIZE];
        std::stack <Cell> track_stack;
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
                default:
                    return &maze_map[x][y];
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

        void rand_gen(int x,int y){
            maze_map[x][y].visited = true;
            bool all_cells_visited = true;
            for (int i = 0; i < MAZE_SIZE; i++){
                for (int j = 0; j < MAZE_SIZE; j++){
                    if (maze_map[i][j].visited == false)
                        all_cells_visited = false;
                }
            }
            if (all_cells_visited == false){
                int unvisited_ways [4] {-1,-1,-1,-1};
                for (int i = 0; i < 4; i++){
                    Cell * next_cell = adjacent(x,y,(directions)i);
                    if(!next_cell->visited){
                        unvisited_ways[i] = (directions)i;
                    }
                    if (x >= MAZE_SIZE -1){
                        unvisited_ways[right] = -1;
                    }
                    else if (x <= 0){
                        unvisited_ways[left] = -1;
                    }
                    if ( y >= MAZE_SIZE -1){
                        unvisited_ways[down] = -1;
                    }
                    else if ( y <= 0){
                        unvisited_ways[up] = -1;
                    }
                }
                bool all_adj_visited = true;
                for (int i : unvisited_ways){
                    if ( i != -1) all_adj_visited = false;
                }

                if (!all_adj_visited){
                    int way_index = rand() % 4;
                    while(unvisited_ways[way_index] == -1){
                    way_index = rand() % 4;
                    }
                    directions direction = (directions) unvisited_ways[way_index];
                    if (DEBUG) printf("Visiting from %i, %i to %i \n",x,y,direction);
                    track_stack.push(maze_map[x][y]);
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
                    Cell next_cell = track_stack.top();
                    track_stack.pop();
                    rand_gen(next_cell.x,next_cell.y);
                }
            }
            else {
                return;
            }

        }

        Maze(){
            for (int i = 0; i < MAZE_SIZE; i++){
                for (int j = 0; j < MAZE_SIZE; j++){
                    maze_map[i][j].init(i,j);
                }
            }
            srand(time(NULL));
            switch (rand() % 2){
                case 0:
                    start_y = (rand() % (MAZE_SIZE - 1)) + 1;
                    switch (rand() % 2){
                        case 0:
                            start_x = 0;
                            break;
                        case 1:
                            start_x = MAZE_SIZE - 1;
                            break;
                    }
                    break;
                case 1:
                    start_x = 1 +(rand() % (MAZE_SIZE - 1));
                    switch (rand() % 2){
                        case 0:
                            start_y = 0;
                            break;
                        case 1:
                            start_y = MAZE_SIZE - 1;
                            break;
                    }
                    break;
            }

            if (DEBUG) std::cout << "initial x: " << start_x << "initial y: " << start_y  << std::endl;
            rand_gen(start_x,start_y);

        }



        void draw(int,int);
};

