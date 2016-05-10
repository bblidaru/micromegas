
#include "GeometrySimple.hh"
#include "MediumMagboltz.hh"
<<<<<<< HEAD
=======
#include <ComponentAnalyticField.hh>
#include <Sensor.hh>
#include <ViewGeometry.hh>
#include <TGeoManager.h>


>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf

using namespace std;
using namespace Garfield;

<<<<<<< HEAD
GeometrySimple* build_geometry(MediumMagboltz* gas);
=======
TGeoManager* build_geometry(MediumMagboltz* gas, ComponentAnalyticField* cmpDrift, ComponentAnalyticField* cmpAmp, Sensor* sensor);
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf
