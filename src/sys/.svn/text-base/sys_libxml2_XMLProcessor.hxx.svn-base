//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/src/libxml2_XMLProcessor.cxx
 *
 * Implements the XML processor.
 */

#include "TrigRingerTools/sys/XMLProcessor.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/libxml2_Codec.h"
#include <sstream>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemas.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

/**
 * The instantiation of this class and its destruction (at program
 * termination) takes care of initializing and destroying libxml parser
 * resources properly
 */
class LibxmlInitializer {
 public:
  /**
   * this initialises the library and check potential ABI mismatches between
   * the version it was compiled for and the actual shared library used.
   */
  LibxmlInitializer() {
    RINGER_DEBUG1("Starting libxml2...");
    LIBXML_TEST_VERSION;
    RINGER_DEBUG1("libxml2 ABI checked.");
    xmlInitParser();
    RINGER_DEBUG1("libxml2 parser initialised, thread-safeness turned on.");
  }

  /**
   * Cleans up libxml
   */
  ~LibxmlInitializer() {
    RINGER_DEBUG1("Cleaning up libxml2...");
    xmlCleanupParser();
    RINGER_DEBUG1("libxml2 parser resources destroyed.");
  }
};

/**
 * Create one static instance of LibxmlInitializer to make all the necessary
 * job for starting and destroying libxml2 static resources...
 */
static LibxmlInitializer _s_libxml_initializer;

/**
 * Loads the Schema description
 * 
 * @param uri The schema URI
 * @param reporter The reporter to report errors and fatal conditions
 */
xmlSchemaPtr load_schema (const std::string& uri, sys::Reporter* reporter)
{
  xmlSchemaParserCtxtPtr parser_ctxt = xmlSchemaNewParserCtxt(uri.c_str());
  xmlSchemaSetParserErrors(parser_ctxt,                 
			   (xmlSchemaValidityErrorFunc) fprintf,
			   (xmlSchemaValidityWarningFunc) fprintf,
			   stderr);
  xmlSchemaPtr parsed_schema = xmlSchemaParse(parser_ctxt);
  if (parsed_schema == NULL) {
    RINGER_WARN(reporter, "Schema \"" << uri 
	      << "\" failed to compile. Exception thrown.");
    throw RINGER_EXCEPTION("Schema cannot be parsed");
  }
  xmlSchemaFreeParserCtxt(parser_ctxt);
  return parsed_schema;
}

/**
 * Validates an XML stream
 *
 * @param schema The schema to use to validate the file
 * @param xml_doc The XML document to validate
 * @param reporter The reporter to report errors and fatal conditions
 */
void validate (xmlSchemaPtr schema, xmlDocPtr xml_doc,
			   sys::Reporter* reporter)
{
  xmlSchemaValidCtxtPtr schema_ctxt = xmlSchemaNewValidCtxt(schema);
  if (!schema_ctxt) {
    xmlSchemaFreeValidCtxt(schema_ctxt);
    RINGER_WARN(reporter, 
		"I cannot create a schema context. Exception thrown.");
    throw RINGER_EXCEPTION("Cannot create schema context");
  }
  xmlSchemaSetValidErrors(schema_ctxt,
			  (xmlSchemaValidityErrorFunc) fprintf,
			  (xmlSchemaValidityWarningFunc) fprintf,
			  stderr);
  int ret = xmlSchemaValidateDoc(schema_ctxt, xml_doc);
  xmlSchemaFreeValidCtxt(schema_ctxt);
  if (ret < 0) {
    RINGER_WARN(reporter, "Validation internal API error. Exception thrown.");
    //throw RINGER_EXCEPTION("Validation internal API exception");
  }
  if (ret) {
    RINGER_WARN(reporter, "Validation failed. Exception thrown.");
    //throw RINGER_EXCEPTION("Invalid XML file");
  }
}

/**
 * Validates and loads a file based on a schema
 *
 * @param schema The schema to use to validate the file
 * @param filename The filename to validate
 * @param reporter The reporter to report errors and fatal conditions
 */
xmlDocPtr load (xmlSchemaPtr schema, const std::string& filename,
				sys::Reporter* reporter)
{
  xmlDocPtr xml_doc = xmlParseFile(filename.c_str());
  if (!xml_doc) {
    RINGER_WARN(reporter, "Could not parse file \"" << filename << "\"."
				<< " Exception thrown.");
    throw RINGER_EXCEPTION("Cannot parse file");
  }
  try {
    validate(schema, xml_doc, reporter);
  }
  catch (const sys::Exception& ex) {
    xmlFreeDoc(xml_doc);
    RINGER_WARN(reporter, "Validation of \"" << filename 
				<< "\" failed. Exception thrown.");
    RINGER_EXCEPT(reporter, ex.what());
    return 0;
  }
  return xml_doc;
}

sys::XMLProcessor::XMLProcessor (const std::string& schema,
								 sys::Reporter* reporter)
  : m_schema(schema),
    m_schema_ptr(0),
    m_current(0),
    m_reporter(reporter)
{
  //loads the schema
  m_schema_ptr = load_schema(m_schema, m_reporter);
  RINGER_DEBUG3("Loaded schema document at \"" << m_schema << "\"");
}

sys::XMLProcessor::~XMLProcessor ()
{
  if (m_current) xmlFreeDoc(m_current);
  if (m_schema_ptr) xmlSchemaFree(m_schema_ptr);
  RINGER_DEBUG3("XML processor destroyed.");
}

xmlNodePtr sys::XMLProcessor::read (const std::string& document)
{
  if (m_current) { //sweeps memory
    xmlFreeDoc(m_current);
    m_current = 0;
  }
  m_current = load(m_schema_ptr, document.c_str(), m_reporter);
  RINGER_DEBUG2("Document \"" << document << "\" was successfuly loaded.");
  return xmlDocGetRootElement(m_current);
}

/**
 * Dumps an XML tree to a string
 */
std::string dump (const xmlDocPtr doc)
{
  xmlChar* data;
  int length;
  xmlDocDumpFormatMemoryEnc(doc, &data, &length, "UTF-8", 1);
  return std::string((char*)data);
}

xmlNodePtr sys::XMLProcessor::new_document (const std::string& root)
{
  if (m_current) { //sweeps memory
    xmlFreeDoc(m_current);
    m_current = 0;
  }
  m_current = 
    xmlNewDoc(sys::default_codec.transcode(XML_DEFAULT_VERSION).c_str());
  xmlNodePtr retval = xmlNewNode(NULL, 
				 sys::default_codec.transcode(root).c_str());
  xmlDocSetRootElement(m_current, retval);
  return retval;
}

bool sys::XMLProcessor::write (xmlNodePtr, const std::string& filename)
{
  RINGER_DEBUG2("Checking existence of \"" << filename << "\" first.");
  if (!sys::backup(filename)) return false;
  RINGER_DEBUG2("Writing node to \"" << filename << "\"");
  
  //validating node
  try {
    validate(m_schema_ptr, m_current, m_reporter);
  }
  catch (const sys::Exception& ex) {
    RINGER_EXCEPT(m_reporter, ex.what());
    RINGER_DEBUG1("This is what I would have dumped:\n" << dump(m_current));
    RINGER_FATAL(m_reporter, "Validation of in-memory XML document failed. "
	       << "This indicates problems on your code. Check it. "
	       << "I'll have to leave now, bye.");
    return false;
  }

  //writing node
  xmlSaveFormatFileEnc(filename.c_str(), m_current, "UTF-8", 1);
  RINGER_DEBUG3("Node written to \"" << filename << "\" and freed.");
  return true;
}
