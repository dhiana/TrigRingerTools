//Dear emacs, this is -*- c++ -*-

/**
 * @file config/config/Header.h
 *
 * @brief Describes how to parse a network configuration file header.
 */

#ifndef CONFIG_HEADER_H
#define CONFIG_HEADER_H

#include <string>
#include <ctime>
#include "TrigRingerTools/sys/xmlutil.h"

namespace config {

  /**
   * Defines and reads information on headers
   */
  class Header {

  public:
    /**
     * A single constructor
     *
     * @param node The XML node with the header
     */
    Header (sys::xml_ptr_const node);

    /**
     * Builds a header from scratch
     *
     * @param author The author name
     * @param name The network name
     * @param version This network version
     * @param created When this network was initially created
     * @param comment An optional comment
     */
    Header (const std::string& author,
	    const std::string& name,
	    const std::string& version,
	    const time_t& created,
	    const std::string& comment);

    /**
     * Builds a header from another header
     *
     * @param other The other header to be copied
     */
    Header (const Header& other);

    /**
     * copies a header
     *
     * @param other The other header to be copied
     */
    Header& operator= (const Header& other);

    /**
     * Destructor virtualisation
     */
    ~Header () { }

    /**
     * Returns the libxml representation for this header.
     *
     * @param any Any node in the XML tree where you want to create this
     * node.
     */
    sys::xml_ptr node (sys::xml_ptr any);

    /**
     * Returns stuff for this node
     */
    inline const char* author () const { return m_author.c_str(); }
    inline const char* name () const { return m_name.c_str(); }
    inline const char* version () const { return m_version.c_str(); }
    inline time_t created () const { return m_created; }
    inline time_t lastSaved () const { return m_lastSaved; }
    inline const char* comment () const { return m_comment.c_str(); }

  private:
    std::string m_author; ///< author name
    std::string m_name; ///< network name
    std::string m_version; ///< network version
    time_t m_created; ///< when it was created
    time_t m_lastSaved; ///< when it was last saved
    std::string m_comment; ///< optional comment
  };

}

#endif /* CONFIG_HEADER_H */
