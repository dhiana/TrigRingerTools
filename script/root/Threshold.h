#ifndef THRESHOLD_H
#define THRESHOLD_H

#include <map>
#include "TString.h"

// Represents a LVL1 THRESHOLD
// Energy values in MeV
class Threshold {
public:
  float m_EmCore;
  float m_EmIsolation;
  float m_HdCore;
  float m_HdIsolation;

  Threshold(const float EmCore = 0.f,
	    const float EmIsolation = 999999.f,
	    const float HdCore = 999999.f,
	    const float HdIsolation = 999999.f)
    : m_EmCore(EmCore),
      m_EmIsolation(EmIsolation),
      m_HdCore(HdCore),
      m_HdIsolation(HdIsolation)
  {
  }
  
  Threshold(const Threshold &m) {
    m_EmCore = m.m_EmCore;
    m_EmIsolation = m.m_EmIsolation;
    m_HdCore = m.m_HdCore;
    m_HdIsolation = m.m_HdIsolation;
  }

  bool operator ==(const Threshold &t) const {
    return ( (m_EmCore == t.m_EmCore) &&
	     (m_EmIsolation == t.m_EmIsolation) &&
	     (m_HdCore == t.m_HdCore) &&
	     (m_HdIsolation == t.m_HdIsolation) );
  }

  bool cut(const float EmCore, const float EmIsolation, const float HdCore, const float HdIsolation) const {
    bool passed = false;
    if (EmCore > m_EmCore) {
      if (EmIsolation <= m_EmIsolation) {
	if (HdCore <= m_HdCore) {
	  if (HdIsolation <= m_HdIsolation) {
	    passed = true;
	  }
	}
      }
    }
    return passed;
  }

  friend ostream &operator <<(ostream &o, const Threshold &m) {
    o << "EmCore:       " << m.m_EmCore << " MeV" << endl
      << "EmIsolation:  " << m.m_EmIsolation << " MeV" << endl
      << "HdCore:       " << m.m_HdCore << " MeV" << endl
      << "HdIsolation:  " << m.m_HdIsolation << " MeV";
    return o;
  }

};

class ThresholdMapLess {
 public:
  bool operator ()(const TString &A, const TString &B) {
    if (A.CompareTo(B) < 0)
      return true;
    return false;
  }
};


typedef std::map<TString, Threshold, ThresholdMapLess> ThresholdMap;

const float GeV = 1000.f;
ThresholdMap thresholdMap;

void loadThresholdMap() {
  thresholdMap["EM7"]  = Threshold(7*GeV, 999*GeV, 999*GeV, 999*GeV);
  thresholdMap["EM11"]  = Threshold(11*GeV, 999*GeV, 999*GeV, 999*GeV);
  thresholdMap["EM11I"] = Threshold(11*GeV, 3*GeV, 2*GeV, 1*GeV);
  thresholdMap["EM18"]  = Threshold(18*GeV, 999*GeV, 999*GeV, 999*GeV);
  thresholdMap["EM18I"] = Threshold(18*GeV, 3*GeV, 2*GeV, 1*GeV);
  thresholdMap["EM50"]  = Threshold(50*GeV, 999*GeV, 999*GeV, 999*GeV);
}

#endif

