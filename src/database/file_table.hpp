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

#ifndef HEADER_GALAPIX_DATABASE_FILE_TABLE_HPP
#define HEADER_GALAPIX_DATABASE_FILE_TABLE_HPP

class FileTable
{
private:
  SQLiteConnection& m_db;

public:
  FileTable(SQLiteConnection& db) :
    m_db(db)
  {
    m_db.exec("CREATE TABLE IF NOT EXISTS files ("
              "fileid    INTEGER PRIMARY KEY AUTOINCREMENT,"
              "url       TEXT UNIQUE, "
              "size      INTEGER, "
              "mtime     INTEGER, "
           
              "width     INTEGER, "
              "height    INTEGER, "
              "format    INTEGER"   // format of the data (0: JPEG, 1: PNG)
              ");");

    m_db.exec("CREATE UNIQUE INDEX IF NOT EXISTS files_index ON files ( url );");
  }

private:
  FileTable(const FileTable&);
  FileTable& operator=(const FileTable&);
};

#endif

/* EOF */
