#include <iostream>
#include <map>
#include <algorithm>

#include "TString.h"
#include "TH3D.h"
#include "TGraph2DAsymmErrors.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TFile.h"
#include "TStyle.h"

#include "MT5Plot.h"
#include "MT5Plot.cpp"
std::map<int, TString> DefineXE();
void Fill_Graph2DErr(std::map<int, TGraph2DAsymmErrors> &map_graphs, const std::map<int, TString> map_data, TString outdir);
void Fill_Graph2DErr(std::map<int, TGraph2DAsymmErrors> &map_graphs, const int &val_y, const TString &dataname, const MT5Plot &obj_xe);
void Set_GraphStyle(TCanvas *v_out, TH3D *&h_tmp, TLegend *&leg, std::map<int, TGraph2DAsymmErrors> &map_graphs, const std::map<int, TString> map_data);
void Draw_Graph(TCanvas *v_out, TH3D *h_tmp, TLegend *leg, std::map<int, TGraph2DAsymmErrors> &map_graphs, const std::map<int, TString> &map_data);
void Write_Graph(TCanvas *v_out, TString outdir);




void MakeXE_Overview(TString outdir)
{
 std::map<int, TString> map_data = DefineXE();
 std::map<int, TGraph2DAsymmErrors> map_graphs;
 TLegend *leg = nullptr;
 TCanvas *v_out = new TCanvas("v_xe", "overview", 2400, 1600);
 TH3D *h_tmp = nullptr;

 Fill_Graph2DErr(map_graphs, map_data, outdir);
 Set_GraphStyle(v_out, h_tmp, leg, map_graphs, map_data);
 Draw_Graph(v_out, h_tmp, leg, map_graphs, map_data);
 Write_Graph(v_out, outdir);
}


std::map<int, TString> DefineXE()
{
 return
 {
  {1, "GBPUSD_H1"}, {2, "USDCHF_H1"}, {3, "USDJPY_H1"}, {4, "USDCAD_H1"},
  {5, "EURUSD_H1"}, {6, "XAUUSD_H1"}, {7, "XAGUSD_H1"}, {8, "XPTUSD_H1"}
 };
}

void Fill_Graph2DErr(std::map<int, TGraph2DAsymmErrors> &map_graphs, const std::map<int, TString> map_data, TString outdir)
{
 int val_y=0;
 TString dataname;

 for (const auto &[val_y, dataname] : map_data)
 {
  std::cout << "(val_y, dataname) = " << val_y << ", " << dataname.Data() << std::endl;
  MT5Plot obj_xe;
  obj_xe.Read(outdir+"/"+dataname+".root");
  Fill_Graph2DErr(map_graphs, val_y, dataname, obj_xe);
 }
}

void Fill_Graph2DErr(std::map<int, TGraph2DAsymmErrors> &map_graphs, const int &val_y, const TString &dataname, const MT5Plot &obj_xe)
{
 int N = obj_xe.Get_N();
 Double_t max_z = 0.;

 std::vector<Double_t> vec_zeros(N, 0.);
 std::vector<Double_t> vec_x = obj_xe.Get_T();
 std::vector<Double_t> vec_y(N, val_y);
 std::vector<Double_t> vec_z = obj_xe.Get_Val();
 std::vector<Double_t> vec_errx = obj_xe.Get_dT();
 std::vector<Double_t> vec_errzdown = obj_xe.Get_dValdown();
 std::vector<Double_t> vec_errzup = obj_xe.Get_dValup();
 max_z = *std::max_element(vec_z.begin(), vec_z.end());
 map_graphs[val_y] = TGraph2DAsymmErrors(
  N,
  vec_x.data(), vec_y.data(), vec_z.data(),
  vec_errx.data(), vec_errx.data(),
  vec_zeros.data(), vec_zeros.data(),
  vec_errzdown.data(), vec_errzdown.data()
 );
 map_graphs[val_y].Scale(1. / max_z);
 map_graphs[val_y].SetName(dataname+"/"+TString::Format("%lf", max_z));
 map_graphs[val_y].SetTitle(dataname+"/"+TString::Format("%lf", max_z));
 map_graphs[val_y].SetLineColor(val_y);
 map_graphs[val_y].SetMarkerColor(val_y);
}

void Set_GraphStyle(TCanvas *v_out, TH3D *&h_tmp, TLegend *&leg, std::map<int, TGraph2DAsymmErrors> &map_graphs, const std::map<int, TString> map_data)
{
 Double_t tmin=0.,tmax=0.;

 tmin = map_graphs[1].GetX()[0];
 tmax = map_graphs[1].GetX()[map_graphs[1].GetN()-1];
 h_tmp = new TH3D("h_tmp", "XE overview", 1000, tmin, tmax, 10, 0.5, 10.5, 600, 0.41, 1.01);
 leg = new TLegend(0.0, 0.0, 0.15, 0.2, h_tmp->GetTitle());

 for (const auto &[val_y, dataname] : map_data)
  leg->AddEntry(&(map_graphs[val_y]), map_graphs[val_y].GetTitle(), "LE");

 leg->SetName("leg_"+TString(v_out->GetTitle()));
 h_tmp->GetXaxis()->SetTimeDisplay(1);
 h_tmp->GetXaxis()->SetTimeOffset(0);
 h_tmp->GetXaxis()->SetTimeFormat("%Y%m%d_H%H%F1970-01-01 00:00:00s0");
 h_tmp->GetXaxis()->SetRangeUser(tmax-86400*10, tmax);
 h_tmp->GetZaxis()->SetRangeUser(0.8, 1.0);
 gStyle->SetOptStat(0);
}

void Draw_Graph(TCanvas *v_out, TH3D *h_tmp, TLegend *leg, std::map<int, TGraph2DAsymmErrors> &map_graphs, const std::map<int, TString> &map_data)
{
 v_out->cd();
 h_tmp->Draw();
 for (const auto &[val_y, graph] : map_data)
  map_graphs[val_y].Draw("ERR P SAME");

 // triggering warnings
 //leg = v_out->BuildLegend();
 //v_out->Update();

 leg->Draw("SAME");
}

void Write_Graph(TCanvas *v_out, TString outdir)
{
 TString fname = "overview";
 TFile *fw = new TFile(outdir+"/"+fname+".root", "RECREATE");
 v_out->Write();
 v_out->SaveAs(outdir+"/"+fname+".json");
 fw->Close();
}
