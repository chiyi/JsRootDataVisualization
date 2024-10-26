#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

#include "TString.h"
#include "TF1.h"
#include "TF2.h"
#include "TH2D.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TFile.h"

#include "SimFunc.h"
std::vector<SimFunc> read_cfg(const std::string &file_cfg);
void write_data(const TString &outjson, TH2D *h_data);




void Gen_HeatMap(TString outjson, TString cfg)
{
 std::vector<SimFunc> funcs = read_cfg(cfg.Data());
 TH2D *h_data = new TH2D("h_data", "Sim-Distribution on (Intrinsic Value, Market Price)-plane", 200, -2., 2., 200, -2., 2.);
 h_data->SetXTitle("log10(Intrinsic Value USD)");
 h_data->SetYTitle("log10(Market Price USD)");

 for (const auto &func : funcs)
 {
  TF2 f_tmp("f_tmp", func.Func2D.Data(), -2., 2., -2., 2.);
  h_data->FillRandom(f_tmp.GetName(), func.N);
 }

 write_data(outjson, h_data);
}


std::vector<SimFunc> read_cfg(const std::string &file_cfg)
{
 std::vector<SimFunc> funcs;

 std::string line;
 std::ifstream ifs(file_cfg);

 if (!ifs)
 {
  std::cerr << "Error opening file: " << file_cfg << std::endl;
  return funcs;
 }

 while (std::getline(ifs, line).good())
 {
  SimFunc func(line);
  if (func.IsValid())
   funcs.push_back(func);
 }
 ifs.close();

 return funcs;
}

void write_data(const TString &outjson, TH2D *h_data)
{
 TString outroot = outjson;

 outroot.ReplaceAll(".json", ".root");
 TF1 *f_line = new TF1("f_line", "x", -2., 2.);

 TFile *fw = new TFile(outroot, "RECREATE");
 fw->cd();
 TCanvas *v_out= new TCanvas("v_data", "canvas(sim data)", 1200, 800);
 v_out->SetGridx(1); 
 v_out->SetGridy(1);
 v_out->cd();
 h_data->Draw("colz");
 f_line->Draw("SAME");
 v_out->SetLogz(1);
 h_data->Write();
 f_line->Write();
 v_out->Write();
 v_out->SaveAs(outjson);

 TString outjson2 = outjson;
 outjson2.ReplaceAll(".json", "_3d.json");
 h_data->Draw("PE");
 v_out->SetLogz(0);
 gPad->Update();
 v_out->SaveAs(outjson2);
 fw->Close();
 std::cout << "file:" << outjson << " has been created" << std::endl;
}
