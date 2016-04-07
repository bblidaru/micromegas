#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TBuffer3D.h>
#include <TBuffer3DTypes.h>
#include <TVirtualViewer3D.h>
#include <TFile.h>
#include "MediumMagboltz.hh"
#include "ComponentAnsys123.hh"
#include "Sensor.hh"
#include "AvalancheMicroscopic.hh"
#include "ViewGeometry.hh"
#include "Plotting.hh"
#include "ViewField.hh"
#include "Random.hh"
#include <TChain.h>
#include "TObjArray.h"
#include <time.h>


using namespace std;
using namespace Garfield;

int main(int argc, char * argv[])
{
	TApplication app("app", &argc, argv);
	plottingEngine.SetDefaultStyle();
	
	ComponentAnsys123* fm = new ComponentAnsys123();
	fm->Initialise("ansys2/ELIST.lis", "ansys2/NLIST.lis", "ansys2/MPLIST.lis", "ansys2/PRNSOL.lis", "mm");
	// Set the periodicities.
	////fm->EnableMirrorPeriodicityX();
	//fm->EnableMirrorPeriodicityY();
	// Print some information about the cell dimensions.
	fm->PrintRange();

	for (float z=-31.75; z<-25.2; z+=0.2)
		{
	TCanvas * c1 = new TCanvas("c1", "c1", 10, 10, 1400, 700);
	c1->Divide(2,1);
		
	ViewField* fieldViewXY = new ViewField();
    fieldViewXY->SetComponent(fm);
    fieldViewXY->SetPlane(0., 0., -1., 0., 0., z);
    fieldViewXY->SetArea(-31.7,8,31.7,15);
    fieldViewXY->SetVoltageRange(-180, 500);
    fieldViewXY->SetCanvas((TCanvas*)c1->cd(1));
    fieldViewXY->PlotContour();
    
   
    
    ViewField* fieldViewYZ = new ViewField();
    fieldViewYZ->SetComponent(fm);
    fieldViewYZ->SetPlane(-1., 0., 0., z, 0., 0.);
    fieldViewYZ->SetArea(-31.7,8,31.7,15);
    fieldViewYZ->SetVoltageRange(-180, 500);
    fieldViewYZ->SetCanvas((TCanvas*)c1->cd(2));
    fieldViewYZ->PlotContour();
    
    
    c1->Print(("output2/field_xy_yz_for_" + to_string(z) + ".png").c_str());
    }
    
    //TCanvas * c2 = new TCanvas("c2", "c2", 10, 10, 700, 700);
	
	//for (float z=12; z<17.2; z+=0.2)
    //{
		//ViewField* fieldViewXZ = new ViewField();
		//fieldViewXZ->SetComponent(fm);
		//fieldViewXZ->SetPlane(0., -1., 0., 0., z, 0.);
		//fieldViewXZ->SetArea(-31.7,-31.7,31.7,31.7);
		//fieldViewXZ->SetVoltageRange(-2, 2);
		//fieldViewXZ->SetCanvas((TCanvas*)c2->cd());
		//fieldViewXZ->PlotContour();
		//c2->Print(("output/field_xz_for_" + to_string(z) + ".png").c_str());
		
    //}
    
    
    //TCanvas * c3 = new TCanvas("c3", "c3", 10, 10, 700, 700);
	
	
	//for (float z=-31.75; z<-23.2; z+=0.1)
    //{
		//ViewField* fieldViewXY = new ViewField();
		//fieldViewXY->SetComponent(fm);
		//fieldViewXY->SetPlane(0., 0., -1., 0., 0., z);
		//fieldViewXY->SetArea(-31.7,14.3,31.7,17.8);
		//fieldViewXY->SetVoltageRange(-5, 5);
		//fieldViewXY->SetCanvas((TCanvas*)c3->cd());
		//fieldViewXY->PlotContour();
		//c3->Print(("output2/field_xy_for_" + to_string(z) + ".png").c_str());
		
    //}
    
    //ViewField* fieldViewZoom = new ViewField();
    //fieldViewZoom->SetComponent(fm);
    //fieldViewZoom->SetPlane(0., 0., -1., 0., 0., 2.3);
    //fieldViewZoom->SetArea(-30,13,30,17);
    //fieldViewZoom->SetVoltageRange(-5, 5);
    //fieldViewZoom->SetCanvas((TCanvas*)c->cd(2));
    //fieldViewZoom->PlotContour();
    
    
	
	
	//fieldView->PlotProfile(0.,-5,0.,0.,2,0.);
	
	
	
	//MediumMagboltz* gas = new MediumMagboltz();
	//gas->SetComposition("ar", 80., "co2", 20.);
	//// Set temperature [K] and pressure [Torr].
	//gas->SetTemperature(293.15);
	//gas->SetPressure(760.);
	
	//gas->SetMaxElectronEnergy(200.);
	//gas->Initialise(true);	
	
	//gas->LoadIonMobility("/opt/garfield/Data/IonMobility_Ar+_Ar.txt");
	
	//const int nMaterials = fm->GetNumberOfMaterials();
	//for (int i = 0; i < nMaterials; ++i) 
	//{
	//const double eps = fm->GetPermittivity(i);
		//if (fabs(eps - 1.) < 1.e-3) fm->SetMedium(i, gas);
	//}	
	
	//Sensor* sensor = new Sensor();
	//sensor->AddComponent(fm);
	
	//sensor->SetArea(-5,-5,-5,5,1,5);
	
	//AvalancheMicroscopic* aval = new AvalancheMicroscopic();
	//aval->SetSensor(sensor);
	
	//// Set the initial position [cm] and starting time [ns].
	//double x0 = 0., y0 = 0., z0 = 0.02, t0 = 0.;
	//// Set the initial energy [eV].
	//double e0 = 250;
	//// Set the initial direction (x, y, z).
	//// In case of a null vector, the direction is randomized.
	//double dx0 = 0., dy0 = 0., dz0 = 0.;
	////aval->EnableDebugging();
	//aval->EnableAvalancheSizeLimit(1000);
	////int SetArea(double x0, double y0, double z0, double dx0, double dy0, double dz0);
	//// Calculate an electron avalanche.
	//aval->AvalancheElectron(x0, y0, z0, t0, e0, dx0, dy0, dz0);
	
	//int ne, ni;
	//// Get the number of electrons and ions in the avalanche.
	//aval->GetAvalancheSize(ne, ni);
	//// Get the number of electron drift lines.
	//int np = aval->GetNumberOfElectronEndpoints();
	//// Initial position and time
	//double x1, y1, z1, t1;
	//// Final position and time
	//double x2, y2, z2, t2;
	//// Initial and final energy
	//double e1, e2;
	//// Flag indicating why the tracking of an electron was stopped.
	//int status;
	//// Loop over the avalanche electrons.
	//ofstream myfile;
	//myfile.open ("example.txt");
	
	//myfile << "i, x1, y1, z1, t1, e1, x2, y2, z2, t2, e2, status" << endl;
	
	//for (int i = 0; i < np; ++i) 
	//{
		//aval->GetElectronEndpoint(i, x1, y1, z1, t1, e1, x2, y2, z2, t2, e2, status);
		//myfile << i << "," << x1 << "," << y1 << "," << z1 << "," << t1 << "," << e1 << "," << x2 << "," << y2 << "," << z2 << "," << t2 << "," << e2 << "," << status << endl;
	//}

	//myfile.close();
	
	app.Run(kTRUE); 
}
