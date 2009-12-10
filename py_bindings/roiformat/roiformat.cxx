#include <boost/python.hpp>

void wrap_Cell();
void wrap_RoI();
void wrap_roi_iterator();

using namespace boost::python;

BOOST_PYTHON_MODULE(roiformat)
{
  wrap_Cell();
  wrap_RoI();
  wrap_roi_iterator();
}
