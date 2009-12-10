//Dear emacs, this is -*- c++ -*-

/**
 * @file prog/filter.cxx
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre Rabello dos Anjos</a> 
 * $Author: rtorres $
 * $Revision: 1.3 $
 * $Date: 2007-04-12 21:04:27 $
 *
 * Gets a specific RoI from a file and dump it in a nicely readable format,
 * which could be easily interpreted by any statistical analysis program.
 */

#include "TrigRingerTools/roiformat/Database.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/Exception.h"
#include <cstdlib>
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/sys/util.h"
#include <cmath>

/**
 * Check the RoI for layout problems. Check on the header file
 * roiformat/Cell.h for the Calorimeter Layer Definition.
 */
bool check_roi (const roiformat::RoI* roi, const double winsize=0.2)
{
  if ( (std::fabs(roi->eta()) + winsize) > 1.5 ) {
    //should have HEC cells, so it is invalid
    RINGER_DEBUG2("ABS RoI's window (max=" 
		  << std::fabs(roi->eta()) + winsize 
		  << ") is greater than 1.5. Therefore it is INVALID.");
    return false;
  }

  typedef std::vector<const roiformat::Cell*> vec_type;
  //if we get here, the event should only have Tile Cells
  if ( (std::fabs(roi->eta()) - winsize) < 1.0 ) {
    bool ok = false;
    //should have Tile cells, from TILEBARREL0 with eta the same as
    //roi->eta(), at least.
    const vec_type* cells = roi->cells(roiformat::Cell::TILEBARREL0);
    if (!cells) return false;
    for (vec_type::const_iterator it=cells->begin(); it!= cells->end(); ++it) {
      if ((std::signbit((*it)->eta()) > 0 && std::signbit(roi->eta()) > 0) ||
	  (std::signbit((*it)->eta()) == 0 && std::signbit(roi->eta()) == 0)) {
	ok = true;
      }
    }
    if (!ok) {
      RINGER_DEBUG2("ABS RoI's window (max="
		    << std::fabs(roi->eta()) + winsize
		    << ") is smaller than 1.0."
		    << " And there are NO  eta cells for TILEBARREL0.");
      return false; //Tile cells do not exist where they are supp. to
    }
    else {
      RINGER_DEBUG2("ABS RoI's window (max="
		    << std::fabs(roi->eta()) + winsize
		    << ") is smaller than 1.0."
		    << " And there are eta cells for TILEBARREL0.");
    }
  }

  //if we get here, the event should have Tile Cells also from the extended
  //barrel and scintilators (if any) only. I test the existence of TILEEXTB0,
  //because at least data from that detector should be there.
  if ( (std::fabs(roi->eta()) + winsize) > 1.0 ) {
    bool ok = false;
    //should have Tile cells, from TILEEXTB0 with eta the same as
    //roi->eta(), at least.
    const vec_type* cells = roi->cells(roiformat::Cell::TILEEXTB0);
    if (!cells) return false;
    for (vec_type::const_iterator it=cells->begin(); it!= cells->end(); ++it) {
      if ((std::signbit((*it)->eta()) > 0 && std::signbit(roi->eta()) > 0) ||
	  (std::signbit((*it)->eta()) == 0 && std::signbit(roi->eta()) == 0)) {
	ok = true;
      }
    }
    if (!ok) {
      RINGER_DEBUG2("ABS RoI's window (max="
		    << std::fabs(roi->eta()) + winsize
		    << ") is greater than 1.0."
		    << " And there are NO  eta cells for TILEEXTB0.");
      return false; //Tile cells do not exist where they are supp. to
    }
    else {
      RINGER_DEBUG2("ABS RoI's window (max="
		    << std::fabs(roi->eta()) + winsize
		    << ") is greater than 1.0."
		    << " And there are eta cells for TILEEXTB0.");
    }
  }
  RINGER_DEBUG1("checkroi() returning true because everything is OK.");
  return true;
}

/**
 * Does all the work
 */
int main (int argc, char** argv) 
{
  sys::Reporter *reporter = new sys::LocalReporter();
  if (argc != 3) {
    RINGER_FATAL(reporter, "usage: " << argv[0] 
		 << " <input.rfd> <output.rfd>");
  }

  std::string filename = argv[1];
  std::string outfile = argv[2];

  RINGER_REPORT(reporter, "Filtering RoI's in \"" << filename << "\"" 
	        << " (problems with tiles) and dumping at \"" << outfile
		<< "\"...");

  try {
    roiformat::Database db(reporter);
    if (!sys::exists(filename)) {
      RINGER_DEBUG1("Input file " << filename << " doesn't exist.");
      throw RINGER_EXCEPTION("Input file doesn't exist");
    }
    db.load(filename);

    //get a handle to all RoI's in the file
    typedef std::vector<const roiformat::RoI*> vec_type;
    vec_type rois;
    db.rois(rois);

    //check every RoI with respect to its position and redump only what seems
    //to be correct.
    vec_type rois_left;
    rois_left.reserve(rois.size()); //max size reserved for optimization
    for (vec_type::iterator it=rois.begin(); it!=rois.end(); ++it) {
      bool ok = check_roi(*it);
      if (ok) {
	rois_left.push_back(*it);
	RINGER_REPORT(reporter, "RoI #" << (*it)->roi_id() 
		      << " with LVL1 Id #" << (*it)->lvl1_id() << " is OK.");
      }
      else {
	RINGER_REPORT(reporter, "RoI #" << (*it)->roi_id() 
		      << " with LVL1 Id #" << (*it)->lvl1_id() 
		      << " got problems with HadCell's.");
      }
    }

    //dump new file
    roiformat::Database outdb(rois_left, reporter);
    outdb.save(outfile);
    
    //print statistics
    RINGER_REPORT(reporter, "SUMMARY:");
    RINGER_REPORT(reporter, " -> Input file \"" << filename 
		<< "\" has a total of " << rois.size() << " RoI's.");
    RINGER_REPORT(reporter, " -> From that total, "
		  << rois.size() - rois_left.size() << " RoI's have problems "
		  << "with HadCall cells. They were eliminated from my dump.");
    RINGER_REPORT(reporter, " -> The new file \"" << outfile << "\" contains "
		  << rois_left.size() 
		  << " checked RoI's and they seem all OK.");
  }
  catch (sys::Exception& ex) {
    RINGER_FATAL(reporter, ex.what());
  }
  
  RINGER_REPORT(reporter, "Just finished.");
  delete reporter;
}



