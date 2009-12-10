//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/sys/xmlutil.h
 *
 * @brief Chooser between the different XML Parsing backends.
 */

#ifndef RINGER_SYS_XMLUTIL_H
#define RINGER_SYS_XMLUTIL_H

#ifdef XERCES_XML_BACK_END 
#include <xercesc/dom/DOMElement.hpp>

namespace sys {

  typedef xercesc::DOMElement* xml_ptr;
  typedef const xercesc::DOMElement* xml_ptr_const;

}

#else
#include <libxml/tree.h>

namespace sys {

  typedef xmlNode* xml_ptr;
  typedef const xmlNode* xml_ptr_const;

}

#endif /* XERCES_XML_BACK_END */

#include <vector>
#include <string>
#include <ctime>

namespace sys {

  /**
   * Takes a string as input and normalise it w.r.t. its white spaces. This
   * means space charaters as new-lines and tabulations are transformed to a
   * single white space. White space in the beginning and end of the string is
   * removed.
   *
   * @param s The string to be normalised.
   */
  std::string normalise (const std::string& s);

  /**
   * Takes a string and divide it into tokens taking the whitespaces
   * (tabulations, new-line characters) as separators.
   */
  void tokenize (const std::string& s, std::vector<std::string>& tokens);

  /**
   * Returns the next element in a set of elements
   */ 
  xml_ptr_const get_next_element (xml_ptr_const node);

  /**
   * Returns the first child of a node
   *
   * @param top The top node to consider
   */
  xml_ptr_const get_first_child (xml_ptr_const top);

  /**
   * Checks if a node is a valid element
   *
   * @param node The node to check
   */
  bool is_element (xml_ptr_const node);

  /**
   * Returns the "translated" name of the given node
   *
   * @param node The node from where to extract the name
   */
  std::string get_element_name (xml_ptr_const node);

  /**
   * Returns the contents of an element, as if it contained a string
   *
   * @param node The DOM node from where to extract the element value
   */
  std::string get_element_string (xml_ptr_const node);

  /**
   * Returns the contents of an element, as if it contained an unsigned integer
   *
   * @param node The DOM node from where to extract the element value
   */
  unsigned int get_element_uint (xml_ptr_const node);

  /**
   * Returns the contents of an element, as if it contained a xsd:dateTime
   *
   * @param node The DOM node from where to extract the element value
   */
  time_t get_element_date (xml_ptr_const node);

  /**
   * Returns the contents of an element, as if it contained a double
   *
   * @param node The DOM node from where to extract the element value
   */
  double get_element_double (xml_ptr_const node);

  /**
   * Returns the contents of an element, as if it contained a list of doubles
   *
   * @param node The DOM node from where to extract the element value
   * @param v Where to put the listed values
   */
  void get_element_doubles (xml_ptr_const node, std::vector<double>& v);

  /**
   * Get an attribute of type text
   *
   * @param node The node from where to extract the attribute
   * @param name The name of the attribute to take from the node
   */
  std::string get_attribute_string (xml_ptr_const node, 
				    const std::string& name);

  /**
   * Get an attribute of type unsigned integer
   *
   * @param node The node from where to extract the attribute
   * @param name The name of the attribute to take from the node
   */
  unsigned int get_attribute_uint (xml_ptr_const node, 
				   const std::string& name);

  /**
   * Get an attribute of type double
   *
   * @param node The node from where to extract the attribute
   * @param name The name of the attribute to take from the node
   */
  double get_attribute_double (xml_ptr_const node, const std::string& name);

  /**
   * Creates a new XML node with the given name
   *
   * @param any A pointer to any node of the document where you want to insert
   * the new node. It does @b not have to be the "future" parent of the
   * new. This is actually not needed by libxml2, but is there just to cope in
   * the differences with Xerces.
   * @param name The name of the node to create
   *
   * @return The newly created node
   */
  xml_ptr make_node (xml_ptr any, const std::string& name);

  /**
   * Places an XML node under another node
   *
   * @param top The node that will become the top node
   * @param child The node that will become the child node
   *
   * @return The top node
   */
  xml_ptr put_node (xml_ptr top, xml_ptr child);

  /**
   * Places an attribute-less element inside another element with the
   * parameters given
   *
   * @param root The root element to insert the new component
   * @param name The node name
   *
   * @return The newly created node.
   */
  xml_ptr put_element (xml_ptr root, const std::string& name);

  /**
   * Places a textual element inside another element with the parameters given
   *
   * @param root The root element to insert the new component
   * @param name The node name
   * @param content The node content
   *
   * @return The newly created element.
   */
  xml_ptr put_element_text (xml_ptr root, const std::string& name,
			    const std::string& content);

  /**
   * Places a textual element inside another element with the parameters given
   *
   * @param root The root element to insert the new component
   * @param name The node name
   * @param content The node content
   *
   * @return The newly created element.
   */
  xml_ptr put_element_date (xml_ptr root, const std::string& name,
			    const time_t& content);

  /**
   * Places a double element inside another element with the parameters given
   *
   * @param root The root element to insert the new component
   * @param name The node name
   * @param content The node content
   *
   * @return The newly created element.
   */
  xml_ptr put_element_double (xml_ptr root, const std::string& name,
			      const double& content);

  /**
   * Places a list of double element inside another element with the
   * parameters given
   *
   * @param root The root element to insert the new component
   * @param name The node name
   * @param content The node content
   *
   * @return The newly created element.
   */
  xml_ptr put_element_doubles (xml_ptr root, const std::string& name,
			       const std::vector<double>& content);

  /**
   * Places a text attribute on a element
   *
   * @param e The element to place the attribute on
   * @param name The attribute name
   * @param value The atribute value
   */
  xml_ptr put_attribute_text (xml_ptr e, const std::string& name,
			      const std::string& value);

  /**
   * Places an unsigned integer attribute on a element
   *
   * @param e The element to place the attribute on
   * @param name The attribute name
   * @param value The atribute value
   */
  xml_ptr put_attribute_uint (xml_ptr e, const std::string& name,
			      unsigned int value);

  /**
   * Places a double attribute on a element
   *
   * @param e The element to place the attribute on
   * @param name The attribute name
   * @param value The atribute value
   */
  xml_ptr put_attribute_double (xml_ptr e, const std::string& name,
				const double& value);

}

#endif /* RINGER_SYS_XMLUTIL_H */
