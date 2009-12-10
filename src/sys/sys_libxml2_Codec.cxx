//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/src/libxml2_Codec.cxx
 *
 * Implements the string converter.
 */

#ifndef XERCES_XML_BACK_END

#include "TrigRingerTools/sys/libxml2_Codec.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include <cstdlib>

sys::Codec sys::default_codec(XML_CHAR_ENCODING_UTF8);

sys::Codec::Codec (const xmlCharEncoding& encoding)
  : m_encoding(encoding)
{
  m_handler = xmlFindCharEncodingHandler(xmlGetCharEncodingName(m_encoding));
  if (!m_handler) {
    RINGER_DEBUG1("I cannot find libxml2's encoding handler for encoding \""
		<< xmlGetCharEncodingName(m_encoding) << "\". "
		<< "Exception thrown.");
    throw RINGER_EXCEPTION("Unavailable encoding handler");
  }
  RINGER_DEBUG3("Codec created for encoding \"" 
	      << xmlGetCharEncodingName(encoding) << "\".");
}

sys::ustring sys::Codec::transcode (const std::string& s) const
{
  RINGER_DEBUG3("Transcoding \"" << s << "\" to \""
	      << xmlGetCharEncodingName(m_encoding) << "\".");
  sys::ustring retval;
  if (!s.length()) return retval;

  int size = (int)s.length() + 1; //real size of string
  int out_size = (sizeof(xmlChar) * (int)s.length()) + 1;
  xmlChar* out = (xmlChar*) xmlMalloc(out_size*sizeof(xmlChar));
  int temp = size - 1;
  int ret = m_handler->input(out, &out_size, (const xmlChar*)s.c_str(), &temp);
  if ((ret < 0) || (temp - size + 1)) {
    RINGER_DEBUG1("Transcoding to \"" << xmlGetCharEncodingName(m_encoding) 
		<< "\" failed. [size=" << size << ", out_size=" << out_size
		<< ", temp=" << temp << ", ret=" << ret << "]");
    xmlFree(out);
    return retval;
  } 
  else {
    out = (xmlChar*) xmlRealloc(out, out_size + 1);
    out[out_size] = 0;  /*null terminating out */
  }
  retval = out;
  xmlFree(out);
  return retval;
}

std::string sys::Codec::transcode (const sys::ustring& s) const
{
  std::string retval;
  if (!s.length()) return retval;

  int size = (sizeof(xmlChar)*(int)s.length()) + 1;
  int out_size = (int)s.length() + 1;
  char* out = (char*) malloc(out_size*sizeof(char));
  int temp = size - 1;
  int ret = m_handler->output((xmlChar*)out, &out_size,
			      (const xmlChar*)s.c_str(), &temp);
  if ((ret < 0) || (temp - size + 1)) {
    RINGER_DEBUG1("Transcoding to \"" << xmlGetCharEncodingName(m_encoding) 
		<< "\" failed. [size=" << size << ", out_size=" << out_size
		<< ", temp=" << temp << ", ret=" << ret << "]");
    xmlFree(out);
    return retval;
  }
  else {
    out = (char*) realloc(out, out_size + 1);
    out[out_size] = 0;  /* null terminating out */
  }
  retval = out;
  free(out);
  RINGER_DEBUG3("Transcoding from \""
	      << xmlGetCharEncodingName(m_encoding) << "\"" 
	      << " generated \"" << retval << "\".");
  return retval;
} 

#endif /* not defined XERCES_XML_BACK_END */

