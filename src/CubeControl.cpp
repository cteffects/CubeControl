/*
===============================================================================
 Name        : main.c
 Author      : Charles Trippe
 Version     : 0.1
 Description : CubeControl LED Cube Controller Board Firmware
===============================================================================
*/

#include "board.h"
#include "bytevoxel.h"
#include <cr_section_macros.h>

//constants
constexpr int packetLength = 16;

unsigned int layerGPIOmask[] = {
        1 << 4,
        1 << 5,
        1 << 6,
        1 << 7,
        1 << 8,
        1 << 9,
        1 << 10,
        1 << 11,
};

//globals
int backBufferID = 0;
int currentPacketLength = 0;
int layer = 0;
CubeControl::ByteVoxelBuffer<8,8,8> buffers[2];
bool inErrorState = false;

void flushUARTIntoBuffer()
{
    while(CubeControl::UART::U0LSR() & 1)
    {
        bool endOfBuffer = buffers[backBufferID].writeNextByte(CubeControl::UART::U0RBR());
        ++currentPacketLength;
        if(currentPacketLength == packetLength || endOfBuffer)
        {
            currentPacketLength = 0;
            if(endOfBuffer)
            {
                backBufferID = 1-backBufferID; //swap buffer ID
                currentPacketLength = 0;
                CubeControl::UART::U0THR() = 'S'; // Buffer 'S'wap
            }
            else
            {
                CubeControl::UART::U0THR() = 'F'; // Packet 'F'inished
            }
        }
    }
}

int main(void) {

    CubeControl::Board::MCUInit();

    //start auto-baud with auto-restart
    CubeControl::UART::U0ACR() = 1 | (1 << 2);
    while(CubeControl::UART::U0ACR() & 1){} // wait until auto-baud done;
    CubeControl::Board::smallDelay(); // very small delay
    CubeControl::UART::U0FCR() = 0b111 | (3 << 6); // clear TX and RX fifo and set RX interrupt trigger level to 8-bytes
    CubeControl::NVIC::EnableIRQ(CubeControl::NVIC::IRQ::UART);
    CubeControl::UART::U0IER() = 1; // enable Receive Data Available interrupt
    CubeControl::UART::U0THR() = 'R'; // signal 'R'eady
    CubeControl::NVIC::SetPriority(CubeControl::NVIC::IRQ::UART, CubeControl::NVIC::InterruptPriority::PRIORITY_2);


    CubeControl::masked_write_bits_unsafe(CubeControl::IOCON::R_PIO0_11(), 3, 0b111); //set PIO0_11 to CT32B0_MAT3 function
    //CubeControl::Timer::GPTimer::EMR(CubeControl::Timer::GPTimer::GPTimerID::CT32B0) = (3 << 10); // set MAT3 channel toggle on match

    CubeControl::Timer::GPTimer::MCR(CubeControl::Timer::GPTimer::GPTimerID::CT32B0) = (1 << 1);
    CubeControl::Timer::GPTimer::MR0(CubeControl::Timer::GPTimer::GPTimerID::CT32B0) = 1; // cycle length = 2 clocks
    CubeControl::Timer::GPTimer::PWMC(CubeControl::Timer::GPTimer::GPTimerID::CT32B0) = (1 << 3); //PWM on MAT3
    CubeControl::Timer::GPTimer::MR3(CubeControl::Timer::GPTimer::GPTimerID::CT32B0) = 1; // go high on 1
    CubeControl::NVIC::EnableIRQ(CubeControl::NVIC::IRQ::CT32B1); //enable interrupt handling

    // set num pulses with trim based on IRQ latency, must be at least 27
    constexpr int desiredPulses = 4096;

    CubeControl::Timer::GPTimer::MR0(CubeControl::Timer::GPTimer::GPTimerID::CT32B1) = (desiredPulses - 27) * 2 + 10;
    CubeControl::Timer::GPTimer::TC(CubeControl::Timer::GPTimer::GPTimerID::CT32B1) = CubeControl::Timer::GPTimer::MR0(CubeControl::Timer::GPTimer::GPTimerID::CT32B1) - 1; // interrupt soon after activate
    CubeControl::Timer::GPTimer::MCR(CubeControl::Timer::GPTimer::GPTimerID::CT32B1) = 1; // interrupt on MR0
    CubeControl::Timer::GPTimer::TCR(CubeControl::Timer::GPTimer::GPTimerID::CT32B1) = 1; //start timer 1

    volatile unsigned int i = 0;
    while(1)
    {
        ++i;
    }
    return 0 ;
}
