#ifndef __MT5PLOT_H__
#define __MT5PLOT_H__

#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>
#include <vector>
#include <map>
#include <string>
#include "TMath.h"
#include "TTimeStamp.h"
#include "TRegexp.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TAxis.h"
#include "TGraphAsymmErrors.h"

class MT5Data
{
 public:
  Double_t ts;
  Double_t Open;
  Double_t Close;
  Double_t High;
  Double_t Low;
  Double_t TickVolume;
  Double_t Volume;
};

class MT5Plot
{
 public:
  MT5Plot();
  MT5Plot(TString symbol, TString tbin="H1");
  void Fill_Data(TString inpfile);
  void Make_Plots();
  void Write(TString outdir);
  void Read(TString inpfile);
  int Get_N() const;
  std::vector<Double_t> Get_T() const;
  std::vector<Double_t> Get_dT() const;
  std::vector<Double_t> Get_Val() const;
  std::vector<Double_t> Get_dValup() const;
  std::vector<Double_t> Get_dValdown() const;

 private:
  int Npoints;
  ULong64_t dt_width;
  TString symbol;
  TString tbin;
  std::vector<Double_t> vec_dtleft;
  std::vector<Double_t> vec_dtmiddle;
  std::vector<Double_t> vec_Open;
  std::vector<Double_t> vec_Close;
  std::vector<Double_t> vec_zeros;
  std::vector<Double_t> vec_barzeros;
  std::vector<Double_t> vec_errx;
  std::vector<Double_t> vec_width;
  std::vector<Double_t> vec_delup;
  std::vector<Double_t> vec_deldown;
  std::vector<Double_t> vec_min_up;
  std::vector<Double_t> vec_min_down;
  std::vector<Double_t> vec_max_up;
  std::vector<Double_t> vec_max_down;
  TGraphAsymmErrors *ohlc;
  TGraphAsymmErrors *oc_up;
  TGraphAsymmErrors *oc_down;
  TGraphAsymmErrors *hl_up;
  TGraphAsymmErrors *hl_down;

  void init_TimeWidth();
  MT5Data parse_data(std::string strline);
  Double_t get_ts(TString dtstr);
  void transpose_row_col(const std::vector<MT5Data> &data);
  void reserve_vec();
  Double_t gen_dtmiddle(Double_t ts);
  void reconstruct_vecClose();
};

#endif
