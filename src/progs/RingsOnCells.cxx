#include "RingsOnCells.h"

#include <iostream>
#include <vector>
#include <map>
#include "TROOT.h"
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include <cmath>

#include "TrigRingerTools/rbuild/Config.h"
#include "TrigRingerTools/rbuild/util.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/Reporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/roiformat/Database.h"
#include "TrigRingerTools/data/PatternSet.h"

using namespace std;

const float RingsOnCells::deltaEtaMinimumMatch = 0.2f;
const float RingsOnCells::deltaPhiMinimumMatch = 0.2f;

RingsOnCells::RingsOnCells(TTree *tree, sys::Reporter *reporter, bool global_center, double eta_window, double phi_window) {
  fChain = tree;
  m_reporter = reporter;
  m_global_center = global_center;
  m_eta_window = eta_window;
  m_phi_window = phi_window;
  
  fChain->SetCacheSize(0);
}

void RingsOnCells::init() {
  Ringer_ECells = 0;
  Ringer_EtaCells = 0;
  Ringer_PhiCells = 0;
  Ringer_NCells = 0;
  Ringer_NClusters = 0;
  Ringer_PhiResCells = 0;
  Ringer_EtaResCells = 0;
  Ringer_DetCells = 0;
  Ringer_Roi_Id = 0;
  Ringer_LVL1_Id = 0;
  Ringer_LVL1_Eta = 0;
  Ringer_LVL1_Phi = 0;
  Ringer_LVL2_Eta = 0;
  Ringer_LVL2_Phi = 0;
  Ringer_LVL2_Et = 0;
  Ringer_Rings = 0;
}

void RingsOnCells::createBranches() {
  if (!fChain) return;
  init();

  fChain->Branch("Ringer_NClusters", &Ringer_NClusters, "Ringer_NClusters/i");
  fChain->Branch("Ringer_Rings", &Ringer_Rings);
  fChain->Branch("Ringer_DetCells", &Ringer_DetCells);
  fChain->Branch("Ringer_EtaCells", &Ringer_EtaCells);
  fChain->Branch("Ringer_PhiCells", &Ringer_PhiCells);
  fChain->Branch("Ringer_EtaResCells", &Ringer_EtaResCells);
  fChain->Branch("Ringer_PhiResCells", &Ringer_PhiResCells);
  fChain->Branch("Ringer_ECells", &Ringer_ECells);
  fChain->Branch("Ringer_NCells", &Ringer_NCells);
  fChain->Branch("Ringer_Roi_Id", &Ringer_Roi_Id);
  fChain->Branch("Ringer_LVL1_Id", &Ringer_LVL1_Id);
  fChain->Branch("Ringer_LVL1_Eta", &Ringer_LVL1_Eta);
  fChain->Branch("Ringer_LVL1_Phi", &Ringer_LVL1_Phi);
  fChain->Branch("Ringer_LVL2_Eta", &Ringer_LVL2_Eta);
  fChain->Branch("Ringer_LVL2_Phi", &Ringer_LVL2_Phi);
  fChain->Branch("Ringer_LVL2_Et", &Ringer_LVL2_Et);
}

void RingsOnCells::setBranches() {
  if (!fChain) return;

  init();

  fChain->SetBranchAddress("Ringer_NClusters", &Ringer_NClusters);
  fChain->SetBranchAddress("Ringer_Rings", &Ringer_Rings);
  fChain->SetBranchAddress("Ringer_DetCells", &Ringer_DetCells);
  fChain->SetBranchAddress("Ringer_EtaCells", &Ringer_EtaCells);
  fChain->SetBranchAddress("Ringer_PhiCells", &Ringer_PhiCells);
  fChain->SetBranchAddress("Ringer_EtaResCells", &Ringer_EtaResCells);
  fChain->SetBranchAddress("Ringer_PhiResCells", &Ringer_PhiResCells);
  fChain->SetBranchAddress("Ringer_ECells", &Ringer_ECells);
  fChain->SetBranchAddress("Ringer_NCells", &Ringer_NCells);
  fChain->SetBranchAddress("Ringer_Roi_Id", &Ringer_Roi_Id);
  fChain->SetBranchAddress("Ringer_LVL1_Id", &Ringer_LVL1_Id);
  fChain->SetBranchAddress("Ringer_LVL1_Eta", &Ringer_LVL1_Eta);
  fChain->SetBranchAddress("Ringer_LVL1_Phi", &Ringer_LVL1_Phi);
  fChain->SetBranchAddress("Ringer_LVL2_Eta", &Ringer_LVL2_Eta);
  fChain->SetBranchAddress("Ringer_LVL2_Phi", &Ringer_LVL2_Phi);
  fChain->SetBranchAddress("Ringer_LVL2_Et", &Ringer_LVL2_Et);
}

void RingsOnCells::setStatusOfBranches() {
  if (!fChain) return;
  
  fChain->SetBranchStatus("*", 0);

  fChain->SetBranchStatus("Ringer_NClusters", 1);
  fChain->SetBranchStatus("Ringer_Rings", 1);
  fChain->SetBranchStatus("Ringer_DetCells", 1);
  fChain->SetBranchStatus("Ringer_EtaCells", 1);
  fChain->SetBranchStatus("Ringer_PhiCells", 1);
  fChain->SetBranchStatus("Ringer_EtaResCells", 1);
  fChain->SetBranchStatus("Ringer_PhiResCells", 1);
  fChain->SetBranchStatus("Ringer_ECells", 1);
  fChain->SetBranchStatus("Ringer_NCells", 1);
  fChain->SetBranchStatus("Ringer_Roi_Id", 1);
  fChain->SetBranchStatus("Ringer_LVL1_Id", 1);
  fChain->SetBranchStatus("Ringer_LVL1_Eta", 1);
  fChain->SetBranchStatus("Ringer_LVL1_Phi", 1);
  fChain->SetBranchStatus("Ringer_LVL2_Eta", 1);
  fChain->SetBranchStatus("Ringer_LVL2_Phi", 1);
  fChain->SetBranchStatus("Ringer_LVL2_Et", 1);
}

void RingsOnCells::Fill() {
  fChain->Fill();
}

RingsOnCells *RingsOnCells::run(std::string &ringconfig) {
  if (!fChain)
    return 0;

  TTree *oTree = new TTree("CollectionTree", "CollectionTree");
  RingsOnCells *o = new RingsOnCells(oTree, m_reporter);
  o->createBranches();

  Long64_t nEntries = fChain->GetEntries();

  // Ringer stuff for building rings ..
  // Load configuration
  rbuild::Config config(ringconfig.c_str(), m_reporter);
  std::vector<rbuild::RingSet> rset;
  const std::map<unsigned int, rbuild::RingConfig>& rconfig = config.config();
  unsigned int nrings = 0;
  for (std::map<unsigned int, rbuild::RingConfig>::const_iterator it = rconfig.begin(); it != rconfig.end(); ++it) {
    rset.push_back(it->second);
    nrings += it->second.max();
  } // creates, obligatorily, ordered ring sets

  for (Long64_t i = 0; i < nEntries; ++i) {
    fChain->GetEntry(i);
    Long64_t NCellsBase = 0;
    for (unsigned int j = 0; j < Ringer_NClusters; ++j) {
      o->Ringer_LVL1_Id->push_back(Ringer_LVL1_Id->at(j));
      o->Ringer_Roi_Id->push_back(Ringer_Roi_Id->at(j));
      o->Ringer_LVL1_Eta->push_back(Ringer_LVL1_Eta->at(j));
      o->Ringer_LVL1_Phi->push_back(Ringer_LVL1_Phi->at(j));
      o->Ringer_LVL2_Et->push_back(Ringer_LVL2_Et->at(j));
      o->Ringer_LVL2_Eta->push_back(Ringer_LVL2_Eta->at(j));
      o->Ringer_LVL2_Phi->push_back(Ringer_LVL2_Phi->at(j));

      UInt_t RingsBase = nrings*j;
      for (Long64_t k = RingsBase; k < (RingsBase+nrings); ++k) {
        o->Ringer_Rings->push_back(Ringer_Rings->at(k));
      }

      // Re-build the RoI (slow!!!)
      roiformat::RoI *roi = new roiformat::RoI(Ringer_LVL1_Id->at(j), Ringer_Roi_Id->at(j), Ringer_LVL1_Eta->at(j), Ringer_LVL1_Phi->at(j));
      for (unsigned int k = NCellsBase; k < (NCellsBase + Ringer_NCells->at(j)); ++k) {
         roiformat::Cell c((roiformat::Cell::Sampling) Ringer_DetCells->at(k),
	                   Ringer_EtaCells->at(k), Ringer_PhiCells->at(k), /* Radius ... unknown ... */ 0,
			   Ringer_EtaResCells->at(k), Ringer_PhiResCells->at(k), /* Radius resolution ... unknown ... */ 0,
			   Ringer_ECells->at(k));
         roi->insertCell(c);
      }
      
      // Find center for each RingSet
      // The types on the code below seem clumsy ... TODO: typedefs ....
      std::map<std::vector<rbuild::RingSet>::iterator, std::pair<double, double> > center;
      for (std::vector<rbuild::RingSet>::iterator rit = rset.begin(); rit != rset.end(); ++rit) {
        std::vector<const roiformat::Cell*> cells;
        const std::vector<roiformat::Cell::Sampling>& dets = rit->config().detectors();
        for (std::vector<roiformat::Cell::Sampling>::const_iterator dit = dets.begin(); dit != dets.end(); ++dit) {
	  roi->cells(*dit, cells);
        }
	// cells is filled with all cells in this RingSet's Samplings
	if (!m_global_center) {
	  double my_eta, my_phi;
	  roiformat::max(cells, my_eta, my_phi, Ringer_LVL2_Eta->at(j), Ringer_LVL2_Phi->at(j), m_eta_window, m_phi_window);
	  center.insert( std::pair< std::vector<rbuild::RingSet>::iterator, std::pair<double, double> >( rit, pair<double, double>(my_eta, my_phi) ) );
	}
      }

      delete roi;

      // check wrap around
      const bool wrap = roiformat::check_wrap_around(Ringer_LVL2_Phi->at(j), false);
      const bool reverse_wrap = roiformat::check_wrap_around(Ringer_LVL2_Phi->at(j), true);

      // Now search all cells in this cluster
      o->Ringer_NCells->push_back(0);
      for (unsigned int k = NCellsBase; k < (NCellsBase+Ringer_NCells->at(j)); ++k) {
          unsigned int ringOffset = 0;
          // Check if it is on a RingSet
          for (std::vector<rbuild::RingSet>::iterator rit = rset.begin(); rit != rset.end(); ++rit) {
            const std::vector<roiformat::Cell::Sampling>& dets = rit->config().detectors();
            // this k-th cell must be on one of the detectors of one RingSet ...
            for (std::vector<roiformat::Cell::Sampling>::const_iterator dit = dets.begin(); dit != dets.end(); ++dit) {
              if (((roiformat::Cell::Sampling) Ringer_DetCells->at(k)) == *dit) {
                // This might be the correct RingSet (others RingSets might include this sampling too)

                float phi_use = Ringer_PhiCells->at(k); //use this value for phi (wrap protection)
                if (wrap) phi_use = roiformat::fix_wrap_around(phi_use, false);
                else if (reverse_wrap) phi_use = roiformat::fix_wrap_around(phi_use, true);

                // Get center for this RingSet
		pair<double, double> my_center;
		if (!m_global_center) {
		  my_center = center[rit];
                } else {
		  my_center.first = Ringer_LVL2_Eta->at(j);
		  my_center.second = Ringer_LVL2_Phi->at(j);
		}
		
                // Measure delta eta and delta phi to find out on which ring we are
                unsigned int ring = 0;
                const double deltaEta = (Ringer_EtaCells->at(k) - my_center.first)/rit->config().eta_size();
                const double deltaPhi = (phi_use - my_center.second)/rit->config().phi_size();
                const double deltaGreater = std::max(fabs(deltaEta), fabs(deltaPhi));
                ring = static_cast<unsigned int>( std::floor (deltaGreater) );
                // Certain compilers have problems with round(), so let's do it ourselves
                if ( (deltaGreater - (double) ring) > 0.5) ++ring;

                if (ring < rit->config().max()) {
                  // We ARE on the ring-th ring!!!
                  o->Ringer_ECells->push_back(Ringer_Rings->at(RingsBase+ringOffset+ring));
                  o->Ringer_EtaCells->push_back(Ringer_EtaCells->at(k));
                  o->Ringer_PhiCells->push_back(Ringer_PhiCells->at(k));
                  o->Ringer_DetCells->push_back(Ringer_DetCells->at(k));
                  o->Ringer_EtaResCells->push_back(Ringer_EtaResCells->at(k));
                  o->Ringer_PhiResCells->push_back(Ringer_PhiResCells->at(k));
                  (*(o->Ringer_NCells))[0]++;
                } // if ring is within boundaries
              } // if we are on the same sampling
            } // loop on this RingSet samplings
	    ringOffset += (*rit).config().max();
          } // loop on RingSets
      } // loop on all cells

      o->Ringer_NClusters = 1;
      o->Fill();

      o->Ringer_ECells->clear();
      o->Ringer_EtaCells->clear();
      o->Ringer_PhiCells->clear();
      o->Ringer_NCells->clear();
      //o->Ringer_NClusters = 0;
      o->Ringer_PhiResCells->clear();
      o->Ringer_EtaResCells->clear();
      o->Ringer_DetCells->clear();
      o->Ringer_Roi_Id->clear();
      o->Ringer_LVL1_Id->clear();
      o->Ringer_LVL1_Eta->clear();
      o->Ringer_LVL1_Phi->clear();
      o->Ringer_LVL2_Eta->clear();
      o->Ringer_LVL2_Phi->clear();
      o->Ringer_LVL2_Et->clear();
      o->Ringer_Rings->clear();
      
      NCellsBase += Ringer_NCells->at(j);

    } // loop on all ringer's clusters
  } // loop on all events
  return o;
}

