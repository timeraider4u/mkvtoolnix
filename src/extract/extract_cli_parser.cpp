/** \brief command line parsing

   mkvextract -- extract tracks from Matroska files into other files

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   \file

   \author Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#include "common/os.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include "common/common.h"
#include "common/ebml.h"
#include "common/strings/formatting.h"
#include "common/translation.h"
#include "extract/extract_cli_parser.h"

extract_cli_parser_c::extract_cli_parser_c(const std::vector<std::string> &args)
  : cli_parser_c(args)
  , m_num_unknown_args(0)
  , m_charset("UTF-8"),
  , m_embed_in_ogg(true)
  , m_blockadd_level(1)
  , m_target_mode(options_c::tm_normal)
{
}

#define OPT(spec, func, description) add_option(spec, boost::bind(&propedit_cli_parser_c::func, this), description)

void
propedit_cli_parser_c::init_parser() {
  add_information(YT("mkvextract <mode> <source-filename> [options] <extraction-spec>"));
  add_separator();

  add_section_header(YT("Usage"));
  add_information(YT("mkvextract tracks <inname> [options] [TID1:out1 [TID2:out2 ...]]"));
  add_information(YT("mkvextract tags <inname> [options]"));
  add_information(YT("mkvextract attachments <inname> [options] [AID1:out1 [AID2:out2 ...]]"));
  add_information(YT("mkvextract chapters <inname> [options]"));
  add_information(YT("mkvextract cuesheet <inname> [options]"));
  add_information(YT("mkvextract timecodes_v2 <inname> [TID1:out1 [TID2:out2 ...]]"));
  add_information(YT("mkvextract <-h|-V>"));

  add_separator();

  add_information(YT("The first word tells mkvextract what to extract. The second must be the source file. "
                     "There are few global options that can be used with all modes. "
                     "All other options depend on the mode."));

  add_section_header(YT("Global options"));
  OPT("-f|parse-fully",   set_parse_fully, YT("Parse the whole file instead of relying on the index."));

  add_section_header(YT("Track extraction"));
  add_information(YT("The first mode extracts some tracks to external files."));
  OPT("-c=charset",       set_charset,     YT("Convert text subtitles to this charset (default: UTF-8)."));
  OPT("--no-ogg",         set_no_ogg,      YT("Write raw FLAC files (default: write OggFLAC files)."));
  OPT("--cuesheet",       set_cuesheet,    YT("Also try to extract the CUE sheet from the chapter information and tags for this track."));
  OPT("--blockadd=level", set_blockadd,    YT("Keep only the BlockAdditions up to this level (default: keep all levels)"));
  OPT("--raw",            set_raw,         YT("Extract the data to a raw file."));
  OPT("--fullraw",        set_fullraw,     YT("Extract the data to a raw file including the CodecPrivate as a header."));
  add_informational_option("TID:out",      YT("Write track with the ID TID to the file 'out'."));

  add_section_header(YT("Example"));

  add_information(YT("mkvextract tracks \"a movie.mkv\" 2:audio.ogg -c ISO8859-1 3:subs.srt"));
  add_separator();

  add_section_header(YT("Tag extraction"));
  add_information(YT("The second mode extracts the tags and converts them to XML. The output is written to the standard output. The output can be used as a source for mkvmerge."));

  add_section_header(YT("Example"));

  add_information(YT("mkvextract tags \"a movie.mkv\" > movie_tags.xml"));

  add_section_header(YT("Atatchment extraction"));

  add_information(YT("The third mode extracts attachments from the source file."));
  add_informational_option("AID:outname", YT("Write the attachment with the ID 'AID' to 'outname'."));

  add_section_header(YT("Example"));

  add_information(YT("mkvextract attachments \"a movie.mkv\" 4:cover.jpg"));

  add_section_header(YT("Chapter extraction"));
  add_information(YT("The fourth mode extracts the chapters and converts them to XML. The output is written to the standard output. The output can be used as a source for mkvmerge."));
  OPT("-s|--simple", set_simple, YT("Exports the chapter infomartion in the simple format used in OGM tools (CHAPTER01=... CHAPTER01NAME=...)."));

  add_section_header(YT("Example"));

  add_information(YT("mkvextract chapters \"a movie.mkv\" > movie_chapters.xml"));

  add_section_header(YT("CUE sheet extraction"));

  add_information(YT("The fifth mode tries to extract chapter information and tags and outputs them as a CUE sheet. This is the reverse of using a CUE sheet with "
                     "mkvmerge's '--chapters' option."));

  add_section_header(YT("Example"));

  add_information(YT("mkvextract cuesheet \"audiofile.mka\" > audiofile.cue"));

  add_section_header(YT("Timecode extraction"));

  add_information(YT("The sixth mode finds the timecodes of all blocks for a track and outpus a timecode v2 file with these timecodes."));

  add_section_header(YT("Example"));

  add_information(YT("mkvextract timecodes_v2 \"a movie.mkv\" 1:timecodes_track1.txt"));

  add_common_options();

  add_hook(cli_parser_c::ht_unknown_option, boost::bind(&extract_cli_parser_c::set_mode_or_extraction_spec, this));
}

#undef OPT

void
extract_cli_parser_c::assert_mode(option_c::extraction_mode_e mode) {
  if      ((option_c::em_tracks   == mode) && (m_options.m_mode != mode))
    mxerror(boost::format(Y("'%1%' is only allowed when extracting tracks.\n"))   % m_current_arg);

  else if ((option_c::em_chapters == mode) && (m_options._mode != mode))
    mxerror(boost::format(Y("'%1%' is only allowed when extracting chapters.\n")) % m_current_arg);
}

void
extract_cli_parser_c::set_parse_fully() {
  m_options.m_parse_fuly = true;
}

void
extract_cli_parser_c::set_charset() {
  assert_mode(option_c::em_tracks);
  m_charset = m_next_arg;
}

void
extract_cli_parser_c::set_no_ogg() {
  assert_mode(option_c::em_tracks);
  m_embed_in_ogg = false;
}

void
extract_cli_parser_c::set_cuesheet() {
  assert_mode(option_c::em_tracks);
  m_extract_cuesheet = true;
}

void
extract_cli_parser_c::set_blockadd() {
  assert_mode(option_c::em_tracks);
  if (!parse_int(m_next_arg, m_extract_blockadd_level) || (-1 > m_extract_blockadd_level))
    mxerror(boost::format(Y("Invalid BlockAddition level in argument '%1%'.\n")) % m_next_arg);
}

void
extract_cli_parser_c::set_raw() {
  assert_mode(option_c::em_tracks);
  m_target_mode = option_c::tm_raw;
}

void
extract_cli_parser_c::set_full_raw() {
  assert_mode(option_c::em_tracks);
  m_target_mode = option_c::tm_full_raw;
}

void
extract_cli_parser_c::set_simple() {
  assert_mode(option_c::em_chapters);
  m_options.m_simple_chapter_format = true;
}

void
extract_cli_parser_c::set_mode_or_extraction_spec() {
  ++m_num_unknown_options;

  if (1 == m_num_unknown_options)
    set_extraction_mode();

  else if (2 == m_num_unknown_options)
    m_options.m_file_name = m_next_arg;

  else
    add_extraction_spec();
}

void
extract_cli_parser_c::set_extraction_mode() {
  static struct {
    const char *name;
    options_c::extraction_mode_e mode;
  } s_mode_map[] = {
    { "tracks",       option_c::em_tracks       },
    { "tags",         option_c::em_tags         },
    { "attachments",  option_c::em_attachments  },
    { "chapters",     option_c::em_chapters     },
    { "cuesheet",     option_c::em_cuesheet     },
    { "timecodes_v2", option_c::em_timecodes_v2 },
    { NULL,           option_c::em_unknown      },
  };

  int i;
  for (i = 0; NULL != s_mode_map[i].name; ++i)
    if (m_next_arg == s_mode_map[i].name) {
      m_options.m_extraction_mode = s_mode_map[i].mode;
      return;
    }

  mxerror(boost::format(Y("Unknown mode '%1%'.\n")) % m_next_arg);
}

void
extract_cli_parser_c::add_extraction_spec() {
  if (   (options_c::em_tracks       != m_options.mode)
      && (options_c::em_timecodes_v2 != m_options.mode)
      && (options_c::em_attachments  != m_options.mode))
    mxerror(boost::format(Y("Unrecognized command line option '%1%'.\n")) % m_current_arg);

  boost::regex s_track_id_re("^(\\d+)(:(.+))?$", boost::regex::perl);

  boost::match_results<std::string::const_iterator> matches;
  if (!boost::regex_search(m_current_arg, matches, s_track_id_re)) {
    if (options_c::em_attachments == m_options.mode)
      mxerror(boost::format(Y("Invalid attachment ID/file name specification in argument '%1%'.\n")) % m_current_arg);
    else
      mxerror(boost::format(Y("Invalid track ID/file name specification in argument '%1%'.\n")) % m_current_arg);
  }

  track_spec_t track;

  parse_int(matches[1].str(), track.tid);

  std::string output_file_name;
  if (matches[3].matched)
    output_file_name = matches[3].str();

  if (output_file_name.empty()) {
    if (option_c::em_attachments == mode)
      mxinfo(Y("No output file name specified, will use attachment name.\n"));
    else
      mxerror(boost::format(Y("Missing output file name in argument '%1%'.\n")) % args[i]);
  }

  track.out_name               = output_file_name;
  track.sub_charset            = m_sub_charset;
  track.embed_in_ogg           = m_embed_in_ogg;
  track.extract_cuesheet       = m_extract_cuesheet;
  track.extract_blockadd_level = m_extract_blockadd_level;
  track.target_mode            = m_target_mode;
  tracks.push_back(track);

  m_sub_charset                = "UTF-8";
  m_embed_in_ogg               = true;
  m_extract_cuesheet           = false;
  m_target_mode                = track_spec_t::tm_normal;
}

options_cptr
extract_cli_parser_c::run() {
  init_parser();

  parse_args();

  return m_options;
}

