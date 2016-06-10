#include <iostream>
#include <fstream>
#include <cmath>
#include <getopt.h>
#include <limits>

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
using namespace std;


typedef enum {
	MODE_FIX,
	MODE_POS_RND_DIR_PERP,
	MODE_POS_RND_DIR_RND
} TypeMode;

int main(int argc, char * argv[])
{

	string output_file = "data.txt";
	
	typedef std::numeric_limits< double > dbl;
	cout.precision(dbl::max_digits10);

	double gas_pressure = 760.;
	double gas_temperature = 273.15;
	double particle_energy = 4.e9; // eV/c

	double top_voltage = -180.;
	double mesh_voltage = 0.;
	double bottom_voltage = 500; 

	double conc_ar = 93.;
	double conc_co2 = 7.;

	int cp;

	while (1)
	{
		static struct option long_options[] =
		{
			/* These options set a flag. */
			// {"verbose", no_argument,       &verbose_flag, 1},
			// {"brief",   no_argument,       &verbose_flag, 0},
			/* These options don’t set a flag.
			We distinguish them by their indices. */
			{"output_file",      required_argument,       0, 'o'},
			{"gas_pressure",     required_argument,       0, 'p'},
			{"gas_temperature",  required_argument,       0, 't'},
			{"particle_energy",  required_argument, 		0, 'e'},
			{"top_voltage",      required_argument, 		0, 'x'},
			{"mesh_voltage",     required_argument, 		0, 'y'},
			{"bottom_voltage",   required_argument, 		0, 'z'},
			{"conc_ar",          required_argument, 		0, 'a'},
			{"conc_co2",         required_argument, 		0, 'c'},

			{0, 0, 0, 0}
		};
		
		/* getopt_long stores the option index here. */
		int option_index = 0;

		cp = getopt_long (argc, argv, "p:t:e:o:x:y:z:a:c:", long_options, &option_index);

		/* Detect the end of the options. */
		if (cp == -1)
			break;

		switch (cp)
		{
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
				break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
				printf (" with arg %s", optarg);
				printf ("\n");
			break;


			case 'o':
				printf ("Setting output file  to `%s'\n", optarg);
				output_file = string(optarg);
			break;

			case 'p':
				printf ("Setting gas pressure to `%s'\n", optarg);
				gas_pressure = stod(optarg);
			break;

			case 't':
				printf ("Setting gas temperature to `%s'\n", optarg);
				gas_temperature = stod(optarg);
			break;

			case 'e':
				printf ("Setting particle energy to `%s'\n", optarg);
				particle_energy = stod(optarg);
			break;

			case 'x':
				printf ("Setting top voltage  to `%s'\n", optarg);
				top_voltage = stod(optarg);
			break;

			case 'y':
				printf ("Setting mesh voltage  to `%s'\n", optarg);
				mesh_voltage = stod(optarg);
			break;

			case 'z':
				printf ("Setting bottom voltage  to `%s'\n", optarg);
				bottom_voltage = stod(optarg);
			break;

			case 'a':
				printf ("Setting Ar conc  to `%s'\n", optarg);
				conc_ar = stod(optarg);
			break;

			case 'c':
				printf ("Setting CO2 conc  to `%s'\n", optarg);
				conc_co2 = stod(optarg);
			break;


			case '?':
				/* getopt_long already printed an error message. */
			break;

			default:
				abort ();
			break;
		}
	}


	///* Print any remaining command line arguments (not options). */
	//if (optind < argc)
	//{
	//printf ("non-option ARGV-elements: ");
	//while (optind < argc)
	//printf ("%s ", argv[optind++]);
	//putchar ('\n');
	//}



	TApplication app("app", &argc, argv);
	plottingEngine.SetDefaultStyle();


	//


	// Load the Magboltz gas file
	MediumMagboltz* gas = new MediumMagboltz();
	gas->SetComposition("ar", conc_ar, "co2", conc_co2);

	gas->SetTemperature(gas_temperature);
	gas->SetPressure(gas_pressure);
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
	const double Hampgap = 0.0128; // Amplification gap height
	const double Htot = Hdriftgap + Hampgap; // Total height
	const double Pitch = 0.07; // Pitch
	const double Interpitch = 0.01; // Distance between two strips
	const double Wstrip = Pitch - Interpitch;

	// Electrode potentials
	const double Vdrift = top_voltage;
	const double Vamp = mesh_voltage;
	const double Vstrip = bottom_voltage;


	// Magnetic field 
	const double MagX = 0.;
	const double MagY = 0.;
	const double MagZ = 0.;

	// Geometry of the structure
	// Parallel planes, SolidBox, height [cm]
	// Width [cm]
	const double w = 3.0;  // x
	// Length [cm]
	const double l = 3.0;  // y

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
	double Xoffset = -w;


	int n_strips_x =  w*2/Pitch;

	double Xstrips[n_strips_x]; 


	//Finally we assemble a Sensor object 
	Sensor* sensor = new Sensor(); 
	// Calculate the electric field using the Component object cmp
	sensor->AddComponent(cmpDrift); 
	sensor->AddComponent(cmpAmp); 


	for(int j=0; j<n_strips_x; j++)
	{	
		const char* name =   ("Strip"+to_string(j)).c_str();
		Xstrips[j]=Xoffset + Wstrip/2.0;
		cmpAmp->AddStripOnPlaneY('z', 0.0, Xoffset, Xoffset + Wstrip, name);
		Xoffset += Pitch;
		cmpAmp->AddReadout(name);
		sensor->AddElectrode(cmpAmp, name);
	}


	//Xstrip1 = Xoffset + Wstrip/2.0;
	//cmpAmp->AddStripOnPlaneY('z', 0.0, Xoffset, Xoffset + Wstrip, "Strip1");
	//Xoffset += (Wstrip + Interpitch); 
	//Xstrip2 = Xoffset + Wstrip/2.0;
	//cmpAmp->AddStripOnPlaneY('z', 0.0, Xoffset, Xoffset + Wstrip, "Strip2");
	//Xoffset += (Wstrip + Interpitch); 
	//Xstrip3 = Xoffset + Wstrip/2.0;
	//cmpAmp->AddStripOnPlaneY('z', 0.0, Xoffset, Xoffset + Wstrip, "Strip3");

	//We want to calculate the signal induced on the strip. 
	//We have to tell this to the ComponentAnalyticalField
	//cmpAmp->AddReadout("Strip1");
	//cmpAmp->AddReadout("Strip2");
	//cmpAmp->AddReadout("Strip3");

	// Set constant magnetic field in [Tesla]
	cmpDrift->SetMagneticField(MagX, MagY, MagZ);



	// Request signal calculation for the electrode named with labels above, 
	// using the weighting field provided by the Component object cmp. 
	//sensor->AddElectrode(cmpAmp, "Strip1"); 
	//sensor->AddElectrode(cmpAmp, "Strip2"); 
	//sensor->AddElectrode(cmpAmp, "Strip3"); 

	// Set Time window for signal integration, units in [ns]
	double tMin = 0.; 
	const double tMax = 100.; 
	const double tStep = 0.05; 
	const int nTimeBins = int((tMax - tMin) / tStep); 
	sensor->SetTimeWindow(0., tStep, nTimeBins);
	
	
	double Ttime[nTimeBins];
	
	for (int i = 0; i < nTimeBins; i++)
		Ttime[i]= i * tStep;

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
	//const int aval_size = 0.1;
	aval->SetSensor(sensor); 
	// Switch on signal calculation. 
	aval->EnableSignalCalculation(); 
	aval->SetTimeWindow(tMin,tMax ); 
	//aval->EnableAvalancheSizeLimit(aval_size);
	aval->EnablePlotting(viewdrift);
	aval->EnableDriftLines();
	aval->EnableMagneticField();
	// Additional optional switches
	//aval->EnableExcitationMarkers();
	//aval->EnableIonisationMarkers();


	//Add ionizing particle using Heed
	// Here we add a negative pion with some momentum, units in [eV/c]

	TrackHeed* track = new TrackHeed();
	track->SetParticle("muon");
	track->SetEnergy(particle_energy);
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


	TypeMode mode = MODE_POS_RND_DIR_PERP;

	switch (mode)
	{
		default:
		case MODE_FIX:
		{
			track_x = 0.15;
			track_y = 0.3;
			track_z = 0.0;
			track_dx = -0.5;
			track_dy = -0.5;
			track_dz = 0.0;
			break;
		}

		case MODE_POS_RND_DIR_PERP:
		{
			double r = ((double) rand() / (RAND_MAX));
			track_x = (r-0.5)*2*w;
			track_y = 0.3;
			track_z = 0.0;
			track_dx = 0.0;
			track_dy = -1.;
			track_dz = 0.0;

			std::cout<<"random position for incoming particle; x="<<track_x<<std::endl;
			break;
		}

		case MODE_POS_RND_DIR_RND:
		{
			double rr = ((double) rand() / (RAND_MAX));
			track_x = (rr-0.5)*2*w;
			track_y = 0.3;
			track_z = 0.0;

			double angle = (((double) rand() / (RAND_MAX)) -0.5 ) * M_PI;

			track_dx = sin (angle);
			track_dy = -cos (angle);
			track_dz = 0.0;

			std::cout<<"random position for incoming particle; x="<<track_x<<std::endl;
			std::cout<<"random angle for incoming particle; angle="<<angle/M_PI*180.<<std::endl;
			break;
		}
	}

	int n_e, n_i;


	int tot_ncls = 0;
	double tot_ecls = 0;
	int tot_n_e_aval = 0, tot_n_i_aval = 0;
	int q=0;

	// To accumulate the electron signal for all time for each strip separately
	double QstripsTot[n_strips_x];
	
	for (int j=0; j<n_strips_x; j++)
	{	
		QstripsTot[j]=0;
	}
	
	// To accumulate the electron signal for all strip for each time separately
	double QtimeTot[nTimeBins];
	
	for (int i = 0; i < nTimeBins; i++)
	{
		QtimeTot[i] = 0;
	}



	int tot_nElastic=0, tot_nIonising=0, tot_nAttachment=0, tot_nInelastic=0, tot_nExcitation=0, tot_nSuperelastic=0;

	track->DisableDeltaElectronTransport();
	
	do
	{


		// Now create a single track
		track->NewTrack(track_x, track_y, track_z, tMin, track_dx, track_dy, track_dz);
		cls_ret = track->GetCluster(xcls, ycls, zcls, tcls, n, e, extra);

		std::cout<<"ycls="<<ycls<<std::endl;
		std::cout<<"ncls="<<n<<std::endl; // primary electrons generated in current interaction
		std::cout<<"ecls="<<e<<std::endl; // transferred energy from particle to electrons

		tot_ncls += n; // no of primary electrons generated in detector
		tot_ecls += e; // total energy lost by particle in detector 

		// Now loop over electrons in cluster
		for(int j = 1; j <= n; j++)
		{
			track->GetElectron(j-1, xele, yele, zele, tele, eele, dxele, dyele, dzele);

			// Simulate an avalanche for the current electron
			aval->AvalancheElectron(xele, yele, zele, tele, eele, dxele, dyele, dzele); 
			aval->GetAvalancheSize(n_e, n_i);
			std::cout<<"n_e="<<n_e<<std::endl; // no of electrons generated in current interaction
			std::cout<<"n_i="<<n_i<<std::endl; // no of ions generated in current interaction
			tot_n_e_aval += n_e; // no of electrons generated in total
			tot_n_i_aval += n_i; // no of electrons generated in total
		}


		track_x=xcls;
		track_y=ycls;
		track_z=zcls;
		tMin=tcls;




		int nElastic;
		int nIonising;
		int nAttachment;
		int nInelastic;
		int nExcitation;
		int nSuperelastic;
		gas-> GetNumberOfElectronCollisions(nElastic,nIonising,nAttachment,nInelastic,nExcitation,nSuperelastic);
		std::cout<<"nElastic="<<nElastic<<std::endl;
		std::cout<<"nIonising="<<nIonising<<std::endl;
		std::cout<<"nAttachment="<<nAttachment<<std::endl;
		std::cout<<"nInelastic="<<nInelastic<<std::endl;
		std::cout<<"nExcitation="<<nExcitation<<std::endl;
		std::cout<<"nSuperelastic="<<nSuperelastic<<std::endl;


		tot_nElastic += nElastic;
		tot_nIonising += nIonising;
		tot_nAttachment += nAttachment;
		tot_nInelastic += nInelastic;
		tot_nExcitation += nExcitation;
		tot_nSuperelastic += nSuperelastic;

		// To get Signal from each strip we get the it from each sensor
		// by integrating over time

		for (int j=0; j<n_strips_x; j++)
		{	
			const char* name =   ("Strip"+to_string(j)).c_str();

			for(int i = 0; i < nTimeBins; i++)
			{
				double value = (sensor->GetElectronSignal(name, i));
				QstripsTot[j] += value;
				QtimeTot[i] += value;
			}    
		}




		// Now calculate the reconstructed pion position in the Micromegas
		// using the weighted average of the Strip signals
		double x_mean = 0.0;
		double x_sum_a = 0;
		double x_sum_b = 0;
		for (int j=0; j<n_strips_x; j++)
		{
			x_sum_a += QstripsTot[j] * Xstrips[j];
			x_sum_b += QstripsTot[j];
		}

		x_mean = x_sum_a/x_sum_b;
		
		std::cout << "---------------------------" << std::endl;
		std::cout << "XMean: " << x_mean << " cm, XTrue: " << track_x << " cm" << std::endl;
		std::cout << "XTrue - XMean: " << fabs(track_x - x_mean)*10000.0 << " um" << std::endl;
		std::cout << "---------------------------" << std::endl;



		// Now calculate the reconstructed pion time in the Micromegas
		// using the weighted average of the Strip signals
		double t_mean = 0.0;
		double t_sum_a = 0;
		double t_sum_b = 0;
		for (int i=0; i<nTimeBins; i++)
		{
			t_sum_a += QtimeTot[i] * Ttime[i];
			t_sum_b += QtimeTot[i];
		}

		t_mean = t_sum_a/t_sum_b;
		
		std::cout << "---------------------------" << std::endl;
		std::cout << "TMean: " << x_mean << " ns, TTrue: " << 0 << " ns" << std::endl;
		std::cout << "TTrue - TMean: " << fabs(0 - t_mean) << " ns" << std::endl;
		std::cout << "---------------------------" << std::endl;






		std::cout<<"ycls="<<ycls<<std::endl;

		if (cls_ret > 0)
			q++;

	} while (track_y> 0 && cls_ret > 0);

	std::cout<<"tot_nElastic="<<tot_nElastic<<std::endl;
	std::cout<<"tot_nIonising="<<tot_nIonising<<std::endl;
	std::cout<<"tot_nAttachment="<<tot_nAttachment<<std::endl;
	std::cout<<"tot_nInelastic="<<tot_nInelastic<<std::endl;
	std::cout<<"tot_nExcitation="<<tot_nExcitation<<std::endl;
	std::cout<<"tot_nSuperelastic="<<tot_nSuperelastic<<std::endl;









	std::ofstream myfile;
	myfile.open (output_file, std::ios::app	);


	if (myfile.tellp() <= 10)
	{
		myfile << "# pressure, temperature, conc_Ar, conc_CO2, top_voltage, mesh_voltage, bottom_voltage, particle_energy, tot_ncls, tot_ecls, tot_n_e_aval, tot_n_i_aval, tnElastic, tnIonising, tnAttachment, tnInelastic, tnExcitation, tnSuperelastic, nr_interactions,";
		
		for (int j=0; j < n_strips_x; j++)
			myfile << "QStrip" << j << " (x:" << Xstrips[j] << "),";
		 
		for (int i=0; i < nTimeBins; i++)
			myfile << "QTime" << i << " (t:" << Ttime[i] << "),";
		 
		 
		 myfile << endl;
	}

	myfile 
		// Initial GAS
		<< gas_pressure << " , " 
		<< gas_temperature << " , "
		<< conc_ar << " , "
		<< conc_co2 << " , "

		// voltage

		<< top_voltage << " , "
		<< mesh_voltage << " , "
		<< bottom_voltage << " , "

		// Initial Particle
		<< particle_energy << " , "

		// Final state

		<< tot_ncls << " , " 
		<< tot_ecls << " , " 
		<< tot_n_e_aval << " , " 
		<< tot_n_i_aval << " , " 
		<< tot_nElastic << " , " 
		<< tot_nIonising << " , " 
		<< tot_nAttachment << " , " 
		<< tot_nInelastic << " , " 
		<< tot_nExcitation << " , " 
		<< tot_nSuperelastic <<  " , "
		<< q << " , ";


	for (int j=0; j<n_strips_x; j++)
		myfile << QstripsTot[j] << " , " ;

	for (int i=0; i < nTimeBins; i++)
		myfile << QtimeTot[i] << " , " ;

	myfile << std::endl;



	myfile.close();
	// -- Plotting --

	// Now plot the drift lines
	//viewdrift->Plot();

	//// View the Field
	//ViewField * viewfield = new ViewField();
	//viewfield->SetComponent(cmpAmp);
	//viewfield->SetSensor(sensor);
	//viewfield->SetCanvas((TCanvas*)c->cd(2));
	//viewfield->SetWeightingFieldRange(0.0, 10000.0);
	//c->cd(2);
	//viewfield->PlotContour();


	//// View the strip signals
	//TCanvas * c2 = new TCanvas("c2", "c2", 1000, 20, 700, 500);
	//ViewSignal* signalView = new ViewSignal();
	//signalView->SetSensor(sensor);
	//signalView->SetCanvas(c2);
	//if(Qstrip1 > 0)signalView->PlotSignal("Strip1");
	//if(Qstrip2 > 0)signalView->PlotSignal("Strip2");
	//if(Qstrip3 > 0)signalView->PlotSignal("Strip3");


	//app.Run();
	return 0;

}
