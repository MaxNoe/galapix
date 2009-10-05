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

#include <stdexcept>
#include <fstream>
#include <string.h>
#include <boost/scoped_array.hpp>
#include "blob.hpp"

class BlobImpl
{
public:
  boost::scoped_array<uint8_t> m_data;
  int      m_len;
  
  BlobImpl(const void* data, int len) :
    m_data(new uint8_t[len]),
    m_len(len)
  {
    memcpy(m_data.get(), data, m_len);
  }

  BlobImpl(const std::vector<uint8_t>& data_in) :
    m_data(new uint8_t[data_in.size()]),
    m_len(data_in.size())
  {
    memcpy(m_data.get(), &*data_in.begin(), m_len);
  }

  ~BlobImpl()
  {
  }
};

Blob::Blob(const std::vector<uint8_t>& data)
  : impl(new BlobImpl(data))
{}

Blob::Blob(const void* data, int len)
  : impl(new BlobImpl(data, len))
{}

Blob::Blob()
  : impl()
{}

int
Blob::size() const 
{
  if (impl.get())
    return impl->m_len;
  else
    return 0;
}

uint8_t* 
Blob::get_data() const 
{
  if (impl.get())
    return impl->m_data.get();
  else
    return 0;
}

std::string
Blob::str() const
{
  if (impl.get())
    return std::string(reinterpret_cast<char*>(impl->m_data.get()), impl->m_len);
  else
    return std::string();
}

void
Blob::write_to_file(const std::string& filename)
{
  std::ofstream out(filename.c_str(), std::ios::binary);
  out.write(reinterpret_cast<char*>(impl->m_data.get()), impl->m_len);
}

Blob
Blob::from_file(const std::string& filename)
{
  std::ifstream in(filename.c_str(), std::ios::binary);
  if (!in)
    {
      throw std::runtime_error("Blob::from_file: Couldn't read " + filename);
    }
  else
    {
      std::vector<uint8_t> data;
      uint8_t buffer[4096];
      while(!in.eof())
        {
          int len = in.read(reinterpret_cast<char*>(buffer), 4096).gcount();
          std::copy(buffer, buffer+len, std::back_inserter(data));
        }
      // FIXME: Useless copy again
      return Blob(data);
    }
}

void
Blob::append(const void* data, int len)
{
  assert(!"Blob::append(const void* data, int len): Implement me");
}

Blob
Blob::copy(const void* data, int len)
{
  return Blob(data, len);
}

Blob
Blob::copy(const std::vector<uint8_t>& data)
{
  return Blob(data);
}

Blob
Blob::wrap(const void* data, int len)
{
  // FIXME: Implement me
  return Blob(data, len);
}

Blob
Blob::wrap(const std::vector<uint8_t>& data)
{
  // FIXME: Implement me
  return Blob(data);
}

/* EOF */
