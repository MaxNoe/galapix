/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2013 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_GALAPIX_DATABASE_STATEMENTS_FILE_INFO_STORE_HPP
#define HEADER_GALAPIX_DATABASE_STATEMENTS_FILE_INFO_STORE_HPP

class FileInfoStore
{
private:
  SQLiteConnection& m_db;
  SQLiteStatement   m_stmt;

public:
  FileInfoStore(SQLiteConnection& db) :
    m_db(db),
    m_stmt(db, "INSERT OR REPLACE INTO file (path, mtime, blob_id) SELECT ?1, ?2, blob.id FROM blob WHERE blob.sha1 = ?3;")
  {}

  RowId operator()(const FileInfo& file_info)
  {
    m_stmt.bind_text (1, file_info.get_path());
    m_stmt.bind_int  (2, file_info.get_mtime());
    m_stmt.bind_text (3, file_info.get_blob_info().get_sha1().str());
    m_stmt.execute();

    return RowId{sqlite3_last_insert_rowid(m_db.get_db())};
  }

private:
  FileInfoStore(const FileInfoStore&) = delete;
  FileInfoStore& operator=(const FileInfoStore&) = delete;
};

#endif

/* EOF */