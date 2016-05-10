#include <iostream>
#include <fstream>
#include <cmath>

#include <TApplication.h>
<<<<<<< HEAD
#include <TCanvas.h>

#include "ComponentAnsys123.hh"
#include "ViewField.hh"
#include "ViewCell.hh"
#include "TrackHeed.hh"
#include "ViewGeometry.hh"
#include "ViewSignal.hh"
#include "MediumMagboltz.hh"
#include "SolidBox.hh"
#include "Sensor.hh"
#include "GeometrySimple.hh"
#include "ComponentAnalyticField.hh"
#include "AvalancheMicroscopic.hh"
#include "Plotting.hh"
=======
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
#include <TGeoManager.h>
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf

using namespace Garfield;
using namespace std;

int main(int argc, char * argv[]){


  TApplication app("app", &argc, argv);
  plottingEngine.SetDefaultStyle();



<<<<<<< HEAD
	
	// Width [cm]
	const double w = 10.0;
	// Length [cm]
	const double l = 10.0;
	// Height [cm]
	const double h_drift = 0.6; // Drift gap height
	const double h_amp = 0.0128; // Amplification gap height
	const double h_tot = h_drift + h_amp; // Total height
	// Voltages [V]
	const double v_top = -180.0;
	const double v_mesh = 0.0;
	const double v_bottom = 500.0;
	// Mesh properties [cm]
	const double m_thickness = 0.0003; // Mesh thickness (3um)
	const double m_width = 0.0011; // Mesh width (11um)
	const double m_interpitch = 0.0039; // Distance between two consecutive mesh lines (39um)
	const double m_pitch = m_width + m_interpitch; // Period of the mesh 
	
	// Strip properties [cm]
	const double s_pitch = 0.025; // Period of the strips (250um) 
	const double s_width = 0.015; // Strip width (150um)	
	const double s_interpitch = s_pitch - s_width; // Distance between two consecutive strip lines 
	
	MediumMagboltz* gas = new MediumMagboltz();
	gas->SetComposition("ar", 93., "co2", 7.);
	gas->SetTemperature(293.15);
	gas->SetPressure(760.);
	gas->Initialise(true);	
	gas->LoadIonMobility("/opt/garfield/Data/IonMobility_Ar+_Ar.txt");
	
	// Building the geometry
	GeometrySimple* geo = new GeometrySimple(); 
	SolidBox* box = new SolidBox(0., h_tot/2.0, 0., w/2.0, h_tot/2.0, l/2.0); 
	geo->AddSolid(box, gas); 
	
	// Building the fields
	ComponentAnalyticField* cmpDrift = new ComponentAnalyticField();
	ComponentAnalyticField* cmpAmp = new ComponentAnalyticField();

	// Pass a pointer of the geometry class to the components. 
	cmpDrift->SetGeometry(geo);
	cmpAmp->SetGeometry(geo);

	// Now we add the planes for the electrodes with labels
	cmpDrift->AddPlaneY(h_tot, v_top, "DriftPlane");
	cmpDrift->AddPlaneY(h_amp, v_mesh, "AmpPlane");

	cmpAmp->AddPlaneY(h_amp, v_mesh, "AmpPlane");
	cmpAmp->AddPlaneY(0.0, v_bottom, "StripPlane");
	
	
	const int x_strip_count = w / s_pitch; // How many strips on x
	const int z_strip_count = l / s_pitch; // How many strips on y
	
	//Next we construct the Strips for readout of te signal, also with labels
	double x_offset = (s_pitch - s_width) / 2.0;
	double z_offset = (s_pitch - s_width) / 2.0;
	
	double x_strip_centers[x_strip_count];
	double z_strip_centers[z_strip_count];
	
	cout << "x_strip_count ="<<x_strip_count;
	cout << "z_strip_count ="<<z_strip_count;
=======
int main(int argc, char * argv[])
{
	TApplication app("micromegas", &argc, argv);
	plottingEngine.SetDefaultStyle();
	
	Sensor* sensor = new Sensor();
	ComponentAnalyticField* cmpDrift = new ComponentAnalyticField();
	ComponentAnalyticField* cmpAmp = new ComponentAnalyticField();


	sensor->AddComponent(cmpDrift);
	sensor->AddComponent(cmpAmp);
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf
	

	for (int i=0; i<x_strip_count; i++)
	{
		x_strip_centers[i] = x_offset + s_width / 2.0;
		cmpAmp->AddStripOnPlaneY('x', 0.0, x_offset, x_offset + s_width, "StripX_"+to_string(i)); 
		cmpAmp->AddReadout("StripX_"+to_string(i));
		
		x_offset += s_pitch;
	}
	
<<<<<<< HEAD
	for (int j=0; j<z_strip_count; j++)
	{
		z_strip_centers[j] = z_offset + s_width / 2.0;
		cmpAmp->AddStripOnPlaneY('z', 0.0, x_offset, x_offset + s_width, "StripZ_"+to_string(j)); 
		cmpAmp->AddReadout("StripZ_"+to_string(j));
		
		z_offset += s_pitch;
	}
=======
	build_geometry(gas, cmpDrift, cmpAmp, sensor);
	//plot_geometry(cmpDrift, cmpAmp, geo);
	
	//plot_fields(cmpDrift, cmpAmp, sensor);
	
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf
	
	
	//Finally we assemble a Sensor object 
	Sensor* sensor = new Sensor(); 
	sensor->AddComponent(cmpDrift);
	sensor->AddComponent(cmpAmp);
	for (int i=0; i<x_strip_count; i++) {
		sensor->AddElectrode(cmpAmp, "StripX_"+to_string(i));
	}
	for (int j=0; j<400; j++) {
		sensor->AddElectrode(cmpAmp, "StripZ_"+to_string(j));
	}
	sensor->SetArea(-5., 0.,-5.,5.,0.61,5.);
	// Set Time window for signal integration, units in [ns]
	const double tMin = 0.; 
	const double tMax = 100.; 
	const double tStep = 0.05; 
	const int nTimeBins = int((tMax - tMin) / tStep); 
	sensor->SetTimeWindow(0., tStep, nTimeBins);
	
	
	
	 // This canvas will be used to display the drift lines and the field
  TCanvas * c = new TCanvas("c", "c", 10, 10, 1000, 700);
  c->Divide(2,1);
  // Construct object to visualise drift lines
  ViewDrift* viewdrift = new ViewDrift();
  viewdrift->SetArea(-1.0, 0.0, -0.1, 1., 0.7,1. );
  viewdrift->SetClusterMarkerSize(0.1);
  viewdrift->SetCollisionMarkerSize(0.5);
  viewdrift->SetCanvas((TCanvas*)c->cd(1));
  
  //For simulating the electron avalanche we use the class AvalancheMicroscopic
  AvalancheMicroscopic* aval = new AvalancheMicroscopic(); 
  const int aval_size = 10;
  aval->SetSensor(sensor); 
  // Switch on signal calculation. 
  aval->EnableSignalCalculation(); 
  aval->SetTimeWindow(tMin,tMax ); 
  //  aval->EnableAvalancheSizeLimit(aval_size);
  aval->EnablePlotting(viewdrift);
  aval->EnableDriftLines();
  aval->EnableMagneticField();
  // Additional optional switches
  //aval->EnableExcitationMarkers();
  //aval->EnableIonisationMarkers();


  //Add ionizing particle using Heed
  // Here we add a negative pion with some momentum, units in [eV/c]
  const double momentum = 300.e+06; // eV/c
  TrackHeed* track = new TrackHeed();
  track->SetParticle("pi-");
  track->SetMomentum(momentum);
  track->SetSensor(sensor);
  track->EnableMagneticField();
  track->EnableElectricField();
  track->EnablePlotting(viewdrift);

  gas->SetMaxElectronEnergy(200.);
  // Cluster info
  double xcls, ycls, zcls, tcls, e, extra;
  xcls = ycls = zcls = tcls = e = extra = -999.;
  // Electron info
  double xele, yele, zele, tele, eele, dxele, dyele, dzele;
  // Electron start and endpoints, momentum direction and status
  double x0ele, y0ele, z0ele, t0ele, e0ele;// start point
  double x1ele, y1ele, z1ele, t1ele, e1ele;// endpoint
  double dx1ele, dy1ele, dz1ele; // momentum direction
  int status1ele; // status
  int n = 0; // number of electrons in cluster
  bool cls_ret;// return OK if cluster is OK

  // The initial impact position of the incoming ionising track
  double track_x = 0.15;// [cm]
  double track_y = 0.6;
  double track_z = 0.0;
  // Momentum direction of incoming track
  double track_dx = 0.0;
  double track_dy = -1.0; // Track coming downstream
  double track_dz = 0.0;

  // Now create a single track
  track->NewTrack(track_x, track_y, track_z, tMin, track_dx, track_dy, track_dz);
  cls_ret = track->GetCluster(xcls, ycls, zcls, tcls, n, e, extra);
  
  
  // To accumulate the electron signal for each strip separately
 //for (int q=0; q<400; q++)
  //{double (Qstrip+to_string(q)) = 0.0;}

  // Now loop over electrons in cluster
  for(int j = 0; j < n; j++){
    track->GetElectron(j-1, xele, yele, zele, tele, eele, dxele, dyele, dzele);

    // Simulate an avalanche for the current electron
    aval->AvalancheElectron(xele, yele, zele, tele, eele, dxele, dyele, dzele); 
  }


  // To get Signal from each strip we get the it from each sensor
  // by integrating over time
 //for(int i = 0; i < nTimeBins; i++){
	 //for(int k=0;k<400;k++)
	  //{Qstrip+to_string(k) += fabs(sensor->GetElectronSignal("Strip"+to_string(k), i));
//}}

  // Now calculate the reconstructed pion position in the Micromegas
  // using the weighted average of the Strip signals
 // double mean = 0.0;
  
  //for (q=0;q<400;q++)
  //{mean=(Qstrip+to_string(q)*Xstrip+to_string(q)
  
  
 // mean = (Qstrip1*Xstrip1 + Qstrip2*Xstrip2 + Qstrip3*Xstrip3)/(Qstrip1+Qstrip2+Qstrip3);
  //std::cout << "---------------------------" << std::endl;
  //std::cout << "XMean: " << mean << " cm, XTrue: " << track_x << " cm" << std::endl;
  //std::cout << "XTrue - XMean: " << fabs(track_x - mean)*10000.0 << " um" << std::endl;
  //std::cout << "---------------------------" << std::endl;

  // -- Plotting --

  // Now plot the drift lines
  viewdrift->Plot();

  // View the Field
  ViewField * viewfield = new ViewField();
  viewfield->SetComponent(cmpAmp);
  viewfield->SetSensor(sensor);
  viewfield->SetCanvas((TCanvas*)c->cd(2));
  viewfield->SetWeightingFieldRange(0.0, 10000.0);
  c->cd(2);
  viewfield->PlotContour();
  
  
  app.Run(kTRUE); 
  }
