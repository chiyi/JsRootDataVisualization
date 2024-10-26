#ifndef TimeSeriesPlots_cpp
#define TimeSeriesPlots_cpp

TimeSeriesPlots::TimeSeriesPlots(std::string sv, THttpServer *serv)
{
 this->sv = sv;
 this->serv = serv;
 this->b_Reg_TimeSeries_Plots = false;
 this->init_plots();
 this->init_registration();
}

std::vector<TString> TimeSeriesPlots::get_filenames()
{
 std::vector<TString> res;
 TString cmd = TString::Format("ls %s/*/*.json | awk -F '/' '{print $(NF-1)\"/\"$NF}'", this->dir_plot.Data());
 TString filestrs = gSystem->GetFromPipe(cmd);
 TString tok;
 Ssiz_t from = 0;
 while (filestrs.Tokenize(tok, from, "\n"))
  res.push_back(tok);

 return res;
}

void TimeSeriesPlots::init_plots()
{
 //always redraw
 this->init_energyplots();
 this->init_MT5plots();

 auto filenames = this->get_filenames();
 if (filenames.size() <= 0)
  std::cout << "default plot is missing\n";

 this->Nfiles = filenames.size();
 this->TimeSeriesFiles.resize(this->Nfiles);
 for(int ifile=0; ifile<this->Nfiles; ifile++)
  this->TimeSeriesFiles[ifile] = filenames[ifile];
}

void TimeSeriesPlots::init_energyplots()
{
 TString tmpout = gSystem->GetFromPipe("TimeSeries/scripts/init_energyplots.sh");
}

void TimeSeriesPlots::init_MT5plots()
{
 TString tmpout1 = gSystem->GetFromPipe("TimeSeries/scripts/init_mt5demodata.sh");
 TString tmpout2 = gSystem->GetFromPipe("TimeSeries/scripts/gen_mt5plots.sh");
}

void TimeSeriesPlots::init_registration()
{
 TString base_path = "/TimeSeries_Plots";
 TString page_energy = "jsrootsys/files/Show_TimeSeriesPlots_energy.htm";
 TString page_mt5 = "jsrootsys/files/Show_TimeSeriesPlots_mt5.htm";
 TString reg_path = "";

 if(!this->b_Reg_TimeSeries_Plots)
 {
  this->serv->CreateItem(base_path, "dir_TimeSeries_Plots");
  this->additems(base_path);
  this->serv->AddLocation(TString(base_path)+"/json_plot/", this->dir_plot);
  this->b_Reg_TimeSeries_Plots = true;
 }

 for( int ifile=0; ifile<this->Nfiles; ifile++)
 {
  if(this->TimeSeriesFiles[ifile])
  {
   reg_path = base_path + "/" + this->TimeSeriesFiles[ifile];
   if (this->TimeSeriesFiles[ifile].BeginsWith("energy/"))
   {
    this->serv->CreateItem(reg_path, "energy_page");
    this->serv->SetItemField(reg_path, "_kind", "Text");
    this->serv->SetItemField(reg_path, "value", TString("<iframe width='100%' height=1000 src='") + page_energy + "?json=" + this->TimeSeriesFiles[ifile] + "'></iframe>");
    this->serv->SetIcon(reg_path, "rootsys/icons/htmlfile.gif");
   }
   if (this->TimeSeriesFiles[ifile].BeginsWith("mt5/"))
   {
    this->serv->CreateItem(reg_path, "mt5_page");
    this->serv->SetItemField(reg_path, "_kind", "Text");
    this->serv->SetItemField(reg_path, "value", TString("<iframe width='100%' height=1000 src='") + page_mt5 + "?json=" + this->TimeSeriesFiles[ifile] + "'></iframe>");
    this->serv->SetIcon(reg_path, "rootsys/icons/htmlfile.gif");
   }
 }}
}

void TimeSeriesPlots::additems(TString path_base)
{
 this->additem_energyinfo(path_base);
 this->additem_compare(path_base);
 this->additem_MT5info(path_base);
 this->additem_overviewXE(path_base);
}

void TimeSeriesPlots::additem_energyinfo(TString path_base)
{
 TString cmd = TString::Format("cat %s", this->file_OurWorldInData_Info.Data());
 TString txt = gSystem->GetFromPipe(cmd);
 TString path = path_base + "/OurWorldInData_Info";
 this->serv->CreateItem(path, "OurWorldInData Info");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", txt.Data());
}

void TimeSeriesPlots::additem_compare(TString path_base)
{
 TString path = path_base + "/CompareEnergy";
 TString page = "jsrootsys/files/Comparison.htm";
 this->serv->CreateItem(path, "Compare two jsons");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", TString("<iframe width='100%' height=1000 src='") + page + "'></iframe>");
 this->serv->SetIcon(path, "rootsys/icons/bld_paste.png");
}

void TimeSeriesPlots::additem_MT5info(TString path_base)
{
 TString cmd = TString::Format("cat %s", this->file_MT5_Info.Data());
 TString txt = gSystem->GetFromPipe(cmd);
 TString path = path_base + "/MetaTrader5_Info";
 this->serv->CreateItem(path, "MetaTrader5 Info");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", txt.Data());
}

void TimeSeriesPlots::additem_overviewXE(TString path_base)
{
 TString path = path_base + "/OverviewXE";
 TString page = "jsrootsys/files/Show_XE_overview.htm";
 this->serv->CreateItem(path, "Overview currency exchange");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", TString("<iframe width='100%' height=1000 src='") + page + "'></iframe>");
 this->serv->SetIcon(path, "rootsys/icons/bld_paste.png");
}

void TimeSeriesPlots::Refresh()
{
 this->init_plots();
 this->init_registration();
}

#endif
