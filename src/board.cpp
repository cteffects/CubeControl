/*
 * board.cpp
 *
 *  Created on: Oct 28, 2018
 *      Author: Charles Trippe
 */

#include "board.h"

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

    //power down IRC and IRCOUT
    CubeControl::SYSCON::POWERCFG::powerDown(
            CubeControl::SYSCON::POWERCFG::DEVICE_POWER_MASK::IRCOUT_PD |
            CubeControl::SYSCON::POWERCFG::DEVICE_POWER_MASK::IRC_PD
            );

    //enable IOCON clock
    CubeControl::SYSCON::CLOCKCFG::enableClock(CubeControl::SYSCON::CLOCKCFG::DEVICE_CLOCK_MASK::IOCON);

    //begin setting up I/O pins

    //disable pull-ups on layer-enable I/O
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_4(), 0, CubeControl::IOCON::PIO2_4_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_5(), 0, CubeControl::IOCON::PIO2_5_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_6(), 0, CubeControl::IOCON::PIO2_6_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_7(), 0, CubeControl::IOCON::PIO2_7_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_8(), 0, CubeControl::IOCON::PIO2_8_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_9(), 0, CubeControl::IOCON::PIO2_9_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_10(), 0, CubeControl::IOCON::PIO2_10_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_11(), 0, CubeControl::IOCON::PIO2_11_WRITABLE_BITS_MASK);

    //set SCK to PIO0_6
    CubeControl::IOCON::SCK_LOC() = 2;

    //set PIO0_6 to SCK0 function
    masked_write_bits_unsafe(CubeControl::IOCON::PIO0_6(), 2, CubeControl::IOCON::PIO0_6_WRITABLE_BITS_MASK);

    //set PIO0_9 to MOSI0 function
    masked_write_bits_unsafe(CubeControl::IOCON::PIO0_9(), 1, CubeControl::IOCON::PIO0_9_WRITABLE_BITS_MASK);

    //set FUNC and disable pull-ups on LED driver communication pins
    masked_write_bits_unsafe(CubeControl::IOCON::PIO0_1(), 1, CubeControl::IOCON::PIO0_1_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::R_PIO1_0(), (1 << 7) | 1, CubeControl::IOCON::R_PIO1_0_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::R_PIO1_1(), (1 << 7) | 1, CubeControl::IOCON::R_PIO1_1_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::R_PIO1_2(), (1 << 7) | 1, CubeControl::IOCON::R_PIO1_2_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO1_4(), (1 << 7) | 0, CubeControl::IOCON::PIO1_4_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO3_0(), 0, CubeControl::IOCON::PIO3_0_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO3_1(), 0, CubeControl::IOCON::PIO3_1_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO3_2(), 0, CubeControl::IOCON::PIO3_2_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO3_3(), 0, CubeControl::IOCON::PIO3_3_WRITABLE_BITS_MASK);

    //set-up SPI channel 1
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_0(), 2, CubeControl::IOCON::PIO2_0_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_1(), 2, CubeControl::IOCON::PIO2_1_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_2(), 2, CubeControl::IOCON::PIO2_2_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO2_3(), 2, CubeControl::IOCON::PIO2_3_WRITABLE_BITS_MASK);

    //set up wireless communication pins
    masked_write_bits_unsafe(CubeControl::IOCON::PIO1_8(), 0, CubeControl::IOCON::PIO1_8_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO1_9(), 0, CubeControl::IOCON::PIO1_9_WRITABLE_BITS_MASK);

    //set up UART pins
    masked_write_bits_unsafe(CubeControl::IOCON::PIO1_6(), 1, CubeControl::IOCON::PIO1_6_WRITABLE_BITS_MASK);
    masked_write_bits_unsafe(CubeControl::IOCON::PIO1_7(), 1, CubeControl::IOCON::PIO1_7_WRITABLE_BITS_MASK);

    //set UART clock divider
    CubeControl::SYSCON::UARTCLKDIV() = 1;

    //enable UART clock
    CubeControl::SYSCON::CLOCKCFG::enableClock(CubeControl::SYSCON::CLOCKCFG::DEVICE_CLOCK_MASK::UART);

    //set UART Word Length to 8-bits
    CubeControl::UART::U0LCR() = 3;

    //enable UART FIFO
    CubeControl::UART::U0FCR() = 1;

    //set SPI0 clock divider and prescale
    CubeControl::SYSCON::SSP0CLKDIV() = 1;
    CubeControl::SPI::CPSR(0) = 2;

    //set SPI frame to 12-bits required by TLC5940
    CubeControl::SPI::CR0(0) = 0xB;

    //set SPI1 clock divider and prescale
    CubeControl::SYSCON::SSP1CLKDIV() = 1;
    CubeControl::SPI::CPSR(1) = 8;
}
