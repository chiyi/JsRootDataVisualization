#ifndef __LAUNCHSIMULATOR_H_
#define __LAUNCHSIMULATOR_H_

#include <iostream>
#include <stdio.h>
#include <vector>

#include "TUrl.h"
#include "THttpWSHandler.h"
#include "THttpServer.h"
#include "TSystem.h"

class LaunchSimulator : public THttpWSHandler
{
 public:
  LaunchSimulator(
   const char *name, const char *title,
   const TString &html,
   const TString &dir_work, const TString &dir_func, const TString &dir_plot,
   const TString &sc_test_f2d, const TString &sc_gen_heatmap
  );

  // load custom HTML page when open correspondent address
  TString GetDefaultPageContent() override;
  Bool_t ProcessWS(THttpCallArg *arg) override;
  void Clean_UserDir();

 private:
  UInt_t fWSId=0;
  Int_t fServCnt=0;
  const TString title_svlog = "[LaunchSimulator]";
  const TString html;
  const TString dir_work;
  const TString dir_func;
  const TString dir_plot;
  const TString sc_test_f2d;
  const TString sc_gen_heatmap;
  const TString sc_mkdir = this->dir_work + "/scripts/make_userdir.sh";
  const TString sc_cleandir = this->dir_work + "/scripts/clean_userdir.sh";
  Bool_t process_postdata(THttpCallArg* arg);
  void print_svlog(const TString &subject, const TString &log);
  void print_sverr(const TString &subject, const TString &err);
  TString gen_cmd(const std::vector<TString> &exe_args);
  TString write_udfs(const TString &user, const TString &outname, TString data);
  int test_udfs(TString path_udf, TString path_log);
  TString get_log(TString path_log);
  int create_simdata(TString outjson, TString user, TString path_udf, TString path_log);
};

#endif
