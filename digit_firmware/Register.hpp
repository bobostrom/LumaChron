#ifndef REGISTER
#define REGISTER
#include "enums.hpp"
#include "constants.hpp"
#include "pico/stdlib.h"
#include "../lib/RGB.hpp"
#include "functions.hpp"
#include "regNum.hpp"

#include "pico/util/queue.h"
#include "Queue.hpp"
#include "../lib/Serial.hpp"


extern queue_t coreQueue;
extern Serial ser;


template <typename T> class Register{
  public:
  regNum_t regnum;
  uint8_t size;
  T data;
  uint8_t blinker = 0;
  uint8_t blinkerCounter=0;
  int getmyData(uint8_t dataSrc){/*uart_putc(uart0,'D');*/return( PICO_ERROR_NOT_PERMITTED);};
  
  Register& operator=(const T& x) &
  {
    data = x;
    return *this;
  };
  
  operator T() const { return data; };

  Register operator + (const Register& obj){
    Register<uint8_t> temp;
    temp.data = data + obj.data;
    return temp;
  };

  Register& operator++()
    {
        ++data;
        // returned value should be a reference to *this
        return *this;
    }
  
  Register operator++(int)
    {
        // returned value should be a copy of the object before increment
        Register temp = *this;
        ++data;
        return temp;
    }

  Register<T> operator * (const Register<T>& obj ){
      Register<T> temp;
      temp.data = data * obj.data;
      return temp;
    };
  Register<RGB> operator * (const int& obj){
      Register<RGB> temp;
      temp.data = data * obj;
      return temp;
    };
  // Register operator * (const Register<uint8_t>& obj){
  //     Register<uint8_t> temp;
  //     temp.data = data * obj.data;
  //     return temp;
  //   };

  // Register operator * (const int& obj){
  //   Register<uint8_t> temp;
  //   temp.data = data*obj;
  //   return temp;
  // };
  // Register<RGB> operator* (const uint8_t obj){
  //   RGB temp;

  // }
    // Register(){};
    // Register(regNum_t); //declaration that applies to the construction templates
    // Register(regNum_t,uint8_t);

};

// template<typename T>
//,typename std::enable_if<!std::is_same<T,std::uint8_t>::value>::type* = nullptr
// template<typename T>
// Register<T> Register<T>::operator * (const Register<T>& obj){
//     Register<T> temp;
//     temp.data = data * obj.data;
//     return temp;
//   };
// template<>
// Register<RGB> Register<RGB>::operator * (const uint8_t& obj){
//     Register<RGB> temp;
//     temp.data = data * obj;
//     return temp;
//   };
// template<>
// Register<short>::Register(regNum_t);
// template<>
// Register<devType>::Register(regNum_t);
// template<>
// Register<uint8_t>::Register(regNum_t);
// template<>
// Register<char>::Register(regNum_t);
// template<>
// Register<RGB>::Register(regNum_t);
// template<>
// Register<digitMode>::Register(regNum_t);
// template<>
// Register<serialNum_t>::Register(regNum_t);
// // template<>
// // Register<uint8_t>::Register(regNum_t, uint8_t);

template<> 
int Register<char>::getmyData(uint8_t);

template<> 
int Register<uint8_t>::getmyData(uint8_t);

template<>
int Register<RGB>::getmyData(uint8_t);
template<>
int Register<digitMode>::getmyData(uint8_t);
template<>
int Register<boot>::getmyData(uint8_t);

#endif