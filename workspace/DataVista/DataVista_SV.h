#ifndef __DataVista_SV_H__
#define __DataVista_SV_H__

#include <iostream>
#include <string>
#include "TString.h"
#include "THttpServer.h"

class DataVista_SV
{
 public:
  THttpServer *serv;

  DataVista_SV(std::string ptr, std::string logfile);
  std::string Get_SVptr();
  void AddItems();
  void Refresh();

 private:
  std::string sv;
  std::string logfile;
  void Init_DebugInfo();
  TString GenHTML_ReadTxtfile(std::string inpfile);
  void AddItem_Refresh();
};

#endif
