#include <iostream>
#include <math.h>

#include "log.hpp"

int main() {
  Log::Logger::init();

  LOG(warning) << "Log started";
  LOG(info) << "Supports weird chars? ~´Ç¨";

  return 0;
}
