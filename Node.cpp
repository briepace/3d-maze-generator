#include <stdio.h>
#include <iostream>
#include <string>

#include "Node.h"

using namespace std;

Node::Node() {}

/*
* This constructor creates a node when you don't know the
* entrance, exit or neither status. Node initialized as 
* neither an entrance nor an exit by default.
*/
Node::Node(int xNPos, int yNPos, int zNPos)
{
	xPos = xNPos;
	yPos = yNPos;
	zPos = zNPos;

	// 0xFC = 0b11111100
	// All walls initialized to 1

	data = 0xFC;
}

/*
* This constructor creates a node when you know the entrance,
* exit or neither status.
* 
* entex : Determines whether node is entrance, exit or neither
*/
Node::Node(int xNPos, int yNPos, int zNPos, ENTEXENUM entex)
{
	xPos = xNPos;
	yPos = yNPos;
	zPos = zNPos;

	// 0xFC = 0b11111100
	// All walls initialized to 1 
	// Initialized as neither entrance nor exit node.
	data = 0xFC;

	// Set entrance, exit or neither status
	switch(entex)
	{
	case Entrance:
		// Changes data to 0b11111110 for an entrance node
		data = data | 2;
		break;
	case Exit:
		// Changes data to 0b11111101 for an exit node
		data = data | 1;
		break;
	case Neither:
	default:
		break;
	}
}

Node::Node(int x, int y, int z, char d) {
	xPos = x;
	yPos = y;
	zPos = z;
	data = d;
}

/*
* Remove a wall from the node.
*/
void Node::removeWall(int wall)
{
	data = data & ~(((char)1)<<wall);
}

/*
* Check for the existance of all walls to determine if the
* node has been visited
*/
bool Node::hasBeenVisited()
{
	return !(checkRightWall() &&
		checkLeftWall() &&
		checkFrontWall() &&
		checkBackWall() &&
		checkUpWall() &&
		checkDownWall());
}

/*
* The following function check to detect the existance of particular walls.
*/
bool Node::checkRightWall()
{
	return (((data>>RIGHT_WALL) & 1) == 1);
}

bool Node::checkLeftWall()
{
	return (((data>>LEFT_WALL) & 1) == 1);
}

bool Node::checkFrontWall()
{
	return (((data>>FRONT_WALL) & 1) == 1);
}

bool Node::checkBackWall()
{
	return (((data>>BACK_WALL) & 1) == 1);
}

bool Node::checkUpWall()
{
	return (((data>>UP_WALL) & 1) == 1);
}

bool Node::checkDownWall()
{
	return (((data>>DOWN_WALL) & 1) == 1);
}
/*
* End of wall check functions.
*/

char Node::getData()
{
	return data;
}

int Node::getX()
{
	return xPos;
}

int Node::getY()
{
	return yPos;
}

int Node::getZ()
{
	return zPos;
}

unsigned char* Node::getDataPtr() {
	return &data;
}
