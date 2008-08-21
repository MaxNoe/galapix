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

#include <assert.h>
#include <iostream>
#include <jerror.h>
#include "jpeg_memory_src.hpp"

struct jpeg_memory_source_mgr {
  struct jpeg_source_mgr pub;	/* public fields */

  uint8_t* mem;
  int      len;
};

void jpeg_memory_init_source(j_decompress_ptr cinfo)
{
  cinfo->src->next_input_byte = NULL;
  cinfo->src->bytes_in_buffer = 0;
}

void jpeg_memory_term_source(j_decompress_ptr cinfo)
{
  //std::cout << "jpeg_memory_term" << std::endl;

  // nothing to do destruct the source
}

boolean jpeg_memory_fill_input_buffer(j_decompress_ptr cinfo)
{
  //std::cout << "jpeg_memory_fill_input_buffer" << std::endl;

  if (cinfo->src->next_input_byte != NULL)
    {
      (cinfo)->err->msg_code = JERR_INPUT_EOF;
      (*(cinfo)->err->error_exit)((j_common_ptr) (cinfo));
      return FALSE;
    }
  else
    {
      struct jpeg_memory_source_mgr* mgr = (struct jpeg_memory_source_mgr*)(cinfo->src);
  
      cinfo->src->next_input_byte = mgr->mem;
      cinfo->src->bytes_in_buffer = mgr->len;

      return TRUE;
    }
}

void jpeg_memory_skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
  std::cout << "jpeg_memory_skip_input_data" << std::endl;

  cinfo->src->next_input_byte = cinfo->src->next_input_byte + num_bytes;
  cinfo->src->bytes_in_buffer = cinfo->src->bytes_in_buffer - num_bytes;

  struct jpeg_memory_source_mgr* mgr = (struct jpeg_memory_source_mgr*)(cinfo->src);

  if (cinfo->src->next_input_byte >= &mgr->mem[mgr->len])
    {
      (cinfo)->err->msg_code = JERR_INPUT_EOF;
      (*(cinfo)->err->error_exit)((j_common_ptr) (cinfo));
    }
}

void jpeg_memory_src(j_decompress_ptr cinfo, uint8_t* mem, int len)
{
  if (cinfo->src == NULL) 
    {
      cinfo->src = (struct jpeg_source_mgr*)((*cinfo->mem->alloc_small)((j_common_ptr)cinfo, 
                                                                 JPOOL_PERMANENT,
                                                                 sizeof(struct jpeg_memory_source_mgr)));
    }
  
  cinfo->src->init_source       = jpeg_memory_init_source;
  cinfo->src->fill_input_buffer = jpeg_memory_fill_input_buffer;
  cinfo->src->skip_input_data   = jpeg_memory_skip_input_data;
  cinfo->src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
  cinfo->src->term_source       = jpeg_memory_term_source;

  cinfo->src->bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  cinfo->src->next_input_byte = NULL; /* until buffer loaded */

  struct jpeg_memory_source_mgr* mgr = (struct jpeg_memory_source_mgr*)(cinfo->src);
  mgr->mem = mem;
  mgr->len = len;
}

/* EOF */