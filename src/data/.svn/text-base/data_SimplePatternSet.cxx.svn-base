//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/SimplePatternSet.cxx
 *
 * @brief Defines the SimplePatternSet class. 
 */

#include <iostream>
#include <sstream>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_errno.h>
#include <cstdio>

#include "TrigRingerTools/data/SimplePatternSet.h"
#include "TrigRingerTools/data/RandomInteger.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/xmlutil.h"

data::SimplePatternSet::SimplePatternSet(const size_t& size, 
					 const size_t& p_size,
					 const double& init)
  : m_data(0) ///< initialize with a NULL pointer
{
  RINGER_DEBUG1("Creating SimplePatternSet with size=" 
		<< size << " and pattern"
		<< " size=" << p_size << ", initializer is " << init);
  //try to allocate enough space with GSL
  m_data = gsl_matrix_alloc (size, p_size);
  //set all elements to the given value
  gsl_matrix_set_all(m_data, init);
  RINGER_DEBUG1("SimplePatternSet created and initialised.");
}

data::SimplePatternSet::SimplePatternSet(sys::xml_ptr_const node)
  : m_data(0)
{
  std::vector<Pattern*> data;
  //for all entries in a class
  for (sys::xml_ptr_const it = sys::get_first_child(node); it; 
       it=sys::get_next_element(it)) {
    if (!sys::is_element(it)) continue;
    std::vector<double> curr;
    sys::xml_ptr_const feats = sys::get_first_child(it);
    if (!sys::is_element(feats)) feats = sys::get_next_element(feats);
    sys::get_element_doubles(feats, curr);
    data.push_back(new data::Pattern(curr));
  }
  //check all patterns first
  size_t std_size = data[0]->size();
  for (size_t i=0; i<data.size(); ++i) {
    if (data[i]->size() != std_size) {
      RINGER_DEBUG1("Pattern[" << i << "] has a different pattern than the"
		    << " rest. Exception thrown");
      throw RINGER_EXCEPTION
	("Pattern has a different pattern than the rest. Exception thrown");
    }
  }
  //Build
  RINGER_DEBUG2("Building SimplePatternSet from XML data.");
  m_data = gsl_matrix_alloc(data.size(), std_size);
  for (unsigned int i=0; i<data.size(); ++i)
    gsl_matrix_set_row(m_data, i, data[i]->m_vector);
  RINGER_DEBUG2("The new SimplePatternSet has " <<data.size()<< " patterns.");
  for (size_t i=0; i<data.size(); ++i) delete data[i];
}

data::SimplePatternSet::SimplePatternSet(const SimplePatternSet& other)
  : PatternSet(), m_data(0)
{
  RINGER_DEBUG2("Building SimplePatternSet from"
		<< " another SimplePatternSet (copy construct).");
  m_data = gsl_matrix_alloc(other.m_data->size1, other.m_data->size2);
  gsl_matrix_memcpy(m_data, other.m_data);
}

data::SimplePatternSet::SimplePatternSet(const SimplePatternSet& other,
					 const std::vector<size_t>& pats)
  : m_data(0)
{
  RINGER_DEBUG2("Building SimplePatternSet from selected patterns of another"
		<< " SimplePatternSet (kind-of-copy construct).");
  m_data = gsl_matrix_alloc(pats.size(), other.m_data->size2);
  if (!m_data) {
    RINGER_DEBUG1("Allocation of internal matrix failed. Exception thrown.");
    throw RINGER_EXCEPTION("Failed internal matrix allocation");
  }
  RINGER_DEBUG1("Allocated new GSL matrix with address " << m_data
		<< ", for " << pats.size() << " patterns and with "
		<< other.m_data->size2 << " features per pattern.");
  for (unsigned int i=0; i<pats.size(); ++i)
    gsl_matrix_set_row(m_data, i, 
		       &gsl_matrix_row(other.m_data, pats[i]).vector);
  RINGER_DEBUG2("The new SimplePatternSet has " <<pats.size()<< " patterns.");
}

data::SimplePatternSet::SimplePatternSet(const std::vector<Pattern*>& pats)
  : m_data(0)
{
  //check all patterns first
  size_t std_size = pats[0]->size();
  for (size_t i=0; i<pats.size(); ++i) {
    if (pats[i]->size() != std_size) {
      RINGER_DEBUG1("Pattern[" << i << "] has a different pattern than the"
		    << " rest. Exception thrown");
    }
  }

  //Build
  RINGER_DEBUG2("Building SimplePatternSet from selected patterns.");
  m_data = gsl_matrix_alloc(pats.size(), std_size);
  for (unsigned int i=0; i<pats.size(); ++i)
    gsl_matrix_set_row(m_data, i, pats[i]->m_vector);
  RINGER_DEBUG2("The new SimplePatternSet has " <<pats.size()<< " patterns.");
}

data::SimplePatternSet::~SimplePatternSet()
{
  if (m_data) gsl_matrix_free(m_data);
}

size_t data::SimplePatternSet::size () const
{
  return m_data->size1;
}

size_t data::SimplePatternSet::pattern_size () const
{
  return m_data->size2;
}

const data::Pattern data::SimplePatternSet::pattern (const size_t& pos) const
{
  RINGER_DEBUG3("Getting const reference to Pattern[" << pos << "]");
  if (pos >= size()) {
    RINGER_DEBUG1("The maximum number of patterns is " << size()
		  << ". You are trying to access pattern " << pos
		  << ". Exception thrown.");
    throw RINGER_EXCEPTION("SimplePatternSet out of (pattern) range");
  }
  return gsl_matrix_row(m_data, pos);
}

const data::Ensemble data::SimplePatternSet::ensemble (const size_t& pos) const
{
  RINGER_DEBUG3("Getting const reference to Ensemble[" << pos << "]");
  if (pos >= pattern_size()) {
    RINGER_DEBUG1("The maximum number of ensembles is " << pattern_size()
		  << ". You are trying to access ensemble " << pos
		  << ". Exception thrown.");
    throw RINGER_EXCEPTION("SimplePatternSet out of (ensemble) range");
  }
  return gsl_matrix_column(m_data, pos);
}

void data::SimplePatternSet::set_pattern (const size_t& pos, 
					  const Pattern& pat)
{
  if (size() <= pos) {
    RINGER_DEBUG1("Trying to set pattern @" << pos 
		  << " but this set has only " 
		  << size() << " patterns. Exception thrown.");
    throw RINGER_EXCEPTION("Unexisting pattern"); 
  }

  if (pattern_size() != pat.size()) {
    RINGER_DEBUG1("Trying to set pattern @" << pos << " with size " 
		  << pattern_size() 
		  << " with a pattern with different length = " << pat.size() 
		  << ". Exception thrown.");
    throw RINGER_EXCEPTION("Different sizes in copy operation.");
  }
  gsl_vector_view view = gsl_matrix_row(m_data, pos);
  gsl_vector_memcpy(&view.vector, pat.m_vector);
  return;
}

void data::SimplePatternSet::set_ensemble (const size_t& pos, 
					   const Ensemble& ens)
{
  if (pattern_size() <= pos) {
    RINGER_DEBUG1("Trying to set ensemble @" << pos 
		  << " but this set has only " 
		  << pattern_size() << " ensembles. Exception thrown.");
    throw RINGER_EXCEPTION("Unexisting ensemble");
  }

  if (size() != ens.size()) {
    RINGER_DEBUG1("Trying to set ensemble @" << pos << " with size " 
		  << size() << " with a pattern with different length = " 
		  << ens.size() << ". Exception thrown.");
    throw RINGER_EXCEPTION("Different sizes in copy operation.");
  }
  gsl_vector_view view = gsl_matrix_column(m_data, pos);
  gsl_vector_memcpy(&view.vector, ens.m_vector);
  return;
}

void data::SimplePatternSet::erase_pattern (const size_t& pos)
{
  RINGER_DEBUG3("Trying to remove pattern " << pos << " from set.");
  if (size() <= pos) {
    RINGER_DEBUG1("Trying to erase pattern @" << pos 
		  << " but this set has only " << size() 
		  << " patterns. Exception thrown.");
    throw RINGER_EXCEPTION("Unexisting pattern");
  }
  gsl_matrix* new_data = gsl_matrix_alloc(m_data->size1-1, m_data->size2);
  if (pos != 0 && pos != m_data->size1-1) {
    //copy before and after removal point
    gsl_matrix_const_view before = 
      gsl_matrix_const_submatrix(m_data, 0, 0, pos, m_data->size2);
    gsl_matrix_const_view after = 
      gsl_matrix_const_submatrix(m_data, pos+1, 0, m_data->size1-pos-1, 
				 m_data->size2);
    gsl_matrix_view new_before = 
      gsl_matrix_submatrix(new_data, 0, 0, pos, m_data->size2);
    gsl_matrix_view new_after = 
      gsl_matrix_submatrix(new_data, pos, 0, 
			   m_data->size1-pos-1, m_data->size2);
    gsl_matrix_memcpy(&new_before.matrix, &before.matrix);
    gsl_matrix_memcpy(&new_after.matrix, &after.matrix);
  }
  else { //in the extremes, it is easier to cut
    if (pos == 0) {
      gsl_matrix_const_view v = 
	gsl_matrix_const_submatrix(m_data, 1, 1, 
				   m_data->size1-1, m_data->size2);
      gsl_matrix_memcpy(new_data, &v.matrix);
    }
    if (pos == m_data->size1-1) {
      gsl_matrix_const_view v = 
	gsl_matrix_const_submatrix(m_data, 0, 0, 
				   m_data->size1-1, m_data->size2);
      gsl_matrix_memcpy(new_data, &v.matrix);
    }
  }
  gsl_matrix_free(m_data);
  m_data = new_data;
  RINGER_DEBUG3("Pattern " << pos << " was removed from set. "
		<< "The new number of patterns is " << size() << ".");
  return;
}

void data::SimplePatternSet::erase_ensemble (const size_t& pos)
{
  RINGER_DEBUG3("Trying to remove ensemble " << pos << " from set.");
  if (pattern_size() <= pos) {
    RINGER_DEBUG1("Trying to erase ensemble @" << pos 
		  << " but this set has only " << pattern_size() 
		  << " ensembles. Exception thrown.");
    throw RINGER_EXCEPTION("Unexisting ensemble");
  }
  gsl_matrix* new_data = gsl_matrix_alloc(m_data->size1, m_data->size2-1);
  if (pos != 0 && pos != m_data->size2-1) {
    //copy before and after removal point
    gsl_matrix_const_view before = 
      gsl_matrix_const_submatrix(m_data, 0, 0, m_data->size1, pos);
    gsl_matrix_const_view after = 
      gsl_matrix_const_submatrix(m_data, 0, pos+1, m_data->size1, 
				 m_data->size2-pos-1);
    gsl_matrix_view new_before = 
      gsl_matrix_submatrix(new_data, 0, 0, m_data->size1, pos);
    gsl_matrix_view new_after = 
      gsl_matrix_submatrix(new_data, 0, pos, m_data->size1, 
			   m_data->size2-pos-1);
    gsl_matrix_memcpy(&new_before.matrix, &before.matrix);
    gsl_matrix_memcpy(&new_after.matrix, &after.matrix);
  }
  else { //in the extremes, it is easier to cut
    if (pos == 0) {
      gsl_matrix_const_view v = 
	gsl_matrix_const_submatrix(m_data, 0, 1, m_data->size1, 
				   m_data->size2-1);
      gsl_matrix_memcpy(new_data, &v.matrix);
    }
    if (pos == m_data->size2-1) {
      gsl_matrix_const_view v = 
	gsl_matrix_const_submatrix(m_data, 0, 0, m_data->size1, 
				   m_data->size2-1);
      gsl_matrix_memcpy(new_data, &v.matrix);
    }
  }
  gsl_matrix_free(m_data);
  m_data = new_data;
  RINGER_DEBUG3("Ensemble " << pos << " was removed from set. "
		<< "The new number of ensembles is " << pattern_size() << ".");
  return;
}

data::PatternSet* data::SimplePatternSet::clone (void) const
{
  return new data::SimplePatternSet(*this);
}

data::PatternSet* data::SimplePatternSet::clone 
(const std::vector<size_t>& pats) const
{
  return new data::SimplePatternSet(*this, pats);
}

void data::SimplePatternSet::shuffle (void)
{
  static data::RandomInteger rnd;
  std::vector<size_t> pos(size());
  rnd.draw(size(), pos);
  data::SimplePatternSet new_order(*this, pos);
  *this = new_order;
}

sys::xml_ptr data::SimplePatternSet::dump (sys::xml_ptr any,
					   const std::string& cname,
					   const size_t start_id) const
{
  sys::xml_ptr node = sys::make_node(any, "class");
  sys::put_attribute_text(node, "name", cname);
  for (size_t i=0; i<size(); ++i) {
    RINGER_DEBUG3("XML'ing pattern[" << i << "] of class \"" << cname 
		  << "\". The accumulated identifier is [" << start_id+i 
		  << "].");
    sys::xml_ptr entry = sys::put_element(node, "entry");
    sys::put_attribute_uint(entry, "id", start_id + i);
    const Pattern pat = pattern(i);
    std::vector<data::Feature> val(pat.size());
    for (size_t j=0; j<pat.size(); ++j) val[j] = pat[j];
    sys::put_element_doubles(entry, "feature", val);
  }
  return node;
}

void data::SimplePatternSet::apply_pattern_op (const data::PatternOperator& op)
{
  RINGER_DEBUG2("Applying PatternOperator to *all* my patterns.");
  //test output size of operator `op'
  data::Pattern tmp(pattern(0).size());
  op(pattern(0), tmp);
  size_t std_size = tmp.size();
  data::SimplePatternSet newset(size(), std_size, 0);
  for (size_t i=0; i<size(); ++i) { //for every pattern
    op(pattern(i), tmp);
    if (tmp.size() != std_size) {
      RINGER_DEBUG1("PatternOperator's that apply to"
		    << " SimplePatternSet's have to "
		    << "generate Pattern's with the same size always.");
      throw RINGER_EXCEPTION("Non-stationary PatternOperator forbidden");
    }
    newset.set_pattern(i, tmp);
  }
  *this = newset; //copies result
}

void data::SimplePatternSet::apply_ensemble_op 
(const data::PatternOperator& op)
{
  RINGER_DEBUG2("Applying PatternOperator to *all* my ensembles.");
  //test output size of operator `op'
  data::Pattern tmp(ensemble(0).size());
  op(ensemble(0), tmp);
  size_t std_size = tmp.size();
  data::SimplePatternSet newset(size(), std_size, 0);
  for (size_t i=0; i<pattern_size(); ++i) { //for every pattern
    op(ensemble(i), tmp);
    if (tmp.size() != std_size) {
      RINGER_DEBUG1("PatternOperator's that apply to"
		    << " SimplePatternSet's have to "
		    << "generate Ensemble's with the same size always.");
      throw RINGER_EXCEPTION("Non-stationary PatternOperator forbidden");
    }
    newset.set_ensemble(i, tmp);
  }
  *this = newset; //copies result
}

std::ostream& data::SimplePatternSet::stream_out (std::ostream& os) const
{
  unsigned int i;
  for (i=0; i<size()-1; ++i) 
    os << "[" << i << "] " << pattern(i) << "\n";
  os << "[" << i << "] " << pattern(i);
  return os;
}


sys::Plain& data::SimplePatternSet::stream_out (sys::Plain& f) const
{
  unsigned int i;
  for (i=0; i<size()-1; ++i) f << pattern(i) << "\n";
  f << pattern(i);
  return f;
}

data::SimplePatternSet& data::SimplePatternSet::merge
(const SimplePatternSet& other)
{
  RINGER_DEBUG3("Trying to merge two SimplePatternSet's");
  if (other.size() == 0) return *this; /// Don't do anything
    
  if (pattern_size() != other.pattern_size()) {
    RINGER_DEBUG1("Trying to merge SimplePatternSet's with "
		  << "different Pattern sizes. "
		  << "LHS has " << pattern_size() << " ensembles while "
		  << "RHS has " << other.pattern_size() << " ensembles.");
    throw RINGER_EXCEPTION("RHS has a different pattern size."); 
  }

  gsl_matrix* new_data = gsl_matrix_alloc(size() + other.size(),
					  pattern_size());
  gsl_matrix_view new_before = gsl_matrix_submatrix(new_data, 0, 0, 
						    m_data->size1, 
						    m_data->size2);
  gsl_matrix_view new_after = gsl_matrix_submatrix(new_data, m_data->size1, 0,
						   other.size(), 
						   m_data->size2);  
  gsl_matrix_memcpy(&new_before.matrix, m_data);
  gsl_matrix_memcpy(&new_after.matrix, other.m_data);
  gsl_matrix_free(m_data);
  m_data = new_data;
  RINGER_DEBUG3("New SimplePatternSet's contains " << size() << " patterns.");
  return *this;
}

data::SimplePatternSet& data::SimplePatternSet::assign
(const SimplePatternSet& other, const std::vector<size_t>& pats)
{
  RINGER_DEBUG2("Reseting SimplePatternSet from selected patterns of another"
		<< " SimplePatternSet (kind-of-copy construct).");
  if (m_data->size1 != other.m_data->size1 || 
      m_data->size2 != other.m_data->size2) {
    gsl_matrix_free(m_data);
    m_data = gsl_matrix_alloc(pats.size(), other.m_data->size2);
    RINGER_DEBUG1("Reallocated this SimplePatternSet (assign()'ing)...");
  }
  for (unsigned int i=0; i<pats.size(); ++i)
    gsl_matrix_set_row(m_data, i, 
		       &gsl_matrix_row(other.m_data, pats[i]).vector);
  RINGER_DEBUG2("The new SimplePatternSet has " <<pats.size()<< " patterns.");
  return *this;
}

data::SimplePatternSet& 
data::SimplePatternSet::operator= (const SimplePatternSet& other)
{
  RINGER_DEBUG2("Copying SimplePatternSet from another"
		<< " SimplePatternSet (operator=).");
  if (m_data->size1 != other.m_data->size1 || 
      m_data->size2 != other.m_data->size2) {
    gsl_matrix_free(m_data);
    m_data = gsl_matrix_alloc(other.m_data->size1, other.m_data->size2);
  }
  gsl_matrix_memcpy(m_data, other.m_data);
  return *this;
}

data::SimplePatternSet& data::SimplePatternSet::operator-= 
(const SimplePatternSet& other)
{
  RINGER_DEBUG2("Copying SimplePatternSet from "
		<< "another SimplePatternSet (operator=).");
  if (m_data->size1 != other.m_data->size1 || 
      m_data->size2 != other.m_data->size2) {
    RINGER_DEBUG1("For operator-=, I need sets with equal sizes."
		  << " The current set is [" << m_data->size1 << ","
		  << m_data->size2 << "] and the assigned set is ["
		  << other.m_data->size1 << "," << other.m_data->size2 << "]");
    throw RINGER_EXCEPTION("Different pattern sizes in subtraction");
  }
  gsl_matrix_sub(m_data, other.m_data);
  return *this;
}
