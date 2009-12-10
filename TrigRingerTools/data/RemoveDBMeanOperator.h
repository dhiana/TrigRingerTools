//Dear emacs, this is -*- c++ -*-

/**
 * @file data/RemoveDBMeanOperator.h
 *
 * @brief This operator removes the mean of a Pattern.
 */

#ifndef DATA_REMOVEDBMEANOPERATOR_H
#define DATA_REMOVEDBMEANOPERATOR_H

#include "TrigRingerTools/data/PatternOperator.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/sys/debug.h"
#include <gsl/gsl_statistics_double.h>

namespace data {

  class RemoveDBMeanOperator : public PatternOperator {

  public:

    /**
     * Constructor. To remove the mean and variance of ensembles in a DB, I
     * have to compute 
     * first the mean for all classes in a DB.
     *
     * @param db The database to extract the ensemble mean from
     */
    template <class TSet>
    RemoveDBMeanOperator(const data::Database<TSet>& db);

    /**
     * Destructor virtualisation
     */
    virtual ~RemoveDBMeanOperator() {}

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

    data::Pattern m_mean; ///< The mean for the DB in question

  };

}

template <class TSet> data::RemoveDBMeanOperator::RemoveDBMeanOperator
(const data::Database<TSet>& db)
  : m_mean(db.pattern_size(),0)
{
  TSet ps(1,1);
  db.merge(ps);
  for (unsigned int i=0; i<ps.pattern_size(); ++i) { //for all ensembles
    data::Ensemble e = ps.ensemble(i);
    const gsl_vector* v = abuse(e);
    m_mean[i] = gsl_stats_mean(v->data, v->stride, v->size);
    RINGER_DEBUG3("Database mean for ensemble[" << i << "] is " << m_mean[i]);
  }
}

#endif /* DATA_REMOVEDBMEANOPERATOR_H */
