#include <TCanvas.h>
#include <ViewField.hh>
#include <ComponentAnsys123.hh>


using namespace std;
using namespace Garfield;

void plot_fields(ComponentAnsys123* fm)
{

	TCanvas * c1 = new TCanvas("c1", "c1", 10, 10, 1400, 700);
	c1->Divide(2,1);
		
	for (float z=-31.75; z<-25.2; z+=0.2)
	{
		ViewField* fieldViewXY = new ViewField();
		fieldViewXY->SetComponent(fm);
		fieldViewXY->SetPlane(0., 0., -1., 0., 0., z);
		fieldViewXY->SetArea(-31.7,8,31.7,15);
		fieldViewXY->SetVoltageRange(-180, 500);
		fieldViewXY->SetCanvas((TCanvas*)c1->cd(1));
		fieldViewXY->PlotContour();   
		
		ViewField* fieldViewYZ = new ViewField();
		fieldViewYZ->SetComponent(fm);
		fieldViewYZ->SetPlane(-1., 0., 0., z, 0., 0.);
		fieldViewYZ->SetArea(-31.7,8,31.7,15);
		fieldViewYZ->SetVoltageRange(-180, 500);
		fieldViewYZ->SetCanvas((TCanvas*)c1->cd(2));
		fieldViewYZ->PlotContour();
		
		c1->Print(("output/field_xy_yz_for_" + to_string(z) + ".png").c_str());
    }
    
    
	TCanvas * c2 = new TCanvas("c2", "c2", 10, 10, 700, 700);
	for (float z=12; z<17.2; z+=0.2)
    {
		ViewField* fieldViewXZ = new ViewField();
		fieldViewXZ->SetComponent(fm);
		fieldViewXZ->SetPlane(0., -1., 0., 0., z, 0.);
		fieldViewXZ->SetArea(-31.7,-31.7,31.7,31.7);
		fieldViewXZ->SetVoltageRange(-2, 2);
		fieldViewXZ->SetCanvas((TCanvas*)c2->cd());
		fieldViewXZ->PlotContour();
		
		c2->Print(("output/field_xz_for_" + to_string(z) + ".png").c_str());
    }
    
}
