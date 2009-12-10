//Dear emacs, this is -*- c++ -*-

/**
 * @file data/RandomInteger.h
 *
 * @brief Declares the RandomInteger class.
 */

#ifndef DATA_RANDOMINTEGER_H
#define DATA_RANDOMINTEGER_H

#include <cstddef>
#include <vector>

namespace data {
  
  /**
   * RandomInteger's abstracts random number genearation.
   *
   * The RandomInteger class abstracts random number genearations by
   * encapsulating a way to genearate random integers and arrays of
   * those numbers. The seed may be set either at initialisation or
   * during the existance of a RandomInteger variable.
   */
  class RandomInteger {
    
  public:
    
    /**
     * A RandomInteger constructor.
     *
     * The only constructor initialises the random genearator.
     *
     * @param seed The seed to the random number genearator. If zero is given
     * (the default parameter), the seed is initialized with
     * <code>time(0)</code>, which is a second based machine counter. If you
     * need to produce different sequences of numbers consecutively, it is
     * recommended that you only instantiate 1 object of this class,
     * statically.
     */
    RandomInteger(const size_t& seed =0);

    /**
     * Returns a random integer between zero (included) and the
     * parameter value (excluded).
     *
     * @warning If the maximum is <b>1</b>, the system always outputs 0. 
     *
     * @param max The maximum value that can be returned.
     */
    size_t draw (const size_t& max) const;

    /**
     * Produces a set of random integers in a container, with the same
     * restrictions as above
     *
     * The result of this method is placed on the std::valarray given
     * as parameter. The number of draws will be the same as the size
     * of the container.
     *
     * @warning If the maximum is <b>1</b>, the system always outputs 0. 
     *
     * @param max The maximum value that can be returned.
     * @param c   The container where to put the values drawn.
     */
    void draw (const size_t& max, std::vector<size_t>& c) const;

  private:
    size_t m_seed; ///< The seed is kept here, for debugging.

  };

}

#endif //DATA_RANDOMINTEGER_H 
