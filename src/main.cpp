/*
 * main.cpp
 *
 *  Created on: 21 Feb 2016
 *      Author: Mason
 */


#include "main.h"
#include "draw.h"
#include <stdlib.h>
#include <ctime>


int main(int argc, char* argv[])
{
     	srand(time(0));
	Draw::initialiseDraw();
    return 0;
}

// todo
/*		game over state
 * 		better shapes
 *
 */
