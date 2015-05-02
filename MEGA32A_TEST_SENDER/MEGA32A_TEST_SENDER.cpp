/*
 * MEGA32A_TEST_SENDER.cpp
 *
 * Created: 5/1/2015 4:11:24 PM
 *  Author: Jimmy
 */ 


#include "RF24.h"
#include <avr/io.h>
#include <util/delay.h>
#include "MEGA32A_UART_LIBRARY.h"
#include "MEGA32A_SPI_LIBRARY.h"

int main(void)
{
	
	DDRB |= (1<<DDRB0);
	PORTB |= (1<<DDRB0);
	initUart();
	InitSPI();
	
	int i;
	for(i=0;i<3;i++){
		PORTB &= ~(1<<DDRB0);
		_delay_ms(1000);
		PORTB |= (1<<DDRB0);
		_delay_ms(1000);
	}
	PORTB |= (1<<DDRB0);
	
	// Single radio pipe address for the 2 nodes to communicate.
	const uint64_t pipe = 0xE8E8F0F0E1LL;
	
	//ce, cs(ss)
	RF24 radio(3,4);
	
	char NRFStatus = 0xFF;
	uint8_t command;
	uint8_t buff[5] = {0x0F, 0XA1, 0X33, 0X4C, 0X79};	
    while(1)
    {
	    command = USART_ReceiveByte();
	    if(command=='S'){
		    serialSendString("Checking NRF status\n\r");
		    NRFStatus = radio.read_register(STATUS);
		    serialSendString("NRF STATUS: ");
		    SendHexiByte(NRFStatus);
		    serialSendString("\r\n");
	    }
	    else if(command=='B'){
		    serialSendString("Beginning the RF\n\r");
		    radio.begin();
		    serialSendString("Radio has begun.\n\r");
	    }
	    else if(command=='A'){
		    serialSendString("Address R0 before setting: ");
		    radio.read_register(RX_ADDR_P0, buff, 5);
		    SendHexiArray(buff,5);
		    serialSendString("\n\r");
		    radio.openWritingPipe(pipe);
		    serialSendString("After setting: ");
		    radio.read_register(RX_ADDR_P0, buff, 5);
		    SendHexiArray(buff,5);
		    serialSendString("\n\r");
	    }
	    else if(command=='O'){
		    serialSendString("Opening writing pipe.\n\r");
		    radio.openWritingPipe(pipe);
		    serialSendString("After setting: ");
		    radio.read_register(RX_ADDR_P0, buff, 5);
		    SendHexiArray(buff,5);
		    serialSendString("\n\r");
	    }
	    else if(command=='W'){
		    serialSendString("Write what? (1 letter) ");
			buff[0] = USART_ReceiveByte();
		    serialSendString("Writing...\n\r");
		    bool result = radio.write(buff, 1);
		    if(result){
			    serialSendString("write() returned true\n\r");
		    }
		    else{
			    serialSendString("write() returned false\n\r");
		    }
	    }
	    else if(command=='R'){
		    serialSendString("Read which register? ");
		    uint8_t reg = USART_ReceiveHexi();
		    SendHexiByte(reg);
		    serialSendString("\n\r");
		    NRFStatus = radio.read_register(reg);
		    serialSendString("Register value: ");
		    SendHexiByte(NRFStatus);
		    serialSendString("\n\r");
	    }
	    else{
		    serialSendString("command not recognized.\n\r");
	    }
        
    }
}