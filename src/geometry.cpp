#include <MediumMagboltz.hh>
#include <GeometrySimple.hh>
#include <ComponentAnalyticField.hh>
#include <SolidBox.hh>
#include <iostream>
#include <MediumSilicon.hh>
#include <SolidTube.hh>
#include <TGeoManager.h>
#include <TGeoToStep.h>
#include <TGeoCompositeShape.h>

#include <ViewGeometry.hh>
#include <Sensor.hh>

using namespace std;
using namespace Garfield;


#define MESH_REDUCING_FACTOR 100

TGeoManager* build_geometry(MediumMagboltz* gas, ComponentAnalyticField* cmpDrift, ComponentAnalyticField* cmpAmp, Sensor* sensor)
{
	
	// Width [cm]
	const double w = 10.0; // X length
	// Length [cm]
	const double l = 10.0; // Y length
	// Height [cm]
	const double h_plate_top = 0.002; // Top plate height
	const double h_drift = 0.6; // Drift gap height
	const double h_mesh = 0.0003; // Mesh height
	const double h_amp = 0.0128; // Amplification gap height
	const double h_plate_bottom = 0.0005; // Bottom plate height
	const double h_tot = h_plate_top + h_drift + h_mesh + h_amp + h_plate_bottom; // Total height
	cout<<"h_tot="<<h_tot;
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
	
	// ROOT GEOMETRY
	TGeoManager *geom = new TGeoManager("Micromegas"," Geometry using ROOT");
	
	//--- define some materials
	TGeoMaterial *matGas = new TGeoMaterial("Gas", 0, 0, 0);
	TGeoMaterial *matAl = new TGeoMaterial("Al", 26.98,13,2.7);
	TGeoMaterial *matNi = new TGeoMaterial("Ni", 58, 28, 8.9);
	TGeoMaterial *matCu = new TGeoMaterial("Cu", 52, 24, 8.96);
	
	//   //--- define some media
	TGeoMedium *Gas= new TGeoMedium("Gas",1, matGas);
	TGeoMedium *Al = new TGeoMedium("Al",2, matAl);
	TGeoMedium *Ni = new TGeoMedium("Ni",3, matNi);
	TGeoMedium *Cu = new TGeoMedium("Cu",4, matCu);
	
	TGeoVolume *top = geom->MakeBox("TOP", Gas, w/2, l/2, h_tot/2);
    geom->SetTopVolume(top);
	geom->SetTopVisible();
	
	
	TGeoVolume *volume_gas = geom->MakeBox("GAS", Gas, w/2, l/2, h_tot/2);
	volume_gas->SetLineColor(kRed);
	
	TGeoVolume *plate_top = geom->MakeBox("PLATE_TOP", Al, w/2, l/2, h_plate_top/2);
    plate_top->SetLineColor(kBlue);
	top->AddNode(plate_top,1,new TGeoTranslation(0, 0,0.3068));
	
	TGeoVolume *plate_bottom = geom->MakeBox("PLATE_BOTTOM", Cu, w/2, l/2, h_plate_bottom/2);
    plate_bottom->SetLineColor(kGreen);
	top->AddNode(plate_bottom, 2, new TGeoTranslation(0, 0, -0.3068));
	
	
	
	
	
	//const int x_strip_count = w / s_pitch; // How many strips on x
	//const int z_strip_count = l / s_pitch; // How many strips on y
	
	////Next we construct the Strips for readout of te signal, also with labels
	//double x_strip_offset = -w/2 + (s_pitch - s_width) / 2.0;
	//double z_strip_offset = -l/2 + (s_pitch - s_width) / 2.0;
	
	//double x_strip_centers[x_strip_count];
	//double z_strip_centers[z_strip_count];
	
	
	
	//for (int i=0; i<x_strip_count; i++)
	//{
		//x_strip_centers[i] = x_strip_offset + s_width / 2.0;
		//cmpAmp->AddStripOnPlaneY('x', 0.0, x_strip_offset, x_strip_offset + s_width, "StripX_"+to_string(i)); 
		//cmpAmp->AddReadout("StripX_"+to_string(i));
		
		//x_strip_offset += s_pitch;
	//}
	
	//for (int j=0; j<z_strip_count; j++)
	//{
		//z_strip_centers[j] = z_strip_offset + s_width / 2.0;
		//cmpAmp->AddStripOnPlaneY('z', 0.0, x_strip_offset, x_strip_offset + s_width, "StripZ_"+to_string(j)); 
		//cmpAmp->AddReadout("StripZ_"+to_string(j));
		
		//z_strip_offset += s_pitch;
	//}

	
	
	const int x_mesh_count = w / m_pitch / MESH_REDUCING_FACTOR; // How many tubes on x
	const int z_mesh_count = l / m_pitch / MESH_REDUCING_FACTOR; // How many tubes on y
	
	cout <<"x_mesh_count="<<x_mesh_count<<endl;
	cout <<"z_mesh_count="<<z_mesh_count<<endl;

	double x_mesh_offset = -w/2;
	double z_mesh_offset = -l/2;
	
	
		//TGeoRotation*   r = new TGeoRotation("rot4",90,90,0,0,90,0);
		
		
		
		TGeoVolume *tube = geom->MakeTube("TUBE_1", Ni, 0, m_thickness/2, w/2);
		tube->SetLineColor(kRed);

	TGeoRotation*   r1 = new TGeoRotation("rot4",0,90,0);
	
	for(int k=0; k<x_mesh_count; k++)
	{
		
		TGeoCombiTrans* t = new TGeoCombiTrans(x_mesh_offset, 0, - h_tot/2 + (m_thickness/2 + h_amp + h_plate_bottom), r1);
        top->AddNode(tube, k, t);
		x_mesh_offset += m_pitch * MESH_REDUCING_FACTOR;
		
	}
	
	TGeoRotation*   r2 = new TGeoRotation("rot5",90,90,90);

	for(int q=0; q<z_mesh_count; q++)
	{
		TGeoCombiTrans* t = new TGeoCombiTrans(0, z_mesh_offset, - h_tot/2 + (m_thickness/2 + h_amp + h_plate_bottom), r2);
        top->AddNode(tube, q, t);
		z_mesh_offset += m_pitch * MESH_REDUCING_FACTOR;
	}
	
	
	
	TGeoCompositeShape *substraction = new TGeoCompositeShape("SUBST", "GAS - PLATE_TOP - PLATE_BOTTOM - TUBE_1");


	TGeoVolume *comp = new TGeoVolume("COMP",substraction);
	comp->SetLineColor(5);

	top->AddNode(comp,10);


	geom->CloseGeometry();

	geom->SetVisLevel(4);
	geom->SetVisOption(0);
	top->Draw();
	
	TGeoToStep* exporter = new TGeoToStep(geom);
	exporter->CreateGeometry();
	
	return geom;
}
