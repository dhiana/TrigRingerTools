//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/src/libxml2_xmlutil.cxx
 *
 * Implements a set of utilities to parse XML configuration files.
 */

#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/sys/libxml2_ustring.h"
#include "TrigRingerTools/sys/libxml2_Codec.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/debug.h"
#include <string>
#include <sstream>

sys::xml_ptr_const sys::get_next_element (sys::xml_ptr_const node)
{
  sys::xml_ptr retval = node->next;
  while (retval && !sys::is_element(retval)) retval = retval->next;
  return retval;
}

sys::xml_ptr_const sys::get_first_child (sys::xml_ptr_const top)
{
  sys::xml_ptr_const retval = top->children;
  if (retval && !sys::is_element(retval)) retval = sys::get_next_element(retval);
  return retval;
}

bool sys::is_element (sys::xml_ptr_const node)
{
  return node->type == XML_ELEMENT_NODE;
}

std::string sys::get_element_name (sys::xml_ptr_const node)
{
  return sys::default_codec.transcode(node->name);
}

std::string sys::get_element_string (sys::xml_ptr_const node)
{
  RINGER_DEBUG3("Getting string on element \"" << node->name << "\".");
  sys::xml_ptr_const text = node->children;
  if (text->type == XML_TEXT_NODE)
    return sys::default_codec.transcode
	    (xmlNodeGetContent(const_cast<sys::xml_ptr>(text)));
  RINGER_DEBUG1("This node does NOT seem to be a text node.");
  return std::string("");
}

std::string sys::get_attribute_string (sys::xml_ptr_const node,
				       const std::string& name)
{
  xmlChar* temp = 
    xmlGetProp(const_cast<sys::xml_ptr>(node), 
		    sys::default_codec.transcode(name).c_str());
  if (!temp) return "";
  sys::ustring retval = temp;
  xmlFree(temp);
  return sys::normalise(sys::default_codec.transcode(retval));
}

sys::xml_ptr sys::make_node (sys::xml_ptr, const std::string& name)
{
  return xmlNewNode(NULL, sys::default_codec.transcode(name).c_str());
}

sys::xml_ptr sys::put_node (sys::xml_ptr top, sys::xml_ptr child)
{
  xmlAddChild(top, child);
  return child;
}

sys::xml_ptr sys::put_element_text (sys::xml_ptr parent, const std::string& name,
				    const std::string& content)
{
  sys::xml_ptr node = put_element(parent, name);
  sys::xml_ptr text = 
    xmlNewText(sys::default_codec.transcode(content).c_str());
  xmlAddChild (node, text);
  return node;
}

sys::xml_ptr sys::put_attribute_text (sys::xml_ptr e, const std::string& name,
				      const std::string& value)
{
  sys::ustring xml_name = sys::default_codec.transcode(name);
  sys::ustring xml_value = sys::default_codec.transcode(value);
  xmlNewProp(e, xml_name.c_str(), xml_value.c_str());
  return e;
}
