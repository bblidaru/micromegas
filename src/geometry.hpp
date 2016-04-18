
#include "GeometrySimple.hh"
#include "MediumMagboltz.hh"
#include <ComponentAnalyticField.hh>
#include <Sensor.hh>
#include <ViewGeometry.hh>
#include <TGeoManager.h>



using namespace std;
using namespace Garfield;

TGeoManager* build_geometry(MediumMagboltz* gas, ComponentAnalyticField* cmpDrift, ComponentAnalyticField* cmpAmp, Sensor* sensor);
