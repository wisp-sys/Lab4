#include "rccregisters.hpp"
#include "gpiocregisters.hpp"
#include "gpioaregisters.hpp"
#include <iostream>

template<typename T> // Данный класс отвечает за работу с портом ( вместо T будем передавать значения портов: GPIOA, GPOIC и т.д.)
struct Port
{
  static void Set(std::uint32_t value)
  {
    T::BSSR::Write(value);
  }
  
  static void Reset(std::uint32_t value)
  {
    T::BSSR::Write(value << 16U);
  }
  
  static void Toggle(std::uint32_t value)
  {
    T::ODR::Toggle(value);
  }
  
  static auto Get()
  {
    return T::IDR::Get();
  }
    
};




template<typename Port, uint8_t pinNum>  // Данынй класс отвечает за переключение одной ножки
struct Pin
{
  static void Set()
  {
    Port::Set(1U << pinNum);
  }
  
  static void Reset()
  {
    Port::Reset(1U << pinNum);
  }
  
  static void Toggle()
  {
    Port::Toggle(1U << pinNum);
  }
  
  static auto IsSet()
  {
    return ((Port::Get() & (1U << pinNum)) != 0 );
  }
  
  
};

template<typename Pin>  // Данынй класс отвечает за переключение одной ножки
struct Button
{
   static auto IsPressed()
  {
    if(!Pin::IsSet())
    {
      while(!Pin::IsSet())
      {
      
      }
      return true;
    }
    
    return false;
       
  }
};

template<typename ... Tleds> // Данный класс отвечает за работу с портом ( вместо T будем передавать значения портов: GPIOA, GPOIC и т.д.)
struct Leds
{
   static void Toggle()
  {
    (Tleds::Toggle(),...);
  }
};


void Delay()
{
  for(int i = 0; i < 1000000; i++)
  {
  }
}



using ButtonPin = Pin<Port<GPIOC>, 13>; // Вводим польовательский тип, который равен пину (Gpioc, 13)
using UserButton = Button<ButtonPin>;

using LED1 = Pin<Port<GPIOC>, 5>;
using LED2 = Pin<Port<GPIOC>, 8>;
using LED3 = Pin<Port<GPIOC>, 9>;
using LED4 = Pin<Port<GPIOA>, 5>;
using MyLeds = Leds<LED1, LED2, LED3, LED4>;



int main()
{

  RCC::CR::HSEON::On::Set();
  while(!RCC::CR::HSERDY::Ready::IsSet())
  {
  }
  RCC::CFGR::SW::Hse::Set();
  
  while(!RCC::CFGR::SWS::Hse::IsSet())
  {
  }
  
  RCC::CR::HSION::Off::Set();
  RCC::PLLCFGR::PLLSRC::HseSource::Set();
  RCC::PLLCFGR::PLLM0::Set(4U); // 8 MHz / 4 MHz = 2 MHz
  RCC::PLLCFGR::PLLN0::Set(50U); // 2 MHz * 50 MHz = 200 MHz (так как значения должны быть от 100 MHz до 432 MHz)
  RCC::PLLCFGR::PLLP0::Pllp6::Set(); // 100 MHz / 6 = 21 MHz 
  RCC::CR::PLLON::On::Set();
  
  
  while(RCC::CR::PLLRDY::Unlocked::IsSet())
  {
  }
  RCC::CFGR::SW::Pll::Set();
  while(!RCC::CFGR::SWS::Pll::IsSet())
  {
  }

    RCC::AHB1ENR::GPIOCEN::Enable::Set();
    RCC::AHB1ENR::GPIOAEN::Enable::Set();
    GPIOC::MODER::MODER13::Input::Set();
    GPIOC::MODER::MODER5::Output::Set();
    GPIOC::MODER::MODER8::Output::Set();
    GPIOC::MODER::MODER9::Output::Set();
    GPIOA::MODER::MODER5::Output::Set();
  
  for(;;)
    {
    if(UserButton::IsPressed())
    {
//      LED1::Toggle();
//      LED2::Toggle();
//      LED3::Toggle();
//      LED4::Toggle();
      MyLeds::Toggle();
    }
  
    Delay();
    
  }
  
  
}
