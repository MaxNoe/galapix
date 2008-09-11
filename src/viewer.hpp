/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_VIEWER_HPP
#define HEADER_VIEWER_HPP

#include <boost/shared_ptr.hpp>
#include <memory>
#include "SDL.h"
#include "surface.hpp"
#include "math/vector2i.hpp"
#include "math/vector2f.hpp"
#include "viewer_state.hpp"

class Tool;
class PanTool;
class MoveTool;
class ZoomTool;
class ResizeTool;
class RotateTool;
class ViewRotateTool;
class Workspace;

class Viewer
{
private:
  Workspace* workspace;
  bool  quit;
  bool  force_redraw;
  bool  draw_grid;
  float gamma;

  ViewerState state;

  boost::shared_ptr<ZoomTool>   keyboard_zoom_in_tool;
  boost::shared_ptr<ZoomTool>   keyboard_zoom_out_tool;
  boost::shared_ptr<ViewRotateTool> keyboard_view_rotate_tool;

  boost::shared_ptr<PanTool>    pan_tool;
  boost::shared_ptr<MoveTool>   move_tool;
  boost::shared_ptr<ZoomTool>   zoom_in_tool;
  boost::shared_ptr<ZoomTool>   zoom_out_tool;
  boost::shared_ptr<ResizeTool> resize_tool;
  boost::shared_ptr<RotateTool> rotate_tool;

  Tool* left_tool;
  Tool* middle_tool;
  Tool* right_tool;

  Vector2i mouse_pos;
public:
  Viewer(Workspace* workspace);

  void draw();
  void update(float delta);
  void process_event(const SDL_Event& event);
  bool done() const { return quit; }

  ViewerState& get_state() { return state; }
  Workspace*   get_workspace() { return workspace; }
private:
  Viewer (const Viewer&);
  Viewer& operator= (const Viewer&);
};

#endif

/* EOF */
