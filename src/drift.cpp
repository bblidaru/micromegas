
using namespace std;
using namespace Garfield;

void plot_drift(ComponentAnsys123* fm)
{
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
	const double tMin = 0.; 
	const double tMax = 100.; 
	const double tStep = 0.05; 
	const int nTimeBins = int((tMax - tMin) / tStep); 
	sensor->SetTimeWindow(0., tStep, nTimeBins);
	
	
	
	
}
