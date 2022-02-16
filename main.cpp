#include "PinNames.h"
#include "mbed.h"
#include <string>
#include <vector>

// マイコン上に実装されたLED
std::vector<DigitalOut> onboard_leds = {
    DigitalOut(LED1), 
    DigitalOut(LED2), 
    DigitalOut(LED3), 
    DigitalOut(LED4)};

// シリアル通信用インスタンス
static UnbufferedSerial *pc;

int main() 
{
    uint8_t count = 0;
    std::string str = "He\r\n";

    while(1) 
    {
        pc = new UnbufferedSerial(USBTX, USBRX,9600);
        
        str = std::to_string(count) + "\n";
        pc->write(str.c_str(), 7);  
        
        onboard_leds[0] = 1;
        wait_us(500*1000);
        onboard_leds[0] = 0;
        wait_us(500*1000);

        ++count;
    }
}
