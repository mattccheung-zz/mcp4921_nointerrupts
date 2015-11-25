/*
 * MCP4921_ramp_from_Hsync_Vsync_interrupts.c
 *
 * Created: 30/11/2014 1:06:17 PM
 *  Author: LC
 */ 


#define F_CPU 16000000UL	//16MHz CPU clock
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);

uint16_t i;
#define MCP_START 12288 //0011 0000 0000 0000 

int main(void) {
	char highbyte, lowbyte;
	SPI_MasterInit();
	int i;
	i = MCP_START;  // MCP4921 initial value
	while(1)
	{
	for (i=MCP_START; i<MCP_START+4096; i+=2)
	{
		highbyte = i>>8;
		lowbyte = i;
		PORTB &= ~(1<<2); //SS sets to low
		SPI_MasterTransmit(highbyte);
		SPI_MasterTransmit(lowbyte);
		PORTB |= 1<<2;   //SS sets to high
	}	
	}
	
}

void SPI_MasterInit(void)
{
	DDRB = (1<<DDB3)|(1<<DDB5) |(1<<DDB2); // Set MOSI, SCK, SS as output, all others input 	
	SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR0)|(0<<SPR1)|(1<<SPI2X); // Enables SPI, Master, set clock rate, double speed SPI
}

void SPI_MasterTransmit(char cData)
{
	SPDR = cData; // Transfer SPI data to register
	while(!(SPSR & (1<<SPIF))); 	// Waits till SPI transfer is completed
}


