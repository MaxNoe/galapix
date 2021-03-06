/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2010 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include <math.h>

#include "math/math.hpp"
#include "util/url.hpp"
#include "util/software_surface_factory.hpp"
#include "util/software_surface.hpp"
#include "util/software_surface_float.hpp"
#include "plugins/jpeg.hpp"

void add(SoftwareSurfaceFloatPtr out, SoftwareSurfacePtr in)
{
  for(int y = 0; y < out->get_height(); ++y)
    for(int x = 0; x < out->get_width(); ++x)
    {
      RGB rgb; 
      in->get_pixel(x, y, rgb);
      RGBAf rgba;
      out->get_pixel(x, y, rgba);

      rgba.r += powf(static_cast<float>(rgb.r) / 255.0f, 0.4545f);
      rgba.g += powf(static_cast<float>(rgb.g) / 255.0f, 0.4545f);
      rgba.b += powf(static_cast<float>(rgb.b) / 255.0f, 0.4545f);

      out->put_pixel(x, y, rgba);
    }
}

void tone_map(SoftwareSurfacePtr out, SoftwareSurfaceFloatPtr in, float factor)
{
  for(int y = 0; y < out->get_height(); ++y)
    for(int x = 0; x < out->get_width(); ++x)
    {
      RGBAf rgba;
      in->get_pixel(x, y, rgba);
      RGB rgb;
      rgb.r = static_cast<uint8_t>(255 * Math::clamp(0.0f, powf(rgba.r / factor, 2.2f), 1.0f));
      rgb.g = static_cast<uint8_t>(255 * Math::clamp(0.0f, powf(rgba.g / factor, 2.2f), 1.0f));
      rgb.b = static_cast<uint8_t>(255 * Math::clamp(0.0f, powf(rgba.b / factor, 2.2f), 1.0f));
      out->put_pixel(x, y, rgb);
    }
}

int main(int argc, char** argv)
{
  if (argc == 1)
  {
    std::cout << "Usage: " << argv[0] << " FILES" << std::endl;
    std::cout << "Simple tool to build an average out of multiple images." << std::endl;
  }
  else
  {
    SoftwareSurfaceFactory factory;

    SoftwareSurfaceFloatPtr out;
    for(int i = 1; i < argc; ++i)
    {
      std::cout << "Loading: " << argv[i] << std::endl;
      SoftwareSurfacePtr image = factory.from_url(URL::from_filename(argv[i]));

      if (!out)
      {
        out = SoftwareSurfaceFloat::create(image->get_size());
      }

      add(out, image);
    }

    SoftwareSurfacePtr out_rgb = SoftwareSurface::create(SoftwareSurface::RGB_FORMAT, out->get_size());
    tone_map(out_rgb, out, static_cast<float>(argc - 1));

    std::string out_filename = "/tmp/out.jpg";
    JPEG::save(out_rgb, 100, out_filename);
    std::cout << "Wrote " << out_filename << std::endl;
    std::cout << "Done" << std::endl;
  }

  return 0;
}

/* EOF */
