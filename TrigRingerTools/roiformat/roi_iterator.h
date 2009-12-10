//roi_iterator.h

#include <vector>
#include <string>

#include <TChain.h>
#include <TFile.h>

namespace roiformat
{

  class RoIIterator
  {
  protected:
    // data attributes.
    unsigned totalEntries;
    unsigned totalRoIs;
  
    //Control attributes.
    bool updateClustersCount;
    bool updateRoICount;
    unsigned clusPos;
    unsigned roiPos;
    unsigned roiStart, roiEnd;
    unsigned ringStart, ringEnd;

    // Variables for the ROOT branches.
    TChain *chain;
    UInt_t rootNClusters, rootT2CaNclus;
    std::vector<Float_t> *rootT2CaEmE;
    std::vector<Float_t> *rootT2CaEta;
    std::vector<Float_t> *rootT2CaPhi;
    std::vector<Float_t> *rootT2CaEratio;
    std::vector<Float_t> *rootT2CaRcore;
    std::vector<Float_t> *rootT2CaHadE;
    std::vector<Float_t> *rootT2CaHadES0;

    std::vector<UInt_t> *rootLVL1Id;
    std::vector<UInt_t> *rootRoIId;
    std::vector<Float_t> *rootLVL1Eta;
    std::vector<Float_t> *rootLVL1Phi;
    std::vector<Float_t> *rootLVL2Eta;
    std::vector<Float_t> *rootLVL2Phi;
    std::vector<Float_t> *rootLVL2Et;
    std::vector<Float_t> *rootRings;
    std::vector<UInt_t> *rootNCells;
    std::vector<UChar_t> *rootDetCells;
    std::vector<Float_t> *rootEta;
    std::vector<Float_t> *rootPhi;
    std::vector<Float_t> *rootEnergy;

    //These are the attributes for the output NTuple
    TFile *outFile;
    TTree *outTree;
    UInt_t outNClusters, outT2CaNclus;
    std::vector<Float_t> *outT2CaEmE;
    std::vector<Float_t> *outT2CaEta;
    std::vector<Float_t> *outT2CaPhi;
    std::vector<Float_t> *outT2CaEratio;
    std::vector<Float_t> *outT2CaRcore;
    std::vector<Float_t> *outT2CaHadE;
    std::vector<Float_t> *outT2CaHadES0;

    std::vector<UInt_t> *outLVL1Id;
    std::vector<UInt_t> *outRoIId;
    std::vector<Float_t> *outLVL1Eta;
    std::vector<Float_t> *outLVL1Phi;
    std::vector<Float_t> *outLVL2Eta;
    std::vector<Float_t> *outLVL2Phi;
    std::vector<Float_t> *outLVL2Et;
    std::vector<Float_t> *outRings;
    std::vector<UInt_t> *outNCells;
    std::vector<UChar_t> *outDetCells;
    std::vector<Float_t> *outEta;
    std::vector<Float_t> *outPhi;
    std::vector<Float_t> *outEnergy;


    void setBranches()
    {
      chain->SetBranchStatus("*",0);  // disable all branches
      chain->SetBranchStatus("Ringer_NClusters",1);
//      chain->SetBranchStatus("Ringer_LVL1_Id",1);
//      chain->SetBranchStatus("Ringer_Roi_Id",1);
      chain->SetBranchStatus("Ringer_LVL1_Eta",1);
      chain->SetBranchStatus("Ringer_LVL1_Phi",1);
      chain->SetBranchStatus("Ringer_LVL2_Eta",1);
      chain->SetBranchStatus("Ringer_LVL2_Phi",1);
      chain->SetBranchStatus("Ringer_LVL2_Et",1);
      chain->SetBranchStatus("Ringer_Rings",1);
      chain->SetBranchStatus("Ringer_NCells",1);
      chain->SetBranchStatus("Ringer_DetCells",1);
      chain->SetBranchStatus("Ringer_EtaCells",1);
      chain->SetBranchStatus("Ringer_PhiCells",1);
      chain->SetBranchStatus("Ringer_ECells",1);
/*
      chain->SetBranchStatus("T2CaEmE", 1);
      chain->SetBranchStatus("T2CaEta", 1);
      chain->SetBranchStatus("T2CaPhi", 1);
      chain->SetBranchStatus("T2CaNclus", 1);
      chain->SetBranchStatus("T2CaEratio", 1);
      chain->SetBranchStatus("T2CaRcore", 1);
      chain->SetBranchStatus("T2CaHadE", 1);
      chain->SetBranchStatus("T2CaHadES0", 1);
*/
    }

    void calcRoIRange()
    {
      roiStart = 0;
      for (unsigned i=0; i<roiPos; i++) roiStart += rootNCells->at(i);
      roiEnd = roiStart + rootNCells->at(roiPos);
    }
    
    void calcRingRange()
    {
      const unsigned ringSize = static_cast<unsigned>(rootRings->size() / rootNClusters);
      ringStart = roiPos * ringSize; 
      ringEnd = (roiPos + 1) * ringSize;
    }

  public:
    RoIIterator(const std::string &outputNTupleName = "")
    {
      clusPos = -1; // what we want here is to assign this attribute the value immediately before zero (2^32-1).
      rootNClusters = totalEntries = totalRoIs = roiPos = 0;
      updateClustersCount = updateRoICount = false;
      rootLVL1Id = new std::vector<UInt_t>;
      rootRoIId = new std::vector<UInt_t>;
      rootLVL1Eta = new std::vector<Float_t>;
      rootLVL1Phi = new std::vector<Float_t>;
      rootLVL2Eta = new std::vector<Float_t>;
      rootLVL2Phi = new std::vector<Float_t>;
      rootLVL2Et = new std::vector<Float_t>;
      rootRings = new std::vector<Float_t>;
      rootNCells = new std::vector<UInt_t>;
      rootDetCells = new std::vector<UChar_t>;
      rootEta = new std::vector<Float_t>;
      rootPhi = new std::vector<Float_t>;
      rootEnergy = new std::vector<Float_t>;

      chain = new TChain("CollectionTree");
      setBranches();
      chain->SetBranchAddress("Ringer_NClusters", &rootNClusters);
//      chain->SetBranchAddress("Ringer_LVL1_Id", &rootLVL1Id);
//      chain->SetBranchAddress("Ringer_Roi_Id", &rootRoIId);
      chain->SetBranchAddress("Ringer_LVL1_Eta",&rootLVL1Eta);
      chain->SetBranchAddress("Ringer_LVL1_Phi",&rootLVL1Phi);
      chain->SetBranchAddress("Ringer_LVL2_Eta", &rootLVL2Eta);
      chain->SetBranchAddress("Ringer_LVL2_Phi", &rootLVL2Phi);
      chain->SetBranchAddress("Ringer_LVL2_Et", &rootLVL2Et);
      chain->SetBranchAddress("Ringer_Rings", &rootRings);
      chain->SetBranchAddress("Ringer_NCells",&rootNCells);
      chain->SetBranchAddress("Ringer_DetCells",&rootDetCells);
      chain->SetBranchAddress("Ringer_EtaCells",&rootEta);
      chain->SetBranchAddress("Ringer_PhiCells",&rootPhi);
      chain->SetBranchAddress("Ringer_ECells",&rootEnergy);
/*      
      chain->SetBranchAddress("T2CaEmE", &rootT2CaEmE);
      chain->SetBranchAddress("T2CaEta", &rootT2CaEta);
      chain->SetBranchAddress("T2CaPhi", &rootT2CaPhi);
      chain->SetBranchAddress("T2CaNclus", &rootT2CaNclus);
      chain->SetBranchAddress("T2CaEratio", &rootT2CaEratio);
      chain->SetBranchAddress("T2CaRcore", &rootT2CaRcore);
      chain->SetBranchAddress("T2CaHadE", &rootT2CaHadE);
      chain->SetBranchAddress("T2CaHadES0", &rootT2CaHadES0);
*/
      outTree = NULL;
      if (outputNTupleName != "") // we will be saving some of the RoIs to a new ntuple.
      {
        outFile = new TFile(outputNTupleName.c_str(), "recreate");
        outTree = new TTree("CollectionTree", "Whatever");
        outNClusters = 0;
        outLVL1Id = new std::vector<UInt_t>;
        outRoIId = new std::vector<UInt_t>;
        outLVL1Eta = new std::vector<Float_t>;
        outLVL1Phi = new std::vector<Float_t>;
        outLVL2Eta = new std::vector<Float_t>;
        outLVL2Phi = new std::vector<Float_t>;
        outLVL2Et = new std::vector<Float_t>;
        outRings = new std::vector<Float_t>;
        outNCells = new std::vector<UInt_t>;
        outDetCells = new std::vector<UChar_t>;
        outEta = new std::vector<Float_t>;
        outPhi = new std::vector<Float_t>;
        outEnergy = new std::vector<Float_t>;
        outT2CaEmE = new std::vector<Float_t>;
        outT2CaEta = new std::vector<Float_t>;
        outT2CaPhi = new std::vector<Float_t>;
        outT2CaEratio = new std::vector<Float_t>;
        outT2CaRcore = new std::vector<Float_t>;
        outT2CaHadE = new std::vector<Float_t>;
        outT2CaHadES0 = new std::vector<Float_t>;

        outTree->Branch("Ringer_NClusters", &outNClusters, "Ringer_NClusters/i");
//        outTree->Branch("Ringer_LVL1_Id", &outLVL1Id);
//        outTree->Branch("Ringer_Roi_Id", &outRoIId);
//        outTree->Branch("Ringer_LVL1_Eta",&outLVL1Eta);
//        outTree->Branch("Ringer_LVL1_Phi",&outLVL1Phi);
        outTree->Branch("Ringer_LVL2_Eta", &outLVL2Eta);
        outTree->Branch("Ringer_LVL2_Phi", &outLVL2Phi);
        outTree->Branch("Ringer_LVL2_Et", &outLVL2Et);
        outTree->Branch("Ringer_Rings", &outRings);
//        outTree->Branch("Ringer_NCells",&outNCells);
//        outTree->Branch("Ringer_DetCells",&outDetCells);
//        outTree->Branch("Ringer_EtaCells",&outEta);
//        outTree->Branch("Ringer_PhiCells",&outPhi);
//        outTree->Branch("Ringer_ECells",&outEnergy);
        outTree->Branch("T2CaEmE", &outT2CaEmE);
        outTree->Branch("T2CaEta", &outT2CaEta);
        outTree->Branch("T2CaPhi", &outT2CaPhi);
        outTree->Branch("T2CaNclus", &outT2CaNclus, "T2CaNclus/i");
        outTree->Branch("T2CaEratio", &outT2CaEratio);
        outTree->Branch("T2CaRcore", &outT2CaRcore);
        outTree->Branch("T2CaHadE", &outT2CaHadE);
        outTree->Branch("T2CaHadES0", &outT2CaHadES0);

      }
    }

    ~RoIIterator()
    {
      delete chain;
      delete rootLVL1Id;
      delete rootRoIId;
      delete rootLVL1Eta;
      delete rootLVL1Phi;
      delete rootLVL2Eta;
      delete rootLVL2Phi;
      delete rootLVL2Et;
      delete rootRings;
      delete rootNCells;
      delete rootDetCells;
      delete rootEta;
      delete rootPhi;
      delete rootEnergy;

      if (outTree)
      {
        outTree->Write();
        delete outTree;
        delete outFile;
        delete outLVL1Id;
        delete outRoIId;
        delete outLVL1Eta;
        delete outLVL1Phi;
        delete outLVL2Eta;
        delete outLVL2Phi;
        delete outLVL2Et;
        delete outRings;
        delete outNCells;
        delete outDetCells;
        delete outEta;
        delete outPhi;
        delete outEnergy;
        delete outT2CaEmE;
        delete outT2CaEta;
        delete outT2CaPhi;
        delete outT2CaEratio;
        delete outT2CaRcore;
        delete outT2CaHadE;
        delete outT2CaHadES0;

      }
    }

    void saveRoI(std::vector<float> &ringer_rings)
    {
      outNClusters = outT2CaNclus = 1;
      outLVL1Id->clear();
      outRoIId->clear();
      outLVL1Eta->clear();
      outLVL1Phi->clear();
      outLVL2Eta->clear();
      outLVL2Phi->clear();
      outLVL2Et->clear();
      outRings->clear();
      outNCells->clear();
      outDetCells->clear();
      outEta->clear();
      outPhi->clear();
      outEnergy->clear();
      outT2CaEmE->clear();
      outT2CaEta->clear();
      outT2CaPhi->clear();
      outT2CaEratio->clear();
      outT2CaRcore->clear();
      outT2CaHadE->clear();
      outT2CaHadES0->clear();

//      outLVL1Id->push_back(lvl1_id());
//      outRoIId->push_back(roi_id());
//      outLVL1Eta->push_back(lvl1_eta());
//      outLVL1Phi->push_back(lvl1_phi());
      outLVL2Eta->push_back(lvl2_eta());
      outLVL2Phi->push_back(lvl2_phi());
      outLVL2Et->push_back(lvl2_et());
        get_rings(ringer_rings);
//      outNCells->push_back(nCells());
//      detectors(*outDetCells);
//      eta(*outEta);
//      phi(*outPhi);
//      energy(*outEnergy);
      outT2CaEmE->push_back(t2ca_em_e());
      outT2CaEta->push_back(t2ca_eta());
      outT2CaPhi->push_back(t2ca_phi());
      outT2CaEratio->push_back(t2ca_eratio());
      outT2CaRcore->push_back(t2ca_rcore());
      outT2CaHadE->push_back(t2ca_had_e());
      outT2CaHadES0->push_back(t2ca_had_es0());

      outTree->Fill();
    }

    void setBranchStatus(const std::string &branch, const bool enable)
    {
      chain->SetBranchStatus(branch.c_str(), static_cast<Bool_t>(enable));
    }
  

    void add(const std::string &path)
    {
      chain->Add(path.c_str());
      updateClustersCount = updateRoICount = true;
    }

  
    unsigned getEntries()
    {
      if ( (!totalEntries) || (updateClustersCount) )
      {
        updateClustersCount = false;
        totalEntries = static_cast<unsigned>(chain->GetEntries());
      }

      return totalEntries;
    }


    unsigned getNumRoIs()
    {
      if ( (totalRoIs) && (!updateRoICount) ) return totalRoIs;

//      chain->SetBranchStatus("*",0);  // disable all branches (faster search)
//      chain->SetBranchStatus("Ringer_NClusters",1);

      totalRoIs = 0;
      for (Int_t ev=0; ev<static_cast<Int_t>(getEntries()); ev++)
      {
        chain->GetEntry(ev);
        totalRoIs += rootNClusters;
      }
    
//      setBranches();
      updateRoICount = false;
      return totalRoIs;
    }
  
  
    bool next()
    {
      // Going to the next RoI;
      roiPos++;
    
      // If true, then we finished the RoIs for this cluster.
      if (roiPos >= rootNClusters)
      {
        // Looking for the next valid cluster.
        for (clusPos = clusPos + 1; clusPos < getEntries(); clusPos++)
        {
          chain->GetEntry(clusPos);
          if (rootNClusters)
          {
            roiPos = 0;
            break;
          }
        }
      }
          
      if (clusPos < getEntries())
      {
        // calculating the starting and ending indexes for the RoI and the rings.
        calcRoIRange();
        calcRingRange();
        return true;
      }
      else return false;
    }
  
    unsigned nClusters() const {return rootNClusters;}
//    unsigned lvl1_id() const {return rootLVL1Id->at(roiPos);}
//    unsigned roi_id() const {return rootRoIId->at(roiPos);}
    unsigned roi_id() const {return 0;}
    unsigned lvl1_id() const {return 0;}
    float lvl1_eta() const {return rootLVL1Eta->at(roiPos);}
    float lvl1_phi() const {return rootLVL1Phi->at(roiPos);}  
    float lvl2_eta() const {return rootLVL2Eta->at(roiPos);}
    float lvl2_phi() const {return rootLVL2Phi->at(roiPos);}
    float lvl2_et() const {return rootLVL2Et->at(roiPos);}
    unsigned nCells() const {return rootNCells->at(roiPos);}
/*
    float t2ca_em_e() const {return rootT2CaEmE->at(roiPos);}
    float t2ca_eta() const {return rootT2CaEta->at(roiPos);}
    float t2ca_phi() const {return rootT2CaPhi->at(roiPos);}
    float t2ca_eratio() const {return rootT2CaEratio->at(roiPos);}
    float t2ca_rcore() const {return rootT2CaRcore->at(roiPos);}
    float t2ca_had_e() const {return rootT2CaHadE->at(roiPos);}
    float t2ca_had_es0() const {return rootT2CaHadES0->at(roiPos);}
*/
    float t2ca_em_e() const {return 0.;}
    float t2ca_eta() const {return 0.;}
    float t2ca_phi() const {return 0.;}
    float t2ca_eratio() const {return 0.;}
    float t2ca_rcore() const {return 0.;}
    float t2ca_had_e() const {return 0.;}
    float t2ca_had_es0() const {return 0.;}


    void rings(std::vector<float> &vec) const
    {
      for (unsigned i = ringStart; i < ringEnd; i++) vec.push_back(rootRings->at(i));
    }

    void detectors(std::vector<unsigned char> &vec) const
    {
      for (unsigned i = roiStart; i < roiEnd; i++) vec.push_back(rootDetCells->at(i));
    }

    void eta(std::vector<float> &vec) const
    {
      for (unsigned i = roiStart; i < roiEnd; i++) vec.push_back(rootEta->at(i));
    }

    void phi(std::vector<float> &vec) const
    {
      for (unsigned i = roiStart; i < roiEnd; i++) vec.push_back(rootPhi->at(i));
    }

    void energy(std::vector<float> &vec) const
    {
      for (unsigned i = roiStart; i < roiEnd; i++) vec.push_back(rootEnergy->at(i));
    }

   void get_rings (std::vector<float> rings)
   {
     for (unsigned i = 0; i < rings.size(); i++) outRings->push_back(rings.at(i));
   }
  };
}
