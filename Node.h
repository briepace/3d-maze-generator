#ifndef NODE_H
#define NODE_H

#define RIGHT_WALL	7
#define LEFT_WALL	6
#define FRONT_WALL	5
#define BACK_WALL	4
#define UP_WALL		3
#define DOWN_WALL	2


enum ENTEXENUM
{
	Entrance,
	Exit,
	Neither
};

class Node
{
public:
	Node();
	Node(int, int, int);
	Node(int, int, int, ENTEXENUM);
	Node(int, int, int, char);
	bool checkRightWall();
	bool checkLeftWall();
	bool checkFrontWall();
	bool checkBackWall();
	bool checkUpWall();
	bool checkDownWall();
	void removeWall(int);
	bool hasBeenVisited();
	char getData();
	unsigned char* getDataPtr();
	int getX();
	int getY();
	int getZ();
	//string toString();
	//bool isEmpty();
private:
	/* char data
	* 
	* The first six bits represent the walls of the node.
	* 1st : Right
	* 2nd : Left
	* 3rd : Front
	* 4th : Back
	* 5th : Up
	* 6th : Down
	* 
	* The last two bits represent whether the node is an
	* entrance, exit or neither
	* Entrance : 10
	* Exit 	  : 01
	* Neither  : 00 or 11
	* 
	*/
	unsigned char data;
	int xPos;
	int yPos;
	int zPos;
};

#endif
