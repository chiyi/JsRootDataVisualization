#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

#include "TString.h"
#include "TF2.h"
#include "TH2D.h"

#include "SimFunc.h"
#include "Gen_HeatMap.C"
bool test_function(const SimFunc &func);




void Test_Funcs2D(std::string file_cfg="")
{
 std::vector<SimFunc> funcs = read_cfg(file_cfg);

 if (funcs.empty())
 {
  std::cerr << "No valid functions found in the configuration file." << std::endl;
  exit(EXIT_FAILURE);
 }

 for (const auto &func : funcs)
 {
  std::cout << "[Test_Funcs2D] SimFunc: ntimes=" << func.N << ", " << func.Func2D << std::endl;
  if (!test_function(func))
  {
   std::cerr << "Test failed. Exiting..." << std::endl;
   exit(EXIT_FAILURE);
  }
 }
}


bool test_function(const SimFunc &func)
{
 bool res = false;

 TF2 f_tmp("f_tmp", func.Func2D.Data(), -2., 2., -2., 2.);
 if (!f_tmp.IsValid())
 {
  std::cerr << "Invalid function: " << func.ToString() << std::endl;
  return res;
 }

 TH2D h_tmp("h_tmp", "tmp", 200, -2., 2., 200, -2., 2.);
 f_tmp.Draw("ALP");
 h_tmp.FillRandom(f_tmp.GetName(), func.N);
 double chk = h_tmp.Integral();
 if (chk > 0)
 {
  std::cout << "Filled events = " << chk << std::endl;
  res = true;
 }
 else
  std::cerr << "Function Failed: " << func.ToString() << std::endl;

 return res;
}
