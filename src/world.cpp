/*
 * World.cpp
 *
 *  Created on: 9 Dec 2015
 *      Author: Mason
 */

#include "world.h"
#include "draw.h"

#include <array>
#include <map>
#include <vector>
#include <iterator>
#include <utility>
#include <ctime>

#define FREEGLUT_STATIC
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

bool World::paused = false;
int World::completed = 0;

int World::prev = 0;

bool World::falling = false;
std::vector <int> World::falling_block;
std::vector <int> World::falling_block_position;
int World::falling_block_colour;

int const World::COLUMNS = 6;
int const World::ROWS = 8;

std::vector < std::vector < int > > World::map;

void World::worldTick() { // Go through and do everything in the world

	long now = time(0);
	if (World::prev + 1 < now) {
//	if (true) {
		World::prev = now;

		if (falling) {
			World::drop();
		} else {
			World::spawnFalling();
		}

	}

}

void World::initMap() {		// fill up the empty map
	static std::vector < std::vector <int> > m;

	for (int i = 0; i < World::COLUMNS*2; i++) {
		std::vector<int> row;
		for (int j = 0; j < World::ROWS*2+4; j++) {
			row.push_back(0);
		}
		m.push_back(row);
	}
	World::map = m;
}

void World::checkRows() {	// check to see if there are any full rows

	static std::vector < std::vector <int> > m = map;

	for (int i = 0; i < ROWS*2+4; i++) {
		bool full = true;
		for (int j = 0; j < World::COLUMNS*2; j++) {
			if (map[j][i] == 0) {
				full = false;
			}
		}
		if (full) {
			World::fullRow(i);
			World::checkRows();		// there may be multiple full rows
			return;
		}
	}

	for (int j = 0; j < COLUMNS*2; j++) {		// top row
		if (map[j][ROWS*2] != 0) {
			World::gameOver();
		}
	}

}

void World::gameOver() {

	World::initMap();
}

void World::fullRow(int j) {	// there is a full row

	// empty the row
	for (int i = 0; i < COLUMNS*2; i++) {
		map[i][j] = 0;
	}

	// drop everything		--	 this may not be right
	for (j; j < ROWS*2+4; j++) {
		for (int i = 0; i < COLUMNS*2; i++) {
			map[i][j] = map[i][j+1];
		}
	}

	// top row needs it own thing
	for (int i = 0; i < COLUMNS*2; i++) {
		map[i][ROWS*2+4-1] = 0;
	}

}

void World::rotate(int d) {		// rotate the falling thing clockwise
	std::vector<int> pos = {0, 0};
	for (int i = 0; i < World::falling_block.size(); i++) {
		int f = falling_block[i]+2;

		if (f > 8) {
			f = f-8;
		}

		switch (f) {
			case 0: pos = {falling_block_position[0],		falling_block_position[1]};
			break;
			case 1: pos = {falling_block_position[0]-1,		falling_block_position[1]+1};
			break;
			case 2: pos = {falling_block_position[0],		falling_block_position[1]+1};
			break;
			case 3: pos = {falling_block_position[0]+1,		falling_block_position[1]+1};
			break;
			case 4: pos = {falling_block_position[0]+1,		falling_block_position[1]};
			break;
			case 5: pos = {falling_block_position[0]+1,		falling_block_position[1]-1};
			break;
			case 6: pos = {falling_block_position[0],		falling_block_position[1]-1};
			break;
			case 7: pos = {falling_block_position[0]-1,		falling_block_position[1]-1};
			break;
			case 8: pos = {falling_block_position[0]-1, 	falling_block_position[1]};
			break;
		}

		if (pos[0] < 0) {
			shift(1);
			rotate(d);
			return;
		} else if (pos[0] >= COLUMNS*2) {
			shift(-1);
			rotate(d);
			return;
		} else if (pos[1] < 0) {
			return;
		} else if (pos[1] >= ROWS*2+4) {
			return;

		} else {
			if (map[pos[0]][pos[1]] != 0) {
				return;
			}
		}
	}

	for (int i = 0; i < World::falling_block.size(); i++) {
		if (falling_block[i] == 0) {

		} else {
			falling_block[i] = falling_block[i]+2;

			if (falling_block[i] > 8) {
				falling_block[i] = falling_block[i]-8;
			}
		}
	}
}


void World::shift(int d) {		// move to the left or right

	// check if there is a point (that is part of the falling block) to the left or right which will block it from moving
	std::vector<int> pos = {0, 0};
	for (int i = 0; i < World::falling_block.size(); i++) {

		int f = falling_block_position[0]+d;
		switch (falling_block[i]) {
			case 0: pos = {f,		falling_block_position[1]};
			break;
			case 1: pos = {f-1,		falling_block_position[1]+1};
			break;
			case 2: pos = {f,		falling_block_position[1]+1};
			break;
			case 3: pos = {f+1,		falling_block_position[1]+1};
			break;
			case 4: pos = {f+1,		falling_block_position[1]};
			break;
			case 5: pos = {f+1,		falling_block_position[1]-1};
			break;
			case 6: pos = {f,		falling_block_position[1]-1};
			break;
			case 7: pos = {f-1,		falling_block_position[1]-1};
			break;
			case 8: pos = {f-1, 	falling_block_position[1]};
			break;
		}

		if (pos[0] < 0) {
			return;
		} else if (pos[0] >= COLUMNS*2) {
			return;
		} else if (pos[1] < 0) {
			return;
		} else if (pos[1] >= ROWS*2+4) {
			return;

		} else {
			if (map[pos[0]][pos[1]] != 0) {
				return;
			}
		}
	}

	falling_block_position[0] = falling_block_position[0] + d;

/*	if (falling_block_position[0] + m >= COLUMNS*2 || falling_block_position[0] + m < 0) {

	} else {
		falling_block_position[0] = falling_block_position[0] + d;

	}
*/

}

void World::emptyBlock() {						// empty the map of the block so it can be redrawn in the new place

	//     remove current block -> reposition -> draw block in new position, is the way this program does this
	for (int i = 0; i < World::falling_block.size(); i++) {
		switch (falling_block[i]) {
			case 0: map[	falling_block_position[0]]		[falling_block_position[1]		]	=	0;
			break;
			case 1: map[	falling_block_position[0]-1] 	[falling_block_position[1]+1 	]	=	0;
			break;
			case 2: map[	falling_block_position[0]] 		[falling_block_position[1]+1 	]	=	0;
			break;
			case 3: map[	falling_block_position[0]+1]	[falling_block_position[1]+1 	]	=	0;
			break;
			case 4: map[	falling_block_position[0]+1] 	[falling_block_position[1]		]	=	0;
			break;
			case 5: map[	falling_block_position[0]+1] 	[falling_block_position[1]-1	]	=	0;
			break;
			case 6: map[	falling_block_position[0]]	 	[falling_block_position[1]-1	]	=	0;
			break;
			case 7: map[	falling_block_position[0]-1]	[falling_block_position[1]-1	]	=	0;
			break;
			case 8: map[	falling_block_position[0]-1] 	[falling_block_position[1]		]	=	0;
			break;
		}
	}
}

void World::spawnFalling() {		// make a falling block
	falling = true;
	int x = rand() % (World::COLUMNS*2-2)+1;
	int y = ROWS*2+1;

	World::falling_block_colour = rand() % (8)+1;
	World::falling_block_position = {x, y};


	int r = rand() % (6);

	// below are the different blocks which can be made
	if (r == 0) {

		World::falling_block.push_back(4);
		World::falling_block.push_back(8);
		World::falling_block.push_back(6);
		World::falling_block.push_back(7);
		World::falling_block.push_back(5);
		World::falling_block.push_back(1);
		World::falling_block.push_back(0);

	} else if (r==1) {
		World::falling_block.push_back(0);
		World::falling_block.push_back(4);
		World::falling_block.push_back(8);

		World::falling_block.push_back(0);

	} else if (r==2) {
		World::falling_block.push_back(6);
		World::falling_block.push_back(7);
		World::falling_block.push_back(5);

		World::falling_block.push_back(1);

		World::falling_block.push_back(0);

	} else if (r==3) {
		World::falling_block.push_back(6);
		World::falling_block.push_back(2);
		World::falling_block.push_back(0);

	} else if (r==4) {
		World::falling_block.push_back(2);
		World::falling_block.push_back(4);
		World::falling_block.push_back(8);
		World::falling_block.push_back(0);

	} else if (r==5) {
		World::falling_block.push_back(2);
		World::falling_block.push_back(3);
		World::falling_block.push_back(1);
		World::falling_block.push_back(0);

	} else if (r==6) {
		World::falling_block.push_back(6);
		World::falling_block.push_back(7);
		World::falling_block.push_back(5);
		World::falling_block.push_back(1);
		World::falling_block.push_back(0);

	}
}

void World::land() {			// the falling block lands
	falling = false;
	falling_block_position[1] = falling_block_position[1]+1;
	drawBlock();
	World::falling_block_position.clear();
	World::falling_block.clear();
	checkRows();
}

void World::drop() {		// make the falling block fall
	emptyBlock();
	falling_block_position[1] = falling_block_position[1]-1;

	int n = 1;

	for (int i = 0; i < World::falling_block.size(); i++) {

		switch (falling_block[i]) {
			case 0: n = falling_block_position[1];
			break;
			case 1: n = falling_block_position[1]+1;
			break;
			case 2: n = falling_block_position[1]+1;
			break;
			case 3: n = falling_block_position[1]+1;
			break;
			case 4: n = falling_block_position[1];
			break;
			case 5: n = falling_block_position[1]-1;
			break;
			case 6: n = falling_block_position[1]-1;
			break;
			case 7: n = falling_block_position[1]-1;
			break;
			case 8: n = falling_block_position[1];
			break;
		}

  		if (n < 0) {
			World::land();
			return;
		}
		n = 0;

		switch (falling_block[i]) {
			case 0: n = World::map[	falling_block_position[0]]		[falling_block_position[1]		];
			break;
			case 1: n = World::map[	falling_block_position[0]-1] 	[falling_block_position[1]+1 	];
			break;
			case 2: n = World::map[	falling_block_position[0]] 		[falling_block_position[1]+1 	];
			break;
			case 3: n = World::map[	falling_block_position[0]+1]	[falling_block_position[1]+1 	];
			break;
			case 4: n = World::map[	falling_block_position[0]+1] 	[falling_block_position[1]		];
			break;
			case 5: n = World::map[	falling_block_position[0]+1] 	[falling_block_position[1]-1	];
			break;
			case 6: n = World::map[	falling_block_position[0]]	 	[falling_block_position[1]-1	];
			break;
			case 7: n = World::map[	falling_block_position[0]-1]	[falling_block_position[1]-1	];
			break;
			case 8: n = World::map[	falling_block_position[0]-1] 	[falling_block_position[1]		];
			break;
		}
		static std::vector < std::vector <int> > m = map;
		if (n != 0) {
			World::land();
			return;
		}
	}
	drawBlock();
}

void World::drawBlock() {				// draw the block in the position specified by global variables
	int x = falling_block_position[0];
	int y = falling_block_position[1];

	for (int i = 0; i < World::falling_block.size(); i++) {
		switch (falling_block[i]) {
			case 0: map[	falling_block_position[0]]		[falling_block_position[1]		]	=	falling_block_colour;
			break;
			case 1: map[	falling_block_position[0]-1] 	[falling_block_position[1]+1 	]	=	falling_block_colour;
			break;
			case 2: map[	falling_block_position[0]] 		[falling_block_position[1]+1 	]	=	falling_block_colour;
			break;
			case 3: map[	falling_block_position[0]+1]	[falling_block_position[1]+1 	]	=	falling_block_colour;
			break;
			case 4: map[	falling_block_position[0]+1] 	[falling_block_position[1]		]	=	falling_block_colour;
			break;
			case 5: map[	falling_block_position[0]+1] 	[falling_block_position[1]-1	]	=	falling_block_colour;
			break;
			case 6: map[	falling_block_position[0]]	 	[falling_block_position[1]-1	]	=	falling_block_colour;
			break;
			case 7: map[	falling_block_position[0]-1]	[falling_block_position[1]-1	]	=	falling_block_colour;
			break;
			case 8: map[	falling_block_position[0]-1] 	[falling_block_position[1]		]	=	falling_block_colour;
			break;
		}
	}
}

void World::mouse(int button, int state, int x, int y) {
	// no mouse support currently
}
void World::key(unsigned char k, int x, int y){
	if (k == ' ') {
		World::prev = 0;		// skip to next step
	}
	if (k == 'z') {
	}

}

void World::passiveMouse(int x, int y) {
	// no mouse support currently
}


void World::specialKey(int key, int x, int y) {
	emptyBlock();

	// rotate and move
	if (key == GLUT_KEY_UP) {
		World::rotate(-1);
	}
	if (key == GLUT_KEY_DOWN) {
		World::rotate(1);
	}

	if (key == GLUT_KEY_LEFT) {
		World::shift(-1);
	}
	if (key == GLUT_KEY_RIGHT) {
		World::shift(1);
	}

	drawBlock();	// update

	glutPostRedisplay();
}





