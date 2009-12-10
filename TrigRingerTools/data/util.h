//Dear emacs, this is -*- c++ -*-

/**
 * @file data/util.h
 *
 * @brief Describes a set of operations appliable to Pattern's and
 * PatternSet's.
 */

#ifndef DATA_UTIL_H
#define DATA_UTIL_H

#include "TrigRingerTools/data/Feature.h"
#include "TrigRingerTools/data/PatternSet.h"

namespace data {

  /**
   * Calculates the Mean Square (MS) of a given PatternSet, taking in
   * consideration both of its dimensions. The formulae for this calculation 
   * is given by:
   * @f[
   * \frac{{\sum_i}^M{{\sum_j}^N{{e_{ij}}^{2}}}}{M \times N}
   * @f]
   * Where <b>M</b> describes the number of patterns the set contains and
   * <b>N</b>, the number of ensembles there is.
   *
   * @param p The pattern set to use for calculating the RMS
   */
  data::Feature mean_square (const data::PatternSet& p);
	
  /**
   * Calculates the Root Mean Square (RMS) of a given PatternSet, taking in
   * consideration both of its dimensions. The formula for this calculation is
   * given by:
   * @f[
   * E = \sqrt{\frac{{\sum_i}^M{{\sum_j}^N{{e_{ij}}^{2}}}}{M \times N}}
   * @f]
   * Where <b>M</b> describes the number of patterns the set contains and
   * <b>N</b>, the number of ensembles there is.
   *
   * @param p The pattern to use for calculating the RMS
   */
  data::Feature root_mean_square (const data::PatternSet& p);

  /**
   * Calculates the mean of a sum of absolute values of a given PatternSet,
   * taking in consideration both of its dimensions. The formula for this
   * calculation is given by:
   * @f[
   * E = \frac{{\sum_i}^M{{\sum_j}^N{{|e_{ij}|}}}}{M \times N}
   * @f]
   * Where <b>M</b> describes the number of patterns the set contains and
   * <b>N</b>, the number of ensembles there is.
   *
   * @param p The pattern to use for calculating the RMS
   */
  data::Feature abs_mean (const data::PatternSet& p);

  /**
   * Given an output and a target, calculates the MSE for that amount of
   * patterns.
   *
   * @param output The output of the network
   * @param target The target of the network
   */
  template <class TSet>
  double mse (const TSet& output, const TSet& target);

  /**
   * Returns the maximum value of the SP product for the given output and
   * targers. This procedure will pass a cut line from the min(target) to
   * max(target) and will check which events are wrongly classified on both
   * scenarios. This will determine the classification efficiency for both
   * classes of data. This is used to compute:
   *
   * @f[
   * SP = max( (eff1 + eff2) \times (eff1 \times eff2) )
   * @f]
   *
   * Which is the returned value for this function.
   *
   * @param output The output of the network
   * @param target The target of the network
   * @param eff1 The efficiency for the classification of class 1
   * @param eff2 The efficiency for the classification of class 2
   * @param thres The threshold that gives the above efficiencies
   */
  double sp (const data::PatternSet& output, const data::PatternSet& target,
	     double& eff1, double& eff2, double& thres);
  
}

template <class TSet>
double data::mse (const TSet& output, const TSet& target)
{
  TSet error(target);
  error -= output;
  return data::mean_square(error);
}

#endif /* DATA_UTIL_H */
