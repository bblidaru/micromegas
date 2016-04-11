#include <ViewGeometry.hh>
#include <GeometrySimple.hh>
#include "plot.hpp"
#include <ComponentAnalyticField.hh>
#include <ViewCell.hh>


void plot_geometry(ComponentAnalyticField* cmpDrift, ComponentAnalyticField* cmpAmp, GeometrySimple* geo)
{
	
	
	//TCanvas * c4 = new TCanvas("c4", "c4", 10, 10, 1000, 700);
	//c4->Divide(2,1);
	
	ViewGeometry* view_geo = new ViewGeometry();
	view_geo->SetGeometry(geo);
	view_geo->Plot();
	
	//ViewCell* view1 = new ViewCell();
	//view1->SetComponent(cmpAmp);
	//view1->SetCanvas((TCanvas*)c4->cd(1));
	//view1->Plot3d();
	
	//ViewCell* view2 = new ViewCell();
    //view2->SetComponent(cmpDrift);
	//view2->SetCanvas((TCanvas*)c4->cd(2));
	//view2->Plot3d();
}
