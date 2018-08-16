
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
    rcvStr = b""
    while 1:
        # length = max(1, min(2048, com.in_waiting))
        # rcv = com.read(length)
        if(time.time() - timestart > timeoutS):
            break
        len = com.in_waiting
        if len:
            rcv = com.read(len)
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

    def calcCheckSum(self):
        if self.len%2 != 0:
            print("len % 2 != 0,len:",self.len)
            return None
        sum = self.data[1]*256 + self.data[0]
        for i in range(1,int(self.len/2)):
            temp = (self.data[i*2+1]*256 + self.data[i*2])
            sum = sum^temp
        return sum

    def toString(self):
        objStr = "\n----------------------------\n"
        objStr += (("start flag:0x%02X\ntotal opack:"+(str)(self.packTotal)+"\npack ID:"+(str)(self.packID)+"\nlen:"+(str)(self.len)+"\ncheck sum:0x%02X\n") %(self.packStart,self.checkSum))
        objStr +="data:\n"
        for i in range(MEMORY_PAGE_SIZE):
            objStr += ("%02X " %(self.data[i]))
        objStr += "\n----------------------------\n"
        return objStr
    
    def toBytes(self):
        ret = []
        ret.append(self.packStart&0xff)
        ret.append(self.packStart>>8&0xff)
        ret.append(self.packStart>>16&0xff)
        ret.append(self.packStart>>24&0xff)
        ret.append(self.packTotal&0xff)
        ret.append(self.packTotal>>8&0xff)
        ret.append(self.packID&0xff)
        ret.append(self.packID>>8&0xff)
        ret.append(self.len&0xff)
        ret.append(self.len>>8&0xff)
        for i in self.data:
            ret.append(i)
        ret.append(self.checkSum&0xff)
        ret.append(self.checkSum>>8&0xff)
        return ret



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

totalLen = (int)(len(appBin)/MEMORY_PAGE_SIZE)
if len(appBin) % MEMORY_PAGE_SIZE != 0:
    totalLen += 1
pack = IAP_USART_Pack()
pack.packTotal = totalLen
print("total pack len:",totalLen)

for i in range(totalLen):
    pack.packID = i
    if i+1 != totalLen:
        pack.len = MEMORY_PAGE_SIZE
    else:
        pack.len = len(appBin) % MEMORY_PAGE_SIZE
    for j in range(pack.len):
        pack.data[j] = appBin[i*MEMORY_PAGE_SIZE+j]
    for j in range(pack.len,MEMORY_PAGE_SIZE):
        pack.data[j] = 0
    pack.checkSum = pack.calcCheckSum()
    transBytes = pack.toBytes()
    print(pack.toString())
    ackBytes=[0x78,0x3E,0x00,0xAE,pack.packID&0xff,pack.packID>>8&0xff,pack.packTotal&0xff,pack.packTotal>>8&0xff,1,0,0]
    ...
    WriteCOM(com,transBytes,ackBytes,1)


com.close()
