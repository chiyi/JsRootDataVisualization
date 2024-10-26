#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

#include "TRandom3.h"
#include "TString.h"
#include "TF1.h"
#include "TF2.h"
#include "TH2D.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TFile.h"

#include "SimFunc.h"
#include "Gen_HeatMap.C"
void write_layer(const TString &outprefix, TH2D *h_data, int idx);




void Gen_Sim7layers(TString outprefix, TString cfg)
{
 int sim_N=0;
 TRandom3 tool_rndm;
 std::vector<SimFunc> funcs = read_cfg(cfg.Data());
 TH2D *h_data = new TH2D("h_data", "", 200, -2., 2., 200, -2., 2.);
 h_data->SetXTitle("log10(Intrinsic Value USD)");
 h_data->SetYTitle("log10(Market Price USD)");

 tool_rndm.SetSeed((ULong64_t)h_data);
 for (int layer_idx=1; layer_idx<=7; ++layer_idx)
 {
  h_data->Reset();
  for (const auto &func : funcs)
  {
   TF2 f_tmp("f_tmp", func.Func2D.Data(), -2., 2., -2., 2.);
   sim_N = tool_rndm.Poisson(func.N * TMath::Exp(2. - layer_idx));
   if (tool_rndm.Rndm() >= layer_idx/10. )
    h_data->FillRandom(f_tmp.GetName(), sim_N);
   write_layer(outprefix, h_data, layer_idx);
  }
 }
}


void write_layer(const TString &outprefix, TH2D *h_data, int idx)
{
 TString title0 = "Sim-Distribution on (Intrinsic Value, Market Price)-plane";
 TString outroot = outprefix + TString::Format("_%d.root", idx);

 h_data->SetTitle(title0 + TString::Format(" layer_%d", idx));

 TF1 *f_line = new TF1("f_line", "x", -2., 2.);
 TFile *fw = new TFile(outroot, "RECREATE");
 fw->cd();
 TCanvas *v_out= new TCanvas("v_data", "canvas(sim data)", 1200, 800);
 v_out->SetGridx(1); 
 v_out->SetGridy(1);
 v_out->cd();
 h_data->Draw("colz");
 f_line->Draw("SAME");
 h_data->Write();
 f_line->Write();
 v_out->Write();
 fw->Close();
 v_out->Close();
}
