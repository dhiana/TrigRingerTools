//Dear emacs, this is -*- c++ -*-

/**
 * @file LMS.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">André DOS ANJOS</a> 
 * $Author: rtorres $
 * $Revision: 1.2 $
 * $Date: 2007-04-12 21:03:40 $
 *
 * @brief Implements a network that implements the common Least Mean Square
 * algorithm for discriminators.
 */

#ifndef NETWORK_LMS_H
#define NETWORK_LMS_H

#include "TrigRingerTools/network/Network.h"
#include "TrigRingerTools/network/NeuronStrategy.h"
#include "TrigRingerTools/network/SynapseStrategy.h"
#include <vector>

namespace network {

  /**
   * An easier way to build a LMS network.
   */ 
  class LMS : public Network {

  public: //interface

    /**
     * Constructor, builds a new LMS discriminator, without biasing on neurons
     * and with a very simple structure.
     *
     * @param input The number of input lines
     * @param learn_rate The learning rate to apply to the back propagation
     * algorithm deployed to learn the discriminator.
     * @param subtract an optional vector that contains "input" entries, one
     * for each input. This is the subtraction factor to apply for every input
     * the network receives.
     * @param divide an optional vector that contains "input" entries, one
     * for each input. This is the division factor to apply for every input
     * the network receives.
     *
     * @param reporter The reporter to inform about changes or errors.
     */
    LMS (const size_t input,
         const data::Pattern& input_subtract,
         const data::Pattern& input_divide,
         sys::Reporter* reporter);

    /**
     * Just to virtualise the destructor
     */
    virtual ~LMS() {}

  };

}

#endif /* NETWORK_LMS_H */
