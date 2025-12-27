#include "Register.hpp"



/*Construction functions where the value given is a different type than the type of the register*/
//create a device type register
// template<>
// Register<short>::Register(regNum_t regNumIn){
//     size=1;
//     regnum=regNumIn;
//     data=0x00;
  
// };
// template<>
// Register<devType>::Register(regNum_t regNumIn){
//     size=1;
//     regnum=regNumIn;
//     data=NONE;
  
// };
// //construct a uint8_t register
// template<>
// Register<uint8_t>::Register(regNum_t regNumIn){
//   size=1;
//   regnum=regNumIn;
//   data=0x00;
// };
// //construct a char register
// template<>
// Register<char>::Register(regNum_t regNumIn){
//   size=1;
//   regnum=regNumIn;
//   data=' ';
// };
// //construct a RGB register
// template<>
// Register<RGB>::Register(regNum_t regNumIn){
//   size=3;
//   regnum=regNumIn;
//   data.r=0xFF;
//   data.g=0xFF;
//   data.b=0xFF;
// };
// //construct a digit mode register (uint8_t)
// template<>
// Register<digitMode>::Register(regNum_t regNumIn){
//   size=1;
//   regnum=regNumIn;
//   data=NORMAL;
// };
// //construct a serial number type register (uint64_t)
// //dependent on a typedef for serialNum_t
// template<>
// Register<serialNum_t>::Register(regNum_t regNumIn){
//   size=8;
//   regnum=regNumIn;
//   data =0x0;
// };
// template<>
// Register<uint8_t>::Register(regNum_t regNumIn, uint8_t dataIn){
//   size=1;
//   regnum=regNumIn;
//   data = dataIn;
// };


/*get data routines that are different depending on the type of data of the register*/
// get data for char
template<> 
int Register<char>::getmyData(uint8_t dataSrc){
    int buf;
    int inCkSum;
    int calcCkSum;
    // sprintf(outStr,"symbol get my data 0x%02x\n",dataSrc);
    // ser.puts('C');
    buf=getDataFrom(dataSrc); if(buf<0) return(buf);
    // printf("symbol got my data 0x%02x\n",buf);
    // stdio_flush();
    inCkSum = getDataFrom(dataSrc); if(inCkSum<0) return(inCkSum);
    calcCkSum = calcFletcherCksum(regnum,size,&buf);
    if(inCkSum != calcCkSum){
        char temp[100];
        sprintf(temp,"\nChecksum error: recieved 0x%x calculated 0x%02x\n", inCkSum,calcCkSum);
        ser.puts(temp);
        // ser.puts( temp);
        if(inCkSum != 0xFF) return(PICO_ERROR_IO);
    }
    data=buf;  //the checksum matched, write the data into the register
    return SUCCESS;  //read and processed one command packet
};
//get data for uint8_t
template<> 
int Register<uint8_t>::getmyData(uint8_t dataSrc){
    int buf;
    int inCkSum;
    int calcCkSum;
    // ser.puts('U');
    buf=getDataFrom(dataSrc); if(buf<0) return(buf);
    inCkSum = getDataFrom(dataSrc); if(inCkSum<0) return(inCkSum);
    calcCkSum = calcFletcherCksum(regnum,size,&buf);
    // ser.puts('B');
    if(inCkSum != calcCkSum){ 
        char outStr[100];
        sprintf(outStr,"\nChecksum error: recieved 0x%02x calculated 0x%02x\n", inCkSum,calcCkSum);
        ser.puts(outStr);
        if(inCkSum != 0xFF) return(PICO_ERROR_IO);
    }
    data=buf;
    return PICO_OK;
};
//get data for RGB
template<>
int Register<RGB>::getmyData(uint8_t dataSrc){
    int buf[3];
    int inCkSum;
    // ser.puts('R');
    for(int i=0;i<size;i++){
        buf[i]=getDataFrom(dataSrc);
        if(buf[i]<0) return(buf[i]);
    }
    inCkSum = getDataFrom(dataSrc); if(inCkSum<0) return(inCkSum);
    if(inCkSum != calcFletcherCksum(regnum,size,buf)){
        char outStr[100];
        sprintf(outStr,"\nChecksum error: recieved 0x%02x calculated 0x%02x\n", inCkSum,calcFletcherCksum(regnum,size,buf));ser.puts(outStr);
        if(inCkSum != 0xFF) return(PICO_ERROR_IO);
    }
    data.r=buf[0];
    data.g=buf[1];
    data.b=buf[2];
    return PICO_OK;
};
template<>
int Register<digitMode>::getmyData(uint8_t dataSrc){
    digitMode buf;
    int inCkSum;
    int calcCkSum;
    // ser.puts('D');
    buf=(digitMode)getDataFrom(dataSrc); if(buf<0) return(buf);
    inCkSum = getDataFrom(dataSrc); if(inCkSum<0) return(inCkSum);
    int junk=(int)buf;
    calcCkSum = calcFletcherCksum(regnum,size,&junk);
    
    if(inCkSum != calcCkSum){ 
        char outStr[100];
        sprintf(outStr,"\nChecksum error: recieved 0x%02x calculated 0x%02x\n", inCkSum,calcCkSum);
        ser.puts(outStr);
        if(inCkSum != 0xFF) return(PICO_ERROR_IO);
    }
    data=buf;

    if(data == RAWPIXEL){
        int in;
        // ser.puts('P');
        queueItem qMessage;
        
        for(int i=0;i<NUMPIXELSPERDIGIT*3;i++){
            in =getDataFrom(dataSrc);
            if(in<0){return PICO_ERROR_GENERIC;}
            qMessage.data[i]=in;
        }
        ser.puts("\nReceived:\n");
        for(int i=0;i<NUMPIXELSPERDIGIT*3;i++){
            char outStr[10];
            if(i%16==0 &&i!=0)ser.putc('\n');
            sprintf(outStr," 0x%02X",qMessage.data[i]);ser.puts(outStr);

        }
        ser.putc('\n');
        //now send a message to the other core with pixel data
        qMessage.command = data;
        qMessage.size = 3*NUMPIXELSPERDIGIT;
        
        queue_add_blocking(&coreQueue,&qMessage);
    
    }

    return PICO_OK;
}

//get data for boot
template<> 
int Register<boot>::getmyData(uint8_t dataSrc){
    boot buf;
    int inCkSum;
    int calcCkSum;
    // ser.puts('b');
    buf=(boot)getDataFrom(dataSrc); if(buf<0) return(buf);
    inCkSum = getDataFrom(dataSrc); if(inCkSum<0) return(inCkSum);
    int junk=(int)buf;
    calcCkSum = calcFletcherCksum(regnum,size,&junk);
    // ser.puts('B');
    if(inCkSum != calcCkSum){ 
        char outStr[100];
        sprintf(outStr,"\nChecksum error: recieved 0x%02x calculated 0x%02x\n", inCkSum,calcCkSum);
        ser.puts( outStr);
        if(inCkSum != 0xFF)return(PICO_ERROR_IO);
    }
    data=buf;
    return PICO_OK;
};