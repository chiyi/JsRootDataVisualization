#include <iostream>

#include "TStyle.h"
#include "TROOT.h"

#include "DataVista/DataVista_SV.h"
#include "GPSPlots/GPSPlots.h"
#include "TimeSeries/TimeSeriesPlots.h"
#include "DataVista/DataVista_SV.cpp"
#include "GPSPlots/GPSPlots.cpp"
#include "TimeSeries/TimeSeriesPlots.cpp"




void SetSV(std::string logfile)
{
 gStyle->SetOptStat(0);
 gROOT->ForceStyle();
 DATAVISTA_SV = new DataVista_SV("DATAVISTA_SV", logfile);
 DATAVISTA_SV->AddItems();
 auto gps = GPSPlots::Add_GPSPlots(DATAVISTA_SV);
 DATAVISTA_SV->gps_plots = gps;
 auto ts = TimeSeriesPlots::Add_TimeSeriesPlots(DATAVISTA_SV);
 DATAVISTA_SV->ts_plots = ts;

 DATAVISTA_SV->Append_IndexPage();
}
