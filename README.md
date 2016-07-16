# STM32f103 Peripherals Driver Lib


_ _ _
_ _ _
# 如何使用

1. 克隆项目到本地目录
2. 复制一个副本
3. 打开/template文件夹，该文件夹下面会有相应IDE的工程模板，直接打开运行即可（onchip表示只有STM32片上驱动，offchip表示有一些常用模块的驱动）
4. 需要关心的文件主要在工程的app文件夹下，有三个文件，Main.cpp,Configuration.h,UserInterrupt.cpp.分别为主函数所在、配置使用哪些资源，中断用户程序。
>  * 首先检查Configuration.h中需要使用的资源是否取消注释了，不需要使用的资源是否注释了
>  * 然后看Main.cpp中硬件定义及其参数,每个外设驱动都有一个文件，具体每个外设驱动的使用请看其头文件。

# 规范
## 目录规范

`lib`：驱动库目录
> `lib/math/`:外设会用到的算法库

> `lib/onchip/`:片上外设驱动
>> `lib/onchip/驱动名`:驱动接口的定义和对驱动的实现


> `lib/offchip/`:片外模块驱动

>> `lib/offchip/驱动名`:驱动接口的定义和对驱动的实现

`example`:驱动库demo，每个驱动一个文件夹,每个芯片一个文件夹
> `example/onchip/驱动名`：具体某个芯片的模块驱动

> `example/offchip/驱动名`：具体某个外围模块驱动

`template`：工程模板，每个IDE或者编译器一个文件夹，下面有特定芯片型号文件夹

`tool`:工具，如删除工程中间文件批处理文件

# 驱动简介

## 片上外设驱动
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


## 片外模块驱动
* **LED**：基于GPIO和PWM对LED的相关操作，包括开、关、闪烁等
* **esp8266**：esp8266是串口WIFI模块，基于USART对esp8266的相关操作，其中包括了驱动文件（esp8266.cpp）和应用层文件(SockeEsp8266.cpp),应用层文件是使用socket的接口进行通信的实现
* **GPS**：使用USART对GPS模块的驱动
* **HMC5883L**：磁力计，基于I2C通信的HMC5883L的驱动
* **StepMotor**：步进电机驱动，基于GPIO进行控制
* **MPU6050**：6轴惯导驱动，包括了角速度、加速度的读取
* **RemoteControl**:RC遥控器驱动，包含了PWM输入捕获和PPM输入捕获，包括了中断方式和定时器输入捕获方式
* **~~Ultrasonic~~** ~~：超声波驱动，包括了systick版本和定时器版本~~

## 代码风格
看[这里](https://github.com/neutree/simple-code-conventions)
