//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/src/xerces_XMLProcessor.cxx
 *
 * Implements the XML processor based on Apache Xerces C++ Parser
 */

#include "TrigRingerTools/sys/XMLProcessor.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <sstream>

static unsigned int s_counter = 0;

/**
 * Initialise Xerces
 */
void initialise_xerces (void) 
{
  if (!s_counter) {
    try {
      xercesc::XMLPlatformUtils::Initialize();
    }
    catch (const xercesc::XMLException& toCatch) {
      std::string message = 
	xercesc::XMLString::transcode(toCatch.getMessage());
      message = "Error during XML parser initialization: " + message;
      RINGER_DEBUG1(message);
    }
    RINGER_DEBUG1("XERCES XML DOM parser was initialised.");
    ++s_counter;
  }
}

/**
 * Terminates Xerces
 */
void terminate_xerces (void)
{
  --s_counter;
  if (!s_counter) {
    xercesc::XMLPlatformUtils::Terminate();
    RINGER_DEBUG1("XERCES XML DOM parser was terminated.");
  }
}

/**
 * Defines my own class of error handlers based on sys::Reporter
 */
class MyDOMErrorHandler: public xercesc::DOMErrorHandler {

 public:
  /**
   * Builds an instance of this handler
   *
   * @param reporter The given reporter
   */
  MyDOMErrorHandler(sys::Reporter* r) : m_rep(r) 
  {
    RINGER_DEBUG1("XML Parser Error Handler created.");
  }

  /**
   * Destructor virtualisation
   */
  virtual ~MyDOMErrorHandler() {}

  /**
   * Implementation of the error handling callback
   *
   * @param error The error to handle
   */
  virtual bool handleError (const xercesc::DOMError &error)
  {
    enum xercesc::DOMError::ErrorSeverity severity = 
      (enum xercesc::DOMError::ErrorSeverity)error.getSeverity();
    const XMLCh* mesg = error.getMessage();
    const xercesc::DOMLocator* location = error.getLocation();
    XMLSSize_t line = location->getLineNumber();
    XMLSSize_t column = location->getColumnNumber();
    const XMLCh* uri = location->getURI();
    std::ostringstream mymesg;
    mymesg << "Xerces DOMBuilder at line " << line << " column " << column
	   << " from URI " << xercesc::XMLString::transcode(uri) << ": " 
	   << xercesc::XMLString::transcode(mesg);
    switch(severity) {
    case xercesc::DOMError::DOM_SEVERITY_WARNING:
      RINGER_REPORT(m_rep, "**XML DOM WARNING** " << mymesg.str());
      break;
    case xercesc::DOMError::DOM_SEVERITY_ERROR:
      RINGER_REPORT(m_rep, "**XML DOM ERROR** " << mymesg.str());
      throw RINGER_EXCEPTION("XML Parser error condition");
      break;
    case xercesc::DOMError::DOM_SEVERITY_FATAL_ERROR:
      RINGER_REPORT(m_rep, "**XML DOM FATAL** " << mymesg.str());
      throw RINGER_EXCEPTION("XML Parser fatal condition");
      break;
    default:
      RINGER_REPORT(m_rep, "**XML DOM Unknown** " << mymesg.str());
      throw RINGER_EXCEPTION("Unknown XML Parser condition");
      break;
    }
    return true;
  }

 private: //representation
  sys::Reporter* m_rep; ///< my private reporter
  
};

/**
 * Parses and caches a schema file into the given parser.
 *
 * @param parser The parser to use for parsing and loading the schema file
 * @param schema The schema URI.
 * @param reporter An optional reporter to report errors.
 */
bool load_schema(xercesc::DOMBuilder& parser, const std::string& schema,
		 sys::Reporter* reporter)
{
  RINGER_DEBUG2("Loading schema file \"" << schema << "\"...");
  try {
    parser.loadGrammar(schema.c_str(), xercesc::Grammar::SchemaGrammarType, 
		       false);
  }
  catch (const xercesc::XMLException& toCatch) {
    RINGER_EXCEPT(reporter, "XML Exception at schema: " 
		<< xercesc::XMLString::transcode(toCatch.getMessage()));
    return false;
  }
  catch (const xercesc::DOMException& toCatch) {
    RINGER_EXCEPT(reporter, "DOM Exception at schema: " 
		<< xercesc::XMLString::transcode(toCatch.msg));
    return false;
  }
  catch (const sys::Exception& sysex) {
    RINGER_EXCEPT(reporter, sysex.what());
    return false;
  }
  catch (...) {
    RINGER_EXCEPT(reporter, "Unexpected exception at schema");
    return false;
  }
  RINGER_DEBUG2("Schema file \"" << schema 
	      << "\" was successfuly parsed and cached.");
  return true;
}

/**
 * This function checks for the existence of a parser and releases it,
 * instantiating a new parser and (re-)loading the schema.
 *
 * @param impl The Xerces DOM implementation to use for deriving a new parser
 * @param parser The parser to check for existence
 * @param schema The schema file to cache into the parser
 * @param handler The error handler to use
 * @param reporter The reporter to use for 
 */
xercesc::DOMBuilder* start_parser (xercesc::DOMImplementation* impl, 
				   const std::string& schema,
				   xercesc::DOMErrorHandler* handler,
				   sys::Reporter* reporter)
{
  RINGER_DEBUG2("Starting a new DOM parser with schema at \"" 
	      << schema << "\".");
  xercesc::DOMBuilder* parser = 
    ((xercesc::DOMImplementationLS*)impl)->createDOMBuilder
    (xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);

  if (!parser) return 0;
  
  // optionally you can set some features on this builder
  if (parser->canSetFeature(xercesc::XMLUni::fgDOMValidation, true)) {
    parser->setFeature(xercesc::XMLUni::fgDOMValidation, true);
    RINGER_DEBUG3("XML parser has DOMValidation set.");
  }
  if (parser->canSetFeature(xercesc::XMLUni::fgDOMNamespaces, true)) {
    parser->setFeature(xercesc::XMLUni::fgDOMNamespaces, true);
    RINGER_DEBUG3("XML parser has DOMNamespaces set.");
  }
  if (parser->canSetFeature(xercesc::XMLUni::fgXercesSchema, true)) {
    parser->setFeature(xercesc::XMLUni::fgXercesSchema, true);
    RINGER_DEBUG3("XML parser has XercesShema set.");
  }
  if (parser->canSetFeature
      (xercesc::XMLUni::fgDOMDatatypeNormalization, true)) {
    parser->setFeature(xercesc::XMLUni::fgDOMDatatypeNormalization, true);
    RINGER_DEBUG3("XML parser has DOMDatatypeNormalization set.");
  }

  //Sets the error reporting handler
  parser->setErrorHandler(handler);

  RINGER_DEBUG2("Parser initialised, loading grammar.");
  //Parses and loads the schema
  if (schema.length()) { //set the parser schema
    if (!load_schema(*parser, schema, reporter)) {
      RINGER_DEBUG1("I could not load your schema file \"" << schema 
		  << "\". Please check it. Exception thrown.");
      throw RINGER_EXCEPTION("Specified schema cannot be loaded");
    }
    RINGER_DEBUG3("Setting parser schema location...");
    XMLCh* prop = xercesc::XMLString::transcode(schema.c_str());
    parser->setProperty
      (xercesc::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, prop);
  }
  RINGER_DEBUG2("Schema grammar loaded.");
  return parser;
}

/**
 * This function checks for the existence of a parser and releases it,
 * instantiating a new parser and (re-)loading the schema.
 *
 * @param impl The Xerces DOM implementation to use for deriving a new parser
 * @param handler The error handler to use for this DOMWriter
 */
xercesc::DOMWriter* start_writer 
(xercesc::DOMImplementation* impl, xercesc::DOMErrorHandler* handler)
{
  xercesc::DOMWriter* writer = 
    ((xercesc::DOMImplementationLS*)impl)->createDOMWriter();

  //I can set some features on this serializer
  if (writer->canSetFeature
      (xercesc::XMLUni::fgDOMWRTDiscardDefaultContent,true)) {
    writer->setFeature(xercesc::XMLUni::fgDOMWRTDiscardDefaultContent, true);
    RINGER_DEBUG3("XML writer has DOMWRTDiscardDefaultContent set.");
  }
  if (writer->canSetFeature
      (xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
    writer->setFeature(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
    RINGER_DEBUG3("XML writer has DOMWRTFormatPrettyPrint set.");
  }
  if (writer->canSetFeature(xercesc::XMLUni::fgDOMWRTValidation, true)) {
    writer->setFeature(xercesc::XMLUni::fgDOMWRTValidation, true);
    RINGER_DEBUG3("XML writer has DOMWRTValidation set.");
  }
  writer->setErrorHandler(handler);
  return writer;
}

sys::XMLProcessor::XMLProcessor (const std::string& schema,
				 sys::Reporter* reporter)
  : m_impl(0),
    m_parser(0),
    m_writer(0),
    m_handler(0),
    m_schema(schema),
    m_reporter(reporter)
{
  initialise_xerces();
  m_impl = xercesc::DOMImplementationRegistry::getDOMImplementation
    (xercesc::XMLString::transcode("LS"));
  RINGER_DEBUG3("Got Xerces DOM Implementation @" << m_impl);
  MyDOMErrorHandler* m_handler = new MyDOMErrorHandler(m_reporter);
  m_parser = start_parser(m_impl, m_schema, m_handler, m_reporter);
  RINGER_DEBUG3("Starting Xerces DOM Parser @" << m_parser);
  m_writer = start_writer(m_impl, m_handler);
  RINGER_DEBUG3("Starting Xerces DOM Writer @" << m_writer);
}

sys::XMLProcessor::~XMLProcessor ()
{
  RINGER_DEBUG3("Releasing Xerces DOM Parser @" << m_parser);
  if (m_writer) m_writer->release();
  RINGER_DEBUG3("Releasing Xerces DOM Writer @" << m_writer);
  if (m_parser) m_parser->release();
  delete m_handler;
  terminate_xerces();
}

xercesc::DOMElement* sys::XMLProcessor::read (const std::string& document)
{
  xercesc::DOMDocument* domdoc = 0;
  RINGER_DEBUG2("Parsing document \"" << document << "\"...");
  try {
    domdoc = m_parser->parseURI(document.c_str());
  }
  catch (const xercesc::XMLException& toCatch) {
    RINGER_WARN(m_reporter, "XML Exception at parsed document: "
	      << xercesc::XMLString::transcode(toCatch.getMessage()));
    return 0;
  }
  catch (const xercesc::DOMException& toCatch) {
    RINGER_WARN(m_reporter, "DOM Exception at parsed document: "
	      << xercesc::XMLString::transcode(toCatch.msg));
    return 0;
  }
  catch (const sys::Exception& sysex) {
    RINGER_EXCEPT(m_reporter, sysex.what());
    return 0;
  }
  catch (...) {
    RINGER_WARN(m_reporter, "Unexpected exception at parsed document");
    return 0;
  }
  RINGER_DEBUG2("Document \"" << document << "\" was successfuly parsed.");
  return domdoc->getDocumentElement();
}

xercesc::DOMElement* sys::XMLProcessor::new_document (const std::string& root) const
{
  xercesc::DOMDocument* doc = m_impl->createDocument
    (0, xercesc::XMLString::transcode(root.c_str()), 0);
  return doc->getDocumentElement();
}

bool sys::XMLProcessor::write (xercesc::DOMElement* document,
			       const std::string& filename)
{
  RINGER_DEBUG2("Checking existence of \"" << filename << "\" first.");
  if (!sys::backup(filename)) return false;
  RINGER_DEBUG2("Writing node to \"" << filename << "\"");
  xercesc::XMLFormatTarget* target = 
    new xercesc::LocalFileFormatTarget(filename.c_str());
  try {
    bool success = m_writer->writeNode
      (target, 
       *static_cast<const xercesc::DOMNode*>(document->getOwnerDocument()));
    static_cast<xercesc::DOMNode*>(document->getOwnerDocument())->release();
    if (!success) {
      RINGER_WARN(m_reporter, "XML DOM Writer indicates an uncatchable error"
		  << " has occured and it couldn't save your XML file.");
      throw RINGER_EXCEPTION("XML DOMWriter cannot perform write task");
    }
  }
  catch (const xercesc::XMLException& toCatch) {
    RINGER_WARN(m_reporter, "DOM Exception at document to be saved: "
	      << xercesc::XMLString::transcode(toCatch.getMessage()));
    delete target;
    return false;
  }
  catch (const xercesc::DOMException& toCatch) {
    RINGER_WARN(m_reporter, "DOM Exception at document to be saved: "
	      << xercesc::XMLString::transcode(toCatch.msg));
    delete target;
    return false;
  }
  catch (const sys::Exception& sysex) {
    RINGER_EXCEPT(m_reporter, sysex.what());
    delete target;
    return false;
  }
  catch (...) {
    RINGER_WARN(m_reporter, "Unexpected exception at document to be saved");
    delete target;
    return false;
  }
  delete target; // file successfuly saved!!
  RINGER_DEBUG3("Node written to \"" << filename << "\"");
  RINGER_DEBUG3("Trying to parse \"" << filename << "\" against \""
		<< m_schema << "\" to check integrity.");
  xercesc::DOMElement* tempdoc = read(filename);
  if (tempdoc) {
    RINGER_DEBUG3("File \"" << filename << "\" was successfuly parsed."); 
  }
  else {
    RINGER_DEBUG1("I could not parse the file I wrote. This indicates problems"
		  << " in your code! Please, revise it. I'll leave now.");
    RINGER_FATAL(m_reporter, "File \"" << filename 
		 << "\" could not be parsed. Removing it!");
  }
  RINGER_DEBUG2("Node successfuly parsed and written to \"" 
		<< filename << "\""); 
  return true;
}
