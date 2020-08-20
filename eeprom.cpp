#include "eeprom.h"

/**
* @brief    nBlock_EEeprom()_
* @details  Constructor
* @param    SPI pins MOSI,MISO,SCLK and CS
* @param	address in eeprom to store data.
* @return   NA.
* 
*/

nBlock_Eeprom :: nBlock_Eeprom(PinName MOSI,PinName MISO, PinName SCLK, PinName CS,uint16_t address): _eeprom(MOSI,MISO,SCLK,CS){
	
	
	_address = address;
	
		
	
}


/**
* @brief    triggerInput()
* @details  Node has 3 inputs write, read and erase when a write input is triggered
*			the value recieved on the write input is written to the extenal memory at 
			the address given in the contructor(node parameter).
			When the read input is triggered the value stored at the address is read
			and then given to the output of the node. When the erase input is triggered
			the memory page(128 bytes) at the given address is erased.
* @param    Number of the input, value of the input
* @return   NA.
* 
*/

void nBlock_Eeprom:: triggerInput(nBlocks_Message message){
	
	//char test_value = 33;// purely for debugging, test value to write to memory, in future will use function parameter "value"
	
	
	if(message.inputNumber == WRITE){
		
		
		_eeprom.write_enable();
		_eeprom.write_byte(_address,(char)message.intValue);
	
	
	}
	
	else if(message.inputNumber == READ){
		
		output[0] = _eeprom.read_byte(_address);
		
		available[0] = 1;
			
		
	}
	
	else if(message.inputNumber == ERASE){
		
		_eeprom.write_enable();
		_eeprom.page_erase(_address);
		
		
	}
	
	
}



void nBlock_Eeprom :: endFrame(void){
	
	
	
}