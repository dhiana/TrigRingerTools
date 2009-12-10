//Dear emacs, this is -*- c++ -*-

/**
 * @file Parameter.h
 *
 * @brief Defines an abstract class to a parameter. It can be used to clone or
 * delete the parameter related to the object without knowing its type.
 */

#ifndef CONFIG_PARAMETER_H
#define CONFIG_PARAMETER_H

#include "TrigRingerTools/sys/xmlutil.h"

namespace config {
  
  /**
   * An abstract interface to paramters given to (usually) hidden or output
   * neurons.
   */
  class Parameter {
    
  public: //interface, completely abstract

    /**
     * Default Constructor
     */
    Parameter() { }

    /**
     * Destructor virtualisation
     */
    virtual ~Parameter() { }

    /**
     * Clones this object
     */
    virtual Parameter* clone () const =0;

    /**
     * Gets the XML representation for these parameters.
     *
     * @param any Any node in the XML document tree where you want to created
     * you new node.
     */
    virtual sys::xml_ptr node (sys::xml_ptr any) =0;

  };

}

#endif /* CONFIG_PARAMETER_H */
