#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <bitset>
#include <vector>
#include <iomanip>

#include "Maze.h"

using namespace std;

const string Maze::base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

Maze::Maze(unsigned int xVal, unsigned int yVal, unsigned int zVal)
{
	xLength = xVal;
	yLength = yVal;
	zLength = zVal;

	//chooseStartEndPos();
	//cout << "Initializing nodes" << endl;
	nodes = new Node**[xVal];

	for(unsigned int i = 0; i < xVal; i++)
		nodes[i] = new Node*[yVal];

	for(unsigned int i = 0; i < xVal; i++)
		for(unsigned int j = 0; j < yVal; j++)
			nodes[i][j] = new Node[zVal];

	for(unsigned int i = 0; i < xVal; i++)
		for(unsigned int j = 0; j < yVal; j++)
			for(unsigned int k = 0; k < zVal; k++)
				nodes[i][j][k] = Node(i, j, k);

	startCreate();
	valid = true;
	filedata = new char[FILE_HEADER_SIZE + xLength*yLength*zLength + 1];
	memset(filedata, 0, FILE_HEADER_SIZE + xLength*yLength*zLength + 1);
}

Maze::Maze(char* filename) {
	ifstream in_file(filename);
	char header[5];
	memset(header, 0, 5);
	in_file.read(header, 4);
	if(strcmp(header, "MAZE") == 0) {
		in_file.read((char*)&xLength, sizeof(int));
		in_file.read((char*)&yLength, sizeof(int));
		in_file.read((char*)&zLength, sizeof(int));

		char *contents = new char[xLength*yLength*zLength + 1];
		memset(contents, 0, xLength*yLength*zLength + 1);
		nodes = new Node**[xLength];

		for(unsigned int i = 0; i < xLength; i++)
			nodes[i] = new Node*[yLength];

		for(unsigned int i = 0; i < xLength; i++)
			for(unsigned int j = 0; j < yLength; j++)
				nodes[i][j] = new Node[zLength];


		in_file.read(contents, xLength*yLength*zLength);

		char temp;
		for(unsigned int z = 0; z < zLength; z++) {
			for(unsigned int y = 0; y < yLength; y++) {
				for(unsigned int x = 0; x < xLength; x++) {
					temp = contents[x + y*xLength + z*xLength*yLength];
					if((temp & 2) == 2) {
						startX = x;
						startY = y;
						startZ = z;
					} else if((temp & 1) == 1) {
						endX = x;
						endY = y;
						endZ = z;
					}
					nodes[x][y][z] = Node(x, y, z, temp);
				}
			}
		}

		delete contents;
	} else {
		valid = false;
	}

	in_file.close();
}

/*
* Check to ensure that a node is within the bounds of the maze.
*/
bool Maze::checkBounds(unsigned int xVal, unsigned int yVal, unsigned int zVal)
{
	if(xVal >= xLength || xVal < 0 || yVal >= yLength || yVal < 0 || zVal >= zLength || zVal < 0)
		return false;
	return true;
}

void Maze::print()
{
	cout << "{" << endl;
	cout << "\"x_len\":" << xLength << "," << endl;
	cout << "\"y_len\":" << yLength << "," << endl;
	cout << "\"z_len\":" << zLength << "," << endl;
	cout << "\"start_x\":" << startX << "," << endl;
	cout << "\"start_y\":" << startY << "," << endl;
	cout << "\"start_z\":" << startZ << "," << endl;
	cout << "\"end_x\":" << endX << "," << endl;
	cout << "\"end_y\":" << endY << "," << endl;
	cout << "\"end_z\":" << endZ << "," << endl;
	cout << "\"data\":\"" << endl;

	char* temp = new char[zLength*yLength*xLength+1];
	memset(temp, '\0', zLength*yLength*xLength+1);
	int x = 0;
	for(unsigned int k = 0; k < zLength; k++)
	{
		for(unsigned int j = 0; j < yLength; j++)
		{
			for(unsigned int i = 0; i < xLength; i++) {
				//cout << bitset<8>(nodes[i][j][k].getData()).to_string();
				temp[x] = nodes[i][j][k].getData();
				x++;
			}
		}
	}
	cout << encode((unsigned char*)temp, strlen(temp)) << endl;

	cout << "\"}" << endl;
}

/*
 * Print binary representation of each node in the maze.
 * An explanation of the binary data can be found in Node.h
 */
void Maze::printData() {
	for(unsigned int k = 0; k < zLength; k++)
	{
		for(unsigned int j = 0; j < yLength; j++)
		{
			for(unsigned int i = 0; i < xLength; i++) {
				cout << i << ',' << j << ',' << k << ":  " 
                    << bitset<8>(nodes[i][j][k].getData()).to_string() << endl;
			}
		}
	}
}

/*
* Initialize the maze creation process.
*/
void Maze::startCreate()
{
	//cout << "Initializing start and end points" << endl;
	srand((unsigned int)time(NULL));
	int xOryOrz = rand() % 3; //0 = x, 1 = y, 2 = z
	if(xOryOrz == 0)
	{
		startX = (rand() % xLength);
		endX = (rand() % xLength);
		int yOrZ = rand() % 2;
		if(yOrZ == 0)
		{
			startY = (rand() % yLength);
			endY = (rand() % yLength);
			startZ = rand() % 2;
			if(startZ == 1)
			{
				startZ = zLength - 1;
				endZ = 0;
			}
			else
				endZ = zLength - 1;
		}
		else
		{
			startZ = (rand() % zLength);
			endZ = (rand() % zLength);
			startY = rand() % 2;
			if(startY == 1)
			{
				startY = yLength - 1;
				endY = 0;
			}
			else
				endY = yLength - 1;
		}
	}
	else if(xOryOrz == 1)
	{
		startY = (rand() % yLength);
		endY = (rand() % yLength);
		int xOrZ = rand() % 2;
		if(xOrZ == 0)
		{
			startX = (rand() % xLength);
			endX = (rand() % xLength);
			startZ = rand() % 2;
			if(startZ == 1)
			{
				startZ = zLength - 1;
				endZ = 0;
			}
			else
				endZ = zLength - 1;
		}
		else
		{
			startZ = (rand() % zLength);
			endZ = (rand() % zLength);
			startX = rand() % 2;
			if(startX == 1)
			{
				startX = xLength - 1;
				endX = 0;
			}
			else
				endX = xLength - 1;
		}
	}
	else
	{
		startZ = (rand() % zLength);
		endZ = (rand() % zLength);
		int xOrY = rand() % 2;
		if(xOrY == 0)
		{
			startX = (rand() % xLength);
			endX = (rand() % xLength);
			startY = rand() % 2;
			if(startY == 1)
			{
				startY = yLength - 1;
				endY = 0;
			}
			else
				endY = zLength - 1;
		}
		else
		{
			startY = (rand() % yLength);
			endY = (rand() % yLength);
			startX = rand() % 2;
			if(startX == 1)
			{
				startX = xLength - 1;
				endX = 0;
			}
			else
				endX = zLength - 1;
		}
	}

	nodes[startX][startY][startZ] = Node(startX, startY, startZ, Entrance);
	nodes[endX][endY][endZ] = Node(endX, endY, endZ, Exit);
	//create(startX, startY, startZ);
	newCreate(startX, startY, startZ);
}

void Maze::newCreate(unsigned int xVal, unsigned int yVal, unsigned int zVal)
{
	vector<Node*> v_nodes = vector<Node*>();
	v_nodes.push_back(&(nodes[xVal][yVal][zVal]));
	// Seed rand with system time to get random results
	srand((unsigned int)time(NULL));

	int nodeChoice = 0;
	Node *tempNode;
	int countNeighbors;
	int visitedNeighbors;
	int dir;
	int dirPossible[6];
	while(v_nodes.size() < (xLength * yLength * zLength))
	{
		// Choose random Node in list.
		nodeChoice = rand() % v_nodes.size();
		tempNode = v_nodes[nodeChoice];
		xVal = tempNode->getX();
		yVal = tempNode->getY();
		zVal = tempNode->getZ();

		/*
		* If tempNode is end Node and end Node has been visited
		* try another Node.
		*/
		if(xVal == endX &&
			yVal == endY &&
			zVal == endZ)
			continue;

		/*
		* Prepare for checking for dead end.
		*/
		bool neighborNodesExist[6] = {false, false, false, false, false, false};
		countNeighbors = 0;
		if(checkBounds(xVal + 1, yVal, zVal))
		{
			neighborNodesExist[0] = true;
			countNeighbors++;
		}
		if(checkBounds(xVal - 1, yVal, zVal))
		{
			neighborNodesExist[1] = true;
			countNeighbors++;
		}
		if(checkBounds(xVal, yVal + 1, zVal))
		{
			neighborNodesExist[2] = true;
			countNeighbors++;
		}
		if(checkBounds(xVal, yVal - 1, zVal))
		{
			neighborNodesExist[3] = true;
			countNeighbors++;
		}
		if(checkBounds(xVal, yVal, zVal + 1))
		{
			neighborNodesExist[4] = true;
			countNeighbors++;
		}
		if(checkBounds(xVal, yVal, zVal - 1))
		{
			neighborNodesExist[5] = true;
			countNeighbors++;
		}

		// If all neighboring Nodes have been visited
		// pick another Node.
		visitedNeighbors = 0;
		if(neighborNodesExist[0] && nodes[xVal + 1][yVal][zVal].hasBeenVisited())
			visitedNeighbors++;
		if(neighborNodesExist[1] && nodes[xVal - 1][yVal][zVal].hasBeenVisited())
			visitedNeighbors++;
		if(neighborNodesExist[2] && nodes[xVal][yVal + 1][zVal].hasBeenVisited())
			visitedNeighbors++;
		if(neighborNodesExist[3] && nodes[xVal][yVal - 1][zVal].hasBeenVisited())
			visitedNeighbors++;
		if(neighborNodesExist[4] && nodes[xVal][yVal][zVal + 1].hasBeenVisited())
			visitedNeighbors++;
		if(neighborNodesExist[5] && nodes[xVal][yVal][zVal - 1].hasBeenVisited())
			visitedNeighbors++;
		if(visitedNeighbors == countNeighbors)
			continue;


		// Check directions that are possible to add to the list.
		bool canMoveRight = checkBounds(xVal + 1, yVal, zVal) &&
			(nodes[xVal][yVal][zVal].checkRightWall()) &&
			!(nodes[xVal + 1][yVal][zVal].hasBeenVisited());

		bool canMoveLeft = checkBounds(xVal - 1, yVal, zVal) &&
			(nodes[xVal][yVal][zVal].checkLeftWall()) &&
			!(nodes[xVal - 1][yVal][zVal].hasBeenVisited());

		bool canMoveForward = checkBounds(xVal, yVal, zVal + 1) &&
			(nodes[xVal][yVal][zVal].checkFrontWall()) &&
			!(nodes[xVal][yVal][zVal + 1].hasBeenVisited());

		bool canMoveBackward = checkBounds(xVal, yVal, zVal - 1) &&
			(nodes[xVal][yVal][zVal].checkBackWall()) &&
			!(nodes[xVal][yVal][zVal - 1].hasBeenVisited());

		bool canMoveUp = checkBounds(xVal, yVal - 1, zVal) &&
			(nodes[xVal][yVal][zVal].checkUpWall()) &&
			!(nodes[xVal][yVal - 1][zVal].hasBeenVisited());

		bool canMoveDown = checkBounds(xVal, yVal + 1, zVal) &&
			(nodes[xVal][yVal][zVal].checkDownWall()) &&
			!(nodes[xVal][yVal + 1][zVal].hasBeenVisited());
		
		dirPossible[0] = 0;
		dirPossible[1] = 0;
		dirPossible[2] = 0;
		dirPossible[3] = 0;
		dirPossible[4] = 0;
		dirPossible[5] = 0;

		if(canMoveRight)
			dirPossible[0] = 1;
		if(canMoveLeft)
			dirPossible[1] = 1;
		if(canMoveForward)
			dirPossible[2] = 1;
		if(canMoveBackward)
			dirPossible[3] = 1;
		if(canMoveUp)
			dirPossible[4] = 1;
		if(canMoveDown)
			dirPossible[5] = 1;


		dir = rand() % 6;

		if(dirPossible[dir] == 1)
		{
			switch(dir)
			{
			case 0: // Right
				if(canMoveRight)
				{
					// Adding right Node. Remove right wall of current node
					// and left wall of next node.
					nodes[xVal][yVal][zVal].removeWall(RIGHT_WALL);
					nodes[xVal + 1][yVal][zVal].removeWall(LEFT_WALL);
					// Add node to list.
					v_nodes.push_back(&(nodes[xVal + 1][yVal][zVal]));
					//cout << "Node " << xVal + 1 << ", " << yVal << ", " << zVal << " added to list." << endl << endl;
					continue;
				}
				break;
			case 1: // Left
				if(canMoveLeft)
				{
					// Adding left Node. Remove left wall of current node
					// and right wall of next node.
					nodes[xVal][yVal][zVal].removeWall(LEFT_WALL);
					nodes[xVal - 1][yVal][zVal].removeWall(RIGHT_WALL);
					// Add node to list.
					v_nodes.push_back(&(nodes[xVal - 1][yVal][zVal]));
					//cout << "Node " << xVal - 1 << ", " << yVal << ", " << zVal << " added to list." << endl << endl;
					continue;
				}
				break;
			case 2: // Front
				if(canMoveForward)
				{
					// Adding forward Node. Remove front wall of current node
					// and back wall of next node.
					nodes[xVal][yVal][zVal].removeWall(FRONT_WALL);
					nodes[xVal][yVal][zVal + 1].removeWall(BACK_WALL);
					// Add node to list.
					v_nodes.push_back(&(nodes[xVal][yVal][zVal + 1]));
					//cout << "Node " << xVal << ", " << yVal + 1 << ", " << zVal << " added to list." << endl << endl;
					continue;
				}
				break;
			case 3: // Back
				if(canMoveBackward)
				{
					// Adding back Node. Remove back wall of current node
					// and front wall of next node.
					nodes[xVal][yVal][zVal].removeWall(BACK_WALL);
					nodes[xVal][yVal][zVal - 1].removeWall(FRONT_WALL);
					// Add node to list.
					v_nodes.push_back(&(nodes[xVal][yVal][zVal - 1]));
					//cout << "Node " << xVal << ", " << yVal - 1 << ", " << zVal << " added to list." << endl << endl;
					continue;
				}
				break;
			case 4: // Up
				if(canMoveUp)
				{
					// Adding up Node. Remove up wall of current node
					// and down wall of next node
					nodes[xVal][yVal][zVal].removeWall(UP_WALL);
					nodes[xVal][yVal - 1][zVal].removeWall(DOWN_WALL);
					// Add node to list.
					v_nodes.push_back(&(nodes[xVal][yVal - 1][zVal]));
					//cout << "Node " << xVal << ", " << yVal << ", " << zVal + 1 << " added to list." << endl << endl;
					continue;
				}
				break;
			case 5: // Down
				if(canMoveDown)
				{
					// Adding down Node. Remove down wall of current node
					// and up wall of next node
					nodes[xVal][yVal][zVal].removeWall(DOWN_WALL);
					nodes[xVal][yVal + 1][zVal].removeWall(UP_WALL);
					// Add node to list.
					v_nodes.push_back(&(nodes[xVal][yVal + 1][zVal]));
					//cout << "Node " << xVal << ", " << yVal << ", " << zVal - 1 << " added to list." << endl << endl;
					continue;
				}
				break;
			}
		}
	}
}

unsigned int Maze::getX()
{
	return xLength;
}

unsigned int Maze::getY()
{
	return yLength;
}

unsigned int Maze::getZ()
{
	return zLength;
}

unsigned int Maze::getStartX()
{
	return startX;
}

unsigned int Maze::getStartY()
{
	return startY;
}

unsigned int Maze::getStartZ()
{
	return startZ;
}

void Maze::toFile(char* filename) {
	ofstream out_file(filename, ios::binary);

	char *allNodes = new char[xLength*yLength*zLength + 1];
	memset(allNodes, 0, xLength*yLength*zLength + 1);

	out_file << "MAZE";
	out_file.write(reinterpret_cast<const char *>(&xLength), sizeof(xLength));
	out_file.write(reinterpret_cast<const char *>(&yLength), sizeof(yLength));
	out_file.write(reinterpret_cast<const char *>(&zLength), sizeof(zLength));

	for(unsigned int z = 0; z < zLength; z++)
		for(unsigned int y = 0; y < yLength; y++)
			for(unsigned int x = 0; x < xLength; x++)
				allNodes[x + y*xLength + z*xLength*yLength] = nodes[x][y][z].getData();

	out_file << allNodes;

	delete allNodes;
	out_file.close();
}

void Maze::deduplicateWalls() {

	for(unsigned int z = 0; z < zLength-1; z++) {
		for(unsigned int y = 0; y < yLength-1; y++) {
			for(unsigned int x = 0; x < xLength-1; x++) {
				if((nodes[x][y][z].getData() & (((char)1)<<RIGHT_WALL))) {
					nodes[x+1][y][z].removeWall(LEFT_WALL);
				}

				if(nodes[x][y][z].getData() & (((char)1)<<FRONT_WALL)) {
					nodes[x][y+1][z].removeWall(BACK_WALL);
				}

				if(nodes[x][y][z].getData() & (((char)1)<<UP_WALL)) {
					nodes[x][y][z+1].removeWall(DOWN_WALL);
				}
			}
		}
	}
}



static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

string Maze::encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

string Maze::decode(std::string const& encoded_string) {
  size_t in_len = encoded_string.size();
  size_t i = 0;
  size_t j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}
