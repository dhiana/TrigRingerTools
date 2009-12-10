//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/sys/xerces_XMLProcessor.h
 *
 * @brief Builds an XML processor and writer that is able to operate on valid
 * XML files.
 */

#ifndef RINGER_SYS_XERCES_XMLPROCESSOR_H
#define RINGER_SYS_XERCES_XMLPROCESSOR_H

#include <string>
#include "TrigRingerTools/sys/Reporter.h"
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMBuilder.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

namespace sys {

  /**
   * Defines an XML processor and writer using Apache's Xerces-C/C++
   */
  class XMLProcessor {

  public:

    /**
     * Starts up the XML processor
     *
     * @param schema The obligatory schema URI to set before validating
     * any document.
     * @param reporter A reporter to write errors to the user from
     * parser initialization.
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
     * @return the (root) DOMElement if the document was successfuly parsed or
     * <code>NULL</code> otherwise.
     */
    xercesc::DOMElement* read (const std::string& document);

    /**
     * Creates a new document of the type I set for this processor
     *
     * @param root The name of the root element of the new XML document
     */
    xercesc::DOMElement* new_document (const std::string& root) const;

    /**
     * Writes the (root) DOMElement to a file.
     *
     * @param document The document to be persistified. After this operation,
     * the document is released.
     * @param filename The name of the file where to persistify the document
     *
     * @return <code>true</code> if the document was successfuly persistified
     * or <code>false</node> otherwise.
     */
    bool write (xercesc::DOMElement* document, const std::string& filename);

  private: //< representation
    xercesc::DOMImplementation* m_impl; ///< the DOM implementation to use
    xercesc::DOMBuilder* m_parser; ///< the Parser to use for my documents
    xercesc::DOMWriter* m_writer; ///< the Writer to use for my documents
    xercesc::DOMErrorHandler* m_handler; ///< the Xerces Error Handler to use
    std::string m_rootname; ///< the name of the root element this type of doc.
    std::string m_schema; ///< the schema name to use for this processor
    sys::Reporter* m_reporter; ///< if present, the reporter to use
  };

}

#endif /* RINGER_SYS_XERCES_XMLPROCESSOR_H */
