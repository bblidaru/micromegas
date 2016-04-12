#include <TCanvas.h>
#include <ViewField.hh>
#include <ComponentAnalyticField.hh>
#include <Sensor.hh>


using namespace std;
using namespace Garfield;

void plot_fields(ComponentAnalyticField* cmpDrift, ComponentAnalyticField* cmpAmp, Sensor* sensor)
{

	TCanvas * c3 = new TCanvas("c", "c", 10, 10, 1000, 700);
	ViewField * viewfield = new ViewField();
	viewfield->SetComponent(cmpAmp);
	viewfield->SetSensor(sensor);
	viewfield->SetCanvas((TCanvas*)c3->cd(1));
	viewfield->SetWeightingFieldRange(0.0, 10000.0);
	c3->cd(1);
	viewfield->PlotContour();
	

	TCanvas * c1 = new TCanvas("c1", "c1", 10, 10, 1400, 700);
	c1->Divide(2,1);
		
	for (float z=-5; z<-4.98; z+=0.0005)
	{
		ViewField* fieldViewXY = new ViewField();
		fieldViewXY->SetComponent(cmpAmp);
		fieldViewXY->SetPlane(0., 0., -1., 0., 0., z);
		fieldViewXY->SetArea(-0.025,0.0120,0.025,0.0136);
		fieldViewXY->SetVoltageRange(-15, 15);
		fieldViewXY->SetCanvas((TCanvas*)c1->cd(1));
		fieldViewXY->PlotContour();   
		
		ViewField* fieldViewYZ = new ViewField();
		fieldViewYZ->SetComponent(cmpAmp);
		fieldViewYZ->SetPlane(-1., 0., 0., z, 0., 0.);
		fieldViewYZ->SetArea(-0.025,0.0120,0.025,0.0136);
		fieldViewYZ->SetVoltageRange(-15, 15);
		fieldViewYZ->SetCanvas((TCanvas*)c1->cd(2));
		fieldViewYZ->PlotContour();
		
		c1->Print(("output/field_xy_yz_for_" + to_string(z) + ".png").c_str());
    }
    
    
	TCanvas * c2 = new TCanvas("c2", "c2", 10, 10, 700, 700);
	for (float y=0.0120; y<0.0140; y+=0.0005)
    {
		ViewField* fieldViewXZ = new ViewField();
		fieldViewXZ->SetComponent(cmpAmp);
		fieldViewXZ->SetPlane(0., -1., 0., 0., y, 0.);
		fieldViewXZ->SetArea(-0.025,-0.025,0.025,0.025);
		fieldViewXZ->SetVoltageRange(-15, 15);
		fieldViewXZ->SetCanvas((TCanvas*)c2->cd());
		fieldViewXZ->PlotContour();
		
		c2->Print(("output/field_xz_for_" + to_string(y) + ".png").c_str());
    }
    
}
