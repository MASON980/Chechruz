/*
 * world.h
 *
 *  Created on: 9 Dec 2015
 *      Author: Mason
 */

#ifndef WORLD_H_
#define WORLD_H_


#include <array>
#include <map>
#include <vector>
// See if I can remove this includes

// World, holds all the agents and various performance measuring things

class World {
private:
	static std::vector < int > falling_block;
	static std::vector < int > falling_block_position;
	static int falling_block_colour;

	static int resolution;
	static int completed;
	static bool falling;
	static bool paused;

	static int prev;



public:
	static std::vector < std::vector <int> > map;


	static int const COLUMNS;
	static int const ROWS;

	static void worldTick(); // Go through and do everything in the world, agent logic then draw


	static int getCompleted() { return completed;}
	static bool getPaused() { return paused;}

	static std::vector< std::vector <int> > getMap () {return map;}

	static void mouse(int, int, int , int);
	static void key(unsigned char, int , int);
	static void passiveMouse(int, int);
	static void specialKey(int, int, int);


	static void initMap();
	static void checkRows();
	static void fullRow(int);
	static void rotate(int);
	static void shift(int);
	static void spawnFalling();
	static void emptyBlock();
	static void drawBlock();
	static void land();
	static void drop();

	static void gameOver();
};


#endif /* WORLD_H_ */
