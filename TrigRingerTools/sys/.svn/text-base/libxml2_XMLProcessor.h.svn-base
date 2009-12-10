//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/sys/libxml2_XMLProcessor.h
 *
 * @brief Builds an XML processor and writer that is able to operate on valid
 * XML files.
 */

#ifndef RINGER_SYS_LIBXML2_XMLPROCESSOR_H
#define RINGER_SYS_LIBXML2_XMLPROCESSOR_H

#include <string>
#include <libxml/xmlschemas.h>
#include "TrigRingerTools/sys/Reporter.h"

namespace sys {

  /**
   * Defines an XML processor and writer using GNOME's libxml2
   */
  class XMLProcessor {

  public:

    /**
     * Starts up the XML processor
     *
     * @param schema The obligatory schema URI to set before validating
     * any document.
     * @param reporter The reporter to use when reporting problems.
     */ 
    XMLProcessor (const std::string& schema, sys::Reporter* reporter);

    /**
     * Virtualisation
     */
    virtual ~XMLProcessor();

  public: //< API

    /**
     * Parses and read an XML document
     *
     * @param document An URI to the document to read
     *
     * @return an xmlNodePtr to the root node if the document was successfuly
     * parsed or <code>NULL</code> otherwise.
     */
    xmlNodePtr read (const std::string& document);

    /**
     * Creates a new document of the type I set for this processor
     *
     * @param root The name of the root node of this document
     */
    xmlNodePtr new_document (const std::string& root);

    /**
     * Writes an xmlDocPtr to a file.
     *
     * @param document The document to be persistified. After this operation,
     * the document is released.
     * @param filename The name of the file where to persistify the document
     *
     * @return <code>true</code> if the document was successfuly persistified
     * or <code>false</code> otherwise.
     */
    bool write (xmlNodePtr document, const std::string& filename);

  private: //< representation

    std::string m_schema; ///< the schema URI to use for this processor
    xmlSchemaPtr m_schema_ptr; ///< My schema for validation
    xmlDocPtr m_current; ///< My current document loaded
    sys::Reporter* m_reporter; ///< The reporter to use

  };

}

#endif /* RINGER_SYS_LIBXML2_XMLPROCESSOR_H */
