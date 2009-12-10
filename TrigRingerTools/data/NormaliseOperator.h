//Dear emacs, this is -*- c++ -*-

/**
 * @file data/NormaliseOperator.h
 *
 * @brief This operator removes the mean of a Pattern.
 */

#ifndef DATA_NORMALISEOPERATOR_H
#define DATA_NORMALISEOPERATOR_H

#include "TrigRingerTools/data/PatternOperator.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/sys/debug.h"
#include <gsl/gsl_vector.h>
#include <cmath>

namespace data {

  class NormaliseOperator : public PatternOperator {

  public:

    /**
     * Constructor. To normalise ensembles in a DB, I have to compute
     * first the maximum for all classes in a DB.
     *
     * @param db The database to extract the ensemble mean from
     */
    template <class TSet>
    NormaliseOperator(const data::Database<TSet>& db);

    /**
     * Destructor virtualisation
     */
    virtual ~NormaliseOperator() {}

    /**
     * Generates a Pattern with the same length as the previous, but with the
     * mean removed from each Pattern feature.
     *
     * @param in The Pattern to transform using this operator
     * @param out The Pattern to output
     */
    virtual void operator() (const data::Pattern& in,
			     data::Pattern& out) const;

  private: //representation

    data::Pattern m_max; ///< The maximum for each ensemble of the DB used

  };

}

template <class TSet>
data::NormaliseOperator::NormaliseOperator(const data::Database<TSet>& db)
  : m_max(db.pattern_size(),0)
{
  TSet ps(1,1);
  db.merge(ps);
  for (unsigned int i=0; i<ps.pattern_size(); ++i) { //for all ensembles
    data::Ensemble e = ps.ensemble(i);
    m_max[i] = fabs(gsl_vector_max(abuse(e)));
    data::Feature min = fabs(gsl_vector_min(abuse(e)));
    if (min > m_max[i]) m_max[i] = min;
    RINGER_DEBUG3("Database absolute maximum for ensemble[" 
		<< i << "] is " << m_max[i]);
  }
}

#endif /* DATA_NORMALISEOPERATOR_H */
