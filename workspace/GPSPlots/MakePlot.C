#include <iostream>
#include <stdexcept>

#include "TString.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TNtupleD.h"
#include "TList.h"
#include "TFile.h"

TH2D* Init_Histogram();
void Fill_Data(TH2D *h2d, TString inpfile);
TH2D* Load_Taipei(TString inpfile);
void Write_RootFile(TString outplot, TCanvas *c_temp, TH2D *h2d, TH2D *h_ref);
void Write_Json(TString outjson, TCanvas *c_temp);




void MakePlot(TString outplot, TString outjson, TString file_tag, TString inpfile, TString file_Taipei="plot/Taipei_edges.root")
{
 std::cout << inpfile << std::endl;

 TH2D *h_temperatureC = Init_Histogram();
 Fill_Data(h_temperatureC, inpfile);
 TH2D *h_Taipei = Load_Taipei(file_Taipei);
 TString cname = TString("c_")+file_tag;
 TString ctitle = TString("temperature ")+file_tag;
 TCanvas *c_temp = new TCanvas(cname, ctitle, 1200, 800);
 Write_RootFile(outplot, c_temp, h_temperatureC, h_Taipei);
 Write_Json(outjson, c_temp);
}


TH2D* Init_Histogram()
{
 int nbinsx=0, nbinsy=0;
 double xmin=121.4375, xmax=121.6725, ymin=24.9475, ymax=25.2125;
 TString hname = "h_temperatureC";
 TString title = "Temperature #circC on (longitude, latitude)";
 TString xtitle = "longitude";
 TString ytitle = "latitude";
 TString ztitle = "temperature";
 TH2D *h_temperatureC;


 nbinsx = std::round((xmax-xmin)*10000/50);
 nbinsy = std::round((ymax-ymin)*10000/50);
 h_temperatureC = new TH2D(hname, title, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
 h_temperatureC->SetXTitle(xtitle);
 h_temperatureC->SetYTitle(ytitle);
 h_temperatureC->SetZTitle(ztitle);

 return h_temperatureC;
}

void Fill_Data(TH2D *h2d, TString inpfile)
{
 double x_i=0., y_i=0., z_i=0.;
 TString xtitle = h2d->GetXaxis()->GetTitle();
 TString ytitle = h2d->GetYaxis()->GetTitle();
 TString ztitle = h2d->GetZaxis()->GetTitle();

 TNtupleD *data = new TNtupleD("data", ztitle, TString::Format("%s:%s:%s", xtitle.Data(), ytitle.Data(), ztitle.Data()));
 data->ReadFile(inpfile, "", '\t');
 data->SetBranchAddress(xtitle, &x_i);
 data->SetBranchAddress(ytitle, &y_i);
 data->SetBranchAddress(ztitle, &z_i);
 for (int idx=0; idx<data->GetEntries(); ++idx)
 {
  data->GetEntry(idx);
  h2d->Fill(x_i, y_i, z_i);
 }
}

TH2D* Load_Taipei(TString inpfile)
{
 TH2D *res;
 TFile *fr = new TFile(inpfile);
 TString hname = "h_seldist";
 if (fr->Get(hname))
  res = (TH2D *)(fr->Get(hname)->Clone());
 else
  throw std::runtime_error(TString("[FAILED]: Load_Taipei(TString inpfile) ")+hname+" is missing.");
 return res;
}

void Write_RootFile(TString outplot, TCanvas *c_temp, TH2D *h2d, TH2D *h_ref)
{
 TFile *fw = new TFile(outplot, "RECREATE");
 fw->cd();
 c_temp->cd();
 h2d->Draw("colz");
 h_ref->Draw("pSAME");
 c_temp->Write();
 fw->Close();
}

void Write_Json(TString outjson, TCanvas *c_temp)
{
 c_temp->SaveAs(outjson);
}

