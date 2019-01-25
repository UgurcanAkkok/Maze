
CXX = g++
CXXFLAGS = -Wall -g
OBJECTS = maze_gen.o draw_maze.o
ALLEGRO_FLAGS = `pkg-config allegro-5 allegro_primitives-5 --libs --cflags`

draw: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(ALLEGRO_FLAGS) -o draw $(OBJECTS)

maze_gen.o: maze_gen.cpp
	$(CXX) $(CXXFLAGS) -c maze_gen.cpp

draw_maze.o: draw_maze.cpp
	$(CXX) $(CXXFLAGS) -c draw_maze.cpp

clean:
	rm $(OBJECTS) draw

