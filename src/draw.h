/*
 * Draw.h
 *
 *  Created on: 8 Dec 2015
 *      Author: Mason
 */

#ifndef DRAW_H_
#define DRAW_H_

#include <string>
#include <vector>

// Draw is for drawing the GUI/Graphics to make stuff easier to follow for humans

class Draw {

private:
	static const int TICK;

	//static char *myargv;
	//static char v;
	//static int myargc;

	static const int RADIUS;
	static const int STARTING_HEIGHT;
	static const int STARTING_WIDTH;
	static double line;
	static int frame_height;
	static float height;
	static float width;
	static int frames;


public:

	static const float DEG2RAD;
	static float ratio;	// it was complaining about this being private

	static void initialiseDraw();
	static void display();

	static void reshape(int, int);

	static void square(int, int);
	static void string(std::string, int, int);
	static void stringWithInt(std::string, int);
	static void colouring(int);

	static std::vector<int> pixelToGlut(int, int);


};


#endif /* DRAW_H_ */
