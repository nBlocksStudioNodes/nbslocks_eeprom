/**
 * @brief       eeprom.h
 * @details    	Header file for EEprom node.
 * @pre         This code belongs to Nimbus Centre ( http://www.nimbus.cit.ie ).
 */
#ifndef __NB_EEPROM
#define __NB_EEPROM

#include "mbed.h"
#include "nworkbench.h"
#include "rm25c512cl.h"

class nBlock_Eeprom: public nBlockSimpleNode<1> {
public:

	typedef enum{
		
		WRITE = 0,
		READ,
		ERASE
		
	}instruction_t;

	
	
    nBlock_Eeprom(PinName MOSI,PinName MISO, PinName SCLK,PinName CS,uint16_t address);
    void triggerInput(uint32_t inputNumber, uint32_t value);
	void endFrame();
	
   
private:

	rm25c512cl _eeprom;
	uint16_t _address;
	
};

#endif