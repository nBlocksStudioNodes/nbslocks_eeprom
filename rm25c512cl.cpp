#include "rm25c512cl.h"


rm25c512cl::rm25c512cl(PinName MOSI,PinName MISO,PinName SCLK,PinName CS): _spi(MOSI,MISO,SCLK),_cs(CS){
   
   _cs = HIGH; // always set chip select to initial state of high 
    
}

/**
* @brief    write_enable()
* @details  Sets write enable latch to 1 to enable writing to eeprom uses the WREN opcode
* @param    NA
* @return   Success or failure of command
* @warning  Write or erase instructions will be ignored if this instruction is not issued first
* 
*/

bool rm25c512cl :: write_enable(){
    
    char cmd[1];
    char data_buffer[1];// not used here but required for spi.write() as a parameter
    char status;
    
    cmd[0] = WREN;
    
    _spi.lock();
      
    _cs = LOW;
    
    _spi.write(&cmd[0],1,&data_buffer[0],0);
        
    _cs = HIGH;
    
    _spi.unlock();
    
    status = read_status_reg();
    status = status & 0x2;
    
    /* make sure write enable is completed, if not somthing is wrong with device*/
    
    if(status == 2){ // device is enabled WEL bit = 1
        
        return SUCCESS;
        
    }
    else{// WEL bit is 0 device is dissabled
        
        return FAILURE;
        
    }   
    
}

/**
* @brief    read_status_reg()
* @details  Reads the status register of EEprom using the RDSR opcode, the register 
*           read can be used to determine if a write has completed, or the write protection status
* @param    NA
* @return   Current eight bit value of register
* @warning  
* 
*/

char  rm25c512cl :: read_status_reg(){
    
   char cmd[1];
   char data_buffer[1]; // to store value returned from EEprom status register
   
       
   cmd[0] = RDSR;
    
   _spi.lock();
    
   _cs = LOW;
    
   _spi.write(&cmd[0],1,&data_buffer[0],0); 
    
   cmd[0] = 0xFF; // write garbage to read something back
    
   _spi.write(&cmd[0],1,&data_buffer[0],1);
       
   _cs = HIGH;
    
   _spi.unlock();
    
   
   
   return data_buffer[0];
       
    
}

/**
* @brief    write_bytes()
* @details  Writes multiple bytes to the specified address given.
* @param    Starting address to write to.
* @param    Pointer to the data array containg bytes to be stored.
* @param    Size of the data array in bytes.
* @return   if a timeout has occured i.e. device is busy for too long
* @warning  A page write(128 bytes) can take up to 5 ms to complete
* 
*/

bool rm25c512cl :: write_bytes(uint16_t address, char* data, uint16_t data_size){
    
    char cmd[3];
    char data_buffer[1];// not used here but required for spi.write() as a parameter
    char status;
    uint32_t i = TIMEOUT_BYTES;
     
    
    cmd[0] = WR;
    cmd[1] = address >> 8;
    cmd[2] = address & 0x00FF;
    
    _spi.lock();
    
    _cs = LOW;   
    
    _spi.write(&cmd[0],3,&data_buffer[0],0);
    _spi.write(&data[0],data_size,&data_buffer[0],0);
  
    _cs = HIGH;
    
    _spi.unlock();
    
    /* make sure data has been written sucessfully by reading bit zero in status register*/
    
   do{
    
    status = read_status_reg();
    status = status & 0x1; 
    
    i--; 
      
    }while(status == BUSY && i>0);
    
    /* if timeout elapses*/
    
    if( i < 1 && status == BUSY){
        
        return TIMEOUT_ERROR;
    }
    else {
        return TIMEOUT_OK;
    }   
    
}

/**
* @brief    write_byte()
* @details  Writes a single byte to EEprom at the given address
* @param    Address to write to
* @param    Value to write in given address
* @return   if a timeout has occured i.e. device is busy for too long
* @warning  Byte write can take up to 100 us to complete
* 
*/

bool rm25c512cl :: write_byte(uint16_t address,char value){
    
    char cmd[4];
    char data_buffer[1]; // not used here but required for spi.write() as a parameter
    char status;
    uint32_t i = TIMEOUT_BYTE;
       
    cmd[0] = WR;
    cmd[1] = address >> 8;
    cmd[2] = address & 0x00FF;
    cmd[3] = value;
    
    _spi.lock();
    
    _cs = LOW;   
    
    _spi.write(&cmd[0],4,&data_buffer[0],0);
    
    _cs = HIGH;
    
    _spi.unlock();
    
     /* keeps checking the WEL bit i status reg to make sure write completes*/
    
    do{
    
    status = read_status_reg();
    status = status & 0x1; 
    
    i--; 
      
    }while(status == BUSY && i>0);
    
    /* if timeout elapses*/
    
    if( i < 1 && status == BUSY){
        
        return TIMEOUT_ERROR;
    }
    else {
        return TIMEOUT_OK;
    }   
    
    
    
}

/**
* @brief    read_bytes()
* @details  Reads multiple bytes starting at given address.
* @param    Page Address to start reading from.
* @param    Pointer to data buffer to store multiple bytes read from EEprom.
* @param    Size of the data buffer in bytes.
* @return   
* @warning  Data buffer must be of size 128 bytes to read entire page.
* 
*/

void rm25c512cl :: read_bytes(uint16_t address, char* data_buffer, uint16_t data_buffer_size){
    
    char cmd[3];
   
    
    cmd[0] = READ;
    cmd[1] = address >> 8;
    cmd[2] = address & 0x00FF;
    
    
    _spi.lock();
    
    _cs = LOW;
    
    _spi.write(&cmd[0],3,&data_buffer[0],0);
    
    cmd[0] = 0xFF;
        
    _spi.write(&cmd[0],1,&data_buffer[0],data_buffer_size);
      
    _cs = HIGH;
    
    _spi.unlock();
    
    
}

/**
* @brief    read_byte()
* @details  Reads a byte of data at given address.
* @param    Address to read byte from.
* @return   Byte of data read from address.
* @warning  
* 
*/

char rm25c512cl :: read_byte(uint16_t address){
    
    char cmd[3];
    char data_buffer[1];// To store data byte returned from EEprom
    
    cmd[0] = READ;
    cmd[1] = address >> 8;
    cmd[2] = address & 0x00FF;
    
    _spi.lock();
    
    _cs = LOW;
    
    _spi.write(&cmd[0],3,&data_buffer[0],0);
    
    cmd[0] = 0xFF;
    
    _spi.write(&cmd[0],1,&data_buffer[0],1);
    
    _cs = HIGH;
    
    _spi.unlock();
    
    return data_buffer[0];
    
}

/**
* @brief    page_erase()
* @details  128 byte erase at page address
* @param    Page address
* @return   if a timeout has occured i.e. device is busy for too long.
* @warning  Write enable must be called prior to calling this function or 
*           instruction will be ignored.
* 
*/


bool rm25c512cl :: page_erase(uint16_t address){
    
    char cmd[3];
    char data_buffer[1];// not used here but required as function parameter
    char status;
    uint32_t i = TIMEOUT_BYTES;
    
    cmd[0] = PERS;
    cmd[1] = address >> 8;
    cmd[2] = address & 0x00FF;
    
    _spi.lock();
    
    _cs = LOW;
    
    _spi.write(&cmd[0],3,&data_buffer[0],0);
    
    _cs = HIGH;
    
    _spi.unlock();
    
    /* keeps checking the WEL bit i status reg to make sure erase completes*/
    
    do{
    
    status = read_status_reg();
    status = status & 0x1; 
    
    i--; 
      
    }while(status == BUSY && i>0);
    
    /* if timeout elapses*/
    
    if( i < 1 && status == BUSY){
        
        return TIMEOUT_ERROR;
    }
    else {
        return TIMEOUT_OK;
    }   
       
}

/**
* @brief    chip_erase()
* @details  erases all memory on chip
* @param    NA
* @return   if a timeout has occured i.e. device is busy for too long
* @warning  write enable must be called prior to calling this function or or
*           instruction will be ignored. 
* 
*/

bool rm25c512cl :: chip_erase(){
    
    char cmd[1];
    char data_buffer[1];// not used here but required as function parameter
    char status;
    uint32_t i = TIMEOUT_BYTES;
    
    cmd[0] = CERS;
        
    _spi.lock();
    
    _cs = LOW;
    
    _spi.write(&cmd[0],1,&data_buffer[0],0);
    
    _cs = HIGH;
    
    _spi.unlock();
    
    /* keeps checking the WEL bit i status reg to make sure erase completes*/
    
    do{
    
    status = read_status_reg();
    status = status & 0x1; 
    
    i--; 
      
    }while(status == BUSY && i>0);
    
    /* if timeout elapses*/
    
    if( i < 1 && status == BUSY){
        
        return TIMEOUT_ERROR;
    }
    else {
        return TIMEOUT_OK;
    }   
    
  
    
}

