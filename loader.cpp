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
#include <iomanip>
#include <stdexcept>
#include "md5.hpp"
#include "filesystem.hpp"
#include "SDL_image.h"
#include "image.hpp"
#include "loader.hpp"
#include "surface.hpp"
#include "thumbnail_store.hpp"

Loader loader;

Loader::Loader()
  : thread(0), 
    keep_thread_running(true)    
{
  mutex  = SDL_CreateMutex();
  thread = 0;
  store  = 0;
}

Loader::~Loader()
{
  SDL_DestroyMutex(mutex);
}

int
Loader::thread_func(void*)
{
  while(loader.keep_thread_running)
    {
      loader.process_job();

      if (loader.empty())
        SDL_Delay(100);
    }
  return 0;
}

void
Loader::start_thread()
{
  if (!store)
    {
      store = new ThumbnailStore();
    }

  if (!thread)
    {
      keep_thread_running = true;
      thread = SDL_CreateThread(&Loader::thread_func, 0);
    }
}

void
Loader::stop_thread()
{
  keep_thread_running = false;
  SDL_WaitThread(thread, NULL);
  thread = 0;
}

void
Loader::request(Image* receiver)
{
  SDL_LockMutex(mutex);
  jobs.push_back(Job(receiver));
  SDL_UnlockMutex(mutex);
}

void
Loader::process_job()
{ // Thread this
  SDL_LockMutex(mutex);
  if (!jobs.empty())
    {
      //std::cout << "process_job: " << jobs.size() << std::endl;
      // Lock
      Job job = jobs.back();
      jobs.pop_back();

      if (!job.image->surface ||
          job.image->surface_resolution != job.image->requested_res)
        {
          SDL_Surface* img = store->get_by_url(job.image->url, job.image->requested_res);
          if (img)
            {
              job.image->receive(img, job.image->requested_res);
            }
          else
            {
              // No thumbnail, assuming we need the original
              std::cout << "Loading original: " << job.image->url << std::endl;
                { 
                  img = IMG_Load(job.image->url.substr(7).c_str()); // cut file:// part
                  std::cout << "Loading: " << job.image->requested_res << " " << img << " " 
                            << img->w << "x" << img->h << " "
                            << job.image->url.substr(7) << std::endl;
                  job.image->receive(img, job.image->requested_res);
                }
            }
        }
    }
  SDL_UnlockMutex(mutex);
}

void
Loader::clear()
{
  SDL_LockMutex(mutex);
  for(Jobs::iterator i = jobs.begin(); i != jobs.end(); ++i)
    (*i).image->receive(0, 0);

  jobs.clear();
  SDL_UnlockMutex(mutex);
}

bool
Loader::empty()
{
  return jobs.empty();
}

/* EOF */
