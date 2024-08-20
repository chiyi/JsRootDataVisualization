#ifndef GPSPlots_cpp
#define GPSPlots_cpp

GPSPlots::GPSPlots(std::string sv, THttpServer *serv)
{
 this->sv = sv;
 this->serv = serv;
 this->b_Reg_GPS_Plots = false;
 this->Init_Plots();
 this->Init_Registration();
}

std::vector<TString> GPSPlots::Get_Filenames()
{
 std::vector<TString> res;
 TString cmd = TString::Format("ls %s/temperature_*.json | awk -F '/' '{print $NF}'", this->dir_plot.Data());
 TString filestrs = gSystem->GetFromPipe(cmd);
 TString tok;
 Ssiz_t from = 0;
 while (filestrs.Tokenize(tok, from, "\n"))
  res.push_back(tok);

 return res;
}

void GPSPlots::Init_Plots()
{
 this->Init_Taipei();

 auto filenames = this->Get_Filenames();
 if (filenames.size() <= 0)
 {
  // retry once
  this->Init_Temperature_Plots();
  filenames = this->Get_Filenames();
  if (filenames.size() <= 0)
   std::cout << "default table is missing\n";
 }

 this->Nfiles = filenames.size();
 this->GPSFiles.resize(this->Nfiles);
 for(int ifile=0; ifile<this->Nfiles; ifile++)
  this->GPSFiles[ifile] = filenames[ifile];
}

void GPSPlots::Init_Taipei()
{
 TString cmd = TString::Format("ls %s/Taipei_edges.root", this->dir_plot.Data());
 TString b_chk = gSystem->GetFromPipe(cmd);
 if (b_chk.Length()<=0)
  b_chk = gSystem->GetFromPipe("GPSPlots/scripts/make_taipeiedges.sh");
}

void GPSPlots::Init_Temperature_Plots()
{
 TString b_chk = gSystem->GetFromPipe("GPSPlots/scripts/fetch_temperature_toplots.sh OLD");
}

void GPSPlots::Init_Registration()
{
 TString base_path = "/GPS_Plots";
 TString page = "jsrootsys/files/Show_GPSPlots_temperature.htm";
 TString reg_path = "";

 if(!this->b_Reg_GPS_Plots)
 {
  this->serv->CreateItem(base_path, "dir_GPS_Plots");
  this->AddItems(base_path);
  this->serv->AddLocation(TString(base_path)+"/json_plot/", this->dir_plot); // for GPSPlotter.mjs
  this->b_Reg_GPS_Plots = true;
 }

 for( int ifile=0; ifile<this->Nfiles; ifile++)
 {
  if(this->GPSFiles[ifile])
  {
   reg_path = base_path + "/" + this->GPSFiles[ifile];
   this->serv->CreateItem(reg_path, "temperature_page");
   this->serv->SetItemField(reg_path, "_kind", "Text");
   this->serv->SetItemField(reg_path, "value", TString("<iframe width='100%' height=1000 src='") + page + "?json=" + this->GPSFiles[ifile] + "'></iframe>");
   this->serv->SetIcon(reg_path, "rootsys/icons/htmlfile.gif");
 }}
}

void GPSPlots::AddItems(TString path_base)
{
 this->AddItem_Info(path_base);
 this->AddItem_FetchDataToPlots(path_base);
}

void GPSPlots::AddItem_Info(TString path_base)
{
 TString path = path_base + "/OpenWeather_Info";
 this->serv->CreateItem(path, "OpenWeather Info");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", "Get Data from API https://api.openweathermap.org/data/2.5/weather. \n Don't forget to import the API Key from your registrated account.");
}

void GPSPlots::AddItem_FetchDataToPlots(TString path_base)
{
 TString path = path_base + "/OpenWeather_FetchDataToPlots";
 this->serv->CreateItem(path, "get current temperature");
 this->serv->RegisterCommand(path, (this->sv+"->gps_plots->FetchDataToPlots()").c_str());
}

void GPSPlots::FetchDataToPlots()
{
 TString res;
 res = gSystem->GetFromPipe("GPSPlots/scripts/fetch_temperature_toplots.sh UPDATE");
 std::cout << res.Data() << std::endl;
}

void GPSPlots::Refresh()
{
 this->Init_Plots();
 this->Init_Registration();
}

#endif
