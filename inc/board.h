/*
 * board.h
 *
 *  Created on: Oct 28, 2018
 *      Author: Charles
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "syscon.h"
#include "fmc.h"
#include "adc.h"
#include "io.h"
#include "nvic.h"
#include "timer.h"
#include "bitutils.h"

namespace CubeControl
{

namespace Board
{

/// Startup function for LPC1114 on the LED Cube Controller Board
void MCUInit();

inline static void smallDelay()
{
    volatile int i = 32; while(--i){};
};

}

}



#endif /* BOARD_H_ */
