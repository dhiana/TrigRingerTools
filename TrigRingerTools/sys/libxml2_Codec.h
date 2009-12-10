//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/sys/libxml2_Codec.h
 *
 * @brief Describes a way to handle strings in differents charsets.
 */

#ifndef RINGER_SYS_LIBXML2_CODEC_H
#define RINGER_SYS_LIBXML2_CODEC_H

#ifndef XERCES_XML_BACK_END

#include <string>
#include <libxml/encoding.h>
#include "TrigRingerTools/sys/libxml2_ustring.h"

namespace sys {

  /**
   * Given an encoding, this class can convert from char* to xmlChar* on the
   * given encoding and vice-versa
   */
  class Codec {
  public:
    
    /**
     * Initialises this codec
     *
     * @param encoding The encoding to use. Options are UTF-8, UTF-16 or
     * ISO8859-1, for example.
     */
    Codec (const xmlCharEncoding& encoding);

    /**
     * Destructor virtualisation
     */
    virtual ~Codec () {}

    /**
     * Encodes a normal string into coded form.
     *
     * @param s The string to encode
     */
    sys::ustring transcode (const std::string& s) const;
    
    /**
     * Decodes a string, from the originating charset to the current charset.
     *
     * @param s The string to decode
     */
    std::string transcode (const sys::ustring& s) const;

  private: //cannot copy codec
    
    /**
     * Copy constructor
     *
     * @param other The codec to copy
     */
    Codec (const Codec& other);

    /**
     * Assignment operator
     *
     * @param other The codec to copy
     */
    Codec& operator= (const Codec& other);

  private: //representation
    xmlCharEncoding m_encoding; ///< my original encoding
    xmlCharEncodingHandlerPtr m_handler; ///< my handler to convertions
  };

  extern sys::Codec default_codec;

}

#endif /* not defined XERCES_XML_BACK_END */

#endif /* RINGER_SYS_LIBXML2_CODEC_H */
