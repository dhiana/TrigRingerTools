//Dear emacs, this is -*- c++ -*-

/**
 * @file lvl1/EMTrigger.cxx
 *
 * Implements the LVL1 e.m. trigger simulator.
 */

#include "TrigRingerTools/lvl1/EMTrigger.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/roiformat/Cell.h"
#include <cmath>

lvl1::EMTrigger::EMTrigger (const double& em_threshold, const double& em_isolation,
			    const double& had_isolation)
  : m_em_threshold(1000*em_threshold), //keep them in MeV because of the cells
    m_em_isolation(1000*em_isolation),
    m_had_isolation(1000*had_isolation)
{
}

bool lvl1::EMTrigger::filter (sys::Reporter* rep, const roiformat::RoI& roi) const
{
  typedef std::vector<const roiformat::Cell*> vec_type;

  //First, calculate the sum on e.m. and hadronic sections for a 0.4 x 0.4
  //cluster. Later evaluate the energy sums of each of the 4 core
  //TT's. Subtract these 4 TT's energy from the 4 x 4 em. energy and that is
  //it. Everything is done. Proceed with the cuts.
  const double HALF_WINDOW = 0.2;
  const double etamin = roi.eta() - HALF_WINDOW;
  const double etamax = roi.eta() + HALF_WINDOW;
  const double phimin = roi.phi() - HALF_WINDOW;
  const double phimax = roi.phi() + HALF_WINDOW;
  const double ONEFOURTH_WINDOW = 0.1;
  const double top_left_eta = roi.eta() - ONEFOURTH_WINDOW;
  const double top_left_phi = roi.phi() + ONEFOURTH_WINDOW;
  const double top_right_eta = roi.eta() + ONEFOURTH_WINDOW;
  const double top_right_phi = top_left_phi;
  const double bottom_left_eta = top_left_eta;
  const double bottom_left_phi = roi.phi() - ONEFOURTH_WINDOW;
  const double bottom_right_eta = top_right_eta;
  const double bottom_right_phi = bottom_left_phi;

  RINGER_DEBUG1("Considering center at (eta,phi) = (" << roi.eta() << "," << roi.phi() << ")"); 
	//are we, possibly at the wrap-around region for phi?
  bool wrap = roiformat::check_wrap_around(roi.phi(), false);
  if (wrap) {
      RINGER_DEBUG3("Possible Ring window at the phi wrap around"
		    << " region *DETECTED*.");
  }
  bool reverse_wrap = roiformat::check_wrap_around(roi.phi(), true);
  if (reverse_wrap) {
      RINGER_DEBUG3("Possible (reverse) Ring window at the phi wrap around"
		    << " region *DETECTED*.");
  }
  
  vec_type cells;
  roi.cells(cells);
  if (!cells.size()) {
    RINGER_WARN(rep, "No cells found on RoI #" << roi.roi_id() 
		<< " from event with LVL1 id #" << roi.lvl1_id() 
		<< ". RoI REJECTED!");
    return false;
  }

  // This is the organization:
  // core[0][0] = top left e.m. TT
  // core[0][1] = top right e.m. TT
  // core[1][0] = bottom left e.m. TT
  // core[1][1] = bottom right e.m. TT
  double core[2][2] = { {0.0, 0.0}, {0.0, 0.0} };

  // e.m. energy on the 2 x 2 TT neighboors
  double em_neigh = 0.0;
  // hadronic energy on 4 x 4 TT region
  double hadronic = 0.0;

  //for all cells
  bool em_cell = true;

  for (vec_type::const_iterator it=cells.begin(); it!=cells.end(); ++it) {
 
    //check if the cell is in a sampling I should handle.
    switch ((*it)->sampling()) {
    case roiformat::Cell::PSBARREL:
    case roiformat::Cell::EMBARREL1:
    case roiformat::Cell::EMBARREL2:
    case roiformat::Cell::EMBARREL3:
    case roiformat::Cell::PSENDCAP:
    case roiformat::Cell::EMENDCAP1:
    case roiformat::Cell::EMENDCAP2:
    case roiformat::Cell::EMENDCAP3:
      break;

    case roiformat::Cell::HADENCAP0:
    case roiformat::Cell::HADENCAP1:
    case roiformat::Cell::HADENCAP2:
    case roiformat::Cell::HADENCAP3:
    case roiformat::Cell::TILEBARREL0:
    case roiformat::Cell::TILEBARREL1:
    case roiformat::Cell::TILEBARREL2:
    case roiformat::Cell::TILEGAPSCI0:
    case roiformat::Cell::TILEGAPSCI1:
    case roiformat::Cell::TILEGAPSCI2:
    case roiformat::Cell::TILEEXTB0:
    case roiformat::Cell::TILEEXTB1:
    case roiformat::Cell::TILEEXTB2:
      em_cell = false;
      break;

    case roiformat::Cell::FORWCAL0:
    case roiformat::Cell::FORWCAL1:
    case roiformat::Cell::FORWCAL2:
    case roiformat::Cell::UNKNOWN:
    default:
      RINGER_WARN(rep, "Cell with sampling = " 
		  << roiformat::sampling2str((*it)->sampling())
		  << "From RoI #" << roi.roi_id() 
		  << " of event with LVL1 id #" << roi.lvl1_id() 
		  << " was not considered for LVL1 filtering.");
      continue;
    }
    
    //when it gets here, it knows if it is a em or hadronic cell, by looking
    //at the "em_cell" variable (true for em cell and false for hadronic cell)

    double phi_use = (*it)->phi(); //use this value for phi (wrap protection)
    if (wrap) phi_use = roiformat::fix_wrap_around(phi_use, false);
    if (reverse_wrap) phi_use = roiformat::fix_wrap_around(phi_use, true);

    if ((*it)->eta() > etamin && (*it)->eta() < etamax &&
	  phi_use > phimin && phi_use < phimax) {
      //falls in 0.4 by 0.4 region around the center defined in the RoI
      //already! 
      double energy = (*it)->energy() / std::cosh(std::fabs((*it)->eta())); 
      //double energy = (*it)->energy();
      if (em_cell) {
	//Test if this cells falls in one of the cores
	if ((*it)->eta() < roi.eta() && (*it)->eta() > top_left_eta &&
	    phi_use > roi.phi() && phi_use < top_left_phi) 
	  core[0][0] += energy;

	else if ((*it)->eta() > roi.eta() && (*it)->eta() < top_right_eta &&
		 phi_use > roi.phi() && phi_use < top_right_phi)
	  core[0][1] += energy;

	else if ((*it)->eta() < roi.eta() && (*it)->eta() > bottom_left_eta &&
		 phi_use < roi.phi() && phi_use > bottom_left_phi)
	  core[1][0] += energy;

	else if ((*it)->eta() > roi.eta() && (*it)->eta() < bottom_right_eta &&
		 phi_use < roi.phi() && phi_use > bottom_right_phi)
	  core[1][1] += energy;

	else { //falls on neighboring cells!
	  em_neigh += energy;
	}

      }

      else hadronic += energy;

    }

  } //loop over cells

  //Et core sums
  const double left = core[0][0] + core[1][0];
  const double right = core[0][1] + core[1][1];
  const double top = core[0][0] + core[0][1];
  const double bottom = core[1][0] + core[1][1];

  //When we get here, we have got all values of interest. Make the core sums
  //and stipulates the maximum there:
  double max_2x1_sum = left;
  if (right > max_2x1_sum) max_2x1_sum = right;
  if (top > max_2x1_sum) max_2x1_sum = top;
  if (bottom > max_2x1_sum) max_2x1_sum = bottom;

  //Now make the cuts:
  if (max_2x1_sum > m_em_threshold 
      && em_neigh < m_em_isolation 
      && hadronic < m_had_isolation) return true;
  
  //By default return false
  return false;
}

size_t lvl1::EMTrigger::filter (sys::Reporter* rep, 
				roiformat::Database& roidb,
				std::vector<const roiformat::RoI*>& vr) const
{
  std::vector<const roiformat::RoI*> rois;
  rois.reserve(roidb.size());
  roidb.rois(rois);
  size_t counter = 0;
  for (size_t i = 0; i < rois.size(); ++i) {
    if (!rois[i]) {
      RINGER_WARN(rep, "The RoI number " << i 
		  << " in the database does not exist?");
      continue;
    }
    RINGER_DEBUG1("Analyzing RoI with RoI id #" << rois[i]->roi_id() 
		  << " and LVL1 id #" << rois[i]->lvl1_id());
    if (filter(rep, *rois[i])) {
      ++counter;
      vr.push_back(rois[i]);
    }
    else {
      RINGER_DEBUG1("The RoI with RoI id #" << rois[i]->roi_id()
		    << " and LVL1 id #" << rois[i]->lvl1_id()
		    << " was rejected by the LVL1 simulation.");
    }
  }

  return counter;
}
