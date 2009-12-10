#include <vector>

#include <boost/python.hpp>

#include "TrigRingerTools/roiformat/RoI.h"
#include "TrigRingerTools/rbuild/util.h"
#include "TrigRingerTools/sys/LocalReporter.h"
#include "TrigRingerTools/bindings/RoIWrap.h"

using namespace boost::python;

PyObject *py_find_center(RoIWrap *roi)
{

  sys::LocalReporter *rep = new sys::LocalReporter();
  double eta, phi;
  rbuild::find_center(rep, roi, eta, phi);
  
  PyObject *ret = PyTuple_New(2);
  PyTuple_SetItem(ret, 0, PyFloat_FromDouble(eta));
  PyTuple_SetItem(ret, 1, PyFloat_FromDouble(phi));
  delete rep;
  return ret;
}

void wrap_util()
{
  def("find_center", py_find_center);
}
