#ifndef __DataVista_SV_H__
#define __DataVista_SV_H__

#include <iostream>
#include <string>
#include "TString.h"
#include "THttpServer.h"

class DataVista_SV
{
 public:
  THttpServer *serv = nullptr;

  DataVista_SV(std::string ptr, std::string logfile);
  ~DataVista_SV();
  std::string Get_SVptr() const;
  void AddItems();
  void Refresh();
  class GPSPlots *gps_plots = nullptr;
  class TimeSeriesPlots *ts_plots = nullptr;
  class HeatMaps *heatmap_plots = nullptr;
  void Append_IndexPage();

 private:
  std::string sv;
  std::string logfile;
  void init_debuginfo();
  TString genhtml_readtxtfile(std::string inpfile);
  void additem_refresh();
};

#endif
