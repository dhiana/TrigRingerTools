//Dear emacs, this is -*- c++ -*-

/**
 * @file network/src/NeuronBackProp.cxx
 *
 * @brief This file contains the definition of the Back Propagation
 * Learning algorithm. It is used for network::Neuron learning in Back
 * Propagation mode.
 */

#include "TrigRingerTools/network/NeuronBackProp.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include <cmath>

/**
 * How to apply <code>tanh</code> in a data::Ensemble
 */
inline data::Feature fwd_tanh (data::Feature d)
{
   return tanh(d);
}

/**
 * How to apply the derivative <code>tanh</code> in a data::Ensemble. Please
 * notice this is not the tanh derivative directly, but takes in consideration
 * the value given by <b>d</b> to be the output of an activation function and
 * therefore the derivative resumes to the given formula. If
 * @f[
 * f(x) = tanh(x)
 * @f]
 * Then
 * @f[
 * f'(x) = 1 - tanh^2(x) = 1 - f^2(x)!
 * @f]
 */
inline data::Feature bwd_tanh (data::Feature d)
{
  return 1 - std::pow(d,2);
}

/**
 * How to apply <code>sigmoid</code> in a data::Ensemble. The sigmoid function
 * is represented by:
 * @f[
 * f(x) = \frac{1}{1+e^{-x}}
 * @f]
 */
inline data::Feature fwd_sigmoid (data::Feature d)
{
  return 1 / (1+std::exp(-d));
}

/**
 * How to apply the derivative <code>sigmoid</code> in a data::Ensemble, if d
 * represents the output value of an activated neuron. In this case, the
 * formula is resumed to:
 * @f[
 * f'(x) = \frac{e^{-x}}{[1+e^{-x}]^2} = f(x)[1-f(x)]!
 * @f]
 */
inline data::Feature bwd_sigmoid (data::Feature d)
{
  return d * (1 - d);
}

/**
 * How to apply <code>linear</code> in a data::Ensemble. The linear function
 * is represented by:
 * @f[
 * f(x) = x
 * @f]
 */
inline data::Feature fwd_linear (data::Feature d)
{
  return d;
}

/**
 * How to apply the derivative <code>linear</code> in a data::Ensemble, if
 * <b>d</b> represents the output value of an activated neuron. In this case,
 * the formula is resumed to:
 * @f[
 * f'(x) = 1
 * @f]
 */
inline data::Feature bwd_linear (data::Feature /*d*/)
{
  return 1;
}

strategy::NeuronBackProp::NeuronBackProp 
(const config::NeuronBackProp::ActivationFunction& af)
  : m_af(af)
{
  switch (m_af) {
  case config::NeuronBackProp::TANH:
    m_forward = fwd_tanh;
    m_backward = bwd_tanh;
    break;
  case config::NeuronBackProp::SIGMOID:
    m_forward = fwd_sigmoid;
    m_backward = bwd_sigmoid;
    break;
  case config::NeuronBackProp::LINEAR:
    m_forward = fwd_linear;
    m_backward = bwd_linear;
    break;
  default:
    RINGER_DEBUG1("Unknown Activation Function type (" << m_af << ")."
		<< " Exception thrown.");
    throw RINGER_EXCEPTION("unknown activation function type");
  }
}

strategy::NeuronBackProp::NeuronBackProp 
(const config::Parameter* config)
  : m_af()
{
  const config::NeuronBackProp* bpparams =
    dynamic_cast<const config::NeuronBackProp*>(config);
  m_af = bpparams->activation_function();
  switch (m_af) {
  case config::NeuronBackProp::TANH:
    m_forward = fwd_tanh;
    m_backward = bwd_tanh;
    break;
  case config::NeuronBackProp::SIGMOID:
    m_forward = fwd_sigmoid;
    m_backward = bwd_sigmoid;
    break;
  case config::NeuronBackProp::LINEAR:
    m_forward = fwd_linear;
    m_backward = bwd_linear;
    break;
  default:
    RINGER_DEBUG1("Unknown Activation Function type (" << m_af << ")."
		<< " Exception thrown.");
    throw RINGER_EXCEPTION("unknown activation function type");
  }
}

void strategy::NeuronBackProp::run (data::Ensemble& data) const
{
  data.apply(m_forward);
}

void strategy::NeuronBackProp::teach
(data::Ensemble& output, const data::Ensemble& lesson) const
{
  output.apply(m_backward);
  output *= lesson;
}

config::NeuronBackProp strategy::NeuronBackProp::dump (void) const
{
  return config::NeuronBackProp(m_af);
}

std::string strategy::NeuronBackProp::dot (void) const
{
  switch (m_af) {
  case config::NeuronBackProp::TANH:
    return "TANH";
    break;
  case config::NeuronBackProp::SIGMOID:
    return "SIGMOID";
    break;
  case config::NeuronBackProp::LINEAR:
    return "LINEAR";
    break;
  default:
    RINGER_DEBUG1("Unknown Activation Function type (" << m_af << ")."
		<< " Exception thrown.");
    throw RINGER_EXCEPTION("unknown activation function type");
  }
  return "";
}
