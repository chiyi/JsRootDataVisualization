#ifndef __GPSPLOTS_H__
#define __GPSPLOTS_H__

#include <vector>
#include <fstream>
#include "TString.h"
#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "THttpServer.h"
#include "TSystem.h"

class GPSPlots
{
 public:
  GPSPlots(std::string sv, THttpServer *serv);
  static GPSPlots* Add_GPSPlots(DataVista_SV *ptr)
  {
   return new GPSPlots(ptr->Get_SVptr(), ptr->serv);
  }
  void FetchDataToPlots();
  void Refresh();

 private:
  bool b_Reg_GPS_Plots = false;
  std::string sv;
  THttpServer *serv = nullptr;
  int Nfiles;
  const TString dir_plot = "GPSPlots/plot";
  std::vector<TString> GPSFiles;
  std::vector<TString> Get_Filenames();
  void Init_Plots();
  void Init_Taipei();
  void Init_Temperature_Plots();
  void Init_Registration();
  void AddItems(TString path_base);
  void AddItem_Info(TString path_base);
  void AddItem_FetchDataToPlots(TString path_base);
};

#endif
