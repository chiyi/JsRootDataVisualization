#ifndef __TIMESERIESPLOTS_H__
#define __TIMESERIESPLOTS_H__

#include <vector>
#include <fstream>
#include "TString.h"
#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "THttpServer.h"
#include "TSystem.h"

class TimeSeriesPlots
{
 public:
  TimeSeriesPlots(std::string sv, THttpServer *serv);
  static TimeSeriesPlots* Add_TimeSeriesPlots(DataVista_SV *ptr)
  {
   return new TimeSeriesPlots(ptr->Get_SVptr(), ptr->serv);
  }
  void FetchDataToPlots();
  void Refresh();

 private:
  bool b_Reg_TimeSeries_Plots = false;
  std::string sv;
  THttpServer *serv;
  int Nfiles;
  const TString dir_plot = "TimeSeries/plot";
  const TString file_OurWorldInData_Info = "TimeSeries/OurWorldInData_Info.txt";
  const TString file_MT5_Info = "TimeSeries/MetaTrader5_Info.txt";
  std::vector<TString> TimeSeriesFiles;
  std::vector<TString> Get_Filenames();
  void Init_Plots();
  void Init_Energy_Plots();
  void Init_MT5_Plots();
  void Init_Registration();
  void AddItems(TString path_base);
  void AddItem_EnergyInfo(TString path_base);
  void AddItem_MT5Info(TString path_base);
  void AddItem_Compare(TString path_base);
  void AddItem_OverviewXE(TString path_base);
};

#endif
