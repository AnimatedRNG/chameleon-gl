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
#include <unordered_map>
#include <functional>
#include <memory>

#include "util.hpp"
#include "opengl_utils.hpp"

class UniformMap {
  public:
    UniformMap() :
        _map(new std::unordered_map <std::string,
             std::function<void(Program&)>>) {

    }

    UniformMap(const UniformMap& other) {
        auto old_map = *(other._map);
        auto map_copy = new std::unordered_map
        <std::string, std::function<void(Program&)>> (old_map);
        this->_map = std::unique_ptr <
                     std::unordered_map<std::string,
                     std::function<void(Program&)>>
                     > (map_copy);
    }

    template <typename T>
    void set(const std::string& key, T value) {
        (*_map)[key] = [key, value](Program & program) {
            program.set_uniform(key, value);
        };
    }

    void set(const std::string& key, Texture value) {
        (*_map)[key] = [&](Program & program) {
            //value.bind();
            program.set_uniform(key, value);
        };
        // Also log the fact that are using a texture -- so bind an image unit
    }

    void apply(Program& program) {
        for (auto& pair : *_map) {
            pair.second(program);
        }
    }
  private:
    std::unique_ptr < std::unordered_map <
    std::string, std::function<void(Program&) >>> _map;
};
