//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/src/xerces_xmlutil.cxx
 *
 * Implements a set of utilities to parse XML configuration files.
 */

#include <sstream>

#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/debug.h"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMNodeList.hpp>

sys::xml_ptr_const sys::get_next_element (sys::xml_ptr_const node)
{
  xercesc::DOMNode* retval = node->getNextSibling();
  while (retval && retval->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    retval = retval->getNextSibling();
  return dynamic_cast<sys::xml_ptr_const>(retval);
}

sys::xml_ptr_const sys::get_first_child (sys::xml_ptr_const top)
{
  if (!top->hasChildNodes()) return 0;
  xercesc::DOMNode* node = top->getFirstChild();
  while (node && node->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) 
    node = node->getNextSibling();
  return dynamic_cast<sys::xml_ptr_const>(node);
}

bool sys::is_element (sys::xml_ptr_const node)
{
  return node->getNodeType() == xercesc::DOMNode::ELEMENT_NODE;
}

std::string sys::get_element_name (sys::xml_ptr_const node)
{
  return xercesc::XMLString::transcode(node->getTagName());
}

std::string sys::get_element_string (sys::xml_ptr_const node)
{
  RINGER_DEBUG3("Getting string on element \"" 
		<< xercesc::XMLString::transcode(node->getTagName()) << "\".");
  if (node->getChildNodes()->item(0)->getNodeType() == 
      xercesc::DOMElement::TEXT_NODE)
    return xercesc::XMLString::transcode
      (node->getChildNodes()->item(0)->getNodeValue());
  RINGER_DEBUG1("This node does NOT seem to be a text node.");
  return std::string("");
}

std::string sys::get_attribute_string (sys::xml_ptr_const node,
				       const std::string& name)
{
  return xercesc::XMLString::transcode
    (node->getAttribute(xercesc::XMLString::transcode(name.c_str())));
}

sys::xml_ptr sys::make_node (sys::xml_ptr any, const std::string& name)
{
  xercesc::DOMDocument* doc = any->getOwnerDocument();
  return doc->createElement(xercesc::XMLString::transcode(name.c_str()));
}

sys::xml_ptr sys::put_node (sys::xml_ptr top, 
			    sys::xml_ptr child)
{
  top->appendChild(child);
  return child;
}

sys::xml_ptr sys::put_element_text (sys::xml_ptr parent, 
				    const std::string& name,
				    const std::string& content)
{
  xercesc::DOMDocument* doc = parent->getOwnerDocument();
  sys::xml_ptr node = sys::put_element(parent, name);
  xercesc::DOMText* new_text = 
    doc->createTextNode(xercesc::XMLString::transcode(content.c_str()));
  node->appendChild(new_text);
  return node;
}

sys::xml_ptr sys::put_attribute_text (sys::xml_ptr e, 
				      const std::string& name,
				      const std::string& value)
{
  e->setAttribute(xercesc::XMLString::transcode(name.c_str()),
		  xercesc::XMLString::transcode(value.c_str()));
  return e;
}
