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
	fm->Initialise("ansys_nou/ELIST.lis", "ansys_nou/NLIST.lis", "ansys_nou/MPLIST.lis", "ansys_nou/PRNSOL.lis", "mm");
	// Set the periodicities.
	fm->EnableMirrorPeriodicityX();
	fm->EnableMirrorPeriodicityZ();
	// Print some information about the cell dimensions.
	fm->PrintRange();

	
	
		TCanvas * c = new TCanvas("c", "c", 10, 10, 1400, 700);
		c->Divide(2,1);
		  
		ViewField* fieldViewTop = new ViewField();
		fieldViewTop->SetComponent(fm);
		
		fieldViewTop->SetPlane(0., 0., 1., 0., 2.1, 0.);
		fieldViewTop->SetArea(-1.5,-2,1.5, 8);
		fieldViewTop->SetVoltageRange(-500, 200);
		fieldViewTop->SetCanvas((TCanvas*)c->cd(1));
		fieldViewTop->PlotContour();
		
		
		ViewField* fieldViewSide = new ViewField();
		fieldViewSide->SetComponent(fm);
		fieldViewSide->SetPlane(0., -1., 0., 0., 2.1, 0.);
		fieldViewSide->SetArea(-1.5,-1.5,1.5,1.5);
		fieldViewSide->SetVoltageRange(-0.1, 0.1);
		fieldViewSide->SetCanvas((TCanvas*)c->cd(2));
		//fieldViewSide->PlotProfile(0.,-5,0.,0.,2,0.);
		fieldViewSide->PlotContour();
		
		
    
		// This canvas will be used to display the drift lines and the field
		TCanvas * c2 = new TCanvas("c2", "c2", 10, 10, 1000, 700);
		c2->Divide(2,1);
		 //Construct object to visualise drift lines
		ViewDrift* viewdrift = new ViewDrift();
	    viewdrift->SetArea(-1.5, 0., -1.5, 1.5, 8.0, 1.5 );
	    viewdrift->SetClusterMarkerSize(0.1);
	    viewdrift->SetCollisionMarkerSize(0.5);
	    viewdrift->SetCanvas((TCanvas*)c2->cd(1));
	  
      
    
    
    
    
    
    
    //TCanvas* cF = new TCanvas();
    //fieldView->SetCanvas(cF);
    //fieldView->PlotContour();
	
	
	
	
	 //TCanvas* cZ = new TCanvas();
    //fieldView->SetCanvas(cZ);
	//fieldView->PlotProfile(0.,-5,0.,0.,2,0.);
	
	
	
	MediumMagboltz* gas = new MediumMagboltz();
	gas->SetComposition("ar", 80., "co2", 20.);
	// Set temperature [K] and pressure [Torr].
	gas->SetTemperature(293.15);
	gas->SetPressure(760.);
	
	gas->SetMaxElectronEnergy(200.);
	gas->Initialise(true);	
	
	gas->LoadIonMobility("/opt/garfield/Data/IonMobility_Ar+_Ar.txt");
	
	const int nMaterials = fm->GetNumberOfMaterials();
	for (int i = 0; i < nMaterials; ++i) 
	{
	const double eps = fm->GetPermittivity(i);
		if (fabs(eps - 1.) < 1.e-3) fm->SetMedium(i, gas);
	}	
	
	Sensor* sensor = new Sensor();
	sensor->AddComponent(fm);
	
	sensor->SetArea(-1.5,0.,-1.5,1.5,7.2,1.5);
	
	AvalancheMicroscopic* aval = new AvalancheMicroscopic();
	aval->SetSensor(sensor);
	
	// Set the initial position [cm] and starting time [ns].
	double x0 = 0., y0 = 5, z0 = 0., t0 = 0.;
	// Set the initial energy [eV].
	double e0 = 250;
	// Set the initial direction (x, y, z).
	// In case of a null vector, the direction is randomized.
	double dx0 = 0., dy0 = 0., dz0 = 0.;
	//aval->EnableDebugging();
	aval->EnableAvalancheSizeLimit(10000);
	//int SetArea(double x0, double y0, double z0, double dx0, double dy0, double dz0);
	// Calculate an electron avalanche.
	aval->EnablePlotting(viewdrift);
    aval->EnableDriftLines();
	aval->AvalancheElectron(x0, y0, z0, t0, e0, dx0, dy0, dz0);
	
	
	

    viewdrift->Plot();
    
	
	
	int ne, ni;
	// Get the number of electrons and ions in the avalanche.
	aval->GetAvalancheSize(ne, ni);
	// Get the number of electron drift lines.
	int np = aval->GetNumberOfElectronEndpoints();
	// Initial position and time
	double x1, y1, z1, t1;
	// Final position and time
	double x2, y2, z2, t2;
	// Initial and final energy
	double e1, e2;
	// Flag indicating why the tracking of an electron was stopped.
	int status;
	// Loop over the avalanche electrons.
	ofstream myfile;
	myfile.open ("example.txt");
	
	myfile << "i, x1, y1, z1, t1, e1, x2, y2, z2, t2, e2, status" << endl;
	
	for (int i = 0; i < np; ++i) 
	{
		aval->GetElectronEndpoint(i, x1, y1, z1, t1, e1, x2, y2, z2, t2, e2, status);
		myfile << i << "," << x1 << "," << y1 << "," << z1 << "," << t1 << "," << e1 << "," << x2 << "," << y2 << "," << z2 << "," << t2 << "," << e2 << "," << status << endl;
	}

	myfile.close();
	
	app.Run(kTRUE); 
}
