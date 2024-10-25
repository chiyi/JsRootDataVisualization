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
  std::vector<TString> get_filenames();
  void init_plots();
  void init_Taipei();
  void init_temperature_plots();
  void init_registration();
  void additems(TString path_base);
  void additem_info(TString path_base);
  void additem_fetchdata_to_plots(TString path_base);
};

#endif
