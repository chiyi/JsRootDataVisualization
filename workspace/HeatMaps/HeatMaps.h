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
#include "LaunchSimulator.cpp"

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
  const TString dir_work = "HeatMaps";
  const TString dir_plot = "plot";
  const TString dir_func = "func";
  const TString file_FakeData_Info = this->dir_work + "/FakeData_Info.txt";
  const TString cfg_basefunc = this->dir_func + "/Base_Functions_2D.cfg";
  const TString sc_test_f2d = this->dir_work + "/scripts/test_functs2d.sh";
  const TString sc_gen_heatmap = this->dir_work + "/scripts/gen_heatmap.sh";
  const TString sc_gen_layers = this->dir_work + "/scripts/gen_sim7layers.sh";
  const TString out_fn_default = this->dir_plot + "/heatmap_default.json";
  const TString out_prefix_layer = this->dir_plot + "/layers/default";
  const TString file_simulator = "/jsrootsys/files/call_simulator.htm";
  TH2D *h_data = nullptr;
  std::vector<TString> HeatMapsFiles;
  LaunchSimulator *simulator = new LaunchSimulator(
   "simulator", "Handler for Launching Simulator",
   this->file_simulator,
   this->dir_work, this->dir_func, this->dir_plot,
   this->sc_test_f2d, this->sc_gen_heatmap
  );
  std::vector<TString> get_filenames();
  void init_plots();
  void init_fakedataplots();
  void init_registration();
  void additems(TString path_base);
  void additem_fakedatainfo(TString path_base);
  void additem_playlayers(TString path_base);
  void additem_clientsimulation(TString path_base);
  void additem_loadplot(TString path_base);
  void launch_wholegeneration();
  bool test_defaultfuncs();
  void gen_defaultheatmap();
  void gen_defaultlayers();
};

#endif
