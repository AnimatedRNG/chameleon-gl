#pragma once

#include <iostream>
#include <chrono>

const std::string red("\033[1m\033[31m");
const std::string reset_code("\033[0m");

#define GET_TIME()       std::chrono::duration_cast<std::chrono::milliseconds> \
    (std::chrono::system_clock::now().time_since_epoch())
#define DIFF(a, b)       (a - b).count()
#define DEBUG(a)         std::cout << a << "\n"
#define ERROR(a)         std::cerr << red << a << reset_code << "\n"
