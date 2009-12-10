//Dear emacs, this is -*- c++ -*-

/**
 * @file network/network/Synapse.h
 *
 * @brief Defines the common Synapse type.
 *
 * This file contains the definition for the Synapse type. This type will
 * not be derived for now, so it was made concrete.
 */

#ifndef NETWORK_SYNAPSE_H
#define NETWORK_SYNAPSE_H

#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/data/Feature.h"
#include "TrigRingerTools/config/type.h"
#include "TrigRingerTools/config/Synapse.h"
#include "TrigRingerTools/config/Parameter.h"

namespace strategy {
  class SynapseStrategy; ///< forward
}

namespace network {

  class Neuron; ///< forward

  /**
   * Defines how to connect a Neuron to another.
   *
   * This class defines the connections that may exist between two
   * Neuron's. The Synapse is a simple entity that traverses data between
   * nodes, possibly amplifying (or attenuating) it using its
   * weight. The Synapse weight can be trained, so a network can learn.
   *
   * @todo implement graphics drawing using graphivz. In this modality
   * each piece contributes with its own description 
   */
  
  class Synapse {

  public:

    /**
     * How to build-up a Synapse.
     *
     * @param init The initial value for the weight of this synapse
     * @param strategy The strategy to adopt for synapses
     * @param params The parameters related to the chosen strategy
     * @param id The optional identifier to this synapse.
     */
    Synapse (const data::Feature& init,
	     const config::SynapseStrategyType& strategy,
	     const config::Parameter* params,
	     const unsigned int* id=0);

    /**
     * How to build-up a Synapse from a configuration
     *
     * @param config The synapse configuration
     */
    Synapse (const config::Synapse& config);

    /**
     * How to destroy a Synapse.
     */
    virtual ~Synapse();

    /**
     * Instructs the Synapse to run.
     *
     * This method, when called, will take the input parameter and make it
     * cross the Synapse (possibly changing it). This method will trigger the
     * output Neuron::run() as well.
     *
     * @param data What to pass to the output Neuron.
     */
    void pass (const data::Ensemble& data);

    /**
     * Instructs the synapse to learn.
     *
     * This command instructs the synapse to learn(). The way a synapse learns
     * depend on its strategy::Learning. It may use each of the neurons state
     * or else in order to accomplish the call. After learning, the Synapse
     * should trigger its input Neuron.
     *
     * @param lesson The current state of the output Neuron
     */
    void learn (const data::Ensemble& lesson);

    /**
     * Changes whether the Synapse can or cannot learn any more.
     *
     * A call to this method will trigger the Synapse not to update its weight
     * anymore, if the parameter is <i>false</i>. If the parameter is
     * <i>true</i>, the Synapse will be able to learn again.
     *
     * @param switch_to Will I be able to learn()?
     * @see learn()
     */
    void learning (const bool& switch_to);

    /**
     * Connect two Neurons together using this Synapse.
     *
     * This method connects two Neuron's together using this Synapse. It will
     * check to see if this Synapse is not already connected perform the
     * relevant operations on both Synapse sides. If you want to re-use a
     * Synapse, first unconnect it and then reconnect it to the right
     * places. It's an error to try to unconnect the Synapse using something
     * like <code> connect(0,0) </code>.
     *
     * @param start The Neuron that will serve as input to the Synapse
     * @param end The Neuron that will serve as output to the Synapse
     */
    void connect (network::Neuron* start, network::Neuron* end);

    /**
     * Disconnects this Synapse.
     *
     * This method disconnects this Synapse to the Neuron's than it is
     * connected. If this Synapse is <b>not</b> connected or only
     * partially connected, the relevant actions are taken for the
     * Synapse to be <em>fully</em> disconnected. The relevant Neuron's
     * are notified of the disconnection.
     */
    void disconnect (void);

    /**
     * Returns a const reference to input neuron
     */
    const network::Neuron* input (void) const { return m_in; }

    /**
     * Returns a const reference to output neuron
     */
    const network::Neuron* output (void) const { return m_out; }

    /**
     * Return the Synapse unique identifier.
     */
    unsigned int id (void) const;

    /**
     * Returns the Synapse weight.
     */
    data::Feature weight (void) const { return m_weight; }

    /**
     * Default meaning
     *
     * @param other The other Synapse
     */
    bool operator== (const Synapse& other) const;

    /**
     * Default meaning
     *
     * @param other The other Synapse
     */
    bool operator!= (const Synapse& other) const;

    /**
     * Dumps its configuration in the given container.
     */
    config::Synapse dump(void) const;

  protected:
    /**
     * Gets a new unique identifier.
     */
    unsigned int new_id ();

  private: 
    static unsigned int s_id; ///< Unique identifier genearator

  private:
    
    /**
     * You can't copy a Synapse.
     */
    Synapse& operator= (const Synapse& other); ///< not implemented

    /**
     * You can't copy a Synapse.
     */
    Synapse(const Synapse& other); ///< not implemented

  private:

    unsigned int m_id; ///< The Synapse unique identifier
    data::Feature m_weight; ///< The Synapse weight value
    network::Neuron* m_in; ///< The Neuron connected to the input
    network::Neuron* m_out; ///< The Neuron connected to the output
    bool m_learns; ///< If this Synapse is able to learn
    data::Ensemble m_state; ///< This is the Synapse's current state
    config::SynapseStrategyType m_strategy; ///< This synapse's learn strategy
    strategy::SynapseStrategy* m_teacher; ///< the learning object
  };

}

#endif //NETWORK_SYNAPSE_H
