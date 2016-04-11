
#include "GeometrySimple.hh"
#include "MediumMagboltz.hh"
#include <ComponentAnalyticField.hh>
#include <Sensor.hh>
#include <ViewGeometry.hh>



using namespace std;
using namespace Garfield;

GeometrySimple* build_geometry(MediumMagboltz* gas, ComponentAnalyticField* cmpDrift, ComponentAnalyticField* cmpAmp, Sensor* sensor, GeometrySimple* geo);
