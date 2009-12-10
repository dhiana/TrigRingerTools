//Dear emacs, this is -*- c++ -*-

/**
 * @file data/src/Pattern.cxx
 *
 * @brief Describes the implementation of the Pattern entity
 */

#include "TrigRingerTools/data/Pattern.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Plain.h"

data::Pattern::Pattern (const size_t& s, const Feature v)
  : m_vector(0)
{
  if (!s) {
    RINGER_DEBUG1("I cannot allocate a vector with size=0! Exception thrown.");
    throw RINGER_EXCEPTION("Size==0 not allowed for Patterns");
  }
  m_vector = gsl_vector_alloc(s);
  gsl_vector_set_all(m_vector, v);
  RINGER_DEBUG3("Constructed pattern with size " << s 
	      << " and initializer " << v);
}

data::Pattern::Pattern(const std::vector<data::Feature>& feat)
  : m_vector(0)
{
  m_vector = gsl_vector_alloc(feat.size());
  for (unsigned int i=0; i<feat.size(); ++i)
    gsl_vector_set(m_vector, i, feat[i]);
  RINGER_DEBUG3("Constructed pattern from feature vector");
}

data::Pattern::Pattern(const Pattern& other)
  : m_vector(0)
{
  m_vector = gsl_vector_alloc(other.m_vector->size);
  gsl_vector_memcpy(m_vector, other.m_vector);
  RINGER_DEBUG3("Constructed pattern from another pattern (copy construct)");
}

/*
data::Pattern::Pattern(Feature *pattern, size_t featSize)
{
	m_vector = &m_static_vector;
	m_vector->stride = 0;
	m_vector->data = pattern;
	m_vector->size = featSize;
	m_vector->owner = 0;
	m_vector->block = (gsl_block*) pattern;
  RINGER_DEBUG3("Constructed pattern from previously allocated memory");
}
*/


data::Pattern::~Pattern()
{
#if RINGER_DEBUG>2
  std::string message = "Destroying pattern ";
  if (m_vector->owner) message += "with private memory allocation.";
  else message += "with a view (no memory allocation).";
  RINGER_DEBUG3(message);
#endif
  //this works because I cannot build a vector using another artifact than by
  //memory copying or through a view. Therefore, if I own the vector memory
  //allocated through the block underneath `m_vector', I should delete the
  //vector, otherwise, it was created through a view and should not be
  //deleted.
  if (m_vector && m_vector->owner) gsl_vector_free(m_vector);
}

data::Pattern& data::Pattern::apply
(data::Feature (*fct)(data::Feature))
{
  RINGER_DEBUG3("Applying function to pattern features one-by-one.");
  for (unsigned int i=0; i<m_vector->size; ++i) {
    Feature* tmp = gsl_vector_ptr(m_vector, i);
    *tmp = fct(*tmp);
  }
  return *this;
}

data::Pattern& data::Pattern::operator= (const Pattern& other)
{
  RINGER_DEBUG3("Pattern assignment operator called (RHS=Pattern).");
  if (size() != other.size()) {
    if (m_vector && m_vector->owner) gsl_vector_free(m_vector);
    m_vector = gsl_vector_alloc(other.size());
  }
  gsl_vector_memcpy(m_vector, other.m_vector);
  return *this;
}

data::Pattern& data::Pattern::operator= (const Feature& value)
{
  RINGER_DEBUG3("Pattern assignment operator called (RHS=Feature).");
  gsl_vector_set_all(m_vector, value);
  return *this;
}

size_t data::Pattern::size () const
{
  return m_vector->size;
}

data::Pattern& data::Pattern::operator+= (const Feature& val)
{
  gsl_vector_add_constant(m_vector, val);
  return *this;
}

data::Pattern& data::Pattern::operator-= (const Feature& val)
{
  gsl_vector_add_constant(m_vector, -val);
  return *this;
}

data::Pattern& data::Pattern::operator*= (const Feature& val)
{
  gsl_vector_scale(m_vector, val);
  return *this;
}

data::Pattern& data::Pattern::operator/= (const Feature& val)
{
  gsl_vector_scale(m_vector, 1/val);
  return *this;
}

/**
 * @todo No checking is done if m_feat is the same size of
 * other.m_feat. That means that the result is undefined if the two
 * Pattern's have different sizes. You have to fix that at some
 * point.
 */
data::Pattern& data::Pattern::operator+= (const data::Pattern& other)
{
  gsl_vector_add(m_vector, other.m_vector);
  return *this;
}

data::Pattern& data::Pattern::operator-= (const data::Pattern& other)
{
  gsl_vector_sub(m_vector, other.m_vector);
  return *this;
}

data::Pattern& data::Pattern::operator*= (const data::Pattern& other)
{
  gsl_vector_mul(m_vector, other.m_vector);
  return *this;
}

data::Pattern& data::Pattern::operator/= (const data::Pattern& other)
{
  gsl_vector_div(m_vector, other.m_vector);
  return *this;
}

data::Feature& data::Pattern::operator[] (const size_t& pos)
{
  if (pos >= size()) {
    RINGER_DEBUG1("Detected out-of-range for pos = " << pos << "."
		<< " An exception is thrown.");
    throw RINGER_EXCEPTION("Invalid range");
  }
  return *(gsl_vector_ptr(m_vector, pos));
}

const data::Feature& data::Pattern::operator[] (const size_t& pos) const
{
  if (pos >= size()) {
    RINGER_DEBUG1("Detected out-of-range for pos = " << pos << "."
		<< " An exception is thrown.");
    throw RINGER_EXCEPTION("Invalid range");
  }
  return *(gsl_vector_ptr(m_vector, pos));
}

data::Pattern::Pattern (gsl_vector_view vector_view)
  : m_view(vector_view),
    m_vector(&m_view.vector)
{
  RINGER_DEBUG3("Built Pattern out of GSL vector (@" << m_vector
	      << ") with size = " << m_vector->size
	      << " and stride = " << m_vector->stride
	      << ". The owner ship flag is " << m_vector->owner << ".");
}

std::ostream& operator<< (std::ostream& os, const data::Pattern& p)
{
  os << "(";
  for (unsigned int i=0; i<(p.size()-1); ++i) 
    os << p[i] << " ";
  os << p[p.size()-1] << ")";
  return os;
}

void data::Pattern::append (const Pattern& other)
{
  RINGER_DEBUG3("Appending contents to a Pattern.");
  if (m_vector && m_vector->owner) {
    gsl_vector* tmp = m_vector;
    m_vector = gsl_vector_alloc(tmp->size + other.size());
    gsl_vector_view view = gsl_vector_subvector(m_vector, 0, tmp->size);
    gsl_vector_memcpy(&(view.vector), tmp);
    view = gsl_vector_subvector(m_vector, tmp->size, other.size());
    gsl_vector_free(tmp);
    gsl_vector_memcpy(&(view.vector), other.m_vector);
    return;
  }
  RINGER_DEBUG1("I don't own the vector I have, I cannot append.");
}


sys::Plain& operator<< (sys::Plain& f, const data::Pattern& p)
{
  std::string t = "";
  t += f.separator();
  for (unsigned int i=0; i<(p.size()-1); ++i) f << p[i] << t;
  f << p[p.size()-1];
  return f;
}


