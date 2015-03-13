# 3d-maze-generator
This is a 3D maze generator written in C++. It was written in about 2008 I believe.

The algorithm used is a 2D algorithm adjusted for a 3rd dimension. It can be compiled with the command

```
g++ -o main Node.cpp Maze.cpp main.cpp
```

The mazes generated are true 3D mazes in that at each point in the maze there is the possibility of being able to go forward, backward, left, right, up, or down.