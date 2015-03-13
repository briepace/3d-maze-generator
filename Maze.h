#ifndef MAZE_H
#define MAZE_H

#include <string>
#include "Node.h"

using namespace std;

#define FILE_HEADER_SIZE 16
#define FILE_HASH_SIZE 4

class Maze
{
public:
	Maze(unsigned int xVal, unsigned int yVal, unsigned int zVal);
	Maze(char* filename);
	void print();
	void printData();
	void startCreate();
	void draw();
	unsigned int getX();
	unsigned int getY();
	unsigned int getZ();
	unsigned int getStartX();
	unsigned int getStartY();
	unsigned int getStartZ();
	void toFile(char* filename);
	void deduplicateWalls();

	string encode(unsigned char const* bytes_to_encode, unsigned int in_len);
	string decode(string const& encoded_string);
	static const string base64_chars;

private:
	Node ***nodes;
	void create(unsigned int xVal, unsigned int yVal, unsigned int zVal);
	void newCreate(unsigned int xVal, unsigned int yVal, unsigned int zVal);
	bool checkBounds(unsigned int xVal, unsigned int yVal, unsigned int zVal);
	unsigned int xLength, yLength, zLength, startX, startY, startZ, endX, endY, endZ;
	bool valid;
	char* filedata;

};

#endif
