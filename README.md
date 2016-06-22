#STM32f103 Peripherals Driver Lib
#And some common module driver
#And come common math lib

_ _ _
_ _ _


> ## [STM32f103 peripherals
_ _ _

# 规范
## 目录规范

`lib`：驱动库目录
> `lib/math/`:外设会用到的算法库

> `lib/onchip/`:片上外设驱动
>> `lib/onchip/驱动名/interface/`:接口抽象

>> `lib/onchip/驱动名/芯片名/`:具体芯片对驱动的实现


> `lib/offchip/`:片外模块驱动
>> `lib/offchip/驱动名/interface/`:接口抽象

>> `lib/offchip/驱动名/芯片名/`:具体芯片对驱动的实现

`example`:驱动库demo，每个驱动一个文件夹,每个芯片一个文件夹
> `example/onchip/驱动名/芯片名/`：具体某个芯片的模块驱动

> `example/offchip/驱动名/芯片名/`：具体某个外围模块驱动

`template`：工程模板，每个IDE或者编译器一个文件夹

`tool`:工具，如删除工程中间文件批处理文件


## 代码风格
看[这里](https://github.com/neutree/simple-code-conventions)
