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

constexpr int packetLength = 16;

//globals
CubeControl::ByteVoxelBuffer<8,8,8> buffers[2];

int main(void) {

    CubeControl::Board::MCUInit();

    //start auto-baud with auto-restart
    CubeControl::UART::U0ACR() = 1 | (1 << 2);
    while(CubeControl::UART::U0ACR() & 1){} // wait until auto-baud done;
    CubeControl::Board::smallDelay(); // very small delay
    CubeControl::UART::U0FCR() = 0b111; // clear TX and RX fifo
    CubeControl::UART::U0THR() = 'R'; // signal 'R'eady

    int currentPacketLength = 0;
    int backBufferID = 0;
    int layer = 0;
    int layerCycle = 0; // current LED layer strobe cycle, data write out on ID 0
    constexpr int numLayerCycles = 4; // how many times to repeat the LED strobe
    constexpr int layers = 8;
    while(1)
    {
        // if layerCycle == 0 write out current layer to TLC5940
        //if(layerCycle == 0)
        //{
            //buffers[backBufferID].transmitLayer(layer);
            //layer = (layer + 1) & 0b111;
        //}
        //layerCycle = (layerCycle + 1) & 0b111;

        // Process any pending UART bytes
        // reply F every 16 bytes to signal FIFO Clear
        // reply S every 16 bytes to signal buffer swap after fill complete
        while(CubeControl::UART::U0LSR() & 1)
        {
            bool endOfBuffer = buffers[backBufferID].writeNextByte(CubeControl::UART::U0RBR());
            ++currentPacketLength;
            if(currentPacketLength == packetLength || endOfBuffer)
            {
                currentPacketLength = 0;
                CubeControl::UART::U0THR() = 'F'; // Packet 'F'inished
            }
            if(endOfBuffer)
            {
                backBufferID = 1-backBufferID; //swap buffer ID
                currentPacketLength = 0;
                CubeControl::UART::U0THR() = 'S'; // Buffer 'S'wap
            }
        }

    }
    return 0 ;
}
