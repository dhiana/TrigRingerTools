//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/src/xmlutil.cxx
 *
 * @brief Chooser between the different XML Parsing backends.
 */

#ifdef XERCES_XML_BACK_END
#include "sys_xerces_xmlutil.hxx"
#else
#include "sys_libxml2_xmlutil.hxx"
#endif
#include <cstdlib>

std::string sys::normalise (const std::string& s)
{
  std::string retval;
  size_t start = 0;
  while (start < s.length()) {
    while (start < s.length() && std::isspace(s[start])) ++start;
    if (start >= s.length()) break;
    size_t end = start+1; //next
    while (!std::isspace(s[end])) {
      if (end == s.length()) break;
      ++end;
    }
    retval += s.substr(start, end-start) + " ";
    start = end+1;
  }
  retval = retval.substr(0, retval.length()-1); //remove last space
  return retval;
}

void sys::tokenize (const std::string& s, std::vector<std::string>& tokens)
{
  size_t start = 0;
  while (start < s.length()) {
    while (start < s.length() && std::isspace(s[start])) ++start;
    if (start >= s.length()) break;
    size_t end = start+1; //next
    while (!std::isspace(s[end])) {
      if (end == s.length()) break;
      ++end;
    }
    tokens.push_back(s.substr(start, end-start));
    start = end+1;
  }
}

unsigned int sys::get_element_uint (sys::xml_ptr_const node)
{
  std::string tmp = normalise(get_element_string(node));
  return strtoul(tmp.c_str(), 0, 0);
}

time_t sys::get_element_date (sys::xml_ptr_const node)
{
  std::string tmp = normalise(get_element_string(node));
  return sys::iso8601totime(tmp);
}

double sys::get_element_double (sys::xml_ptr_const node)
{
  std::string tmp = normalise(get_element_string(node));
  return strtod(tmp.c_str(), 0);
}

void sys::get_element_doubles (sys::xml_ptr_const node, std::vector<double>& v)
{
  //A hack to workaround libxml2 (2.6.8) limitations
  std::vector<std::string> tokens;
  sys::tokenize(get_element_string(node), tokens);
  for (std::vector<std::string>::const_iterator it = tokens.begin();
       it != tokens.end(); ++it) {
    RINGER_DEBUG3("Converting token \"" << *it << "\" to double.");
    v.push_back(std::strtod(it->c_str(), 0));
  }
}

unsigned sys::get_attribute_uint (sys::xml_ptr_const node,
				  const std::string& name)
{
  std::string c = normalise(get_attribute_string(node, name));
  return strtoul(c.c_str(), 0, 0);
}

double sys::get_attribute_double (sys::xml_ptr_const node,
				  const std::string& name)
{
  std::string c = normalise(get_attribute_string(node, name));
  return strtod(c.c_str(), 0);
}

sys::xml_ptr sys::put_element (sys::xml_ptr parent, const std::string& name)
{
  sys::xml_ptr node = sys::make_node(parent, name);
  return sys::put_node(parent, node);
}

sys::xml_ptr sys::put_element_date (sys::xml_ptr parent, const std::string& name,
				    const time_t& content)
{
  return put_element_text(parent, name, sys::timetoiso8601(&content));
}

sys::xml_ptr sys::put_element_double (sys::xml_ptr parent, const std::string& name,
				      const double& content)
{
  std::ostringstream os;
  os << std::scientific << content;
  return put_element_text(parent, name, os.str());
}

sys::xml_ptr sys::put_attribute_uint (sys::xml_ptr e, const std::string& name,
				      unsigned int value)
{
  std::ostringstream os;
  os << value;
  return put_attribute_text(e, name, os.str());
}

sys::xml_ptr sys::put_attribute_double (sys::xml_ptr e, const std::string& name,
					const double& value)
{
  std::ostringstream os;
  os << std::scientific << value;
  return put_attribute_text(e, name, os.str());
}

sys::xml_ptr sys::put_element_doubles (sys::xml_ptr root, 
				       const std::string& name,
				       const std::vector<double>& content)
{
  std::ostringstream oss;
  oss << std::scientific;
  for (size_t i=0; i<content.size()-1; ++i) oss << content[i] << " ";
  oss << content[content.size()-1];
  return put_element_text(root, name, oss.str());
}
