#include <iostream>
#include <chrono>
#include <thread>
#include <stdlib.h>

int main()
{
  while(true)
  {
    std::cout << "two!" << std::endl;
    std::this_thread::sleep_for( std::chrono::nanoseconds(1000000000));
  }
  
  return 0;
}
