
#include <TApplication.h>
#include <ComponentAnsys123.hh>
#include <Plotting.hh>
#include <MediumMagboltz.hh>
#include "GeometrySimple.hh"
#include <ComponentAnalyticField.hh>
#include <Sensor.hh>
#include <ViewGeometry.hh>
#include "GeometrySimple.hh"
#include "field.hpp"
#include "drift.hpp"
#include "geometry.hpp"
#include "plot.hpp"

using namespace std;
using namespace Garfield;

int main(int argc, char * argv[])
{
	TApplication app("micromegas", &argc, argv);
	plottingEngine.SetDefaultStyle();
	
	Sensor* sensor = new Sensor();
	ComponentAnalyticField* cmpDrift = new ComponentAnalyticField();
	ComponentAnalyticField* cmpAmp = new ComponentAnalyticField();
	GeometrySimple* geo = new GeometrySimple(); 

	sensor->AddComponent(cmpDrift);
	sensor->AddComponent(cmpAmp);
	
	// Gas declaration
	MediumMagboltz* gas = new MediumMagboltz();
	gas->SetComposition("ar", 93., "co2", 7.);
	gas->SetTemperature(293.15);
	gas->SetPressure(760.);
	gas->Initialise(true);	
	gas->LoadIonMobility("/opt/garfield/Data/IonMobility_Ar+_Ar.txt");
	
	build_geometry(gas, cmpDrift, cmpAmp, sensor, geo);
	plot_geometry(cmpDrift, cmpAmp, geo);
	
	plot_fields(cmpDrift, cmpAmp, sensor);
	
	
	
	app.Run(kTRUE); 
}
