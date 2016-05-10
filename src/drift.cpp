#include <MediumMagboltz.hh>
#include <AvalancheMicroscopic.hh>
#include <ViewDrift.hh>
#include <TCanvas.h>
#include <TrackHeed.hh>
#include <ComponentAnsys123.hh>
#include <Sensor.hh>


using namespace std;
using namespace Garfield;

void plot_drift(ComponentAnsys123* fm)
<<<<<<< HEAD
{
=======
{	
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf
	// This canvas will be used to display the drift lines and the field
	TCanvas * c = new TCanvas("c", "c", 10, 10, 1000, 700);
	c->Divide(2,1);
	// Construct object to visualise drift lines
	ViewDrift* viewdrift = new ViewDrift();
	viewdrift->SetArea(-0.02555,-0.0128,-0.02555, 0.02555, 0.0980,0.02555); //6 param
	viewdrift->SetClusterMarkerSize(0.1);
	viewdrift->SetCollisionMarkerSize(0.5);
	viewdrift->SetCanvas((TCanvas*)c->cd(1));
	Sensor* sensor = new Sensor(); 
	//For simulating the electron avalanche we use the class AvalancheMicroscopic
	AvalancheMicroscopic* aval = new AvalancheMicroscopic(); 
	//const int aval_size = 10;
	aval->SetSensor(sensor); 
	
	const double tMin = 0.; 
	const double tMax = 100.; 
	const double tStep = 0.05; 
	const int nTimeBins = int((tMax - tMin) / tStep); 
	sensor->SetTimeWindow(0., tStep, nTimeBins);
	
	// Switch on signal calculation. 
	aval->EnableSignalCalculation(); 
	aval->SetTimeWindow(tMin,tMax ); 
	//  aval->EnableAvalancheSizeLimit(aval_size);
	aval->EnablePlotting(viewdrift);
	aval->EnableDriftLines();
	//aval->EnableMagneticField();
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
	//track->EnableMagneticField();
	track->EnableElectricField();
	track->EnablePlotting(viewdrift);

	// Cluster info
	double xcls, ycls, zcls, tcls, e, extra;
	xcls = ycls = zcls = tcls = e = extra = -999.;
	// Electron info
	double xele, yele, zele, tele, eele, dxele, dyele, dzele;
	// Electron start and endpoints, momentum direction and status
<<<<<<< HEAD
	double x0ele, y0ele, z0ele, t0ele, e0ele;// start point
	double x1ele, y1ele, z1ele, t1ele, e1ele;// endpoint
	double dx1ele, dy1ele, dz1ele; // momentum direction
	int status1ele; // status
=======
	//double x0ele, y0ele, z0ele, t0ele, e0ele;// start point
	//double x1ele, y1ele, z1ele, t1ele, e1ele;// endpoint
	//double dx1ele, dy1ele, dz1ele; // momentum direction
	//int status1ele; // status
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf
	int n = 0; // number of electrons in cluster
	bool cls_ret;// return OK if cluster is OK

	// The initial impact position of the incoming ionising track
	double track_x = 0.;// [cm]
	double track_y = 0.9;
	double track_z = 0.;
	// Momentum direction of incoming track
	double track_dx = 0.0;
	double track_dy = -1.0; // Track coming downstream
	double track_dz = 0.0;

	// Now create a single track
	track->NewTrack(track_x, track_y, track_z, tMin, track_dx, track_dy, track_dz);
	cls_ret = track->GetCluster(xcls, ycls, zcls, tcls, n, e, extra);

	// To accumulate the electron signal for each strip separately
<<<<<<< HEAD
	double Qstrip1 = 0.0; double Qstrip2 = 0.0; double Qstrip3 = 0.0;

	// Now loop over electrons in cluster
	for(int j = 1; j <= n; j++){
	track->GetElectron(j-1, xele, yele, zele, tele, eele, dxele, dyele, dzele);

	// Simulate an avalanche for the current electron
	aval->AvalancheElectron(xele, yele, zele, tele, eele, dxele, dyele, dzele); 
=======
	//double Qstrip1 = 0.0; double Qstrip2 = 0.0; double Qstrip3 = 0.0;

	// Now loop over electrons in cluster
	for(int j = 1; j <= n; j++)
	{
		track->GetElectron(j-1, xele, yele, zele, tele, eele, dxele, dyele, dzele);
		// Simulate an avalanche for the current electron
		aval->AvalancheElectron(xele, yele, zele, tele, eele, dxele, dyele, dzele); 
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf
	}

	// To get Signal from each strip we get the it from each sensor
	// by integrating over time
	//for(int i = 0; i < nTimeBins; i++){
	//Qstrip1 += fabs(sensor->GetElectronSignal("Strip1", i));
	//Qstrip2 += fabs(sensor->GetElectronSignal("Strip2", i));
	//Qstrip3 += fabs(sensor->GetElectronSignal("Strip3", i));
	//}    

	// Now calculate the reconstructed pion position in the Micromegas
	// using the weighted average of the Strip signals
	//double mean = 0.0;
	//mean = (Qstrip1*Xstrip1 + Qstrip2*Xstrip2 + Qstrip3*Xstrip3)/(Qstrip1+Qstrip2+Qstrip3);
	//std::cout << "---------------------------" << std::endl;
	//std::cout << "XMean: " << mean << " cm, XTrue: " << track_x << " cm" << std::endl;
	//std::cout << "XTrue - XMean: " << fabs(track_x - mean)*10000.0 << " um" << std::endl;
	//std::cout << "---------------------------" << std::endl;

}
