#include <iostream>
#include <fstream>
#include <cmath>

#include <TApplication.h>
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

using namespace Garfield;

int main(int argc, char * argv[]){


  TApplication app("app", &argc, argv);
  plottingEngine.SetDefaultStyle();

  // Load the Magboltz gas file
  MediumMagboltz* gas = new MediumMagboltz();
  gas->SetComposition("ar", 93., "co2", 7.);
  gas->SetTemperature(293.15);
  gas->SetPressure(760.);
  gas->Initialise(true);	
  gas->LoadIonMobility("/opt/garfield/Data/IonMobility_Ar+_Ar.txt");

  //
  // Structure of Micromegas cell (from top to bottom):
  //------------------------
  // | Drift electrode (Vdrift = -2500 V)
  // | Drift gap (Hdriftgap = 3 mm)
  // | Mesh electrode (Vmesh = -500 V)
  // | Amplification gap (Hampgap = 100 um)
  // | Strips (Vstrip = 0 V)
  //------------------------
  //


  // 
  // We place 3 strips in z direction in order to
  // be able to resolve the avalanche signal.
  // Strips can be parametrized by width and pitch
  //

  // Units given in [cm]
  const double Hdriftgap = 0.3; // Drift gap height
  const double Hampgap = 0.01; // Amplification gap height
  const double Htot = Hdriftgap + Hampgap; // Total height
  const double Pitch = 0.07; // Pitch
  const double Interpitch = 0.01; // Distance between two strips
  const double Wstrip = Pitch - Interpitch;

  // Electrode potentials
  const double Vdrift = -3000.;
  const double Vamp = -500.;
  const double Vstrip = 0.;

  // Magnetic field 
  const double MagX = 0.;
  const double MagY = 0.;
  const double MagZ = 0.;

  // Geometry of the structure
  // Parallel planes, SolidBox, height [cm]
  // Width [cm]
  const double w = 3.0;
  // Length [cm]
  const double l = 3.0;

  GeometrySimple* geo = new GeometrySimple(); 
  SolidBox* box = new SolidBox(0.,Htot/2.0, 0., w, Htot/2.0, l); 
  geo->AddSolid(box, gas); 

  //
  // The Component requires a description of the geometry, that is a list of volumes 
  // and associated media. 
  // Here we construct 2 ComponentAnalyticalFields, to treat the
  // arift region and the amplification separately
  //
  ComponentAnalyticField* cmpDrift = new ComponentAnalyticField();
  ComponentAnalyticField* cmpAmp = new ComponentAnalyticField();


  // Pass a pointer of the geometry class to the components. 
  cmpDrift->SetGeometry(geo);
  cmpAmp->SetGeometry(geo);


  // Now we add the planes for the electrodes with labels
  cmpDrift->AddPlaneY(Htot, Vdrift, "DriftPlane");
  cmpDrift->AddPlaneY(Hampgap, Vamp, "AmpPlane");

  cmpAmp->AddPlaneY(Hampgap, Vamp, "AmpPlane");
  cmpAmp->AddPlaneY(0.0, Vstrip, "StripPlane");


  //Next we construct the Strips for readout of te signal, also with labels
  double Xoffset = 0.;
  double Xstrip1, Xstrip2, Xstrip3; // Store the center of the strips
  Xstrip1 = Xoffset + Wstrip/2.0;
  cmpAmp->AddStripOnPlaneY('z', 0.0, Xoffset, Xoffset + Wstrip, "Strip1");
  Xoffset += (Wstrip + Interpitch); Xstrip2 = Xoffset + Wstrip/2.0;
  cmpAmp->AddStripOnPlaneY('z', 0.0, Xoffset, Xoffset + Wstrip, "Strip2");
  Xoffset += (Wstrip + Interpitch); Xstrip3 = Xoffset + Wstrip/2.0;
  cmpAmp->AddStripOnPlaneY('z', 0.0, Xoffset, Xoffset + Wstrip, "Strip3");

  //We want to calculate the signal induced on the strip. 
  //We have to tell this to the ComponentAnalyticalField
  cmpAmp->AddReadout("Strip1");
  cmpAmp->AddReadout("Strip2");
  cmpAmp->AddReadout("Strip3");

  // Set constant magnetic field in [Tesla]
  cmpDrift->SetMagneticField(MagX, MagY, MagZ);

  //Finally we assemble a Sensor object 
  Sensor* sensor = new Sensor(); 
  // Calculate the electric field using the Component object cmp
  sensor->AddComponent(cmpDrift); 
  sensor->AddComponent(cmpAmp); 

  // Request signal calculation for the electrode named with labels above, 
  // using the weighting field provided by the Component object cmp. 
  sensor->AddElectrode(cmpAmp, "Strip1"); 
  sensor->AddElectrode(cmpAmp, "Strip2"); 
  sensor->AddElectrode(cmpAmp, "Strip3"); 

  // Set Time window for signal integration, units in [ns]
   double tMin = 0.; 
  const double tMax = 100.; 
  const double tStep = 0.05; 
  const int nTimeBins = int((tMax - tMin) / tStep); 
  sensor->SetTimeWindow(0., tStep, nTimeBins);


  // This canvas will be used to display the drift lines and the field
  TCanvas * c = new TCanvas("c", "c", 10, 10, 1000, 700);
  c->Divide(2,1);
  // Construct object to visualise drift lines
  ViewDrift* viewdrift = new ViewDrift();
  viewdrift->SetArea(-0.2, 0.0, -0.1, 0.2, Htot,0.1 );
  viewdrift->SetClusterMarkerSize(0.1);
  viewdrift->SetCollisionMarkerSize(0.5);
  viewdrift->SetCanvas((TCanvas*)c->cd(1));
  
  //For simulating the electron avalanche we use the class AvalancheMicroscopic
  AvalancheMicroscopic* aval = new AvalancheMicroscopic(); 
  const int aval_size = 0.1;
  aval->SetSensor(sensor); 
  // Switch on signal calculation. 
  aval->EnableSignalCalculation(); 
  aval->SetTimeWindow(tMin,tMax ); 
  aval->EnableAvalancheSizeLimit(aval_size);
  aval->EnablePlotting(viewdrift);
  aval->EnableDriftLines();
  aval->EnableMagneticField();
  // Additional optional switches
  //aval->EnableExcitationMarkers();
  //aval->EnableIonisationMarkers();


  //Add ionizing particle using Heed
  // Here we add a negative pion with some momentum, units in [eV/c]
  const double energy = 170.e+09; // eV/c
  TrackHeed* track = new TrackHeed();
  track->SetParticle("muon");
  track->SetEnergy(energy);
  track->SetSensor(sensor);
  track->EnableMagneticField();
  track->EnableElectricField();
  track->EnablePlotting(viewdrift);
  
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
  bool cls_ret2;
  bool cls_ret3;
  bool cls_ret4;

  // The initial impact position of the incoming ionising track
  double track_x = 0.15;// [cm]
  double track_y = 0.3;
  double track_z = 0.0;
  // Momentum direction of incoming track
  double track_dx = -0.5;
  double track_dy = -0.5; // Track coming downstream
  double track_dz = 0.0;







track->DisableDeltaElectronTransport();
do
{


  // Now create a single track
  track->NewTrack(track_x, track_y, track_z, tMin, track_dx, track_dy, track_dz);
  cls_ret = track->GetCluster(xcls, ycls, zcls, tcls, n, e, extra);
  
  std::cout<<"ycls="<<ycls<<std::endl;
  std::cout<<"ncls="<<n<<std::endl;
  std::cout<<"ecls="<<e<<std::endl;

  // Now loop over electrons in cluster
  for(int j = 1; j <= n; j++){
    track->GetElectron(j-1, xele, yele, zele, tele, eele, dxele, dyele, dzele);

    // Simulate an avalanche for the current electron
    aval->AvalancheElectron(xele, yele, zele, tele, eele, dxele, dyele, dzele); 
  }
  
  
  track_x=xcls;
   track_y=ycls;
    track_z=zcls;
     tMin=tcls;
  
    std::cout<<"ycls="<<ycls<<std::endl;
    
    
} while (track_y> 0 && cls_ret);
  
  //track->NewTrack(track_x, track_y, track_z, tMin, track_dx, track_dy, track_dz);
  //cls_ret2 = track->GetCluster(xcls, ycls, zcls, tcls, n, e, extra);
  
  //// Now loop over electrons in cluster
  //for(int j = 1; j <= n; j++){
    //track->GetElectron(j-1, xele, yele, zele, tele, eele, dxele, dyele, dzele);

    //// Simulate an avalanche for the current electron
    //aval->AvalancheElectron(xele, yele, zele, tele, eele, dxele, dyele, dzele); 
  //}
  
 //track_x=xcls;
   //track_y=ycls;
    //track_z=zcls;
     //tMin=tcls;
  
  
    //std::cout<<"ycls="<<ycls<<std::endl;

  
  
  //track->NewTrack(track_x, track_y, track_z, tMin, track_dx, track_dy, track_dz);
  //cls_ret3 = track->GetCluster(xcls, ycls, zcls, tcls, n, e, extra);
  
  //// Now loop over electrons in cluster
  //for(int j = 1; j <= n; j++){
    //track->GetElectron(j-1, xele, yele, zele, tele, eele, dxele, dyele, dzele);

    //// Simulate an avalanche for the current electron
    //aval->AvalancheElectron(xele, yele, zele, tele, eele, dxele, dyele, dzele); 
  //}
  
 //track_x=xcls;
   //track_y=ycls;
    //track_z=zcls;
     //tMin=tcls;
  
  
    //std::cout<<"ycls="<<ycls<<std::endl;

  
  
  //track->NewTrack(track_x, track_y, track_z, tMin, track_dx, track_dy, track_dz);
  //cls_ret4 = track->GetCluster(xcls, ycls, zcls, tcls, n, e, extra);
  
  //// Now loop over electrons in cluster
  //for(int j = 1; j <= n; j++){
    //track->GetElectron(j-1, xele, yele, zele, tele, eele, dxele, dyele, dzele);

    //// Simulate an avalanche for the current electron
    //aval->AvalancheElectron(xele, yele, zele, tele, eele, dxele, dyele, dzele); 
  //}
  
  
  
  
  
  
  
  
  // To accumulate the electron signal for each strip separately
  double Qstrip1 = 0.0; double Qstrip2 = 0.0; double Qstrip3 = 0.0;

  // To get Signal from each strip we get the it from each sensor
  // by integrating over time
  for(int i = 0; i < nTimeBins; i++){
    Qstrip1 += fabs(sensor->GetElectronSignal("Strip1", i));
    Qstrip2 += fabs(sensor->GetElectronSignal("Strip2", i));
    Qstrip3 += fabs(sensor->GetElectronSignal("Strip3", i));
  }    

  // Now calculate the reconstructed pion position in the Micromegas
  // using the weighted average of the Strip signals
  double mean = 0.0;
  mean = (Qstrip1*Xstrip1 + Qstrip2*Xstrip2 + Qstrip3*Xstrip3)/(Qstrip1+Qstrip2+Qstrip3);
  std::cout << "---------------------------" << std::endl;
  std::cout << "XMean: " << mean << " cm, XTrue: " << track_x << " cm" << std::endl;
  std::cout << "XTrue - XMean: " << fabs(track_x - mean)*10000.0 << " um" << std::endl;
  std::cout << "---------------------------" << std::endl;

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


  // View the strip signals
  TCanvas * c2 = new TCanvas("c2", "c2", 1000, 20, 700, 500);
  ViewSignal* signalView = new ViewSignal();
  signalView->SetSensor(sensor);
  signalView->SetCanvas(c2);
  if(Qstrip1 > 0)signalView->PlotSignal("Strip1");
  if(Qstrip2 > 0)signalView->PlotSignal("Strip2");
  if(Qstrip3 > 0)signalView->PlotSignal("Strip3");


  app.Run();
  return 0;

}
