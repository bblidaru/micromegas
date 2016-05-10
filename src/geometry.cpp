#include <MediumMagboltz.hh>
#include <GeometrySimple.hh>
#include <ComponentAnalyticField.hh>
#include <SolidBox.hh>
#include <iostream>
<<<<<<< HEAD
=======
#include <MediumSilicon.hh>
#include <SolidTube.hh>
#include <TGeoManager.h>
#include <TGeoToStep.h>
#include <TGeoTube.h>
#include <TGeoCompositeShape.h>

#include <ViewGeometry.hh>
#include <Sensor.hh>
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf

using namespace std;
using namespace Garfield;

<<<<<<< HEAD
GeometrySimple* build_geometry(MediumMagboltz* gas)
{
	
	// Width [cm]
	const double w = 10.0;
	// Length [cm]
	const double l = 10.0;
	// Height [cm]
	const double h_drift = 0.6; // Drift gap height
	const double h_amp = 0.0128; // Amplification gap height
	const double h_tot = h_drift + h_amp; // Total height
=======

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
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf
	// Voltages [V]
	const double v_top = -180.0;
	const double v_mesh = 0.0;
	const double v_bottom = 500.0;
	// Mesh properties [cm]
<<<<<<< HEAD
	const double m_thickness = 0.0003; // Mesh thickness (3um)
=======
	const double m_thickness = 0.0011; // Mesh thickness (11um)
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf
	const double m_width = 0.0011; // Mesh width (11um)
	const double m_interpitch = 0.0039; // Distance between two consecutive mesh lines (39um)
	const double m_pitch = m_width + m_interpitch; // Period of the mesh 
	
	// Strip properties [cm]
	const double s_pitch = 0.025; // Period of the strips (250um) 
	const double s_width = 0.015; // Strip width (150um)	
	const double s_interpitch = s_pitch - s_width; // Distance between two consecutive strip lines 
	
<<<<<<< HEAD
	
	
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
	
	
	for (int i=0; i<x_strip_count; i++)
	{
		x_strip_centers[i] = x_offset + s_width / 2.0;
		cmpAmp->AddStripOnPlaneY('x', 0.0, x_offset, x_offset + s_width, "StripX_"+to_string(i)); 
		cmpAmp->AddReadout("StripX_"+to_string(i));
		
		x_offset += s_pitch;
	}
	
	for (int j=0; j<z_strip_count; j++)
	{
		z_strip_centers[j] = z_offset + s_width / 2.0;
		cmpAmp->AddStripOnPlaneY('z', 0.0, x_offset, x_offset + s_width, "StripZ_"+to_string(j)); 
		cmpAmp->AddReadout("StripZ_"+to_string(j));
		
		z_offset += s_pitch;
	}


	// Set constant magnetic field in [Tesla]
	//cmpDrift->SetMagneticField(MagX, MagY, MagZ);
	return geo;
=======
	// Mesh count
	const int x_mesh_count = w / m_pitch; // How many tubes on x
	const int y_mesh_count = l / m_pitch; // How many tubes on y
	cout <<"x_mesh_count="<<x_mesh_count<<endl;
	cout <<"y_mesh_count="<<y_mesh_count<<endl;
	
	// CELL INFO
	// Width [cm]
	const double w_cell = w/x_mesh_count; // X_cell length
	// Length [cm]
	const double l_cell = l/y_mesh_count; // Y_cell length
		
	
	// ROOT GEOMETRY
	TGeoManager *geom = new TGeoManager("Micromegas"," Geometry using ROOT");
	
	//--- define some materials
	TGeoMaterial *matGas = new TGeoMaterial("Gas", 0, 0, 0);
	TGeoMaterial *matAl = new TGeoMaterial("Al", 26.98,13,2.7);
	TGeoMaterial *matNi = new TGeoMaterial("Ni", 58, 28, 8.9);
	TGeoMaterial *matCu = new TGeoMaterial("Cu", 52, 24, 8.96);
	
	//   //--- define some media
	TGeoMedium *medGas= new TGeoMedium("Gas",1, matGas);
	TGeoMedium *medAl = new TGeoMedium("Al",2, matAl);
	TGeoMedium *medNi = new TGeoMedium("Ni",3, matNi);
	TGeoMedium *medCu = new TGeoMedium("Cu",4, matCu);
	
	
	TGeoVolume *top = geom->MakeBox("TOP", medGas, w_cell/2, l_cell/2, h_tot/2);
    geom->SetTopVolume(top);
	geom->SetTopVisible();
	
	
	//TGeoVolume *volume_gas = geom->MakeBox("GAS", medGas, w_cell/2, l_cell/2, h_tot/2);
	//top->AddNode(volume_gas,8);
	//volume_gas->SetLineColor(kRed);
	
	
	
	TGeoVolume *plate_top = geom->MakeBox("PLATE_TOP", medAl, w_cell/2, l_cell/2, h_plate_top/2);
    plate_top->SetLineColor(kBlue);
    TGeoTranslation* tr_top = new TGeoTranslation("TR_TOP", 0, 0, h_tot/2 - h_plate_top/2);
	top->AddNode(plate_top,1, tr_top);
	
	TGeoVolume *plate_bottom = geom->MakeBox("PLATE_BOTTOM", medCu, w_cell/2, l_cell/2, h_plate_bottom/2);
    plate_bottom->SetLineColor(kGreen);
    TGeoTranslation* tr_bottom = new TGeoTranslation("TR_BOTTOM", 0, 0, -(h_tot/2 - h_plate_bottom/2));
	top->AddNode(plate_bottom, 2, tr_bottom);
	
	TGeoTube *tube1  = new TGeoTube("TUBE_X", 0, m_thickness/2, w_cell/2);
	
	
	
	//TGeoVolume *tube1 = geom->MakeTube("TUBE_X", medNi, 0, m_thickness/2, w_cell/2);
	//tube1->SetLineColor(kRed);
	//TGeoRotation*   r1 = new TGeoRotation("ROT_X",0,90,0);
	//TGeoCombiTrans* t1 = new TGeoCombiTrans("TR_X", 0, 0, - h_tot/2 + (m_thickness/2 + h_amp + h_plate_bottom), r1);
	//top->AddNode(tube1,3, t1);
	//t1 -> RegisterYourself();
	
	//TGeoVolume *tube2 = geom->MakeTube("TUBE_Y", medNi, 0, m_thickness/2, w_cell/2);
	//tube2->SetLineColor(kOrange);
	//TGeoRotation*   r2 = new TGeoRotation("ROT_Y",90,90,90);
	//TGeoCombiTrans* t2 = new TGeoCombiTrans("TR_Y", 0, 0, - h_tot/2 + (m_thickness/2 + h_amp + h_plate_bottom), r2);
	////top->AddNode(tube2, 4, t2);
	//t2 -> RegisterYourself();
	
	
	
	TGeoCompositeShape *substraction = new TGeoCompositeShape("SUBST", "TOP - PLATE_TOP:TR_TOP - PLATE_BOTTOM:TR_BOTTOM - TUBE_X"); //  - ( TUBE_X:TR_X + TUBE_Y:TR_Y)
	TGeoVolume *comp = new TGeoVolume("COMP",substraction);
	comp->SetLineColor(kYellow);
	top->AddNode(comp,5);
	
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

	
	

		//TGeoRotation*   r = new TGeoRotation("rot4",90,90,0,0,90,0);
		
		
	
	

	
	//TGeoCompositeShape *substraction = new TGeoCompositeShape("SUBST", op.c_str());


	//TGeoVolume *comp = new TGeoVolume("COMP",substraction);
	//comp->SetLineColor(5);

	//top->AddNode(comp,10);


	geom->CloseGeometry();

	geom->SetVisLevel(4);
	geom->SetVisOption(0);
	top->Draw();
	
	TGeoToStep* exporter = new TGeoToStep(geom);
	exporter->CreateGeometry();
	
	return geom;
>>>>>>> 4457d833faaf35007b7df4ee556f9da54c2d0ebf
}
