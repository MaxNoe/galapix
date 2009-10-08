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

#include "plugins/imagemagick.hpp"

#include <assert.h>
#include <stdexcept>
#include <Magick++.h>
#include <iostream>

#include "util/url.hpp"
#include "math/size.hpp"

bool
Imagemagick::get_size(const std::string& filename, Size& size)
{
  try {
    Magick::Image image(filename);
  
    size.width  = image.columns();
    size.height = image.rows();

    return true;
  } catch(std::exception& err) {
    std::cout << "Imagemagick: " << filename << ": " << err.what() << std::endl;
    return false;
  }
}

SoftwareSurfaceHandle
Imagemagick::load_from_url(const URL& url)
{
  if (url.has_stdio_name())
    {
      return load_from_file(url.get_stdio_name());
    }
  else
    {
      BlobHandle blob = url.get_blob();
      return load_from_mem(blob->get_data(), blob->size());
    }
}

static 
SoftwareSurfaceHandle
MagickImage2SoftwareSurface(const Magick::Image& image)
{
  SoftwareSurfaceHandle surface;

  int width  = image.columns();
  int height = image.rows();

  int shift;
  if (MaxRGB == 65535)
    shift = 8;
  else if (MaxRGB == 255)
    shift = 0;
  else
    assert(!"Imagemagick: Unknown MaxRGB");

  if (image.matte())
    {
      surface = SoftwareSurface::create(SoftwareSurface::RGBA_FORMAT, 
                                        Size(width, height));

      for(int y = 0; y < height; ++y)
        {
          const Magick::PixelPacket* src_pixels = image.getConstPixels(0, y, width, 1);
          uint8_t* dst_pixels = surface->get_row_data(y);

          for(int x = 0; x < width; ++x)
            {
              dst_pixels[4*x + 0] = static_cast<uint8_t>(src_pixels[x].red     >> shift);
              dst_pixels[4*x + 1] = static_cast<uint8_t>(src_pixels[x].green   >> shift);
              dst_pixels[4*x + 2] = static_cast<uint8_t>(src_pixels[x].blue    >> shift);
              dst_pixels[4*x + 3] = static_cast<uint8_t>(255 - (src_pixels[x].opacity >> shift));
            }
        }
    }
  else
    {
      surface = SoftwareSurface::create(SoftwareSurface::RGB_FORMAT, 
                                        Size(width, height));
      for(int y = 0; y < height; ++y)
        {
          uint8_t* dst_pixels = surface->get_row_data(y);
          const Magick::PixelPacket* src_pixels = image.getConstPixels(0, y, width, 1);

          for(int x = 0; x < width; ++x)
            {
              dst_pixels[3*x + 0] = static_cast<uint8_t>(src_pixels[x].red     >> shift);
              dst_pixels[3*x + 1] = static_cast<uint8_t>(src_pixels[x].green   >> shift);
              dst_pixels[3*x + 2] = static_cast<uint8_t>(src_pixels[x].blue    >> shift);
            }
        }
    }  

  return surface;
}

SoftwareSurfaceHandle
Imagemagick::load_from_mem(void* data, int len)
{
  // FIXME: Magick::Blob creates an unneeded copy of the data
  return MagickImage2SoftwareSurface(Magick::Image(Magick::Blob(data, len))); 
}

SoftwareSurfaceHandle
Imagemagick::load_from_file(const std::string& filename)
{
  return MagickImage2SoftwareSurface(Magick::Image(filename));
}

/* EOF */
