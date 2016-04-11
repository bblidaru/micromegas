#include <MediumMagboltz.hh>
#include <GeometrySimple.hh>
#include <ComponentAnalyticField.hh>
#include <SolidBox.hh>
#include <iostream>
#include <MediumSilicon.hh>
#include <SolidTube.hh>

#include <ViewGeometry.hh>
#include <Sensor.hh>

using namespace std;
using namespace Garfield;

GeometrySimple* build_geometry(MediumMagboltz* gas, ComponentAnalyticField* cmpDrift, ComponentAnalyticField* cmpAmp, Sensor* sensor, GeometrySimple* geo)
{
	
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
	
	
	
	// Building the geometry
	
	SolidBox* box = new SolidBox(0., h_tot/2.0, 0., w/2.0, h_tot/2.0, l/2.0); 
	geo->AddSolid(box, gas); 
	

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
	double x_strip_offset = -w/2 + (s_pitch - s_width) / 2.0;
	double z_strip_offset = -l/2 + (s_pitch - s_width) / 2.0;
	
	double x_strip_centers[x_strip_count];
	double z_strip_centers[z_strip_count];
	
	
	
	for (int i=0; i<x_strip_count; i++)
	{
		x_strip_centers[i] = x_strip_offset + s_width / 2.0;
		cmpAmp->AddStripOnPlaneY('x', 0.0, x_strip_offset, x_strip_offset + s_width, "StripX_"+to_string(i)); 
		cmpAmp->AddReadout("StripX_"+to_string(i));
		
		x_strip_offset += s_pitch;
	}
	
	for (int j=0; j<z_strip_count; j++)
	{
		z_strip_centers[j] = z_strip_offset + s_width / 2.0;
		cmpAmp->AddStripOnPlaneY('z', 0.0, x_strip_offset, x_strip_offset + s_width, "StripZ_"+to_string(j)); 
		cmpAmp->AddReadout("StripZ_"+to_string(j));
		
		z_strip_offset += s_pitch;
	}

	// Mesh wires
	Medium* silicon = new MediumSilicon();
	
	const int x_mesh_count = w / m_pitch; // How many tubes on x
	const int z_mesh_count = l / m_pitch; // How many tubes on y
	
	cout <<"x_mesh_count="<<x_mesh_count<<endl;
	cout <<"z_mesh_count="<<z_mesh_count<<endl;

	double x_mesh_offset = -w/2;
	double z_mesh_offset = -l/2;
	
	for(int k=0; k<x_mesh_count; k++)
	{
		SolidTube* tube = new SolidTube(x_mesh_offset, h_amp, 0., 0., m_thickness, l/2, 0., 0. ,1.);
		geo->AddSolid(tube, silicon);
		
		x_mesh_offset += m_pitch;
	}

	for(int q=0; q<z_mesh_count; q++)
	{
		SolidTube* tube = new SolidTube(0., h_amp, z_mesh_offset, 0., m_thickness, w/2, 1., 0. ,0.);
		geo->AddSolid(tube, silicon);
		
		z_mesh_offset += m_pitch;
	}




	//Finally we assemble a Sensor object 
	
	for (int i=0; i<x_strip_count; i++) {
		sensor->AddElectrode(cmpAmp, "StripX_"+to_string(i));
	}
	for (int j=0; j<z_strip_count; j++) {
		sensor->AddElectrode(cmpAmp, "StripZ_"+to_string(j));
	}
	sensor->SetArea(-w/2, 0.,-l/2.,w/2,h_tot,l/2);
	
	// Set Time window for signal integration, units in [ns]
	const double tMin = 0.; 
	const double tMax = 100.; 
	const double tStep = 0.05; 
	const int nTimeBins = int((tMax - tMin) / tStep); 
	sensor->SetTimeWindow(0., tStep, nTimeBins);

	// Set constant magnetic field in [Tesla]
	//cmpDrift->SetMagneticField(MagX, MagY, MagZ);
	return geo;
}
