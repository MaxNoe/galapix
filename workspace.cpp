/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <iostream>
#include <math.h>
#include "workspace.hpp"

inline bool has_suffix(const std::string& str, const std::string& suffix)
{
  if (str.length() >= suffix.length())
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
  else
    return false;
}

Workspace::Workspace()
{
  res = 4;
}

void
Workspace::draw()
{
  int w = int(sqrt(4 * images.size() / 3));
  for(int i = 0; i < int(images.size()); ++i)
    {
      images[i]->draw(int((i % w) * (res+res/32) + x_offset + Framebuffer::get_width()/2),
                      int((i / w) * (res+res/32) + y_offset + Framebuffer::get_height()/2),
                      int(res));
    }
}

void
Workspace::add(const std::string& filename)
{
  // if directory, do recursion
  if (Filesystem::is_directory(filename))
    {
      std::cout << '.' << std::flush;
      std::vector<std::string> dir_list = Filesystem::open_directory(filename);
      for(std::vector<std::string>::iterator i = dir_list.begin(); i != dir_list.end(); ++i)
        {
          if (*i != ".xvpics")
            add(*i);
        }
    }
  else if (has_suffix(filename, ".jpg") ||
           has_suffix(filename, ".JPG")
           )
    {
      std::string url = "file://" + Filesystem::realpath(filename);
      images.push_back(new Image(url));
    }
}

void
Workspace::zoom_in(int x, int y, float zoom)
{
  float old_res = res;
  res *= zoom;
  
  if (res > 8192) 
    {
      res = 8192;
      zoom = res / old_res;
    }

  x_offset *= zoom;
  y_offset *= zoom;

  x_offset += x - x*zoom;
  y_offset += y - y*zoom;
}

void
Workspace::zoom_out(int x, int y, float zoom)
{
  float old_res = res;
  res /= zoom;

  if (res < 4)
    {
      res = 4;
      zoom = old_res / res;
    }

  x_offset += x * zoom - x;
  y_offset += y * zoom - y;

  x_offset /= zoom;
  y_offset /= zoom;
}

/* EOF */