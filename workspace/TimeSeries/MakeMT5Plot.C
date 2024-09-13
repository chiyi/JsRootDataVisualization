#include <iostream>

#include "TString.h"
#include "TCanvas.h"
#include "TNtupleD.h"
#include "TFile.h"

#include "MT5Plot.h"
#include "MT5Plot.cpp"
#include "MakeXE_Overview.cpp"




void MakeMT5Plot(TString outdir, TString inpfile, TString symbol, TString tbin)
{
 std::cout << inpfile << std::endl;

 MT5Plot *mt5plot = new MT5Plot(symbol, tbin);
 mt5plot->Fill_Data(inpfile);
 mt5plot->Make_Plots();
 mt5plot->Write(outdir);
}

void MakeMT5Plot(TString outdir, TString br)
{
 if (br != "Overview")
  return;
 std::cout << "Generating overview plot for CurrencyExchange" << std::endl;

 MakeXE_Overview(outdir);
}
