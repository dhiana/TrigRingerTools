//Dear emacs, this is -*- c++ -*-

/**
 * @file OptParser.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre DOS ANJOS</a> 
 * $Author: rtorres $
 * $Revision: 1.3 $
 * $Date: 2007-11-09 15:06:11 $
 *
 * Implements this ultra-simple option parser.
 *
 * The system maps the C++ options the user wants to have set into C options
 * that POPT can digest (it has restrictions on variable aligment and
 * others). It also uses the POPT infrastructure to generate help and usage
 * messages.
 */

#ifndef ATHENA_VERSION

#include <sstream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "TrigRingerTools/sys/OptParser.h" 

sys::OptParser::OptParser (const char* progname, const char* extra_args)
  : m_progname(progname),
    m_extra_args()
{
  if (extra_args) {
    m_extra_args = "[OPTIONS]* ";
    m_extra_args += extra_args;
  }
}

sys::OptParser::~OptParser()
{
  for (unsigned int i=0; i<m_opt.size(); ++i) {
    switch (m_opt[i].argument_type) {
    case POPT_ARG_NONE: //type is boolean
      delete static_cast<int*>(m_opt[i].local);
      break;
    case POPT_ARG_INT: //type is long integer
      delete static_cast<long int*>(m_opt[i].local);
      break;
    case POPT_ARG_DOUBLE: //type is double
      delete static_cast<double*>(m_opt[i].local);
      break;
    case POPT_ARG_STRING: //type is string
      delete static_cast<char**>(m_opt[i].local);
      break;
    default:
      break;
    }
  }
}

void sys::OptParser::add_option (const std::string& long_name, 
				 char short_name, bool& binding, 
				 const std::string& description)
{
  //convert from boolean to int, since popt has problems with this type.
  int* local = new int(binding?1:0);
  opt_t newopt = { long_name, short_name, description, "", POPT_ARG_NONE, 
		   &binding, local, false};
  if (binding) newopt.arg_description = "boolean (true)";
  else newopt.arg_description = "boolean (false)";
  m_opt.push_back(newopt);
}

void sys::OptParser::add_option (const std::string& long_name, 
				 char short_name, std::string& binding,
				 const std::string& description)
{
  char** local = new char*;
  (*local) = 0;
  opt_t newopt = { long_name, short_name, description, "", POPT_ARG_STRING, 
		   &binding, local, false};
  newopt.arg_description = "string (\"" + binding + "\")";
  m_opt.push_back(newopt);
}

void sys::OptParser::add_option (const std::string& long_name, 
				 char short_name, 
				 std::vector<std::string>& binding,
				 const std::string& description)
{
  char** local = new char*;
  (*local) = 0;
  opt_t newopt = { long_name, short_name, description, "", POPT_ARG_STRING,
		   &binding, local, true};
  newopt.arg_description = "string[ string]* (";
  if (binding.size() != 0) {
    for (size_t i = 0; i < binding.size()-1; ++i)
      newopt.arg_description += binding[i] + ",";
    newopt.arg_description += binding[binding.size()-1];
  }
  newopt.arg_description += ")";
  m_opt.push_back(newopt);
}

void sys::OptParser::add_option (const std::string& long_name,
				 char short_name, long int& binding,
				 const std::string& description)
{
  long int* local = new long int(binding);
  opt_t newopt = { long_name, short_name, description, "", POPT_ARG_INT, 
		   &binding, local, false};
  std::ostringstream arg_description;
  arg_description << "integer (" << binding << ")";
  newopt.arg_description = arg_description.str();
  m_opt.push_back(newopt);
}
		
void sys::OptParser::add_option (const std::string& long_name,
				 char short_name, double& binding,
				 const std::string& description)
{
  double* local = new double(binding);
  opt_t newopt = { long_name, short_name, description, "", POPT_ARG_DOUBLE,
		   &binding, local, false};
  std::ostringstream arg_description;
  arg_description << "double (" << binding << ")";
  newopt.arg_description = arg_description.str();
  m_opt.push_back(newopt);
}

struct poptOption* sys::OptParser::build_table (void)
{
  static std::string HELPOPT_STRING = "Help options:";
  struct poptOption* table = new struct poptOption[2+m_opt.size()];
  //taken from /usr/include/popt.h
  struct poptOption helpopt = { 0, '\0', POPT_ARG_INCLUDE_TABLE, 
				poptHelpOptions, 0, 
				HELPOPT_STRING.c_str(), 0 };
  table[m_opt.size()] = helpopt;
  struct poptOption tableend = POPT_TABLEEND;
  table[1+m_opt.size()] = tableend;
  for (unsigned int i=0; i<m_opt.size(); ++i) {
    if (m_opt[i].multi) {
      //if is multi-valued, I have to parse the parts myself
      struct poptOption newopt = { m_opt[i].long_name.c_str(),
				   m_opt[i].short_name, POPT_ARG_STRING,
				   m_opt[i].local, 0,
				   m_opt[i].description.c_str(),
				   m_opt[i].arg_description.c_str() };
      table[i] = newopt;
    }
    else {
      struct poptOption newopt = { m_opt[i].long_name.c_str(),
				   m_opt[i].short_name, m_opt[i].argument_type,
				   m_opt[i].local, 0,
				   m_opt[i].description.c_str(),
				   m_opt[i].arg_description.c_str() };
      table[i] = newopt;
    }
  }
  return table;
}

void sys::OptParser::destroy_table (struct poptOption* table)
{
  delete[] table;
}

/**
 * Parses the string `src' and put the parsed contents in `dest'
 *	
 * @param dest The destination object to hold the parsed strings
 * @param src The source string to be parsed
 *
 * @return <code>true</code>, if the parsing goes Ok, or <code>false</code>
 * otherwise.
 */
bool parse_strings(std::vector<std::string>& dest, const char* src)
{
  if (!src) return true;
  dest.clear(); ///< replace all default entries in dest
  char srccpy[1024];
  strncpy(srccpy, src, 1024);
  const char* component = strtok(srccpy, " ");
  while (component != 0) {
    dest.push_back(component);
    component = strtok (0, " ");
  }
  return true;
}

bool sys::OptParser::update_bindings (void)
{
  //Update the bindings for all types
  for (unsigned int i=0; i<m_opt.size(); ++i) {
    switch (m_opt[i].argument_type) {
    case POPT_ARG_NONE:
      if (*static_cast<int*>(m_opt[i].local)) 
	(*static_cast<bool*>(m_opt[i].binding)) = true;
      else (*static_cast<bool*>(m_opt[i].binding)) = false;
      break;
    case POPT_ARG_INT:
      (*static_cast<long int*>(m_opt[i].binding)) = 
	(*static_cast<long int*>(m_opt[i].local));
      break;
    case POPT_ARG_DOUBLE:
      (*static_cast<double*>(m_opt[i].binding)) = 
	(*static_cast<double*>(m_opt[i].local));
      break;
    case POPT_ARG_STRING:
      if (m_opt[i].multi) {
	if (!parse_strings
	  (*static_cast<std::vector<std::string>*>(m_opt[i].binding),
	   *static_cast<char**>(m_opt[i].local))) return false;
      }
      else {
	if (*static_cast<char**>(m_opt[i].local)) {
	  (*static_cast<std::string*>(m_opt[i].binding)) = 
	    (*static_cast<char**>(m_opt[i].local));
	}
      }
      break;
    default:
      break;
    }
  }
  return true;
}

bool sys::OptParser::parse (int& argc, char**& argv)
{
  //clear leftover arguments
  m_leftover.erase(m_leftover.begin(), m_leftover.end());

  struct poptOption* opts = build_table();
  poptContext optCon = poptGetContext(0, argc, (const char**)argv, opts, 0);
  if (m_extra_args.size() != 0)
    poptSetOtherOptionHelp(optCon, m_extra_args.c_str());

  if (argc == 1) {
    poptPrintUsage(optCon, stderr, 0);
    poptFreeContext(optCon);
    destroy_table(opts);
    return false;
  }

  int rc = poptGetNextOpt(optCon);
  if (rc < -1) {
    /* an error occurred during option processing */
    std::cerr << "Error during option processing with popt! "
	      << poptBadOption(optCon, POPT_BADOPTION_NOALIAS) << ": "
	      << poptStrerror(rc) << std::endl;
    poptFreeContext(optCon);
    destroy_table(opts);
    return false;
  }
  update_bindings();

  //Check leftovers
  const char* l = 0;
  while ((l = poptGetArg(optCon))) m_leftover.push_back(std::string(l));

  poptFreeContext(optCon);
  destroy_table(opts);

  //Check required entries
  return true;
}

void sys::OptParser::help (void)
{
  struct poptOption* opts = build_table();
  const char* pname = m_progname.c_str();
  poptContext optCon = poptGetContext(NULL, 1, &pname, opts, 0);
  if (m_extra_args.size() != 0)
    poptSetOtherOptionHelp(optCon, m_extra_args.c_str());
  poptPrintHelp(optCon, stderr, 0);
  poptFreeContext(optCon);
  destroy_table(opts);
}

void sys::OptParser::usage (void)
{
  struct poptOption* opts = build_table();
  const char* pname = m_progname.c_str();
  poptContext optCon = poptGetContext(NULL, 1, &pname, opts, 0);
  if (m_extra_args.size() != 0)
    poptSetOtherOptionHelp(optCon, m_extra_args.c_str());
  poptPrintUsage(optCon, stderr, 0);
  poptFreeContext(optCon);
  destroy_table(opts);
}

#endif
