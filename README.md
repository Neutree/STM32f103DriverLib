[STM32f103 Peripherals Driver Lib](https://github.com/Neutree/STM32f103DriverLib)
======

To make program on STM32 more easier, we packet every peripheral and module as a C++ class, mask operation steps instead of function-oriented interface.

**e.g. How to use USART to send data**

```cpp
# include "USART.h"
USART com(1,115200);
void main()
{
    com<<"test string\r\n";
}
```


**e.g. Use temperature and humidity sensor**

```cpp
#include "I2C.h"
#include "Si7021.h"
#include "USART.h"
#include "TaskManager.h"

USART log(1,115200);
I2C i2c1(1);
Si7021 tempHumi;

int main()
{
  tempHumi.Init(&i2c1);
  while(1)
  {
    tempHumi.Update();

    log<<"temperature:"<<tempHumi.GetTemperature()<<"\n";
    log<<"humidity:"<<tempHumi.GetHumidity()<<"\n";

    TaskManager::DelayMs(100);
  }
}
```

---------------------
# How to use

1. Install keil MDK 5 on windows
2. clone or download this project to your PC
```
git clone https://github.com/Neutree/STM32f103DriverLib.git
```
3. open example project in `example` folder with MDK5, compile and download to your board and run
4. There's three files you should pay attention: `Main.cpp`,`Configuration.h`,`UserInterrupt.cpp`, code your code and config configuration.
  * Uncomment what you need in `Configuration.h`
  * And every one driver have one folder, you should see the header file before you start code, there's some attention and doc in the header file.

# Convention

## File encode format

select **UTF-8 without signature** before you start code

> keil->edit->configuration->editor->encoding
 
## Folders

`lib`：driver lib
> `lib/math/`: math lib

> `lib/onchip/`: driver releated to stm32
>> `lib/onchip/driverName`: driver source code folder


> `lib/offchip/`: module driver not belong to stm32

>> `lib/offchip/driverName`: driver source code folder

`example`: driver demo
> `example/onchip/driverName`：example related to stm32

> `example/offchip/driverName`：module example not belong to stm32

`template`：project template

`tool`: tools

# Driver Brief

## On chip
* **Interrupt**：片上外设中断管理，所有的中断相关的逻辑都在这里
* **GPIO**：通用IO驱动，包括基本IO配置与设置引脚和读取引脚状态
* **USART**：串口驱动，包括了串口1到串口4的驱动，发送可使用DMA，接收使用中断方式，每个串口使用单独的FIFO
* **ADC**：ADC驱动
* **I2C**：I2C驱动，包含了中断方式和DMA方式
* **PWM**：PWM驱动
* **Timer**：定时器驱动
* **TaskManager**：使用systick作为时基，可用来获取系统时间戳、延时（包含软件延时）和定时调用函数
* **Capture**：输入捕获，包括了定时器捕获和外部中断捕获
* **Flash**：STM32内部Flash相关操作，包括对Flash的读写
* **CAN**
* **SPI**
* **IWDG**


## Module Driver
* **LED**：基于GPIO和PWM对LED的相关操作，包括开、关、闪烁等
* **esp8266**：esp8266是串口WIFI模块，基于USART对esp8266的相关操作，其中包括了驱动文件（esp8266.cpp）和应用层文件(SockeEsp8266.cpp),应用层文件是使用socket的接口进行通信的实现
* **GPS**：使用USART对GPS模块的驱动
* **HMC5883L**：磁力计，基于I2C通信的HMC5883L的驱动
* **StepMotor**：步进电机驱动，基于GPIO进行控制
* **MPU6050**：6轴惯导驱动，包括了角速度、加速度的读取
* **RemoteControl**:RC遥控器驱动，包含了PWM输入捕获和PPM输入捕获，包括了中断方式和定时器输入捕获方式
* **~~Ultrasonic~~** ~~：超声波驱动，包括了systick版本和定时器版本~~
* **HMI**
* **Si7021**: Temperature and humidity sensor
* **PN53x**: RFID/NFC reader
* **Relay**
* **SPI Flash**
* **Ethernet**: W5500 ethernet communication
* **GNSS**: GNSS NMEA parser (GPS)
* **ZPH01**
* **SHARP_1014_PM2.5**
* **MFRC522**
* **MG996R**
* **MZH14**
* **yishan_PM2.5**
* **Door**: Reed Switch
* **HCHO**
* **Joystick**



## Code Style
Refer [Here](https://github.com/neutree/simple-code-conventions)

# Contributors

[AllenWang](https://github.com/afshare)

Danyuanhong

[infiniteYuan](https://github.com/InfiniteYuan1)

[lissettecarlr](https://github.com/lissettecarlr)

[neucrack](http://neucrack.com)

[xz1996](http://git.oschina.net/xz1996)

