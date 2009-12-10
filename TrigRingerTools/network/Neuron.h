//Dear emacs, this is -*- c++ -*-

/**
 * @file network/network/Neuron.h
 *
 * @brief The base Neuron class definition
 *
 * This file contains the definition for the Neuron base class, the
 * Neuron namespace and the RunningMode.
 */

#ifndef NETWORK_NEURON_H
#define NETWORK_NEURON_H

#include <vector>
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/config/Neuron.h"

/**
 * Includes all network elements.  This namespace includes all neural network
 * related classes. If you would like to extend it, think of putting your new
 * class into it. It will make the code more readable and consistent
 */
namespace network {

  class Synapse; ///< forward

  /**
   * Describes the neuron "base" element.  
   *
   * The Neuron is composed of several input and output synapses. It's able to
   * tell and be set of its current running mode. This Neuron entity is an
   * abstract class, and <B>cannot</B> be instantiated.
   *
   * @see Synapse
   * @see Feature 
   */

  class Neuron {

  public:

    /**
     * Builds a neuron with a pre-defined state.
     *
     * @param id The optional identifier to this neuron.
     */
    Neuron (const unsigned int* id=0);

    /**
     * Builds a neuron with parameters in a configuration object.
     *
     * @param config The configuration object to use for loading this neuron
     */
    Neuron (const config::Neuron& config);

    /**
     * Destructor virtualisation.
     */
    virtual ~Neuron ();
    
    /**
     * Runs some data through the neuron.
     *
     * This method will issue the neuron to get some data and process
     * it through. It's an abstract function and is designed to be
     * called by the <B>input</B> of the neuron. To follow the
     * sequentiality of the process, this function may call somebody
     * else's (a Synapse) run().
     *
     * @param data The input data for the run, given by the calling
     * synapse
     */
    virtual void run (const data::Ensemble& data) =0;
    
    /**
     * Trains some error through the neuron.
     *
     * This method does exactly the analogue of run(), it gets some
     * data from the <B>output</B>, waiting for all synapses to
     * call. When done, the data is processed. This function is also
     * abstract. To follow the sequentiality of the process, this
     * function may call somebody else's train().
     *
     * @param error The input data for the train, given by the calling
     * synapse.
     */
    virtual void train (const data::Ensemble& error) =0;

    /**
     * Returns the current Neuron state.
     *
     * This method is used during both run()'ing and train()'ing. It
     * returns the Neuron state in the sense of a
     * data::Ensemble's. That may represent the Neuron's output, if
     * its mode is RunningMode::RAN, or the output of the training
     * operation, if its mode is RunningMode::TRAINED.
     */
    virtual const data::Ensemble& state (void) const =0;

    /**
     * Connects two Neuron's together.
     *
     * This method connects two Neuron's together. The Synapse between
     * them is created on demand. If the Neuron's are already
     * connected, nothing happens.
     *
     * @param l The input network::Synapse to use
     */
    virtual Neuron& in_connect (network::Synapse* l) = 0;

    /**
     * Connects two Neuron's together.
     *
     * This method connects two Neuron's together. The Synapse between
     * them is created on demand. If the Neuron's are already
     * connected, nothing happens.
     *
     * @param l The output network::Synapse to use
     */
    virtual Neuron& out_connect (network::Synapse* l) = 0;

    /**
     * Disconnects two Neuron's that are connected.
     *
     * This method disconnects two Neuron's that are connected. If
     * they are already disconnected, nothing is done.
     *
     * @param l The input network::Synapse to disconnect
     */
    virtual Neuron& in_disconnect (network::Synapse* l) = 0;

    /**
     * Disconnects two Neuron's that are connected.
     *
     * This method disconnects two Neuron's that are connected. If
     * they are already disconnected, nothing is done.
     *
     * @param l The output network::Synapse to disconnect
     */
    virtual Neuron& out_disconnect (network::Synapse* l) = 0;

    /**
     * Return the Neuron unique identifier.
     */
    inline unsigned int id (void) const { return m_id; }

    /**
     * The default meaning.
     *
     * @param other The other Neuron to consider
     */
    inline bool operator== (const Neuron& other) const
    { return m_id == other.m_id; }

    /**
     * The default meaning.
     *
     * @param other The other Neuron to consider
     */
    inline bool operator!= (const Neuron& other) const
    { return ! (*this == other); }

    /**
     * This will dump the current configuration into a config::Neuron
     */
    virtual config::Neuron dump(void) const = 0;

    /**
     * This will dump the connections of this neuron to an output stream, in
     * dot-file format.
     *
     * @param os The output stream to consider
     */
    virtual bool dot(std::ostream& os) const = 0;

  protected:
    /**
     * Checks if I have already some Synapse at a given Synapse collection.
     *
     * This method checks in a Synapse collection if it contains a given
     * Synapse. If it does, the method returns an iterator to this
     * Synapse. If it doesn't it returns the outcomes of end() for that
     * collection.
     *
     * @param c The Synapse container to check
     * @param l The Synapse to be checked against my collection.
     */
    std::vector<network::Synapse*>::iterator get_synapse 
    (std::vector<network::Synapse*>& c, const network::Synapse* l);

  private:
    static unsigned int s_id; ///< Unique identifier genearator

  private: //representation
    unsigned int m_id; ///< The unique identification number for this neuron

  };

}

#endif //NETWORK_NEURON_H


