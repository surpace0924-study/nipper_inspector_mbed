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

void receivedData();

// PCから送られてくる制御用データ
std::string cmd_data = "";
std::string cmd_data_buf = "";

void printString(std::string str)
{
    pc->write(str.c_str(), str.length());
}

// 受信データをデコード
std::vector<int> decodeReceivedData()
{
    // 文字を数値化
    char cmd_data_char = cmd_data[0];
    int data_int = (int)cmd_data_char - 33;

    // pc->write(std::to_string(data_int).c_str(), 5);

    // vectorに変換
    std::vector<int> data(4);
    for (int i = 0; i < 4; ++i) {
        data[i] = (data_int >> i) & 1;
    }

    return data;
}

int main() 
{
    uint8_t count = 0;
    std::string str = "He\r\n";

    while(1) 
    {       
        pc = new UnbufferedSerial(USBTX, USBRX,9600);
        pc->attach(receivedData, SerialBase::RxIrq);

        // 受信データのデコード
        std::vector<int> rcv_data = decodeReceivedData();
 
        for (int i = 0; i < onboard_leds.size(); ++i)
        {
            onboard_leds[i] = rcv_data[i];
        }
        wait_us(500*1000);
        
        for (int i = 0; i < onboard_leds.size(); ++i)
        {
            onboard_leds[i] = 0;
        }
        wait_us(500*1000);

        // ++count;
    }
}

// シリアルデータ受信割り込み
void receivedData()
{
    char gotchar[10]={0,0,0,0,0,0,0,0,0,0};
    
    //読み取り
    pc->read(gotchar, 5);
    cmd_data = gotchar;
    cmd_data = cmd_data[0];
}
