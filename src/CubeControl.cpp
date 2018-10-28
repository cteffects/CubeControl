/*
===============================================================================
 Name        : main.c
 Author      : Charles Trippe
 Version     : 0.1
 Description : CubeControl LED Cube Controller Board Firmware
===============================================================================
*/

#include "board.h"
#include <cr_section_macros.h>

int main(void) {

    CubeControl::Board::MCUInit();

    volatile static int i = 0 ;
    while(1) {
        i++ ;
    }
    return 0 ;
}
