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

#ifndef HEADER_GALAPIX_JOB_FILE_ENTRY_GENERATION_JOB_HPP
#define HEADER_GALAPIX_JOB_FILE_ENTRY_GENERATION_JOB_HPP

#include <boost/function.hpp>

#include "job/job.hpp"
#include "database/file_entry.hpp"

class FileEntryGenerationJob : public Job
{
private:
  JobHandle m_job_handle;
  URL m_url;
  boost::function<void (const FileEntry&)> m_callback;

public:
  FileEntryGenerationJob(const JobHandle& job_handle, const URL& url,
                         const boost::function<void (const FileEntry&)>& callback);
  void run();

private:
  FileEntryGenerationJob(const FileEntryGenerationJob&);
  FileEntryGenerationJob& operator=(const FileEntryGenerationJob&);
};

#endif

/* EOF */