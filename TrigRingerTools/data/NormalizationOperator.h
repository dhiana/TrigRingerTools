//Dear emacs, this is -*- c++ -*-

/**
 * @file data/NormalizationOperator.h
 *
 * @brief This operator removes the mean of a Pattern.
 */

#ifndef DATA_NORMALIZATIONOPERATOR_H
#define DATA_NORMALIZATIONOPERATOR_H

#include "TrigRingerTools/data/PatternOperator.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/sys/debug.h"
#include <gsl/gsl_statistics_double.h>

namespace data {

  class NormalizationOperator : public PatternOperator {

  public:

    /**
     * Constructor. To remove the mean and variance of ensembles in a DB, I
     * have to compute first the mean for all classes in a DB.
     *
     * @param db The database to extract the ensemble mean from
     */
    template <class TSet>
    NormalizationOperator(const data::Database<TSet>& db);

    /**
     * Destructor virtualisation
     */
    virtual ~NormalizationOperator() {}

    /**
     * Generates a Pattern with the same length as the previous, but with the
     * mean removed and divided by the variance from each Pattern feature.
     *
     * @param in The Pattern to transform using this operator
     * @param out The Pattern to output
     */
    virtual void operator() (const data::Pattern& in,
			     data::Pattern& out) const;

    /**
     * Returns a handle to the means of all ensembles in the Database
     */
    inline const data::Pattern& mean (void) const { return m_mean; }

    /**
     * Returns a handle to the standard deviations of all ensembles in the
     * Database. 
     */
    inline const data::Pattern& stddev (void) const { return m_sd; }

  private: //representation

    data::Pattern m_mean; ///< The mean for the DB in question
    data::Pattern m_sd; ///< The standard deviation for the DB in question

  };

}

template <class TSet> data::NormalizationOperator::NormalizationOperator
(const data::Database<TSet>& db)
  : m_mean(db.pattern_size(),0),
    m_sd(db.pattern_size(),1)
{
  TSet ps(1,1);
  db.merge(ps);
  for (unsigned int i=0; i<ps.pattern_size(); ++i) { //for all ensembles
    data::Ensemble e = ps.ensemble(i);
    const gsl_vector* v = abuse(e);
    m_mean[i] = gsl_stats_mean(v->data, v->stride, v->size);
    m_sd[i] = gsl_stats_sd(v->data, v->stride, v->size);
    if (m_sd[i] < 1e-5) m_sd[i] = 1; ///to prevent overflowing...
    RINGER_DEBUG1("Database mean for ensemble[" << i << "] is " << m_mean[i]);
    RINGER_DEBUG1("Database standard deviation for ensemble[" << i 
		  << "] is " << m_sd[i]);
  }
}

#endif /* DATA_NORMALIZATIONOPERATOR_H */
