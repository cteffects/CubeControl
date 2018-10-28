/*
 * board.cpp
 *
 *  Created on: Oct 28, 2018
 *      Author: Charles Trippe
 */

#include "board.h"
#include "syscon.h"
#include "fmc.h"
#include "adc.h"
#include "io.h"
#include "nvic.h"
#include "timer.h"

void CubeControl::Board::MCUInit()
{
    //system should currently be running on default settings with IRC oscillator clock

    //Power Up the System Oscillator
    CubeControl::SYSCON::POWERCFG::powerUp(CubeControl::SYSCON::POWERCFG::SYSOSC_PD);

    //set PLL source to system oscillator
    CubeControl::SYSCON::PLLCFG::setSystemPLLSource(CubeControl::SYSCON::PLLCFG::SYS_OSC);

    //set PLL multiplier and divider ratio
    CubeControl::SYSCON::PLLCFG::setPLLDivider(3, 1);

    //power up PLL
    CubeControl::SYSCON::POWERCFG::powerUp(CubeControl::SYSCON::POWERCFG::SYSPLL_PD);

    //wait for PLL lock
    while (!CubeControl::SYSCON::PLLCFG::isPLLLocked()) {}

    //set flash access time to 3 clocks
    CubeControl::FMC::Set_FLASHTIM(CubeControl::FMC::Timing::FLASHTIM_50_MHZ);

    //set main clock to pll
    CubeControl::SYSCON::CLOCKCFG::setMainClockSource(CubeControl::SYSCON::CLOCKCFG::MAIN_CLOCK_SOURCE::SYSTEM_PLL_OUTPUT);

    //enable IOCON clock
    CubeControl::SYSCON::CLOCKCFG::enableClock(CubeControl::SYSCON::CLOCKCFG::DEVICE_CLOCK_MASK::IOCON);

    //todo: set up GPIO pins

    //todo: set up UART
}
