/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   class definitions for the HEVC/h.265 ES demultiplexer module

*/

#pragma once

#include "common/common_pch.h"

#include "common/hevc.h"
#include "merge/generic_reader.h"

class hevc_es_reader_c: public generic_reader_c {
private:
  int m_width, m_height;

  memory_cptr m_buffer;

public:
  hevc_es_reader_c(const track_info_c &ti, const mm_io_cptr &in);

  virtual file_type_e get_format_type() const {
    return FILE_TYPE_HEVC_ES;
  }

  virtual void read_headers();
  virtual file_status_e read(generic_packetizer_c *ptzr, bool force = false);
  virtual void identify();
  virtual void create_packetizer(int64_t id);
  virtual bool is_providing_timestamps() const {
    return false;
  }

  static int probe_file(mm_io_c *in, uint64_t size);
};
