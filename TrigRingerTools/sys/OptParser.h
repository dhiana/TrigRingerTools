//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/OptParser.h
 *
 * @brief An ultra-simple command line parser that uses popt.
 */

#ifndef SYS_OPTPARSER_H
#define SYS_OPTPARSER_H

#include <string>
#include <vector>
#include <popt.h>

namespace sys {

  /**
   * A simple command line parser based on POPT.
   */
  class OptParser {

  public:

    /**
     * Builds a new parser
     *
     * @param progname The program name (what is in argv[0])
     * @param extra_args The extra arguments the program will accept
     */
    OptParser (const char* progname, const char* extra_args=0);

    /**
     * Destroyes the parser and all dynamic resources
     */
    virtual ~OptParser ();

    /**
     * Add a new flag option for this parser.
     *
     * @param long_name Long option name
     * @param short_name Short option name
     * @param binding An argument of this type will be bounded to
     * @param description of this arguments purpose
     */
    void add_option (const std::string& long_name, char short_name,
		     bool& binding, const std::string& description);

    /**
     * Add a new string option for this parser.
     *
     * @param long_name Long option name
     * @param short_name Short option name
     * @param def The default value for this variable
     * @param binding An argument of this type will be bounded to
     * @param description of this arguments purpose
     */
    void add_option (const std::string& long_name, char short_name,
		     std::string& binding, const std::string& description);

    /**
     * Add a new vector of strings option for this parser.
     *
     * @param long_name Long option name
     * @param short_name Short option name
     * @param def The default value for this variable
     * @param binding An argument of this type will be bounded to
     * @param description of this arguments purpose
     */
    void add_option (const std::string& long_name, char short_name, 
		     std::vector<std::string>& binding, 
		     const std::string& description);

    /**
     * Add a new integer option for this parser.
     *
     * @param long_name Long option name
     * @param short_name Short option name
     * @param binding An argument of this type will be bounded to
     * @param description of this arguments purpose
     * @param def An optional default value for this variable
     */
    void add_option (const std::string& long_name, char short_name,
		     long int& binding, const std::string& description);
		
    /**
     * Add a new float option for this parser.
     *
     * @param long_name Long option name
     * @param short_name Short option name
     * @param def The default value for this variable
     * @param binding An argument of this type will be bounded to
     * @param description of this arguments purpose
     * @param def An optional default value for this variable
     */
    void add_option (const std::string& long_name, char short_name,
		     double& binding, const std::string& description);

    /**
     * Prints the short usage information on std::err
     */
    void help (void);

    /**
     * Prints the long usage information on std::err
     */
    void usage (void);

    /**
     * Parses and binds the variables in the command line. Problems without
     * solution are reported with a <code>false</code> return value.
     *
     * @param argc The number of arguments to parse
     * @param argv The arguments
     */
    bool parse (int& argc, char**& argv);

    /**
     * Returns the left over arguments
     */
    inline const std::vector<std::string>& leftover (void) const
    { return m_leftover; }

  private:
    
    /**
     * Builds the popt context
     */
    struct poptOption* build_table (void);

    /**
     * Destroys the popt context
     *
     * @param table The POPT option table
     */
    void destroy_table (struct poptOption* table);

    /**
     * Updates the parser bindings to the external world
     *
     * @return <code>true</code> if the update was sucessfull or
     * <code>false</code> otherwise.
     */
    bool update_bindings (void);

    /**
     * Defines the option values
     */
    typedef struct opt_t {
      std::string long_name; ///< The long name of this option
      char short_name; ///< The short name of this opt
      std::string description; ///< Option description
      std::string arg_description; ///< The argument description (automatic)
      int argument_type; ///< The type of option
      void* binding; ///< The binding of this variable
      void* local; ///< The local variable to be set
      bool multi; ///< If this option is multi-valued
    } opt_t;

  private: //representation

    std::string m_progname; ///< The program name
    std::string m_extra_args; ///< Extra arguments to the help function
    std::vector<opt_t> m_opt; ///< My option table
    std::vector<std::string> m_leftover; ///< Leftover arguments
  };

}

#endif /* SYS_OPTPARSER_H */
