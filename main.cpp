#include <iostream>
#include <bitset>
#include <cstdlib>
#include <cstring>
#include "Maze.h"

using namespace std;

int main(int argc, char* argv[])
{
	bool two_dim = false;
	bool three_dim = false;

	if((argc < 4) || (strcmp("-h", argv[1]) == 0) || (strcmp("--help", argv[1]) == 0)) {
		cout << "Usage: " << argv[0] << " <2d|3d> <x> <y> <z>" << endl;
		cout << "\t<z> only needs to be included if you specify 3d" << endl;
		return 0;
	}

	if(strcmp("2d", argv[1]) == 0)
		two_dim = true;
	else if(strcmp("3d", argv[1]) == 0)
		three_dim = true;

	if(!two_dim && !three_dim) {
		cout << "Error: You must choose either a 2d or 3d maze" << endl;
		return 0;
	}

	int x = atoi(argv[2]);
	int y = atoi(argv[3]);
	int z;
	if(!two_dim) {
		z = atoi(argv[4]);
	} else {
		z = y;
		y = 1;
	}

	Maze m = Maze(x, y, z);
	m.print();
	m.toFile("test.maz");
	
	Maze n = Maze("test.maz");
	n.print();
	n.deduplicateWalls();
	n.printData();
	return 0;
}
