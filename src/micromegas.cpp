
#include <TApplication.h>
#include <ComponentAnsys123.hh>
#include <Plotting.hh>
#include <MediumMagboltz.hh>
#include "GeometrySimple.hh"

#include "field.hpp"
#include "drift.hpp"
#include "geometry.hpp"

using namespace std;
using namespace Garfield;

int main(int argc, char * argv[])
{
	TApplication app("micromegas", &argc, argv);
	plottingEngine.SetDefaultStyle();
	
	
	
	
	// Include ANSYS lists
	//string ansys_dir = "ansys/v4/lis/";
		
	//ComponentAnsys123* fm = new ComponentAnsys123();
	//fm->Initialise(
		//(ansys_dir  + "ELIST.lis").c_str(), 
		//(ansys_dir  + "NLIST.lis").c_str(), 
		//(ansys_dir  + "MPLIST.lis").c_str(), 
		//(ansys_dir  + "PRNSOL.lis").c_str(), 
	    //"mm");
	
	//fm->PrintRange();
	
	
	
	
	// Plot field
	//plot_fields(fm);
	
	
	
	
	// Gas declaration
	MediumMagboltz* gas = new MediumMagboltz();
	gas->SetComposition("ar", 93., "co2", 7.);
	gas->SetTemperature(293.15);
	gas->SetPressure(760.);
	gas->Initialise(true);	
	gas->LoadIonMobility("/opt/garfield/Data/IonMobility_Ar+_Ar.txt");
	
	//// Assign material for components in ANSYS
	//const int nMaterials = fm->GetNumberOfMaterials();
	//for (int i = 0; i < nMaterials; ++i) 
	//{
		//const double eps = fm->GetPermittivity(i);
		//if (fabs(eps - 1.) < 1.e-3) fm->SetMedium(i, gas);
	//}
	build_geometry(gas);
	
	
	
	
	// Plot drift
	//plot_drift(fm);
	
	app.Run(kTRUE); 
}
