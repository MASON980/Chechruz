/*
 * Draw.cpp
 *
 *  Created on: 8 Dec 2015
 *      Author: Mason
 */

#include "draw.h"

#include "world.h"
#include <windows.h>  // For MS Windows
#include <math.h>

#define FREEGLUT_STATIC
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h


#include <string>  // For MS Windows
#include <thread>
#include <stdio.h>		// sprintf

int const static TICK = 20;

// Glut init
char *myargv[1];
char v = ' ';
int myargc = 1;

const float Draw::DEG2RAD = 3.14159 / 180;
const int Draw::RADIUS = 75;
const int Draw::STARTING_HEIGHT = 720;
const int Draw::STARTING_WIDTH = 540;


int Draw::frames = 0;
double Draw::line = 1;

int Draw::frame_height = Draw::STARTING_HEIGHT;
// maybe left top or something
float Draw::height = Draw::STARTING_HEIGHT / Draw::STARTING_WIDTH;
float Draw::width = 1;
float Draw::ratio  = Draw::width / Draw::height;


void Draw::square(int x, int y) {					// draw a square
	glBegin(GL_TRIANGLE_FAN);         // Each set of 4 vertices form a something

	// make it over the negative thing as well
	x = x - World::COLUMNS;
	y = y - World::ROWS;
	// change the colour depending on the status of the thing (moving or not)	-	maybe

	double fx = (x+0.5) * Draw::width / World::COLUMNS;
	double fy = (y+0.5) * Draw::height / World::ROWS;
	//float fy = (1 - y * height) / 100;
	for (int i = 0; i <= 4; i++) {
		float degInRad = (i+0.6) * Draw::DEG2RAD * 90;		// 10 * 36(used for how many loops) is where the 360 degrees come from
		glVertex2d(fx + cos(degInRad) * Draw::RADIUS / 750 * Draw::ratio,
				fy + sin(degInRad) * Draw::RADIUS / 900);
	}
	glEnd();

}


void Draw::stringWithInt(std::string str, int i) {				// append an int to a string and draw it
	Draw::line = Draw::line + 0.05;

	char* c = new char;
	sprintf(c,"%d",i);
	std::string s(c);
	std::string text = str;
	if (i != NULL) {
		text = text + ": " + s;

	}

	glColor3d(1.0, 0.0, 0.0);
	glRasterPos2d(0.01, 1 - line); // line
	for (int i = 0; i < text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text.at(i));
	}
	// 48 == 0
	// 65 == A
}

void Draw::display() {
	World::worldTick();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to white and opaque


	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	std::vector <std::vector <int>> map = World::map;

	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			colouring(map[i][j]);
			square(i, j);
		}
	}

	glFlush();  // Render now

}


void Draw::colouring(int c) {			// set to a colour based upon the number

	// I think glColor4f may be different from what I need
	if (c == 0) {
		glColor4f(0.1f, 0.1f, 0.1f, 1.0f); // Set background color to white and opaque
	} else if (c == 1) {
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // Set background color to white and opaque
	} else if (c == 2) {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white and opaque
	} else if (c == 3) {
		glColor4f(0.2f, 1.0f, 0.2f, 1.0f); // Set background color to white and opaque
	} else if (c == 4) {
		glColor4f(0.5f, 0.5f, 1.0f, 1.0f); // Set background color to white and opaque
	} else if (c == 5) {
		glColor4f(1.0f, 0.2f, 1.0f, 1.0f); // Set background color to white and opaque
	} else if (c == 6) {
		glColor4f(0.7f, 1.0f, 0.7f, 1.0f); // Set background color to white and opaque
	} else if (c == 7) {
		glColor4f(0.2f, 0.5f, 0.7f, 1.0f); // Set background color to white and opaque
	} else if (c == 8) {
		glColor4f(1.0f, 0.5f, 1.0f, 1.0f); // Set background color to white and opaque
	}
}



void Draw::reshape(int new_width, int new_height) {			// if the window is resized/shaped
	Draw::height = (float) new_height / Draw::STARTING_HEIGHT;
	Draw::width =  (float) new_width / Draw::STARTING_WIDTH;
	Draw::ratio = Draw::width / Draw::height;
	Draw::frame_height = new_height;

}

void Draw::string(std::string text, int x, int y) {			// draw a string

	double  fx = x * Draw::width / World::COLUMNS;					// this probably isn't the width but the radius or something
	double fy = y * Draw::height / World::ROWS;

	glColor3d(0.0, 0.0, 0.0);
	glRasterPos2d(fx, fy);
	for (int i = 0; i < text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text.at(i));
	}

}

void Draw::initialiseDraw() {

	World::initMap();


	// TODO Auto-generated constructor stub
	myargv[0] = &v;
	glutInit(&myargc, myargv);
	glutInitWindowSize(Draw::STARTING_WIDTH, Draw::STARTING_HEIGHT); // Set the window's initial width & height
	glutInitWindowPosition(50, 100); // Position the window's initial top-left corner
	glutCreateWindow(
			"Chechruz?"); // Create a window with the given title
	glutDisplayFunc(Draw::display);
	glutIdleFunc(Draw::display); // Register display callback handler for window re-paint
	glutReshapeFunc(Draw::reshape);
	glutKeyboardFunc(World::key);
	//glutMouseFunc(World::mouse);
	glutPassiveMotionFunc(World::passiveMouse);
	glutSpecialFunc(World::specialKey);

	glutMainLoop();           // Enter the infinitely event-processing loop

}

std::vector< int > Draw::pixelToGlut(int px, int py) {			// convert a pixels position to the glut position
	// could just save the actual height and width
	double current_height = Draw::height * Draw::STARTING_HEIGHT;
	double current_width =  Draw::width * Draw::STARTING_WIDTH;

	double dx = ((px / current_width) * 2) * World::COLUMNS;
	double dy = ((1 - (py / current_height)) * 2) * World::ROWS;

	int x = dx;
	int y = dy;

	return std::vector<int> {x, y};
}


