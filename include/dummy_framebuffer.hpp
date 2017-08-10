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

#include "abstract_surface.hpp"

class DummyFramebuffer : public AbstractSurface {
  public:
    DummyFramebuffer() :
        _width(-1),
        _height(-1) {

    }

    DummyFramebuffer(const int& width, const int& height) :
        _width(width),
        _height(height) {

    }

    virtual int get_width() const {
        return _width;
    }

    virtual int get_height() const {
        return _height;
    }

    virtual void on_resize(const int& width, const int& height) {
        _width = width;
        _height = height;
    }

    virtual void bind() {
        // Does absolutely nothing
    }

    virtual void unbind() {
        // Also does absolutely nothing
    }
  private:
    int _width;
    int _height;
};
