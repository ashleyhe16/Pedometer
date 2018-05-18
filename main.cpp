
#include "mbed.h"
#include "FXOS8700CQ.h"
#include "Adafruit_RGBLCDShield.h"
#include "MCP23017.h"
#include "RTclock.h"
#include "keyreaderadafruit.h"
#include "keyreadernull.h"
#include "lcd2004.h"
#include "lcdadafruit.h"

Serial pc(USBTX, USBRX);

I2C cI2C(I2C_SDA, I2C_SCL);    // D3, D6
MCP23017 mcp23017 = MCP23017(cI2C, 0x40);
Adafruit_RGBLCDShield lcd(mcp23017);

FXOS8700CQ device(I2C_SDA, I2C_SCL);

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
#define ON 0x1
#define OFF 0x0   

volatile int num_steps = 0; // initialize number of steps
volatile int actual_steps = 0; // reduced noise version
volatile int current = 0; // the current z value

// initialize first 20 readings to base z values
volatile int accelerations[10] = {2042,2042,2042,2042,2042,2042,2042,2042,2042,2042}; // array to store acceleration values
int threshold = 800; // threshold to start counting steps
volatile int difference = 0;

// Allows to set the backlight, if the LCD backpack is used
void SetBacklight(unsigned char status)
{
    pc.printf("Backlight: %i\n\r", status);
    
    mcp23017.digitalWrite(8, (~(status >> 2) & 0x1));
    mcp23017.digitalWrite(7, (~(status >> 1) & 0x1));
    mcp23017.digitalWrite(6, (~status & 0x1));
}

int main()
{    
    pc.printf("\n\rSTART\n\r");
    pc.baud(110000);
    // Share the I2C across multiple devices
    I2C cI2C(I2C_SDA, I2C_SCL);    // D3, D6
    cI2C.frequency(110000);    // I2C can handle two different frequencies - switch to high speed if asked, changed from 400000
    LCDadafruit cLCD(cI2C);
    KeyReaderAdafruit cKeys(cI2C);
    //device.baud(9600);
    // I2C init
    //mcp23017.i2c.frequency(400000);
    
    mcp23017.config(0,1,0);
    wait(0.5);
            
    lcd.begin(16,2);
    lcd.display();
    
    lcd._putc('A');
    wait(0.5);
    lcd.clear();
    
    while (1){
        
        Data values = device.get_values();
        
        current = values.az*1000;
        pc.printf("%i\n\r", current);
            // update the array of z values
        for (int k = 10; k > 0; k--){  // shift old values down by one to insert new value in the front      
            accelerations[k] = accelerations[k-1];
            }
            
        accelerations[0] = current; // set first element to newly read z value
        
        // find the minimum of stored values
        int minimum = accelerations[0]; 
        for ( int c = 1 ; c < 10 ; c++ ) 
        {
            if ( accelerations[c] < minimum ) 
            {
               minimum = accelerations[c];
            }
         } 
        
        // find the maximum of stored values
        int maximum = accelerations[0]; 
        for ( int c = 1 ; c < 10 ; c++ ) 
        {
            if ( accelerations[c] > maximum ) 
            {
               minimum = accelerations[c];
            }
        } 
        
        difference = maximum - minimum;
        
        if (difference >= threshold){
            num_steps++;
        }
        
        actual_steps = num_steps;
        //lcd.printf("%i", actual_steps);
        pc.printf("Steps:%02i\n\r", actual_steps);
        lcd.printf("Steps:");
        lcd.setCursor(7,0);
        lcd.printf("%02i", actual_steps);
        wait(0.3);
        lcd.clear();
        
    }
    //lcd.printf("Steps:%02i", actual_steps);
    //lcd.setBacklight(GREEN);
    
     
    //LCD2004 cLCD(cI2C);
    //KeyReaderNull cKeys(cI2C);
            
}
