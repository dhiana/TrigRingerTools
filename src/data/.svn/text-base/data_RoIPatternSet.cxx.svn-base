//Dear emacs, this is -*- c++ -*-

/**
 * @file RoIPatternSet.cxx
 *
 * Implements a PatternSet in which data::Pattern's are associated to RoI
 * attributes.
 */

#include "TrigRingerTools/data/RoIPatternSet.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/xmlutil.h"
#include "TrigRingerTools/data/RandomInteger.h"
#include "TFile.h"
#include "TTree.h"
#include "TrigRingerTools/sys/Plain.h"

data::RoIPatternSet::RoIPatternSet(const size_t& size,
				   const size_t& p_size,
				   const double& init)
  : m_set(size, p_size, init),
    m_attr(size)
{
  RINGER_DEBUG1("Created RoIPatternSet with size=" << size 
		<< " and pattern" << " size=" << p_size 
		<< ", initializer is " << init);
}

data::RoIPatternSet::RoIPatternSet(const data::RoIPatternSet& other)
  : PatternSet(),
    m_set(other.m_set),
    m_attr(other.m_attr)
{
  RINGER_DEBUG1("Created RoIPatternSet by copying");
}

data::RoIPatternSet::RoIPatternSet(sys::xml_ptr_const node)
  : m_set(1, 1),
    m_attr(0)
{
  std::vector<Pattern*> data;
  //for all entries in a class
  for (sys::xml_ptr_const it = sys::get_first_child(node); 
       it; it=sys::get_next_element(it)) {
    if (!sys::is_element(it)) continue;
    std::vector<double> curr;
    RoIAttribute attr;
    attr.lvl1_id = sys::get_attribute_uint(it, "lvl1_id");
    attr.roi_id = sys::get_attribute_uint(it, "roi_id");
    attr.eta = sys::get_attribute_double(it, "eta");
    attr.phi = sys::get_attribute_double(it, "phi");
    sys::xml_ptr_const feats = sys::get_first_child(it);
    if (!sys::is_element(feats)) feats = sys::get_next_element(feats);
    sys::get_element_doubles(feats, curr);
    data.push_back(new data::Pattern(curr));
    m_attr.push_back(attr);
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
  m_set = data::SimplePatternSet(data); //single-shot update
  for (size_t i=0; i<data.size(); ++i) delete data[i];
}

data::RoIPatternSet::RoIPatternSet (const data::RootClassInfo &infoBranches)
 : m_set(1, 1),
   m_attr(0)
{  
  RINGER_DEBUG3("data::RoIPatternSet::RoIPatternSet(const data::RootClassInfo &)");
  TFile f(infoBranches.FileName.c_str());
  if (f.IsZombie()) {
    RINGER_DEBUG1("Couldn't open file " << infoBranches.FileName.c_str() << "! Maybe a wrong path has been supplied?");
    throw RINGER_EXCEPTION("Can't open ROOT file");
  }
  
  TTree *tree = dynamic_cast<TTree *>(f.Get(infoBranches.TreeName.c_str()));
  if (!tree) {
    RINGER_DEBUG1("Error! Couldn't find TTree " << infoBranches.TreeName.c_str());
    throw RINGER_EXCEPTION("TTree not found");
  }
  
  std::vector<Pattern*> data;
  //for all entries in a class

  std::vector<Feature> curr;
  std::vector<Float_t> *branchData = 0;
  std::vector<unsigned int> *branchLvl1Id = 0;
  std::vector<unsigned int> *branchRoiId = 0;
  std::vector<Float_t> *branchEta = 0;
  std::vector<Float_t> *branchPhi = 0;
  
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("Ringer_Rings", 1);
  tree->SetBranchStatus("Ringer_LVL1_Id", 1);
  tree->SetBranchStatus("Ringer_Roi_Id", 1);
  tree->SetBranchStatus("Ringer_LVL2_Eta", 1);
  tree->SetBranchStatus("Ringer_LVL2_Phi", 1);
  
  tree->SetBranchAddress("Ringer_Rings", &branchData);
  tree->SetBranchAddress("Ringer_LVL1_Id", &branchLvl1Id);
  tree->SetBranchAddress("Ringer_Roi_Id", &branchRoiId);
  tree->SetBranchAddress("Ringer_LVL2_Eta", &branchEta);
  tree->SetBranchAddress("Ringer_LVL2_Phi", &branchPhi);
  
  // TODO: number of cluster information is not being used!
  for (unsigned int i = 0; i < tree->GetEntries(); i++) {
    tree->GetEntry(i);
    RINGER_DEBUG3("Data size == "<<branchData->size());
    RINGER_DEBUG3("LVL1ID size == "<<branchLvl1Id->size());
    RINGER_DEBUG3("ROIID size == "<<branchRoiId->size());
    RINGER_DEBUG3("Eta size == "<<branchEta->size());
    RINGER_DEBUG3("Phi size == "<<branchPhi->size());
    unsigned int dataCounter = 0;
    for (unsigned int cluster = 0; cluster < branchLvl1Id->size(); cluster++) {
      RoIAttribute attr;
      attr.lvl1_id = branchLvl1Id->at(cluster);
      attr.roi_id = branchRoiId->at(cluster);
      attr.eta = branchEta->at(cluster);
      attr.phi = branchPhi->at(cluster);
      m_attr.push_back(attr);
      
      // Conversion from std::vector<float> to std::vector<double> (Feature == double)
      curr.clear();
      unsigned int dataEnd = dataCounter + m_set.pattern_size();
      for (; dataCounter != dataEnd; dataCounter++) {
        curr.push_back((Feature) branchData->at(dataCounter));
      }
      
      data.push_back(new data::Pattern(curr));
    }
  }
  f.Close();

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
  RINGER_DEBUG2("Building SimplePatternSet from ROOT data.");
  m_set = data::SimplePatternSet(data); //single-shot update
  for (size_t i=0; i<data.size(); ++i) delete data[i];
}

data::RoIPatternSet::RoIPatternSet
(const data::RoIPatternSet& other, const std::vector<size_t>& pats)
  : m_set(other.m_set, pats),
    m_attr(pats.size())
{
  RINGER_DEBUG1("Building RoIPatternSet from partial copy...");
  for (size_t i=0; i<pats.size(); ++i) m_attr[i] = other.m_attr[pats[i]];
  RINGER_DEBUG1("Building of RoIPatternSet from partial copy DONE!");
}

data::RoIPatternSet::RoIPatternSet(const data::RoIPatternSet& other, 
				   double start, double end,
				   variable_t var)
  : m_set(1, 1),
    m_attr()
{
  RINGER_DEBUG1("Building RoIPatternSet from partial interval copy...");
  std::vector<size_t> pats;
  for (size_t i=0; i<other.size(); ++i) {
    double ref = 0;
    switch(var) {
    case ETA:
      ref = other.m_attr[i].eta;
      break;
    case PHI:
      ref = other.m_attr[i].phi;
      break;
    }
    if (ref >= start && ref < end) pats.push_back(i);
  }
  this->assign(other, pats);
  RINGER_DEBUG1("Building of RoIPatternSet from partial interval copy DONE!");
}

data::RoIPatternSet::RoIPatternSet
(const std::vector<Pattern*>& pats,
 const std::vector<data::RoIPatternSet::RoIAttribute>& attr)
  : m_set(pats),
    m_attr(attr)
{
  if (pats.size() != attr.size()) {
    RINGER_DEBUG1("Patterns (" << pats.size() << ") and attributes (" 
		  << attr.size() << ") have different sizes. Exception!");
    throw RINGER_EXCEPTION
      ("Patterns and attributes in c'tor cannot differ in size.");
  }
  RINGER_DEBUG1("Built RoIPatternSet with Patterns and RoIAttributes...");
}

data::RoIPatternSet::RoIPatternSet
(const data::SimplePatternSet& pset,
 const std::vector<data::RoIPatternSet::RoIAttribute>& attr)
  : m_set(pset),
    m_attr(attr)
{
  if (pset.size() != attr.size()) {
    RINGER_DEBUG1("SimplePatternSet (" << pset.size() << ") and attributes (" 
		  << attr.size() << ") have different sizes. Exception!");
    throw RINGER_EXCEPTION
      ("SimplePatternSet and attributes in c'tor cannot differ in size.");
  }
  RINGER_DEBUG1("Built RoIPatternSet with SimplePatternSet and attributes.");
}

data::RoIPatternSet::~RoIPatternSet() 
{
  RINGER_DEBUG1("Destroying RoIPatternSet...");
}

size_t data::RoIPatternSet::size () const
{
  return m_set.size();
}

size_t data::RoIPatternSet::pattern_size () const
{
  return m_set.pattern_size();
}

const data::Pattern data::RoIPatternSet::pattern (const size_t& pos) const
{
  return m_set.pattern(pos);
}

const data::Ensemble data::RoIPatternSet::ensemble (const size_t& pos) const
{
  return m_set.ensemble(pos);
}

void data::RoIPatternSet::set_pattern (const size_t& pos, const Pattern& pat)
{
  m_set.set_pattern(pos, pat);
}

void data::RoIPatternSet::set_ensemble (const size_t& pos, const Ensemble& ens)
{
  m_set.set_ensemble(pos, ens);
}

void data::RoIPatternSet::erase_pattern (const size_t& pos)
{
  m_set.erase_pattern(pos);
  m_attr.erase(m_attr.begin() + pos);
}

void data::RoIPatternSet::erase_ensemble (const size_t& pos)
{
  m_set.erase_ensemble(pos);
}

data::PatternSet* data::RoIPatternSet::clone (void) const
{
  return new data::RoIPatternSet(*this);
}

data::PatternSet* data::RoIPatternSet::clone 
(const std::vector<size_t>& pats) const
{
  return new data::RoIPatternSet(*this, pats);
}

void data::RoIPatternSet::shuffle (void)
{
  static data::RandomInteger rnd;
  std::vector<size_t> pos(size());
  rnd.draw(size(), pos);
  data::RoIPatternSet new_order(*this, pos);
  *this = new_order;
}

sys::xml_ptr data::RoIPatternSet::dump (sys::xml_ptr any,
					const std::string& cname,
					const size_t start_id) const
{
  sys::xml_ptr node = sys::make_node(any, "class");
  sys::put_attribute_text(node, "name", cname);
  for (size_t i=0; i<size(); ++i) {
    RINGER_DEBUG3("XML'ing pattern[" << i << "] of class \"" << cname 
		  << "\". The accumulated identifier is [" << start_id+i 
		  << "].");
    sys::xml_ptr entry = sys::put_element(node, "roientry");
    sys::put_attribute_uint(entry, "id", start_id + i);
    sys::put_attribute_uint(entry, "lvl1_id", m_attr[i].lvl1_id);
    sys::put_attribute_uint(entry, "roi_id", m_attr[i].roi_id);
    sys::put_attribute_double(entry, "eta", m_attr[i].eta);
    sys::put_attribute_double(entry, "phi", m_attr[i].phi);
    const Pattern pat = pattern(i);
    std::vector<data::Feature> val(pat.size());
    for (size_t j=0; j<pat.size(); ++j) val[j] = pat[j];
    sys::put_element_doubles(entry, "feature", val);
  }
  return node;
}

void data::RoIPatternSet::dump (const RootClassInfo &info) const {
  RINGER_DEBUG3("data::RoIPatternSet::dump(const data::RootClassInfo &) const");
  
  TFile f(info.FileName.c_str(), "RECREATE");
  
  if (f.IsZombie()) {
    RINGER_DEBUG1("Couldn't open file " << info.FileName.c_str() << "! Maybe a wrong path has been supplied?");
    throw RINGER_EXCEPTION("Can't open ROOT file");
  }
  
  TTree *tree = new TTree(info.TreeName.c_str(), info.TreeName.c_str());
  
  std::vector<Pattern*> data;
  //for all entries in a class

  UInt_t branchNClusters = 0;
  std::vector<Float_t> *branchData = 0;
  std::vector<unsigned int> *branchLvl1Id = 0;
  std::vector<unsigned int> *branchRoiId = 0;
  std::vector<Float_t> *branchEta = 0;
  std::vector<Float_t> *branchPhi = 0;
 
  tree->Branch("Ringer_NClusters", &branchNClusters, "Ringer_NClusters/i");
  tree->Branch("Ringer_Rings", &branchData);
  tree->Branch("Ringer_LVL1_Id", &branchLvl1Id);
  tree->Branch("Ringer_Roi_Id", &branchRoiId);
  tree->Branch("Ringer_LVL2_Eta", &branchEta);
  tree->Branch("Ringer_LVL2_Phi", &branchPhi);
  
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("Ringer_NClusters", 1);
  tree->SetBranchStatus("Ringer_Rings", 1);
  tree->SetBranchStatus("Ringer_LVL1_Id", 1);
  tree->SetBranchStatus("Ringer_Roi_Id", 1);
  tree->SetBranchStatus("Ringer_LVL2_Eta", 1);
  tree->SetBranchStatus("Ringer_LVL2_Phi", 1);
  
  branchData->clear();
  branchLvl1Id->clear();
  branchRoiId->clear();
  branchEta->clear();
  branchPhi->clear();

  unsigned int lastL1Id = m_attr[0].lvl1_id;
  
  // TODO: number of cluster information is not being used!
  for (Long64_t i = 0; i < size(); ++i) {
    // If this RoI has the same LVL1 Id as the previous,
    // put it in the same cluster
    if (i != 0) {
      if (m_attr[i].lvl1_id != lastL1Id) {
        branchNClusters++;
        tree->Fill();
        branchNClusters = 0;
        branchLvl1Id->clear();
        branchRoiId->clear();
        branchEta->clear();
        branchPhi->clear();
        branchData->clear();
        lastL1Id = m_attr[i].lvl1_id;
      } else {
        branchNClusters++;
      }
    }
   
    branchLvl1Id->push_back(m_attr[i].lvl1_id);
    branchRoiId->push_back(m_attr[i].roi_id);
    branchEta->push_back(m_attr[i].eta);
    branchPhi->push_back(m_attr[i].phi);
    
    const Pattern pat = pattern(i);
    for (unsigned int patCounter = 0; patCounter < m_set.pattern_size(); ++patCounter) {
      branchData->push_back((float) pat[patCounter]);
    }
  }

  branchNClusters++;
  tree->Fill();

  tree->Write(info.TreeName.c_str(), TObject::kOverwrite);
  f.Close();

  RINGER_DEBUG2("Built ROOT file from data.");

}

void data::RoIPatternSet::apply_pattern_op (const data::PatternOperator& op)
{
  m_set.apply_pattern_op(op);
}

void data::RoIPatternSet::apply_ensemble_op (const data::PatternOperator& op)
{
  m_set.apply_ensemble_op(op);
}

std::ostream& data::RoIPatternSet::stream_out (std::ostream& os) const
{
  unsigned int i;
  for (i=0; i<size()-1; ++i)
    os << "[" << i << "] " << attribute(i) << " " << pattern(i) << "\n";
  os << "[" << i << "] " << attribute(i) << " " << pattern(i);
  return os;
}
	
sys::Plain& data::RoIPatternSet::stream_out (sys::Plain& f) const
{
  unsigned int i;
  for (i=0; i<size()-1; ++i) 
    f << attribute(i) << " " << pattern(i) << "\n";
  f << attribute(i) << " " << pattern(i);
  return f;
}


const data::RoIPatternSet::RoIAttribute& data::RoIPatternSet::attribute 
(const size_t& pos) const {
  if (size() <= pos) {
    RINGER_DEBUG1("Trying to get attribute @" << pos 
		  << " but this set has only " << size() 
		  << " patterns. Exception thrown.");
    throw RINGER_EXCEPTION("Unexisting RoI pattern.");
  }
  return m_attr[pos];
}

void data::RoIPatternSet::set_attribute 
(const size_t& pos, const data::RoIPatternSet::RoIAttribute& attr)
{
  if (size() <= pos) {
    RINGER_DEBUG1("Trying to get attribute @" << pos 
		  << " but this set has only " << size() 
		  << " patterns. Exception thrown.");
    throw RINGER_EXCEPTION("Unexisting RoI pattern.");
  }
  m_attr[pos] = attr;
}

void data::RoIPatternSet::set_attribute 
(const std::vector<data::RoIPatternSet::RoIAttribute>& attr)
{
  if (attr.size() != size()) {
    RINGER_DEBUG1("This set size is " << size() 
		  << ", but you have me " << attr.size() 
		  << " attributes to set. Exception thrown.");
    throw RINGER_EXCEPTION("Different number of attributes to set.");
  }
  for (size_t i = 0; i < size(); ++i) set_attribute(i, attr[i]);
}

void data::RoIPatternSet::set_pattern 
(const size_t& pos, const data::Pattern& pat,
 const data::RoIPatternSet::RoIAttribute& attr)
{
  if (size() <= pos) {
    RINGER_DEBUG1("Trying to get attribute @" << pos 
		  << " but this set has only " << size() 
		  << " patterns. Exception thrown.");
    throw RINGER_EXCEPTION("Unexisting RoI pattern.");
  }
  m_set.set_pattern(pos, pat);
  m_attr[pos] = attr;
}

data::RoIPatternSet& data::RoIPatternSet::merge 
(const data::RoIPatternSet& other)
{
  m_set.merge(other.m_set);
  m_attr.insert(m_attr.end(), other.m_attr.begin(), other.m_attr.end());
  return *this;
}
	
data::RoIPatternSet& data::RoIPatternSet::assign
(const data::RoIPatternSet& other, const std::vector<size_t>& pats)
{
  m_set.assign(other.m_set, pats);
  m_attr.resize(other.m_attr.size());
  for (size_t i=0; i<pats.size(); ++i) m_attr[i] = other.m_attr[pats[i]];
  return *this;
}

data::RoIPatternSet& 
data::RoIPatternSet::operator= (const data::RoIPatternSet& other)
{
  m_set = other.m_set;
  m_attr = other.m_attr;
  return *this;
}

std::ostream& operator<< (std::ostream& os, 
			  const data::RoIPatternSet::RoIAttribute& attr)
{
  os << "(lvl1_id=" << attr.lvl1_id << ",roi_id=" << attr.roi_id
     << ",eta=" << attr.eta << ",phi=" << attr.phi << ")";
  return os;
}

sys::Plain& operator<< (sys::Plain& os, 
		       const data::RoIPatternSet::RoIAttribute& attr)
{
  os << attr.lvl1_id << " " << attr.roi_id << " " << attr.eta << " " 
     << attr.phi;
  return os;
}

