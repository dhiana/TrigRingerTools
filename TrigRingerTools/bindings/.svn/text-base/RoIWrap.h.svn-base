#ifndef ROI_WRAP_H
#define ROI_WRAP_H

#include <boost/python.hpp>
#include <vector>
#include "TrigRingerTools/roiformat/RoI.h"

using namespace boost::python;

class RoIWrap : public roiformat::RoI
{
public:
  RoIWrap(unsigned lvl1_id, unsigned roi, double eta, double phi)
  : roiformat::RoI(lvl1_id, roi, eta, phi) {}
  
  PyObject *all_cells()
  {
    std::vector<const roiformat::Cell*> vc;
    cells(vc);
    return toPyObj(vc);
  }

  PyObject *filter_cells(roiformat::Cell::Sampling s)
  {
    std::vector<const roiformat::Cell*> vc;
    cells(s, vc);
    return toPyObj(vc);
  }
  
private:
  PyObject *toPyObj(const std::vector<const roiformat::Cell*> &vc)
  {    
    PyObject *ret = PyList_New(0);
    for (std::vector<const roiformat::Cell*>::const_iterator itr = vc.begin(); itr != vc.end(); itr++)
    {
      const roiformat::Cell *pt = *itr;
      roiformat::Cell c(pt->sampling(), pt->eta(), pt->phi(), pt->r(), pt->deta(), pt->dphi(), pt->dr(), pt->energy());
      converter::arg_to_python<const roiformat::Cell> pyObj(c);
      PyList_Append(ret, pyObj.get());
    }
    return ret;
  }
};
#endif
