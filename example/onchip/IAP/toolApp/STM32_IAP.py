
import serial
import time
import sys



# #define IAP_USART_PACK_START_FLAG  0xAE003D72       //little edian
# #define IAP_USART_PACK_ACK_START_FLAG  0xAE003E78   //little edian

# typedef struct{
#     uint32_t packStart;
#     uint16_t packTotal;
#     uint16_t packID;                 // start from 0
#     uint16_t len;
#     uint8_t  data[MEMORY_PAGE_SIZE]; //fixed length MEMORY_PAGE_SIZE, if less than MEMORY_PAGE_SIZE, padding with 0
#     uint16_t checkSum;    //^ all bytes before
# }IAP_USART_Pack_t;  //little edian

MEMORY_PAGE_SIZE = 1024

def WriteCOM(com,bytes,ack,timeoutS):
    com.write(bytes)
    time.sleep(0.5)
    timestart = time.time()
    rcvStr = ""
    while 1:
        # length = max(1, min(2048, com.in_waiting))
        # rcv = com.read(length)
        if(time.time() - timestart > timeoutS):
            break
        len = com.in_waiting
        if len:
            rcv = com.read(len).decode()
            rcvStr = rcvStr+rcv
        if ack in rcvStr:
            break
        time.sleep(0.1)
    return rcvStr


def readBinfile(path):
    file = open(path, "rb")
    content = file.read()
    file.close()
    return content
    

class IAP_USART_Pack:
    packStart = 0xAE003D72
    packTotal = 0x0000
    packID    = 0x0000
    len       = 0x0000
    data = [0 for i in range(MEMORY_PAGE_SIZE)]
    checkSum = 0x0000


#######################
# main
try:
    comName = sys.argv[1]
    binFilePath = sys.argv[2]
except Exception as e:
    print("please enter param, STM32_IAP.py COM3 ./app.bin")
    exit(1)

com = serial.Serial()
com.baudrate = 115200
com.port = comName
com.bytesize = 8
com.parity = "N"
com.stopbits = 1
com.timeout = None
com.open()


appBin = readBinfile(binFilePath)
print(appBin)



com.close()
