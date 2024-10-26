#ifndef GPSPlots_cpp
#define GPSPlots_cpp

GPSPlots::GPSPlots(std::string sv, THttpServer *serv)
{
 this->sv = sv;
 this->serv = serv;
 this->b_Reg_GPS_Plots = false;
 this->init_plots();
 this->init_registration();
}

std::vector<TString> GPSPlots::get_filenames()
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

void GPSPlots::init_plots()
{
 this->init_Taipei();

 auto filenames = this->get_filenames();
 if (filenames.size() <= 0)
 {
  // retry once
  this->init_temperature_plots();
  filenames = this->get_filenames();
  if (filenames.size() <= 0)
   std::cout << "default table is missing\n";
 }

 this->Nfiles = filenames.size();
 this->GPSFiles.resize(this->Nfiles);
 for(int ifile=0; ifile<this->Nfiles; ifile++)
  this->GPSFiles[ifile] = filenames[ifile];
}

void GPSPlots::init_Taipei()
{
 TString cmd = TString::Format("ls %s/Taipei_edges.root", this->dir_plot.Data());
 TString b_chk = gSystem->GetFromPipe(cmd);
 if (b_chk.Length()<=0)
  b_chk = gSystem->GetFromPipe("GPSPlots/scripts/make_taipeiedges.sh");
}

void GPSPlots::init_temperature_plots()
{
 TString b_chk = gSystem->GetFromPipe("GPSPlots/scripts/fetch_temperature_toplots.sh OLD");
}

void GPSPlots::init_registration()
{
 TString base_path = "/GPS_Plots";
 TString page = "jsrootsys/files/Show_GPSPlots_temperature.htm";
 TString reg_path = "";

 if(!this->b_Reg_GPS_Plots)
 {
  this->serv->CreateItem(base_path, "dir_GPS_Plots");
  this->additems(base_path);
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

void GPSPlots::additems(TString path_base)
{
 this->additem_info(path_base);
 this->additem_fetchdata_to_plots(path_base);
}

void GPSPlots::additem_info(TString path_base)
{
 TString path = path_base + "/OpenWeather_Info";
 this->serv->CreateItem(path, "OpenWeather Info");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", "Get Data from API https://api.openweathermap.org/data/2.5/weather. \n Don't forget to import the API Key from your registrated account.");
}

void GPSPlots::additem_fetchdata_to_plots(TString path_base)
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
 this->init_plots();
 this->init_registration();
}

#endif
