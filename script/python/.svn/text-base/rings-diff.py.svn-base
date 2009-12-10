#!/usr/bin/env python

import ROOT
import sys
import os
import math

#Set this value to an appropriate error margin. The rings are checked in the following way:
#one ring is subtracted from the other, and if the abs value of that difference is greater
#than the error limit, than the rings are considered different.
ERROR_LIMIT = 1e-12;

def loadRings(ringRootFile):
  file = ROOT.TFile(ringRootFile);
  chain = file.Get("CollectionTree");
  chain.SetCacheSize(0);
  chain.SetBranchStatus("*",0);
  chain.SetBranchStatus("Ringer_Rings",1);
  chain.SetBranchStatus("Ringer_NClusters",1);
  chain.SetBranchStatus("Ringer_LVL1_Id",1);
  chain.SetBranchStatus("Ringer_Roi_Id",1);

  retMap = {};
  for e in range(chain.GetEntries()):
    chain.GetEntry(e);
    if chain.Ringer_NClusters == 0: continue;
    
    #Getting the number of rings per RoI.
    numRings = len(chain.Ringer_Rings) / chain.Ringer_NClusters;
    
    # The LVL1 Id is the same for all clusters within an event.
    # We then test whether the LVL1 Id already exixts in the return map. If so, then it is
    # an error. Maybe you had the same event processed twice.
    lvl1Id = chain.Ringer_LVL1_Id[0];
    if retMap.has_key(lvl1Id):
      print "WARNING: repeated LVL1 Id (%d)! Please check for events repetition!" % lvl1Id;
    retMap[lvl1Id] = {};
    
    #Now we do the same check as the LVL1 Id for the RoI Ids.
    for c in range(chain.Ringer_NClusters):
      roiId = chain.Ringer_Roi_Id[c];
      if retMap[lvl1Id].has_key(roiId):
        print "WARNING: You have at least two RoI with the same Id in the same event!";
      
      #Getting the rings.
      retMap[lvl1Id][roiId] = chain.Ringer_Rings[int(c*numRings):int((c+1)*numRings)];
  
  return retMap


def main():

  if len(sys.argv) != 3:
    print """ This script will check whether the rings in 2 files are equal.
    The script will organize them first by LVL1 Id and RoI Id, and will use these
    information to ensure you are testing the same ring. Doing so, the order how
    the rings are stored in each file will not matter.
    The tests consider a error margin. To find out whether 2 rings are NOT equal, the script
    will test abs(r1 - r2) > min_error, where min_error is %e""" % ERROR_LIMIT;
    print "Usage: %s rings_file1.root rings_file2.root" % os.path.basename(sys.argv[0]);
    sys.exit(1)

  print "Loading %s" % sys.argv[1]
  file1 = loadRings(sys.argv[1]);

  print "Loading %s" % sys.argv[2]
  file2 = loadRings(sys.argv[2]);

  #Testing whether both have the same number of events.
  if len(file1) != len(file2):
    print "Number of events in each file are not equal (%d != %d)! Exiting... " % (len(file1), len(file2)),
    sys.exit(2);
  
  #Testing each ring.
  print "Testing each ring..."
  numFails = 0;
  for (lvl1_1, lvl1_2) in zip(sorted(file1.keys()), sorted(file2.keys())):
    if lvl1_1 != lvl1_2:
      print "LVL1 ID Mismatch! (%d != %d). Skipping RoI Id and Rings check for this event." % (lvl1_1, lvl1_2);
      numFails += 1;
      continue;
  
    if len(file1[lvl1_1]) != len(file2[lvl1_2]):
      print "You don't have the same number of clusters for the event with LVL1 ID = %d (%d != %d). Skipping Rings test for this event." % (lvl1_1, len(file1[lvl1_1]), len(file2[lvl1_2]));
      numFails += 1;
      continue;
    
    for (roi_1, roi_2) in zip(sorted(file1[lvl1_1].keys()), sorted(file2[lvl1_2].keys())):
      if roi_1 != roi_2:
        print "RoI ID Mismatch! (%d != %d) in event with LVL1 ID = %d. Skipping Rings check for this event." % (roi_1, roi_2, lvl1_1);
        numFails += 1;
        continue;

      for (idx,r1,r2) in zip(range(len(file1[lvl1_1][roi_1])), file1[lvl1_1][roi_1], file2[lvl1_2][roi_2]):
        if math.fabs(r1 - r2) > ERROR_LIMIT:
          numFails += 1;
          print "LVL1 Id = %d, RoI ID = %d: rings[%d] are different! (%e != %e)." % (lvl1_1, roi_1, idx, r1, r2);

  if numFails == 0:
    print "The rings in both files are equal!";
    sys.exit(0);
  else:
    print "The rings in both files are NOT equal! The total count of differences is %d" % numFails;
    sys.exit(3);


if __name__ == "__main__":
	main();