//Dear emacs, this is -*- c++ -*-

/**
 * @file data/Database.h
 *
 * A database abstraction that holds all data.
 *
 */

#ifndef DATA_DATABASE_H
#define DATA_DATABASE_H

#include <string>
#include <map>
#include "TrigRingerTools/data/PatternOperator.h"
#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/data/Ensemble.h"
#include "TrigRingerTools/data/RoIPatternSet.h"
#include <cmath>

namespace data {

  /**
   * Abstracts the concept of database. An implementation must be provided to fill this class. 
   */
  template <class TSet> class Database {

  public:

    /**
     * Builds a new interface.
     *
     * @param reporter The reporter to give to the configuration system
     */
    Database (sys::Reporter *reporter);

    /**
     * Builds a new database out of scratch parameters
     *
     * @param data The PatternSets for this database, classified
     * @param reporter The reporter to give to the configuration system
     */
    Database (const std::map<std::string, TSet*>& data,
	      sys::Reporter *reporter);

    /**
     * Copy constructor
     *
     * @param other The other database to copy data from
     */
    Database (const Database& other);

    /**
     * Destructor virtualisation
     */
    virtual ~Database();

    /**
     * Returns the number of classes I have in this database.
     */
    inline size_t size() const { return m_data.size(); }

    /**
     * Returns the size of each pattern in this database
     */
    inline size_t pattern_size() const { return m_patsize; }

    /**
     * Returns the input data set for a particular class
     *
     * @param class_id The class identifier
     */
    inline const TSet* data (const std::string& class_id)
    { return m_data[class_id]; }

    /**
     * Returns all data sets
     */
    inline const std::map<std::string, TSet*>& data () const
    { return m_data; }

    /**
     * Fills in a vector with all my class names
     *
     * @param cn Where to place my class names for the caller
     */
    void class_names (std::vector<std::string>& cn);

    /**
     * Merges this database in a single PatternSet
     *
     * @param ps The PatternSet to put the result to
     */
    void merge (TSet& ps) const;

    /**
     * Returns a PatternSet which express the class of each 'merged' Pattern
     * returned by merge(). There are two possible options "minimal"
     * representation, in which the number of ensembles will be minimised to
     * the closest power of two possibility. For example, if I have two
     * classes, only one ensemble will exist (<code>min</code> indicates the
     * value for the first class of Pattern's while <code>max</code> indicates
     * the value for the second class of Pattern's). The second possibility is
     * "normal" where each output will represent one and only one class. In
     * this case, looking at the targets, the maximum happening in ensemble 0,
     * indicates the Pattern's are from class 0, when the maximum happens at
     * ensemble 2, the Pattern's related belong to class 2, and on.
     *
     * @param minimal Do we use "minimal" representation? A value of
     * <code>false</code> imples we use the "normal" representation.
     * @param min The minimum value the reader can interpret (neuron off).
     * @param max The maximum value the reader can interpret (neuron on).
     * @param target The target PatternSet according to the parameters before
     */
    void merge_target (const bool minimal, const double& min,
		       const double& max, 
		       TSet& target) const;

    /**
     * Normalises the database contents with respect to its classes. This
     * process will calculate the number of Patterns in each class and will
     * concatenate each PatternSet (class) so each class has the same amount
     * of Pattern's.
     */
    void normalise (void);

    /**
     * This will split the Database in two components: train and test data,
     * according to the proportion given by the argument (which should be
     * greater than zero and less than the unity). The <b>new</b> databases
     * for training and testing are returned to the caller.
     *
     * @param prop The amount (between <i>0&lt;prop&lt;1</i>) of the data that
     * will be dedicated to training. If this amount is between 0 and -1, the
     * partition happens the contrary.
     * @param train The newly created training database
     * @param test The newly created testing database
     *
     * @return <code>true</code> if the split was successful,
     * <code>false</code> otherwise. This method can return <code>false</code>
     * in the case the value of prop is less or equal to 0 or greater or equal
     * to 1.
     */
    bool split (const double& prop, Database*& train, Database*& test) const;

    /**
     * This will shuffle all PatternSet's inside this database, randomly.
     */
    void shuffle ();

    /**
     * Applies the given Operator to all Patterns in all my PatternSet
     * classes.
     *
     * @param op The PatternOperator to apply
     */
    void apply_pattern_op (const data::PatternOperator& op);

    /**
     * Applies the given Operator to all Ensembles in all my PatternSet
     * classes.
     *
     * @param op The EnsembleOperator to apply
     */
    void apply_ensemble_op (const data::PatternOperator& op);

  //private: //forbidden

    /**
     * Assignment
     */
    Database& operator= (const Database& other);

  protected: //representation
    
    std::map<std::string, TSet*> m_data; ///< The data
    sys::Reporter *m_reporter; ///< The system reporter for errors/warnings
    size_t m_patsize; ///< The number of feature each pattern has

  };

}

/**
 * Dumps a database with a nice representation on a ostream
 *
 * @param os The output stream to use
 * @param db The database to dump
 */
template <class TSet>
std::ostream& operator<< (std::ostream& os, const data::Database<TSet>& db);

/**
 * Returns a Pattern which represents 'n' in binary with the maximum and
 * minimum set by the available parameters. The length of the returned Pattern
 * is represented by 's'
 *
 * @param n The binary to represent
 * @param s The size of the Pattern to return
 * @param min The representation of "zero" to use
 * @param max The representation of "one" to use
 */
data::Pattern make_target (const size_t n, const size_t s,
			   const double& min, const double& max);

//------------------------------
// Template Implementation
//------------------------------

template <class TSet>
data::Database<TSet>::Database (sys::Reporter *reporter)
  : m_data(),
    m_reporter(reporter),
    m_patsize(0)
{
}

template <class TSet>
data::Database<TSet>::Database (const std::map<std::string, TSet*>& data,
				sys::Reporter *reporter)
  : m_data(),
    m_reporter(reporter),
    m_patsize(0)
{
  for (typename std::map<std::string, TSet*>::const_iterator 
	 it = data.begin(); it != data.end(); ++it)
    m_data[it->first] = new TSet(*it->second);

  //check pattern sizes
  if (m_data.size() != 0) {
    m_patsize = m_data.begin()->second->pattern_size();
    for (typename std::map<std::string, TSet*>::const_iterator
	   it = m_data.begin(); it != m_data.end(); ++it) {
      if (m_patsize != it->second->pattern_size()) {
	RINGER_DEBUG1("PatternSet from class \"" << it->first << "\" seems to"
		      << " have a different pattern size ("
		      << it->second->pattern_size() << ") then the rest of"
		      << " the database (" << m_patsize 
		      << "). Exception thrown.");
	//unwind
	for (typename std::map<std::string, TSet*>::const_iterator it 
	       = m_data.begin(); it != m_data.end(); ++it) delete it->second;
	throw 
	  RINGER_EXCEPTION("Uncoherent database (different pattern sizes)");
      }
    }
  }

  RINGER_DEBUG3("Database created from scratch.");
}

template <class TSet>
data::Database<TSet>::Database (const Database& other)
  : m_data(),
    m_reporter(other.m_reporter),
    m_patsize(0)
{
  for (typename std::map<std::string, TSet*>::const_iterator 
	 it = other.m_data.begin(); it != other.m_data.end(); ++it)
    m_data[it->first] = new TSet(*it->second);

  //check pattern sizes
  if (m_data.size() != 0) {
    m_patsize = m_data.begin()->second->pattern_size();
    for (typename std::map<std::string, TSet*>::const_iterator
	   it = m_data.begin(); it != m_data.end(); ++it) {
      if (m_patsize != it->second->pattern_size()) {
	RINGER_DEBUG1("PatternSet from class \"" << it->first << "\" seems to"
		      << " have a different pattern size ("
		      << it->second->pattern_size() << ") then the rest of"
		      << " the database (" << m_patsize 
		      << "). Exception thrown.");
	//unwind
	for (typename std::map<std::string, TSet*>::const_iterator it 
	       = m_data.begin(); it != m_data.end(); ++it) delete it->second;
	throw 
	  RINGER_EXCEPTION("Uncoherent database (different pattern sizes)");
      }
    }
  }

  RINGER_DEBUG3("Database created from copy.");
}

template <class TSet>
data::Database<TSet> & data::Database<TSet>::operator= (const data::Database<TSet>& other) {
  m_data.clear();
  m_reporter = other.m_reporter;

  for (typename std::map<std::string, TSet*>::const_iterator 
	 it = other.m_data.begin(); it != other.m_data.end(); ++it)
    m_data[it->first] = new TSet(*it->second);

  //check pattern sizes
  if (m_data.size() != 0) {
    m_patsize = m_data.begin()->second->pattern_size();
    for (typename std::map<std::string, TSet*>::const_iterator
	   it = m_data.begin(); it != m_data.end(); ++it) {
      if (m_patsize != it->second->pattern_size()) {
	RINGER_DEBUG1("PatternSet from class \"" << it->first << "\" seems to"
		      << " have a different pattern size ("
		      << it->second->pattern_size() << ") then the rest of"
		      << " the database (" << m_patsize 
		      << "). Exception thrown.");
	//unwind
	for (typename std::map<std::string, TSet*>::const_iterator it 
	       = m_data.begin(); it != m_data.end(); ++it) delete it->second;
	throw 
	  RINGER_EXCEPTION("Uncoherent database (different pattern sizes)");
      }
    }
  }

  RINGER_DEBUG3("Database created from copy.");
  return *this;
}

template <class TSet>
data::Database<TSet>::~Database() 
{
  for (typename std::map<std::string, TSet*>::const_iterator 
	 it = m_data.begin(); it != m_data.end(); ++it) delete it->second;
  RINGER_DEBUG3("Database destroyed.");
}

template <class TSet>
void data::Database<TSet>::class_names (std::vector<std::string>& cn)
{
  for (typename std::map<std::string, TSet*>::const_iterator 
	 it = m_data.begin(); it != m_data.end(); ++it) 
    cn.push_back(it->first);
}

namespace data {

  /**
   * Some hackish way to emulate partial template specialization for the merge
   * functionality on data::Database
   *
   * @see data::Database::merge()
   * 
   * @param dest The destination set
   * @param source The source of data information that has to be merged
   */
  template <class T>
  void merge_sets (T& dest, const std::map<std::string, T*>& source)
  {
    bool init = false;
    for (typename std::map<std::string, T*>::const_iterator
	   it = source.begin(); it != source.end(); ++it) {
      if (!init) {
	dest = *it->second;
	init = true;
    }
      else dest.merge(*it->second);
    }
  }

  /**
   * Some hackish way to emulate partial template specialization for the merge
   * functionality on data::Database. This is the data::RoIPatternSet
   * specialization.
   *
   * @see data::Database::merge()
   * 
   * @param dest The destination set
   * @param source The source of data information that has to be merged
   */
  template <> void merge_sets<data::RoIPatternSet> 
  (data::RoIPatternSet& dest, 
   const std::map<std::string, data::RoIPatternSet*>& source);

}

template <class TSet>
void data::Database<TSet>::merge (TSet& ps) const
{
  data::merge_sets<TSet>(ps, m_data);
}

namespace data {

  /**
   * Some hackish way to emulate partial template specialization for the
   * merge_target() functionality on data::Database.
   *
   * @see data::Database::merge_target()
   * 
   * @param minimal Do we use "minimal" representation? A value of
   * <code>false</code> imples we use the "normal" representation.
   * @param min The minimum value the reader can interpret (neuron off).
   * @param max The maximum value the reader can interpret (neuron on).
   * @param target The target PatternSet according to the parameters before
   * @param source The source of data information that has to be merged
   */
  template <class T> void merge_target_sets
  (const bool minimal, const double& min, const double& max,
   T& target, const std::map<std::string, T*>& source)
  {
    //The next test probably characterizes this should be split in two
    //functions, but let's leave it this way for the time being
    if (minimal) {
      //Condense the targets to the minimal binary set possible
      size_t total_size = 0;
      for (typename std::map<std::string, T*>::const_iterator
	     it = source.begin(); it != source.end(); ++it)
	total_size += it->second->size();
      size_t next_power_of_2 = lrint(std::ceil(log2(source.size())));
      T my_target(total_size, next_power_of_2, min);
      size_t class_counter=0;
      size_t pattern_counter=0;
      for (typename std::map<std::string, T*>::const_iterator
	     it = source.begin(); it != source.end(); ++it) {
	for (size_t j=0; j<it->second->size(); ++j) {
	  data::Pattern curr_target = make_target(class_counter, 
						  next_power_of_2,
						  min, max);
	  my_target.set_pattern(pattern_counter, curr_target);
	  ++pattern_counter;
	}
	++class_counter;
      }
      target = my_target;
    }
    else {
      if (source.size() >= 1) {
	T tmp(source.begin()->second->size(), source.size(), min);
	data::Ensemble tmpe(source.begin()->second->size(), max);
	tmp.set_ensemble(0, tmpe);
	target = tmp;
      }
      if (source.size() > 1) {
	size_t i=1;
	bool init = false;
	for (typename std::map<std::string, T*>::const_iterator
	       it = source.begin(); it != source.end(); ++it) {
	  T tmp(it->second->size(), source.size(), min);
	  data::Ensemble tmpe(it->second->size(), max);
	  tmp.set_ensemble(i++, tmpe);
	  if (!init) {
	    target = tmp;
	    init = true;
	  }
	  else target.merge(tmp);
	}
      }
    }
  }

  /**
   * Some hackish way to emulate partial template specialization for the
   * merge_target() functionality on data::Database. This is the
   * data::RoIPatternSet specialization.
   *
   * @see data::Database::merge_target()
   * 
   * @param minimal Do we use "minimal" representation? A value of
   * <code>false</code> imples we use the "normal" representation.
   * @param min The minimum value the reader can interpret (neuron off).
   * @param max The maximum value the reader can interpret (neuron on).
   * @param target The target PatternSet according to the parameters before
   * @param source The source of data information that has to be merged
   */
  template <> void merge_target_sets<data::RoIPatternSet>
  (const bool minimal, const double& min, const double& max, 
   data::RoIPatternSet& target, 
   const std::map<std::string, data::RoIPatternSet*>& source);

}

template <class TSet>
void data::Database<TSet>::merge_target (const bool minimal,
					 const double& min,
					 const double& max,
					 TSet& target) const
{
  data::merge_target_sets<TSet>(minimal, min, max, target, m_data);
}

template <class TSet>
void data::Database<TSet>::normalise (void)
{
  //get the greatest PatternSet size in this Database
  size_t greater = 0;
  for (typename std::map<std::string, TSet*>::const_iterator
	 it = m_data.begin(); it != m_data.end(); ++it) {
    if (it->second->size() > greater) greater = it->second->size();
  }

  //check who is 50% less and adjust it
  for (typename std::map<std::string, TSet*>::iterator
	 it = m_data.begin(); it != m_data.end(); ++it) {
    if (it->second->size() < 0.9*greater) {
      //increase its size by copying the Patterns all over
      if (it->second->size() > 0.5*greater) { //only partial copy
	size_t to_copy = greater - it->second->size();
	RINGER_DEBUG2("Class \"" << it->first 
		      << "\" is more than 10% smaller ("
		      << it->second->size() 
		      << ") than this DB's greatest size (" << greater << "),"
		      << " but it is still more than 50% of that size."
		      << " So, I'm increasing it by " << to_copy << ".");
	std::vector<size_t> patnumber(to_copy);
	for (size_t i=0; i<to_copy; ++i) patnumber[i] = i;
	TSet merge_this(*it->second, patnumber);
	it->second->merge(merge_this);
      }
      else { //first copy N times the PatternSet
	size_t n_times = lrint(std::floor(log2(greater/it->second->size())));
	RINGER_DEBUG2("Class \"" << it->first 
		      << "\" is more than 10% smaller ("
		      << it->second->size() 
		      << ") than this DB's greatest size (" << greater << ")."
		      << " I'll need to copy it over at least "
		      << n_times << " times.");
	for (size_t i=0; i<n_times; ++i) it->second->merge(*it->second);
	//if we are still bellow 10% difference
	if (it->second->size() < 0.9*greater) {
	  size_t to_copy = greater - it->second->size();
	  RINGER_DEBUG2("Class \"" << it->first 
			<< "\" is still more than 10% smaller (" 
			<< it->second->size() 
			<< ") than this DB's greatest size (" 
			<< greater << ")."
			<< " So, I'm increasing it by " << to_copy << ".");
	  std::vector<size_t> patnumber(to_copy);
	  for (size_t i=0; i<to_copy; ++i) patnumber[i] = i;
	  TSet merge_this(*it->second, patnumber);
	  it->second->merge(merge_this);
	}
      }
    }
  }
}

template <class TSet>
bool data::Database<TSet>::split (const double& prop, Database<TSet>*& train, 
				  Database<TSet>*& test) const
{
  if (prop <= -1 || prop >= 1) return false;
  double prop_use = std::fabs(prop);
  RINGER_DEBUG2("Splitting database by " << prop_use*100 << "%.");
  std::map<std::string, TSet*> train_data;
  std::map<std::string, TSet*> test_data;
  for (typename std::map<std::string, TSet*>::const_iterator
	 it = m_data.begin(); it != m_data.end(); ++it) {
    size_t first_part = lrint(it->second->size() * prop_use);
    RINGER_DEBUG2("PatternSet for class \"" << it->first << "\" will provide "
		  << first_part << " patterns and "
		  << it->second->size() - first_part << " patterns.");
    std::vector<size_t> patnumber(first_part);
    if (prop > 0)
      for (size_t i=0; i<first_part; ++i) patnumber[i] = i;
    else {
      patnumber.resize(it->second->size() - first_part);
      for (size_t i=first_part; i<it->second->size(); ++i)
	patnumber[i-first_part] = i;
    }
    train_data[it->first] = new TSet(*it->second, patnumber);
    if (prop > 0) {
      patnumber.resize(it->second->size() - first_part);
      for (size_t i=first_part; i<it->second->size(); ++i)
	patnumber[i-first_part] = i;
    }
    else {
      patnumber.resize(first_part);
      for (size_t i=0; i<first_part; ++i) patnumber[i] = i;
    }
    test_data[it->first] = new TSet(*it->second, patnumber);
  }
  train = new data::Database<TSet>(train_data, m_reporter);
  for (typename std::map<std::string, TSet*>::iterator
	 it = train_data.begin(); it != train_data.end(); ++it) 
    delete it->second;
  test = new data::Database<TSet>(test_data, m_reporter);
  for (typename std::map<std::string, TSet*>::iterator
	 it = test_data.begin(); it != test_data.end(); ++it) 
    delete it->second;
  return true;
}

template <class TSet>
void data::Database<TSet>::shuffle ()
{
  for (typename std::map<std::string, TSet*>::const_iterator
	 it = m_data.begin(); it != m_data.end(); ++it) it->second->shuffle();
}

template <class TSet>
void data::Database<TSet>::apply_pattern_op (const data::PatternOperator& op)
{
  for (typename std::map<std::string, TSet*>::const_iterator
	 it = m_data.begin(); it != m_data.end(); ++it) 
    it->second->apply_pattern_op(op);
}

template <class TSet>
void data::Database<TSet>::apply_ensemble_op (const data::PatternOperator& op)
{
  for (typename std::map<std::string, TSet*>::const_iterator
	 it = m_data.begin(); it != m_data.end(); ++it)
    it->second->apply_ensemble_op(op);
}

template <class TSet>
std::ostream& operator<< (std::ostream& os, const data::Database<TSet>& db)
{
  for (typename std::map<std::string, TSet*>::const_iterator
	 it = db.data().begin(); it != db.data().end(); ++it)
    os << "Database class [" << it->first << "] is:" << std::endl
       << *it->second << std::endl;
  return os;
}


#endif /* DATA_DATABASE_H */
