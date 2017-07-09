//
// ChameleonGL - A small framework for OpenGL.
// Copyright (C) 2012-2017 Srinivas Kaza
//
// This file is part of ChameleonGL.
//
// ChameleonGL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ChameleonGL is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ChameleonGL.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>

const std::string red("\033[1m\033[31m");
const std::string reset_code("\033[0m");

#define GET_TIME()       std::chrono::duration_cast<std::chrono::milliseconds> \
    (std::chrono::system_clock::now().time_since_epoch())
#define DIFF(a, b)       (a - b).count()
#define DEBUG(a)         std::cout << a << "\n"
#define ERROR(a)         std::cerr << red << a << reset_code << "\n"

/**
 * Returns the string representation of an object,
 * assuming that it has defined the stream operator
 *
 * @param s object to get string representation of
 * @return string representation of {@code s}
 */
template<class T>
inline std::string TOS(const T& s) {
    std::stringstream a;
    a << s;
    return a.str();
}

std::string read_file(const std::string& filename) {
    std::ifstream t(filename);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}
