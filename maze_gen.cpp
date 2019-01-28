#include <cmath>
#include <ctime>

#define MAZE_SIZE 20
#define CELL_SIZE 5

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
        void visit(int x, int y, int direction);
        void backtrack(int x, int y, int direction);

        void rand_gen(int x,int y){
            maze_map[x][y].visited = true;
            int unvisited_ways [4] {};
            for (int i = 0; i < 4; i++){
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

        Cell adjacent(int x, int y,directions d){
            switch (d){
                case right: 
                    if (x < MAZE_SIZE){
                        return maze_map[x + 1][y];
                    }
                    else {
                        return maze_map[x][y];
                    }
                case left:
                    if (x > 0){
                        return maze_map[x - 1][y];
                    }
                    else {
                        return maze_map[x][y];
                    }
                case up:
                    if (y > 0){
                        return maze_map[x][y - 1];
                    }
                    else {
                        return maze_map[x][y];
                    }
                case down:
                    if (y < MAZE_SIZE){
                        return maze_map[x][y + 1];
                    }
                    else {
                        return maze_map[x][y];
                    }
            }
        }



        void draw(int,int);
};
