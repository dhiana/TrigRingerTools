/**
* @file load_rings.cxx
* @author <a href="mailto:Rodrigo.Torres@cern.ch">Rodrigo TORRES</a> 
*
* Reads a ROOT NTuple and return the rings sums found.
*/

#include <list>
#include <vector>

#include "mex.h"
#include "matrix.h"
#include "mat.h"

#include "TrigRingerTools/matlab/MatlabReporter.h"

#include "TChain.h"
#include "TMath.h"

#define ROOT_FILE 0
#define OUT_MAT_STR 0

using namespace std;

const int NFIELDS = 13;
const char *FIELD_NAMES[] = {"lvl2_eta", "lvl2_phi", "et", "rings", "eventNo", "roiNo", "t2ca_em_e", "t2ca_eta", "t2ca_phi", "t2ca_eratio", "t2ca_rcore", "t2ca_had_e", "t2ca_had_es0"};

UInt_t getNumRoIs(TChain *rootChain, const Int_t nEvents)
{
  UInt_t nClusters;
  rootChain->SetBranchStatus("*",0);  // disable all branches
  rootChain->SetBranchStatus("Ringer_NClusters",1);
  rootChain->SetBranchAddress("Ringer_NClusters", &nClusters);

  UInt_t numRoIs = 0;
  for (Int_t ev=0; ev<nEvents; ev++)
  {
    rootChain->GetEntry(ev);
    numRoIs += nClusters;
  }

  return numRoIs;
}

UInt_t getRingSize(TChain *rootChain, const Int_t nEvents)
{
  UInt_t nClusters;
  vector<Float_t> *rootRings = new vector<Float_t>;
  rootChain->SetBranchStatus("*",0);  // disable all branches
  rootChain->SetBranchStatus("Ringer_NClusters",1);
  rootChain->SetBranchStatus("Ringer_Rings",1);
  rootChain->SetBranchAddress("Ringer_NClusters", &nClusters);
  rootChain->SetBranchAddress("Ringer_Rings", &rootRings);
  
  for (Int_t ev=0; ev<nEvents; ev++)
  {
    rootChain->GetEntry(ev);
    if (nClusters)
    {
      UInt_t ringSize = static_cast<UInt_t>(rootRings->size() / nClusters);
      delete rootRings;
      return ringSize;
    }
  }
}



mxArray *getRings(TChain *rootChain, const Int_t nEvents, const UInt_t numRoIs, const UInt_t ringSize, sys::Reporter *reporter)
{
  UInt_t nClusters, rootT2CaNclus;
  vector<UInt_t> *rootLVL1Id = new vector<UInt_t>;
  vector<UInt_t> *rootRoIId = new vector<UInt_t>;
  vector<Float_t> *rootLVL2Eta = new vector<Float_t>;
  vector<Float_t> *rootLVL2Phi = new vector<Float_t>;
  vector<Float_t> *rootRings = new vector<Float_t>;
  vector<Float_t> *rootEt = new vector<Float_t>;
  
  vector<Float_t> *rootT2CaEmE = new vector<Float_t>;
  vector<Float_t> *rootT2CaEta = new vector<Float_t>;
  vector<Float_t> *rootT2CaPhi = new vector<Float_t>;
  vector<Float_t> *rootT2CaEratio = new vector<Float_t>;
  vector<Float_t> *rootT2CaRcore = new vector<Float_t>;
  vector<Float_t> *rootT2CaHadE = new vector<Float_t>;
  vector<Float_t> *rootT2CaHadES0 = new vector<Float_t>;

  rootChain->SetBranchStatus("*",0);  // disable all branches
  rootChain->SetBranchStatus("Ringer_NClusters",1);
  rootChain->SetBranchStatus("Ringer_LVL2_Eta",1);
  rootChain->SetBranchStatus("Ringer_LVL2_Phi",1);
  rootChain->SetBranchStatus("Ringer_Rings", 1);
  rootChain->SetBranchStatus("Ringer_LVL2_Et",1);

  rootChain->SetBranchStatus("T2CaEmE", 1);
  rootChain->SetBranchStatus("T2CaEta", 1);
  rootChain->SetBranchStatus("T2CaPhi", 1);
  rootChain->SetBranchStatus("T2CaNclus", 1);
  rootChain->SetBranchStatus("T2CaEratio", 1);
  rootChain->SetBranchStatus("T2CaRcore", 1);
  rootChain->SetBranchStatus("T2CaHadE", 1);
  rootChain->SetBranchStatus("T2CaHadES0", 1);

  rootChain->SetBranchAddress("Ringer_NClusters", &nClusters);
  rootChain->SetBranchAddress("Ringer_LVL2_Eta", &rootLVL2Eta);
  rootChain->SetBranchAddress("Ringer_LVL2_Phi", &rootLVL2Phi);
  rootChain->SetBranchAddress("Ringer_Rings", &rootRings);
  rootChain->SetBranchAddress("Ringer_LVL2_Et", &rootEt);

  rootChain->SetBranchAddress("T2CaEmE", &rootT2CaEmE);
  rootChain->SetBranchAddress("T2CaEta", &rootT2CaEta);
  rootChain->SetBranchAddress("T2CaPhi", &rootT2CaPhi);
  rootChain->SetBranchAddress("T2CaNclus", &rootT2CaNclus);
  rootChain->SetBranchAddress("T2CaEratio", &rootT2CaEratio);
  rootChain->SetBranchAddress("T2CaRcore", &rootT2CaRcore);
  rootChain->SetBranchAddress("T2CaHadE", &rootT2CaHadE);
  rootChain->SetBranchAddress("T2CaHadES0", &rootT2CaHadES0);

  //Creating the Matlab structure that will contain the rings info..
  const mwSize dim[2] = {1,1};
  mxArray *matRet = mxCreateStructArray(2, dim, NFIELDS, FIELD_NAMES);
  mxArray *mx_lvl2_eta = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);
  mxArray *mx_lvl2_phi = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);
  mxArray *mx_et = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);
  mxArray *mx_rings = mxCreateNumericMatrix(ringSize, numRoIs, mxSINGLE_CLASS, mxREAL);

  mxArray *mx_event = mxCreateNumericMatrix(1, numRoIs, mxUINT32_CLASS, mxREAL);
  mxArray *mx_roi = mxCreateNumericMatrix(1, numRoIs, mxUINT32_CLASS, mxREAL);
  
  mxArray *mx_t2caeme = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);
  mxArray *mx_t2caeta = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);
  mxArray *mx_t2caphi = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);
  mxArray *mx_t2caeratio = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);
  mxArray *mx_t2carcore = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);
  mxArray *mx_t2cahade = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);
  mxArray *mx_t2cahades0 = mxCreateNumericMatrix(1, numRoIs, mxSINGLE_CLASS, mxREAL);

  //Taking the pointers to the structure's fields, so we can fill them up.
  float *lvl2_eta = (float*) mxGetData(mx_lvl2_eta);
  float *lvl2_phi = (float*) mxGetData(mx_lvl2_phi);
  float *et = (float*) mxGetData(mx_et);
  float *rings = (float*) mxGetData(mx_rings);

  float *t2caeme = (float *) mxGetData(mx_t2caeme);
  float *t2caeta = (float *) mxGetData(mx_t2caeta);
  float *t2caphi = (float *) mxGetData(mx_t2caphi);
  float *t2caeratio = (float *) mxGetData(mx_t2caeratio);
  float *t2carcore = (float *) mxGetData(mx_t2carcore);
  float *t2cahade = (float *) mxGetData(mx_t2cahade);
  float *t2cahades0 = (float *) mxGetData(mx_t2cahades0);
  
  unsigned *matlab_event = (unsigned *) mxGetData(mx_event);
  unsigned *matlab_roi = (unsigned *) mxGetData(mx_roi);
  
  //Reading the rings info.
  for (Int_t ev=0; ev<nEvents; ev++)
  {
    rootChain->GetEntry(ev);
    if (!nClusters) continue;
    
    //if (nClusters != rootT2CaNclus) {
    //  RINGER_REPORT(reporter, "T2Calo variables at event " << ev << ": Ringer detected " << nClusters << " clusters and T2CaloEgamma detected " << rootT2CaNclus << " clusters.");
    //}

    //Reading the RoI info.
    for (unsigned roi=0; roi<nClusters; roi++)
    {
      *lvl2_eta++ = rootLVL2Eta->at(roi);
      *lvl2_phi++ = rootLVL2Phi->at(roi);
      *et++ = rootEt->at(roi);
      *matlab_event++ = (unsigned) ev;
      *matlab_roi++ = (unsigned) roi;
      
      // Match T2CaloEgamma variables and Ringer variables
      float deltaEta = 999, deltaPhi = 999;
      unsigned match = 999;
      for (unsigned k = 0; k < rootT2CaNclus; ++k) {
        if ( ((fabs(rootLVL2Eta->at(roi) - rootT2CaEta->at(k))) < deltaEta) &&
             ((fabs(TMath::Pi() - fabs(TMath::Pi() - fabs(rootLVL2Phi->at(roi) - rootT2CaPhi->at(k))))) < deltaPhi) ) {
          deltaEta = fabs(rootLVL2Eta->at(roi) - rootT2CaEta->at(k));
          deltaPhi = fabs(TMath::Pi() - fabs(TMath::Pi() - fabs(rootLVL2Phi->at(roi) - rootT2CaPhi->at(k))));
          match = k;
        }
      }
      if (match == 999) {
        *t2caeme++ = -999;
        *t2caeta++ = -999;
        *t2caphi++ = -999;
        *t2caeratio++ = -999;
        *t2carcore++ = -999;
        *t2cahade++ = -999;
        *t2cahades0++ = -999;
      } else {
        *t2caeme++ = rootT2CaEmE->at(match);
        *t2caeta++ = rootT2CaEta->at(match);
        *t2caphi++ = rootT2CaPhi->at(match);
        *t2caeratio++ = rootT2CaEratio->at(match);
        *t2carcore++ = rootT2CaRcore->at(match);
        *t2cahade++ = rootT2CaHadE->at(match);
        *t2cahades0++ = rootT2CaHadES0->at(match);
      }
    }
    
    //Reading the rings values.
    unsigned r = 0;
    for (vector<Float_t>::const_iterator itr=rootRings->begin(); itr!=rootRings->end(); itr++)
    {
      const Float_t val = *itr;
      *rings++ = val;
    }
    
  }

  //Deleting the not anymore needed root variables.
  delete rootLVL2Eta;
  delete rootLVL2Phi;
  delete rootRings;

  delete rootT2CaEmE;
  delete rootT2CaEta;
  delete rootT2CaPhi;
  delete rootT2CaEratio;
  delete rootT2CaRcore;
  delete rootT2CaHadE;
  delete rootT2CaHadES0;

  mxSetField(matRet, 0, "lvl2_eta", mx_lvl2_eta);
  mxSetField(matRet, 0, "lvl2_phi", mx_lvl2_phi);
  mxSetField(matRet, 0, "et", mx_et);
  mxSetField(matRet, 0, "rings", mx_rings);
  
  mxSetField(matRet, 0, "eventNo", mx_event);
  mxSetField(matRet, 0, "roiNo", mx_roi);

  mxSetField(matRet, 0, "t2ca_em_e", mx_t2caeme);
  mxSetField(matRet, 0, "t2ca_eta", mx_t2caeta);
  mxSetField(matRet, 0, "t2ca_phi", mx_t2caphi);
  mxSetField(matRet, 0, "t2ca_eratio", mx_t2caeratio);
  mxSetField(matRet, 0, "t2ca_rcore", mx_t2carcore);
  mxSetField(matRet, 0, "t2ca_had_e", mx_t2cahade);
  mxSetField(matRet, 0, "t2ca_had_es0", mx_t2cahades0);

  return matRet;
}


void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{	
  sys::Reporter *reporter = new sys::MatlabReporter();
	
	//Check for proper number of arguments.
  if (nrhs < 1) {RINGER_FATAL(reporter, "Too few input parameters passed! See help.");}
  
  TChain *rootChain = new TChain("CollectionTree");
//  rootChain->SetCacheSize(0);
  rootChain->Add(mxArrayToString(prhs[ROOT_FILE]));
  
  const Int_t nEvents = rootChain->GetEntries();
  RINGER_REPORT(reporter, "Found " << nEvents << " Events...");
  
  const UInt_t numRoIs = getNumRoIs(rootChain, nEvents);
  RINGER_REPORT(reporter, "Found " << numRoIs << " RoIs...");
  
  const UInt_t ringSize = getRingSize(rootChain, nEvents);
  RINGER_REPORT(reporter, "Ring size is " << ringSize << " rings...");
  
  RINGER_REPORT(reporter, "Loading the rings...");
  plhs[OUT_MAT_STR] = getRings(rootChain, nEvents, numRoIs, ringSize, reporter);
  
  delete rootChain;
  delete reporter;
}
