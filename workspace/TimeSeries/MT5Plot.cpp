#ifndef MT5Plot_cpp
#define MT5Plot_cpp

MT5Plot::MT5Plot()
{
}

MT5Plot::MT5Plot(TString symbol, TString tbin)
 :symbol(symbol), tbin(tbin)
{
 this->init_TimeWidth();
}

void MT5Plot::init_TimeWidth()
{
 int unit = 0;
 const TRegexp pat("^[H,M][0-9]+$");
 std::map<TString, ULong64_t> sec = {
  {"Monthly",2592000}, {"Weekly",604800}, {"Daily",86400}
 };

 if (sec.find(this->tbin) != sec.end())
  this->dt_width = sec[this->tbin];
 else if (this->tbin.Contains(pat))
 {
  this->tbin[0]=='H' ? unit=3600 : unit=60;
  this->dt_width = unit * TString(this->tbin(1, this->tbin.Length())).Atoi();
 }
 else
  std::cerr << "[MT5Plot::init_TimeWidth]: dt_width is not initialized." << std::endl;
}

void MT5Plot::Fill_Data(TString inpfile)
{
 std::string strline;
 std::vector<MT5Data> data;
 std::ifstream ifs(inpfile, std::ifstream::in);
 while (getline(ifs, strline).good())
 {
  MT5Data data_i = this->parse_data(strline);
  data.push_back(data_i);
 }
 ifs.close();

 this->transpose_row_col(data);
}

MT5Data MT5Plot::parse_data(std::string strline)
{
 MT5Data res;

 int from=0;
 TString tok;
 TString tstrline = TString(strline);
 std::vector<TString> var;
 while (tstrline.Tokenize(tok, from, ","))
  var.push_back(tok);

 if (var.size() == 7)
 {
  res.ts = get_ts(var[0]);
  res.Open = var[1].Atof();
  res.High = var[2].Atof();
  res.Low = var[3].Atof();
  res.Close = var[4].Atof();
  res.TickVolume = var[5].Atoll();
  res.Volume = var[6].Atoll();
 }

 return res;
}

Double_t MT5Plot::get_ts(TString dtstr)
{
 //expect dtstr is "%Y.%m.%d" or "%Y.%m.%d %H:%M"
 dtstr.ReplaceAll(".", "");
 dtstr.ReplaceAll(":", "");
 int intdate = TString(dtstr(0,8)).Atoi();
 int inttime = TString(dtstr(9,4)).Atoi()*100;
 TTimeStamp T0(intdate, inttime, 0, true, 0);
 return T0.AsDouble();
}

void MT5Plot::transpose_row_col(const std::vector<MT5Data> &data)
{
 Double_t dt_middle=0., errx=0., bin_width=0.;
 this->Npoints = data.size();
 this->reserve_vec();
 for (auto row : data)
 {
  dt_middle = gen_dtmiddle(row.ts);
  // add small non-zero double to avoid JSROOT shortcut as zeros and missing bars.
  vec_barzeros.push_back(row.Open*1.0e-9);
  vec_dtleft.push_back(row.ts);
  vec_dtmiddle.push_back(dt_middle);

  vec_Open.push_back(row.Open);
  vec_Close.push_back(row.Close);

  errx = (this->dt_width != 2592000) ? 0.5 * this->dt_width : dt_middle - row.ts;
  vec_errx.push_back(errx);
  bin_width = (this->dt_width != 2592000) ? this->dt_width : (dt_middle - row.ts)*2.;
  vec_width.push_back(bin_width);

  vec_delup.push_back(TMath::Max(row.Close - row.Open, 0.));
  vec_deldown.push_back(TMath::Max(row.Open - row.Close, 0.));

  vec_min_up.push_back(TMath::Max((row.Open - row.Low) * (row.Close > row.Open), 0.));
  vec_min_down.push_back(TMath::Max((row.Open - row.Low) * (row.Close <= row.Open), 0.));
  vec_max_up.push_back(TMath::Max((row.High - row.Open) * (row.Close > row.Open), 0.));
  vec_max_down.push_back(TMath::Max((row.High - row.Open) * (row.Close <= row.Open), 0.));
 }
}

void MT5Plot::reserve_vec()
{
 vec_zeros.resize(this->Npoints);

 vec_dtleft.reserve(this->Npoints);
 vec_dtmiddle.reserve(this->Npoints);
 vec_Open.reserve(this->Npoints);
 vec_Close.reserve(this->Npoints);
 vec_barzeros.reserve(this->Npoints);
 vec_errx.reserve(this->Npoints);
 vec_width.reserve(this->Npoints);
 vec_delup.reserve(this->Npoints);
 vec_deldown.reserve(this->Npoints);
 vec_min_up.reserve(this->Npoints);
 vec_min_down.reserve(this->Npoints);
 vec_max_up.reserve(this->Npoints);
 vec_max_down.reserve(this->Npoints);
}

Double_t MT5Plot::gen_dtmiddle(Double_t ts)
{
 Double_t res = ts;

 int intym = 0;
 if (this->dt_width != 2592000)
  res += (0.5 * this->dt_width);
 else
 {
  TTimeStamp T_0(ts);
  intym = T_0.GetDate()/100;
  TTimeStamp T_1, T_2;
  T_1.Set(intym*100+1, 0, 0, true, 0);
  if (T_0.GetMonth()<12)
   T_2.Set((intym+1)*100+1, 0, 0, true, 0);
  else
   T_2.Set((intym/100+1)*10000+101, 0, 0, true, 0);
  res = (T_1.AsDouble()+T_2.AsDouble())*0.5;
 }

 return res;
}

void MT5Plot::Make_Plots()
{
 TString title_0 = this->symbol + "_" + this->tbin;
 this->ohlc = new TGraphAsymmErrors(
  this->Npoints,
  vec_dtleft.data(), vec_Open.data(),
  vec_zeros.data(), vec_width.data(),
  vec_deldown.data(), vec_delup.data()
 );
 this->ohlc->SetName("ohlc");
 this->ohlc->SetTitle(title_0);

 this->oc_up = new TGraphAsymmErrors(
  this->Npoints,
  vec_dtmiddle.data(), vec_Open.data(),
  vec_errx.data(), vec_errx.data(),
  vec_barzeros.data(), vec_delup.data()
 );
 this->oc_up->SetName("oc_up");
 this->oc_up->SetTitle(title_0 + "_up");

 this->oc_down = new TGraphAsymmErrors(
  this->Npoints,
  vec_dtmiddle.data(), vec_Open.data(),
  vec_errx.data(), vec_errx.data(),
  vec_deldown.data(), vec_barzeros.data()
 );
 this->oc_down->SetName("oc_down");
 this->oc_down->SetTitle(title_0 + "_down");

 this->hl_up = new TGraphAsymmErrors(
  this->Npoints,
  vec_dtmiddle.data(), vec_Open.data(),
  vec_zeros.data(), vec_zeros.data(),
  vec_min_up.data(), vec_max_up.data()
 );
 this->hl_up->SetName("hl_up");
 this->hl_up->SetTitle(title_0 + "_minmax_up");

 this->hl_down = new TGraphAsymmErrors(
  this->Npoints,
  vec_dtmiddle.data(), vec_Open.data(),
  vec_zeros.data(), vec_zeros.data(),
  vec_min_down.data(), vec_max_down.data()
 );
 this->hl_down->SetName("hl_down");
 this->hl_up->SetTitle(title_0 + "_minmax_down");

 this->ohlc->SetFillStyle(0);
 this->oc_up->SetFillColor(4);
 this->oc_up->SetFillStyle(1001);
 this->oc_down->SetFillColor(2);
 this->oc_down->SetFillStyle(1001);
 this->hl_up->SetFillStyle(0);
 this->hl_up->SetLineColor(4);
 this->hl_up->SetLineWidth(2);
 this->hl_down->SetFillStyle(0);
 this->hl_down->SetLineColor(2);
 this->hl_down->SetLineWidth(2);


 this->ohlc->GetXaxis()->SetTimeDisplay(1);
 this->ohlc->GetXaxis()->SetTimeOffset(0);
 this->ohlc->GetXaxis()->SetTimeFormat("%Y%m%d_H%H");
 this->ohlc->GetXaxis()->SetNdivisions(5, 5, 5, true);
}

void MT5Plot::Write(TString outdir)
{
 TString dataname = this->symbol + "_" + this->tbin;
 TFile *fw = new TFile(outdir+"/"+dataname+".root", "RECREATE");
 fw->cd();
 TCanvas *v_out= new TCanvas("v_"+dataname, dataname, 2400, 1600);
 v_out->SetGridx(1);
 v_out->SetGridy(1);
 v_out->cd();

 this->ohlc->Draw("AY+");
 this->hl_up->Draw("ZSAME");
 this->hl_down->Draw("ZSAME");
 this->oc_up->Draw("2SAME");
 this->oc_down->Draw("2SAME");
 this->ohlc->Draw("PSAME");

 this->ohlc->Write();
 this->oc_up->Write();
 this->oc_down->Write();
 this->hl_up->Write();
 this->hl_down->Write();
 v_out->Write();
 v_out->SaveAs(outdir+"/"+dataname+".json");
 fw->Close();
}

void MT5Plot::Read(TString inpfile)
{
 TFile *fr = new TFile(inpfile.Data());
 this->ohlc = (TGraphAsymmErrors*) fr->Get("ohlc");
 this->oc_up = (TGraphAsymmErrors*) fr->Get("oc_up");
 this->oc_down = (TGraphAsymmErrors*) fr->Get("oc_down");
 this->hl_up = (TGraphAsymmErrors*) fr->Get("hl_up");
 this->hl_down = (TGraphAsymmErrors*) fr->Get("hl_down");

 this->Npoints = this->ohlc->GetN();
 TString dataname = this->ohlc->GetTitle();
 int idx_tok = dataname.First('_');
 this->symbol = dataname(0, idx_tok);
 this->tbin = dataname(idx_tok+1, dataname.Length()-(idx_tok+1));
 this->vec_dtleft.assign(this->ohlc->GetX(), this->ohlc->GetX()+this->Npoints);
 this->vec_dtmiddle.assign(this->hl_up->GetX(), this->hl_up->GetX()+this->Npoints);
 this->vec_Open.assign(this->ohlc->GetY(), this->ohlc->GetY()+this->Npoints);
 this->vec_zeros.resize(this->Npoints);
 this->vec_barzeros.assign(this->oc_up->GetEYlow(), this->oc_up->GetEYlow()+this->Npoints);
 this->vec_errx.assign(this->oc_up->GetEXlow(), this->oc_up->GetEXlow()+this->Npoints);
 this->init_TimeWidth();    // this->vec_width
 this->vec_delup.assign(this->ohlc->GetEYhigh(), this->ohlc->GetEYhigh()+this->Npoints);
 this->vec_deldown.assign(this->ohlc->GetEYlow(), this->ohlc->GetEYlow()+this->Npoints);
 this->vec_min_up.assign(this->hl_up->GetEYlow(), this->hl_up->GetEYlow()+this->Npoints);
 this->vec_min_down.assign(this->hl_down->GetEYlow(), this->hl_down->GetEYlow()+this->Npoints);
 this->vec_max_up.assign(this->hl_up->GetEYhigh(), this->hl_up->GetEYhigh()+this->Npoints);
 this->vec_max_down.assign(this->hl_down->GetEYhigh(), this->hl_down->GetEYhigh()+this->Npoints);

 this->reconstruct_vecClose();    // this->vec_Close;

 fr->Close();
}

void MT5Plot::reconstruct_vecClose()
{
 vec_Close.reserve(this->Npoints);

 for (int idx=0; idx<this->Npoints; ++idx)
 {
  if (this->vec_delup[idx] > 0.)
   this->vec_Close[idx] = this->vec_Open[idx] + this->vec_delup[idx];
  else
   this->vec_Close[idx] = this->vec_Open[idx] - this->vec_deldown[idx];
 }
}

int MT5Plot::Get_N() const
{
 return this->Npoints;
}

std::vector<Double_t> MT5Plot::Get_T() const
{
 return this->vec_dtmiddle;
}

std::vector<Double_t> MT5Plot::Get_dT() const
{
 return this->vec_errx;
}

std::vector<Double_t> MT5Plot::Get_Val() const
{
 return this->vec_Open;
}

std::vector<Double_t> MT5Plot::Get_dValup() const
{
 return this->vec_delup;
}

std::vector<Double_t> MT5Plot::Get_dValdown() const
{
 return this->vec_deldown;
}

#endif
