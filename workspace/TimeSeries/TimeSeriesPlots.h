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
  THttpServer *serv = nullptr;
  int Nfiles;
  const TString dir_plot = "TimeSeries/plot";
  const TString file_OurWorldInData_Info = "TimeSeries/OurWorldInData_Info.txt";
  const TString file_MT5_Info = "TimeSeries/MetaTrader5_Info.txt";
  std::vector<TString> TimeSeriesFiles;
  std::vector<TString> get_filenames();
  void init_plots();
  void init_energyplots();
  void init_MT5plots();
  void init_registration();
  void additems(TString path_base);
  void additem_energyinfo(TString path_base);
  void additem_MT5info(TString path_base);
  void additem_compare(TString path_base);
  void additem_overviewXE(TString path_base);
};

#endif
