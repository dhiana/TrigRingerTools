#include <boost/python.hpp>

#include "TrigRingerTools/sys/Exception.h"

void wrap_Config();
void wrap_RingConfig();
void wrap_util();

using namespace boost::python;

void translator(const sys::Exception &x)
{
    PyErr_SetString(PyExc_RuntimeError, x.what());
}

BOOST_PYTHON_MODULE(rbuild)
{
  wrap_Config();
  wrap_RingConfig();
  wrap_util();
  register_exception_translator<sys::Exception>(translator);
}
