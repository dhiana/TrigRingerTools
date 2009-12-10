//Dear emacs, this is -*- c++ -*-

/**
 * @file network/src/SynapseRProp.cxx
 *
 * @brief This file contains the definition of the Back Propagation
 * Learning algorithm. It is used for network::Synapse learning in Back
 * Propagation mode.
 */

#include "TrigRingerTools/network/SynapseRProp.h"
#include "TrigRingerTools/config/SynapseRProp.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/data/MeanExtractor.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include <cmath>

static const data::Feature MIN_STEP = 0.5;
static const data::Feature MAX_STEP = 1.2;
static const data::Feature DELTA_MAX = 50.0;
static const data::Feature DELTA_MIN = 1e-6;

inline int SIGN(const data::Feature& x) {
  if (x>0) return +1;
  else if (x<0) return -1;
  return 0;
}

strategy::SynapseRProp::SynapseRProp 
(const data::Feature& weight_update)
  : m_weight_update(weight_update),
    m_prev_delta(0),
    m_prev_deriv(0)
{
  RINGER_DEBUG3("Instantiated with Weight update = " << m_weight_update);
}

strategy::SynapseRProp::SynapseRProp (const config::Parameter* config)
  : m_weight_update(0),
    m_prev_delta(0),
    m_prev_deriv(0)
{
  const config::SynapseRProp* rpparams =
    dynamic_cast<const config::SynapseRProp*>(config);
  m_weight_update = rpparams->weight_update();
  //checks on ranges are performed at XML configuration parsing!
  RINGER_DEBUG3("Instantiated with Weight update = " << m_weight_update);
}

data::Feature strategy::SynapseRProp::teach (const data::Ensemble& input,
                                             const data::Ensemble& lesson)
{
  RINGER_DEBUG3("SynapseRProp::teach called.");
  data::MeanExtractor mean;
  data::Feature deriv = mean(lesson * input);
  RINGER_DEBUG1("Calculating synaptic weight adjustment with change = "
                << deriv << ", weight update = " << m_weight_update
                << ", previous derivative = " << m_prev_deriv
                << ", previous delta = " << m_prev_delta);

  data::Feature retval = 0;
  int sign_change = SIGN(deriv*m_prev_deriv);

  if (sign_change > 0) {
    m_weight_update = std::min(m_weight_update*MAX_STEP, DELTA_MAX);
  }
  else if (sign_change < 0) {
    m_weight_update = std::max(m_weight_update*MIN_STEP, DELTA_MIN);
    m_prev_deriv = 0;
  }
  else {
    retval = -1 * m_prev_delta;
  }

  if (deriv < 0) retval = -1 * m_weight_update;
  else retval = m_weight_update;
  
  m_prev_deriv = deriv;
  m_prev_delta = retval;
  return retval;
}

config::SynapseRProp strategy::SynapseRProp::dump (void) const
{
  return config::SynapseRProp(m_weight_update);
}
