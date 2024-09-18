#ifndef DataVista_SV_cpp
#define DataVista_SV_cpp

DataVista_SV *DATAVISTA_SV;

DataVista_SV::DataVista_SV(std::string ptr, std::string logfile)
{
 TString engine_args = TString::Format(
  "https:%d?%s&%s;%s",
  13931,
  "nocache",
  "ssl_cert=/etc/ssl/server.pem",
  "rw"
 );
 // "dirlisting=yes",
 this->serv = new THttpServer(engine_args);
 if (!this->serv->IsAnyEngine())
  std::cout << "[DataVista_SV::DataVista_SV]: Initialization Failed." << std::endl;

 this->logfile = logfile;
 this->sv = ptr;
 this->Init_DebugInfo();
}

DataVista_SV::~DataVista_SV()
{
 delete gps_plots;
 delete ts_plots;
}

std::string DataVista_SV::Get_SVptr()
{
 return this->sv;
}

void DataVista_SV::Init_DebugInfo()
{
 char path[] = "/SV_log";
 this->serv->CreateItem(path, this->logfile.c_str());
 this->serv->SetItemField(path, "_kind", "Text");
 TString html = this->GenHTML_ReadTxtfile(this->logfile);
 this->serv->SetItemField(path, "value", html.Data());
}

TString DataVista_SV::GenHTML_ReadTxtfile(std::string inpfile)
{
 return TString::Format("<iframe width='100%%' height=1000 src='jsrootsys/%s?nocache=<?=time()?>'></iframe>", this->logfile.c_str());
}

void DataVista_SV::AddItems()
{
 this->AddItem_Refresh();
}

void DataVista_SV::AddItem_Refresh()
{
 char path[] = "/Refresh";
 this->serv->CreateItem(path, "refresh");
 this->serv->RegisterCommand(path, (this->sv+"->Refresh()").c_str(), "button;rootsys/icons/refresh.png");
 this->serv->Hide(path);
}

void DataVista_SV::Refresh()
{
 this->Init_DebugInfo();
 if (this->gps_plots)
  this->gps_plots->Refresh();
 if (this->ts_plots)
  this->ts_plots->Refresh();
}

TString JoinStr(const std::vector<TString> &vec, const TString &sep=",")
{
 if (vec.empty())
  return "";
 TString res = vec[0];
 for (size_t i = 1; i < vec.size(); ++i)
  res += sep + vec[i];
 return res;
}

void DataVista_SV::Append_IndexPage()
{
 this->serv->SetItemField("/", "_layout", "tabs");
 this->serv->SetItemField("/", "_optimize", "2");

 if (this->ts_plots)
 {
  std::vector<TString> ts_items = {
   "TimeSeries_Plots/energy/c_stacked_energy.json",
   "TimeSeries_Plots/OverviewXE"
  };
  std::vector<TString> ts_options = {"", ""};
  this->serv->SetItemField("/","_drawitem", "[" + JoinStr(ts_items) + "]");
  this->serv->SetItemField("/","_drawopt", "[" + JoinStr(ts_options) + "]");
 }
}

#endif
