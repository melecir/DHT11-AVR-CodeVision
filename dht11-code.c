/*
 * dht11-code.c
 *
 * Created: 18/01/25 7:57:20 PM
 * Author: W7U  :
 * Site:  Melec.ir
 */

#include <io.h>
#include <mega16.h>
#include <alcd.h>
#include <delay.h>
#include <stdio.h>


#define DHT11_PORT PORTA
#define DHT11_PORTPIN PORTA1
#define DHT11_DDR DDRA
#define DHT11_DDRPIN DDA1
#define DHT11_PINPORT PINA
#define DHT11_PIN PINA1

#define DHT11_INPUT_MODE() DHT11_DDR&=~(1<<DHT11_DDRPIN)
#define DHT11_OUTPUT_MODE() DHT11_DDR|=(1<<DHT11_DDRPIN)
#define DHT11_LOW() DHT11_PORT&=~(1<<DHT11_PORTPIN)
#define DHT11_HIGH() DHT11_PORT|=(1<<DHT11_PORTPIN)

#define DHTLIB_OK                               0
#define DHTLIB_ERROR_CHECKSUM                    -1
#define DHTLIB_ERROR_TIMEOUT                    -2

int read_dht11(int *temp,int *humidity);

void main(void)
{
char lcd_buff[17];
int temp=0;
int humidity=0;
int status=0;

lcd_init(16);
lcd_clear();
lcd_puts("DHT11*Melec.ir");
delay_ms(2000); //dht11 needs 2000ms ...   

while (1)
    {
   status=read_dht11(&temp,&humidity);
      sprintf(lcd_buff,"Temp:%02d H:%02d  ",temp,humidity);      
      lcd_clear();   
      lcd_puts(lcd_buff);
      delay_ms(1);
      lcd_gotoxy(0,1);
      lcd_putsf("Melec.ir*DHT11");
      delay_ms(2000); //dht11 needs 2000ms ...   
    }
}

// read_dht11 functions
// read_dht11 functions
int read_dht11(int *temp,int *humidity){
unsigned char bytes[5];
unsigned char sum;
unsigned int bitcount=8;
unsigned int loopcount=0;
unsigned int byteid=0;
int i=0;
// EMPTY BUFFER
    for (i=0; i< 5; i++) bytes[i] = 0;
// REQUEST SAMPLE
    DHT11_OUTPUT_MODE();  
    DHT11_LOW();    
    delay_ms(20);
    DHT11_HIGH();
    delay_us(2);
    DHT11_INPUT_MODE();   
    DHT11_LOW();
    loopcount=0;    
    while(!(DHT11_PINPORT&(1<<DHT11_PIN))){ //pin is low
     delay_us(40);
     loopcount++;  
    }      
    if (loopcount>80) return DHTLIB_ERROR_TIMEOUT;
    loopcount=0;    
    while((DHT11_PINPORT&(1<<DHT11_PIN))){ //pin is high
     delay_us(1);
     loopcount++;
    }     
     if (loopcount>80) return DHTLIB_ERROR_TIMEOUT;
    for(i=0;i<40;i++){ 
    loopcount=0;    
    while(!(DHT11_PINPORT&(1<<DHT11_PIN))){}  
    while((DHT11_PINPORT&(1<<DHT11_PIN)) && loopcount<100){loopcount++;delay_us(1);}
    if(loopcount>15)
    bytes[byteid] |= (1 << bitcount);
     if (bitcount == 0)   // next byte?
        {
            bitcount = 7;    // restart at MSB
            byteid++;      // next byte!
        }
        else bitcount--; 
    } 
    *humidity    = bytes[0];
    *temp = bytes[2];
    sum = (bytes[0] + bytes[1]+ bytes[2]+ bytes[3])&0xff;  
    if (bytes[4] != sum) return DHTLIB_ERROR_CHECKSUM;  
    return DHTLIB_OK;
}