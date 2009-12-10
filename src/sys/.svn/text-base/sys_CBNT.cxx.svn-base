//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/src/CBNT.cxx
 * @author <a href="mailto:Danilo.Enoque.Ferreira.de.Lima@cern.ch">Danilo Enoque Ferreira de Lima</a> 
 * $Author: dferreir $
 * $Revision: 1.2 $
 * $Date: 2008-06-04 14:07:23 $
 *
 * Implements a CBNT Root file.
 */

#include <iomanip>
#include "TrigRingerTools/sys/CBNT.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TTree.h"
#include "TObject.h"
#include "TFile.h"
#include "TrigRingerTools/roiformat/Cell.h"
#include "TrigRingerTools/roiformat/RoI.h"
#include <vector>

sys::CBNT::CBNT (const std::string& filename, 
		       std::ios_base::openmode m,
		 const std::string& treename)
  : m_filename(filename),
    m_treename(treename),
    m_entry(0)
{
  m_openMode = "READ";
  if (m & std::ios_base::app)
    m_openMode = "UPDATE";
  if (m & std::ios_base::ate)
    m_openMode = "UPDATE";
  if (m & std::ios_base::in)
    m_openMode = "READ";
  if (m & std::ios_base::out)
    m_openMode = "UPDATE";
  if (m & std::ios_base::trunc)
    m_openMode = "RECREATE";

  m_file = new TFile(filename.c_str(), m_openMode.c_str());
  if (m_file->IsZombie()) {
    RINGER_DEBUG1("Couldn't open file \"" << m_filename << "\". Exception thrown.");
    throw RINGER_EXCEPTION("Cannot open file.");
  }
  if ( (m_openMode == "READ") || (m_openMode == "UPDATE") ) {
    m_tree = dynamic_cast<TTree *>(m_file->Get(m_treename.c_str()));
    if (!m_tree) {
      RINGER_DEBUG1("Couldn't find TTree \"" << m_treename << "\" on file \"" << m_filename << "\". Exception thrown.");
      throw RINGER_EXCEPTION("Cannot find TTree on Root file");
    }
    Ringer_Rings = 0;
    Ringer_DetCells = 0;
    Ringer_EtaCells = 0;
    Ringer_PhiCells = 0;
    Ringer_EtaResCells = 0;
    Ringer_PhiResCells = 0;
    Ringer_ECells = 0;
    Ringer_NCells = 0;
    Ringer_Roi_Id = 0;
    Ringer_LVL1_Id = 0;
    Ringer_LVL1_Eta = 0;
    Ringer_LVL1_Phi = 0;
    Ringer_LVL2_Eta = 0;
    Ringer_LVL2_Phi = 0;
    Ringer_LVL2_Et = 0;
    // SetBranchAddress
    m_tree->SetBranchAddress("Ringer_NClusters", &Ringer_NClusters);
    m_tree->SetBranchAddress("Ringer_Rings", &Ringer_Rings);
    m_tree->SetBranchAddress("Ringer_DetCells", &Ringer_DetCells);
    m_tree->SetBranchAddress("Ringer_EtaCells", &Ringer_EtaCells);
    m_tree->SetBranchAddress("Ringer_PhiCells", &Ringer_PhiCells);
    m_tree->SetBranchAddress("Ringer_EtaResCells", &Ringer_EtaResCells);
    m_tree->SetBranchAddress("Ringer_PhiResCells", &Ringer_PhiResCells);
    m_tree->SetBranchAddress("Ringer_ECells", &Ringer_ECells);
    m_tree->SetBranchAddress("Ringer_NCells", &Ringer_NCells);
    m_tree->SetBranchAddress("Ringer_Roi_Id", &Ringer_Roi_Id);
    m_tree->SetBranchAddress("Ringer_LVL1_Id", &Ringer_LVL1_Id);
    m_tree->SetBranchAddress("Ringer_LVL1_Eta", &Ringer_LVL1_Eta);
    m_tree->SetBranchAddress("Ringer_LVL1_Phi", &Ringer_LVL1_Phi);
    m_tree->SetBranchAddress("Ringer_LVL2_Eta", &Ringer_LVL2_Eta);
    m_tree->SetBranchAddress("Ringer_LVL2_Phi", &Ringer_LVL2_Phi);
    m_tree->SetBranchAddress("Ringer_LVL2_Et", &Ringer_LVL2_Et);

    m_tree->SetBranchStatus("*", 0);
    
    m_tree->SetBranchStatus("Ringer_LVL1_Id",1);
    m_tree->SetBranchStatus("Ringer_NCells",1);
    m_tree->SetBranchStatus("Ringer_NClusters",1);
    m_tree->SetBranchStatus("Ringer_Roi_Id",1);
    m_tree->SetBranchStatus("Ringer_LVL1_Eta",1);
    m_tree->SetBranchStatus("Ringer_LVL1_Phi",1);
    m_tree->SetBranchStatus("Ringer_DetCells",1);
    m_tree->SetBranchStatus("Ringer_PhiResCells",1);
    m_tree->SetBranchStatus("Ringer_EtaResCells",1);
    m_tree->SetBranchStatus("Ringer_EtaCells",1);
    m_tree->SetBranchStatus("Ringer_PhiCells",1);
    m_tree->SetBranchStatus("Ringer_ECells",1);
    m_tree->SetBranchStatus("Ringer_LVL2_Et",1);
    m_tree->SetBranchStatus("Ringer_LVL2_Eta",1);
    m_tree->SetBranchStatus("Ringer_LVL2_Phi",1);
    m_tree->SetBranchStatus("Ringer_Rings",1);

  } else {
    m_tree = new TTree(treename.c_str(), treename.c_str());
    // Add branches
    Ringer_NClusters = 0;
    Ringer_Rings = 0;
    Ringer_DetCells = 0;
    Ringer_EtaCells = 0;
    Ringer_PhiCells = 0;
    Ringer_EtaResCells = 0;
    Ringer_PhiResCells = 0;
    Ringer_ECells = 0;
    Ringer_NCells = 0;
    Ringer_Roi_Id = 0;
    Ringer_LVL1_Id = 0;
    Ringer_LVL1_Eta = 0;
    Ringer_LVL1_Phi = 0;
    Ringer_LVL2_Eta = 0;
    Ringer_LVL2_Phi = 0;
    Ringer_LVL2_Et = 0;
    
    // Create branches
    m_tree->Branch("Ringer_NClusters", &Ringer_NClusters, "Ringer_NClusters/i");
    m_tree->Branch("Ringer_Rings", &Ringer_Rings);
    m_tree->Branch("Ringer_DetCells", &Ringer_DetCells);
    m_tree->Branch("Ringer_EtaCells", &Ringer_EtaCells);
    m_tree->Branch("Ringer_PhiCells", &Ringer_PhiCells);
    m_tree->Branch("Ringer_EtaResCells", &Ringer_EtaResCells);
    m_tree->Branch("Ringer_PhiResCells", &Ringer_PhiResCells);
    m_tree->Branch("Ringer_ECells", &Ringer_ECells);
    m_tree->Branch("Ringer_NCells", &Ringer_NCells);
    m_tree->Branch("Ringer_Roi_Id", &Ringer_Roi_Id);
    m_tree->Branch("Ringer_LVL1_Id", &Ringer_LVL1_Id);
    m_tree->Branch("Ringer_LVL1_Eta", &Ringer_LVL1_Eta);
    m_tree->Branch("Ringer_LVL1_Phi", &Ringer_LVL1_Phi);
    m_tree->Branch("Ringer_LVL2_Eta", &Ringer_LVL2_Eta);
    m_tree->Branch("Ringer_LVL2_Phi", &Ringer_LVL2_Phi);
    m_tree->Branch("Ringer_LVL2_Et", &Ringer_LVL2_Et);

    m_tree->SetBranchStatus("*", 0);
    m_tree->SetBranchStatus("Ringer_LVL1_Id",1);
    m_tree->SetBranchStatus("Ringer_NCells",1);
    m_tree->SetBranchStatus("Ringer_NClusters",1);
    m_tree->SetBranchStatus("Ringer_Roi_Id",1);
    m_tree->SetBranchStatus("Ringer_LVL1_Eta",1);
    m_tree->SetBranchStatus("Ringer_LVL1_Phi",1);
    m_tree->SetBranchStatus("Ringer_DetCells",1);
    m_tree->SetBranchStatus("Ringer_PhiResCells",1);
    m_tree->SetBranchStatus("Ringer_EtaResCells",1);
    m_tree->SetBranchStatus("Ringer_EtaCells",1);
    m_tree->SetBranchStatus("Ringer_PhiCells",1);
    m_tree->SetBranchStatus("Ringer_ECells",1);
    m_tree->SetBranchStatus("Ringer_LVL2_Et",1);
    m_tree->SetBranchStatus("Ringer_LVL2_Eta",1);
    m_tree->SetBranchStatus("Ringer_LVL2_Phi",1);
    m_tree->SetBranchStatus("Ringer_Rings",1);
  }
  m_tree->SetCacheSize(0);
  if ( (m_openMode == "UPDATE") && ( (m & std::ios_base::app) || (m & std::ios_base::ate)) ) {
    m_entry = m_tree->GetEntries() - 1;
  } else if (m_openMode == "UPDATE") {
    m_entry = 0;
  }
  if (m_openMode == "READ") {
    m_entry = -1;
  }
  if (m_openMode == "RECREATE") {
    m_entry = 0;
  }
  m_cell = 0;
  m_cluster = 0;
  RINGER_DEBUG3("CBNT file \"" << m_filename << "\" opened successfuly.");
}

sys::CBNT::~CBNT () {
  if (m_openMode != "READ") {
    m_tree->Write(m_treename.c_str(), TObject::kOverwrite);
  } else {
    m_tree = 0;
  }
  if (m_file) {
    m_file->Close();
    delete m_file;
  }
  m_file = 0;
  // TODO Should m_tree be deleted or does ROOT do this?!?
  RINGER_DEBUG3("File \"" << m_filename << "\" closed successfuly.");
}

sys::FileImplementation& sys::CBNT::writeEvent() {
  if ( (m_openMode == "UPDATE") || (m_openMode == "RECREATE") ) {
    m_tree->Fill();
    Ringer_NClusters = 0;
    m_cluster = 0;
    m_cell = 0;
    Ringer_NCells->clear();
    Ringer_LVL1_Id->clear();
    Ringer_Roi_Id->clear();
    Ringer_LVL1_Eta->clear();
    Ringer_LVL1_Phi->clear();
    Ringer_DetCells->clear();
    Ringer_PhiResCells->clear();
    Ringer_EtaResCells->clear();
    Ringer_EtaCells->clear();
    Ringer_PhiCells->clear();
    Ringer_ECells->clear();
    Ringer_LVL2_Et->clear();
    Ringer_LVL2_Eta->clear();
    Ringer_LVL2_Phi->clear();
    Ringer_Rings->clear();
  }
  return *this;
}

sys::FileImplementation &sys::CBNT::operator<< (const roiformat::RoI &roi) {
  roi.updateSamp();
  Ringer_Roi_Id->push_back(roi.m_roi_id);
  Ringer_LVL1_Eta->push_back(roi.m_eta);
  Ringer_LVL1_Phi->push_back(roi.m_phi);
  Ringer_LVL1_Id->push_back(roi.m_lvl1_id);
  typedef std::vector<roiformat::Cell> vec_type;
  for (vec_type::const_iterator it = roi.m_cells.begin(); it != roi.m_cells.end(); ++it) {
    *this << (*it);
  }
  Ringer_NCells->push_back(static_cast<unsigned int>(roi.m_cells.size()));
  Ringer_NClusters++;
  return *this;
}

sys::FileImplementation &sys::CBNT::operator<< (const roiformat::Cell &cell) {
  Ringer_DetCells->push_back(static_cast<unsigned char>(cell.sampling()));
  Ringer_EtaCells->push_back(static_cast<float>(cell.eta()));
  Ringer_PhiCells->push_back(static_cast<float>(cell.phi()));
  Ringer_EtaResCells->push_back(static_cast<float>(cell.deta()));
  Ringer_PhiResCells->push_back(static_cast<float>(cell.dphi()));
  Ringer_ECells->push_back(static_cast<float>(cell.energy()));
  return *this;
}

sys::FileImplementation &sys::CBNT::operator>> (roiformat::RoI &roi) {
  if (!good())
    return *this;
    
  roi.m_cells.erase(roi.m_cells.begin(), roi.m_cells.end());

  if (m_entry == -1) { // First time!
    m_cluster = 0;
    m_cell = 0;
    do {
      m_entry++;
      if (m_entry < m_tree->GetEntries()) {
        m_tree->GetEntry(m_entry);
      } else {
        // We've been through the whole file, and no good clusters have been found!
	throw RINGER_EXCEPTION("No clusters on this CBNT!");
      }
    } while (Ringer_NClusters == 0);
  }
  
  RINGER_DEBUG3("sys::CBNT::operator>>(roiformat::RoI &) is reading cluster " << m_cluster << " at event " << m_entry);
  roi.m_roi_id = Ringer_Roi_Id->at(m_cluster);
  roi.m_eta = Ringer_LVL1_Eta->at(m_cluster);
  roi.m_phi = Ringer_LVL1_Phi->at(m_cluster);
  roi.m_lvl1_id = Ringer_LVL1_Id->at(m_cluster);
  typedef std::vector<roiformat::Cell> vec_type;

  unsigned int maxCell = Ringer_NCells->at(m_cluster)+m_cell;
  while (m_cell < maxCell) {
    roiformat::Cell c;
    *this >> c;
    roi.m_cells.push_back(c);
  }

  // Get next ROI
  roi.updateSamp();
  
  m_cluster++;
  while (m_cluster >= Ringer_NClusters) { // If we went through all clusters, get a new event
    m_cluster = 0;
    m_entry++;
    if (m_entry < m_tree->GetEntries()) {
      m_tree->GetEntry(m_entry);
      m_cell = 0;
    } else {
      // EOF
      // eof() should return true and next time this won't be run
      m_cluster = 0;
      break;
    }
  }
  return *this;
}

sys::FileImplementation &sys::CBNT::operator>> (roiformat::Cell &cell) {
  cell.m_samp = (roiformat::Cell::Sampling) Ringer_DetCells->at(m_cell);
  cell.m_eta = Ringer_EtaCells->at(m_cell);
  cell.m_phi = Ringer_PhiCells->at(m_cell);
  cell.m_deta = Ringer_EtaResCells->at(m_cell);
  cell.m_dphi = Ringer_PhiResCells->at(m_cell);
  cell.m_energy = Ringer_ECells->at(m_cell);
  ++m_cell;
  return *this;
}

bool sys::CBNT::eof (void) const
{
  return (m_entry == m_tree->GetEntries());
}

bool sys::CBNT::good (void) const
{
  return !eof();
}

bool sys::CBNT::readmore (void) const
{
  return good();
}

bool sys::CBNT::is_open (void) {
  return true;
}

void sys::CBNT::close () {
}

