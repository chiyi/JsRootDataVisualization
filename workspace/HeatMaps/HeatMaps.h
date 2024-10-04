#ifndef __HEATMAPS_H__
#define __HEATMAPS_H__

#include <vector>
#include <fstream>
#include "TString.h"
#include "TFile.h"
#include "TF2.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "THttpServer.h"
#include "TSystem.h"

class HeatMaps
{
 public:
  HeatMaps(std::string sv, THttpServer *serv);
  ~HeatMaps();
  static HeatMaps* Add_HeatMaps(DataVista_SV *ptr)
  {
   return new HeatMaps(ptr->Get_SVptr(), ptr->serv);
  }
  void Refresh();

 private:
  bool b_Reg_HeatMaps_Plots = false;
  std::string sv;
  THttpServer *serv = nullptr;
  int Nfiles;
  const TString dir_plot = "HeatMaps/plot";
  const TString file_FakeData_Info = "HeatMaps/FakeData_Info.txt";
  const TString cfg_basefunc = "scripts/Base_Functions_2D.cfg";
  const TString sc_test_f2d = "HeatMaps/scripts/test_functs2d.sh";
  const TString sc_gen_heatmap = "HeatMaps/scripts/gen_heatmap.sh";
  const TString sc_gen_layers = "HeatMaps/scripts/gen_sim7layers.sh";
  const TString out_fn_default = "plot/heatmap_default.json";
  const TString out_prefix_layer = "plot/layers/default";
  TH2D *h_data = nullptr;
  std::vector<TString> HeatMapsFiles;
  std::vector<TString> Get_Filenames();
  void Init_Plots();
  void Init_FakeData_Plots();
  void Init_Registration();
  void AddItems(TString path_base);
  void AddItem_FakeDataInfo(TString path_base);
  void AddItem_PlayLayers(TString path_base);
  void LaunchProc_WholeGeneration();
  bool Test_DefaultFuncs();
  void Gen_DefaultHeatMap();
  void Gen_DefaultLayers();
};

#endif
