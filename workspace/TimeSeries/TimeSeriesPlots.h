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
  std::vector<TString> TimeSeriesFiles;
  std::vector<TString> Get_Filenames();
  void Init_Plots();
  void Init_Energy_Plots();
  void Init_Registration();
  void AddItems(TString path_base);
  void AddItem_Info(TString path_base);
  void AddItem_Compare(TString path_base);
};

#endif
