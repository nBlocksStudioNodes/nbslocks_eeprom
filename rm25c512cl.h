/**
* @file     rm25c512cl.h
* @brief    header file for EEPROM interface
* 
*
* @author   Ian Aherne
*
* @date     05/09/18
*/

#ifndef RM25C512CL_H
#define RM25C512CL_H


#include "mbed.h"


class rm25c512cl{
    
    
    public:
    
    /*Opcodes for EEprom*/
        typedef enum{
            
            WREN = 0x06,
            WR = 0x02,
            READ = 0x03,
            RDSR = 0x05,
            WRSR = 0x01,
            PERS = 0x42,
            CERS = 0x60
            
            
        }opCode_t;
        
        
        
        typedef enum{
            
            LOW = 0,
            HIGH = 1
            
        }pinState_t;
        
        /*these values are time out for writes to EEprom, values can be changed if timeout is occuring too early*/
        
        typedef enum{
            
            
            TIMEOUT_OK = 0,
            TIMEOUT_ERROR = 1,
            
            TIMEOUT_BYTE =  322632,  
            TIMEOUT_BYTES = 32263226
                     
        }timeouts_t;
        
        typedef enum{
            
            SUCCESS = 0,
            FAILURE = 1
            
        }result_t;
        
        typedef enum{
            
            READY = 0x0,
            BUSY = 0x1,
            DISABLED = 0x0,
            ENABLED = 0x2
            
        }device_status_t;
        
        
            
        
        
        
        rm25c512cl(PinName MOSI,PinName MISO,PinName SCLK,PinName CS);
        bool write_enable();
        char read_status_reg();
        bool write_bytes(uint16_t address,char* data,uint16_t data_size);
        bool write_byte(uint16_t address,char value);
        void read_bytes(uint16_t address, char* data_buffer,uint16_t data_buffer_size);
        char read_byte(uint16_t address);
        bool page_erase(uint16_t address);
        bool chip_erase();
        
    
    
    
    private:
    
        SPI _spi;
        DigitalOut _cs;
    
    
    
    
};







#endif
