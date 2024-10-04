#ifndef __SIMFUNC_H_
#define __SIMFUNC_H_

class SimFunc
{
 public:
  int N=0;
  TString Func2D="";

  SimFunc() {};
  SimFunc(TString tstrline)
  {
   int from=0;
   TString tok;
   std::vector<TString> var;
   if (tstrline.First('#') >= 0)
    tstrline = TString(tstrline(0, tstrline.First('#')));
   while (tstrline.Tokenize(tok, from, "\t"))
    var.push_back(tok);
  
   if (var.size() == 2)
   {
    this->N = var[0].Atoi();
    this->Func2D = var[1];
   }
  };

  TString ToString() const
  {
   return TString::Format("%d\t%s", this->N, this->Func2D.Data());
  }

  bool IsValid()
  {
   return this->N > 0 && !this->Func2D.IsWhitespace();
  }
};

#endif
