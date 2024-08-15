#include <iostream>

#include "TStyle.h"

#include "DataVista/DataVista_SV.h"
#include "DataVista/DataVista_SV.cpp"




void SetSV(std::string logfile)
{
 DATAVISTA_SV = new DataVista_SV("DATAVISTA_SV", logfile);
 DATAVISTA_SV->AddItems();
}
