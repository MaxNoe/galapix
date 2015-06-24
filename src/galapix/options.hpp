/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HEADER_GALAPIX_GALAPIX_OPTIONS_HPP
#define HEADER_GALAPIX_GALAPIX_OPTIONS_HPP

#include <vector>
#include <string>

#include "math/size.hpp"

class Options
{
public:
  bool fullscreen;
  Size geometry;
  int  anti_aliasing;

  std::string database;
  std::vector<std::string> patterns;
  int         threads;
  std::vector<std::string> rest;

  Options() :
    fullscreen(false),
    geometry(800, 600),
    anti_aliasing(0),

    database(),
    patterns(),
    threads(),
    rest()
  {}
};

#endif

/* EOF */
