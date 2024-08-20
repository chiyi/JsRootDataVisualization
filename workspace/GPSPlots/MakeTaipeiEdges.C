#include <iostream>

#include "TString.h"
#include "TH2D.h"
#include "TFile.h"
#include "TNtupleD.h"

TH2D* Init_Histogram();
void Fill_Data(TH2D *h_seldist, TString inpfile);




void MakeTaipeiEdges(TString outplot, TString inpfile)
{
 std::cout << inpfile << std::endl;

 TH2D *h_seldist = Init_Histogram();
 Fill_Data(h_seldist, inpfile);

 TFile *fw = new TFile(outplot, "RECREATE");
 fw->cd();
 h_seldist->Write();
 fw->Close();
}


TH2D* Init_Histogram()
{
 int nbinsx=0, nbinsy=0;
 double xmin=121.4570, xmax=121.6660, ymin=24.9605, ymax=25.2102;
 TString hname = "h_seldist";
 TString title = "Taipei (longitude, latitude)";
 TString xtitle = "longitude";
 TString ytitle = "latitude";
 TH2D *h_seldist;


 nbinsx = std::round((xmax-xmin)*10000);
 nbinsy = std::round((ymax-ymin)*10000);
 h_seldist = new TH2D(hname, title, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
 h_seldist->SetXTitle(xtitle);
 h_seldist->SetYTitle(ytitle);

 return h_seldist;
}

void Fill_Data(TH2D *h_seldist, TString inpfile)
{
 double x_i=0., y_i=0.;
 TString xtitle = h_seldist->GetXaxis()->GetTitle();
 TString ytitle = h_seldist->GetYaxis()->GetTitle();

 TNtupleD *data = new TNtupleD("data", "", TString::Format("%s:%s", xtitle.Data(), ytitle.Data()));
 data->ReadFile(inpfile, "", ',');
 data->SetBranchAddress(xtitle, &x_i);
 data->SetBranchAddress(ytitle, &y_i);
 for (int idx=0; idx<data->GetEntries(); ++idx)
 {
  data->GetEntry(idx);
  h_seldist->Fill(x_i, y_i, 1);
 }

 for (int ibinx=0; ibinx<=1+h_seldist->GetNbinsX(); ibinx++)
  for (int ibiny=0; ibiny<=1+h_seldist->GetNbinsY(); ibiny++)
   if (h_seldist->GetBinContent(ibinx, ibiny)>1)
    h_seldist->SetBinContent(ibinx, ibiny, 1);

 h_seldist->Sumw2();
}

