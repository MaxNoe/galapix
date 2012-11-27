/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2012 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_GALAPIX_ARCHIVE_DIRECTORY_EXTRACTION_HPP
#define HEADER_GALAPIX_ARCHIVE_DIRECTORY_EXTRACTION_HPP

#include "archive/extraction.hpp"

#include <boost/filesystem.hpp>

class DirectoryExtraction : public Extraction
{
private:
  boost::filesystem::path m_path;

public:
  DirectoryExtraction(const std::string& path);

  std::vector<std::string> get_filenames() const;
  BlobPtr get_file(const std::string& filename) const;
  std::string get_file_as_path(const std::string& filename) const;

private:
  DirectoryExtraction(const DirectoryExtraction&);
  DirectoryExtraction& operator=(const DirectoryExtraction&);
};

#endif

/* EOF */
