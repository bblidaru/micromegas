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

int main()
{

	ComponentAnsys123* fm = new ComponentAnsys123();
	fm->Initialise("ansys/ELIST.lis", "ansys/NLIST.lis", "ansys/MPLIST.lis", "ansys/PRNSOL.lis", "mm");
	// Set the periodicities.
	fm->EnableMirrorPeriodicityX();
	fm->EnableMirrorPeriodicityY();
	// Print some information about the cell dimensions.
	fm->PrintRange();

	//ViewField* fieldView = new ViewField();
	//fieldView->SetComponent(fm);
	//fieldView->PlotProfile(0.,0.,0.02,0.,0.,-0.02);
	
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
	
	sensor->SetArea();
	
	AvalancheMicroscopic* aval = new AvalancheMicroscopic();
	aval->SetSensor(sensor);
	
	// Set the initial position [cm] and starting time [ns].
	double x0 = 0., y0 = 0., z0 = 0.02, t0 = 0.;
	// Set the initial energy [eV].
	double e0 = 25;
	// Set the initial direction (x, y, z).
	// In case of a null vector, the direction is randomized.
	double dx0 = 0., dy0 = 0., dz0 = 0.;
	//aval->EnableDebugging();
	aval->EnableAvalancheSizeLimit(1000);
	//int SetArea(double x0, double y0, double z0, double dx0, double dy0, double dz0);
	// Calculate an electron avalanche.
	aval->AvalancheElectron(x0, y0, z0, t0, e0, dx0, dy0, dz0);
	
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
	
}
