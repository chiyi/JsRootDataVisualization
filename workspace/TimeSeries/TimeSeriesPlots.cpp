#ifndef TimeSeriesPlots_cpp
#define TimeSeriesPlots_cpp

TimeSeriesPlots::TimeSeriesPlots(std::string sv, THttpServer *serv)
{
 this->sv = sv;
 this->serv = serv;
 this->b_Reg_TimeSeries_Plots = false;
 this->Init_Plots();
 this->Init_Registration();
}

std::vector<TString> TimeSeriesPlots::Get_Filenames()
{
 std::vector<TString> res;
 TString cmd = TString::Format("ls %s/*.json | awk -F '/' '{print $NF}'", this->dir_plot.Data());
 TString filestrs = gSystem->GetFromPipe(cmd);
 TString tok;
 Ssiz_t from = 0;
 while (filestrs.Tokenize(tok, from, "\n"))
  res.push_back(tok);

 return res;
}

void TimeSeriesPlots::Init_Plots()
{
 //always redraw
 this->Init_Energy_Plots();

 auto filenames = this->Get_Filenames();
 if (filenames.size() <= 0)
   std::cout << "default plot is missing\n";

 this->Nfiles = filenames.size();
 this->TimeSeriesFiles.resize(this->Nfiles);
 for(int ifile=0; ifile<this->Nfiles; ifile++)
  this->TimeSeriesFiles[ifile] = filenames[ifile];
}

void TimeSeriesPlots::Init_Energy_Plots()
{
 TString b_chk = gSystem->GetFromPipe("TimeSeries/scripts/init_energyplot.sh");
}

void TimeSeriesPlots::Init_Registration()
{
 TString base_path = "/TimeSeries_Plots";
 TString page = "jsrootsys/files/Show_TimeSeriesPlots_energy.htm";
 TString reg_path = "";

 if(!this->b_Reg_TimeSeries_Plots)
 {
  this->serv->CreateItem(base_path, "dir_TimeSeries_Plots");
  this->AddItems(base_path);
  this->serv->AddLocation(TString(base_path)+"/json_plot/", this->dir_plot);
  this->b_Reg_TimeSeries_Plots = true;
 }

 for( int ifile=0; ifile<this->Nfiles; ifile++)
 {
  if(this->TimeSeriesFiles[ifile])
  {
   reg_path = base_path + "/" + this->TimeSeriesFiles[ifile];
   this->serv->CreateItem(reg_path, "energy_page");
   this->serv->SetItemField(reg_path, "_kind", "Text");
   this->serv->SetItemField(reg_path, "value", TString("<iframe width='100%' height=1000 src='") + page + "?json=" + this->TimeSeriesFiles[ifile] + "'></iframe>");
   this->serv->SetIcon(reg_path, "rootsys/icons/htmlfile.gif");
 }}
}

void TimeSeriesPlots::AddItems(TString path_base)
{
 this->AddItem_Info(path_base);
 this->AddItem_Compare(path_base);
}

void TimeSeriesPlots::AddItem_Info(TString path_base)
{
 TString path = path_base + "/OurWorldInData_Info";
 this->serv->CreateItem(path, "OurWorldInData Info");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", "Obtain the global primary energy consumption by source \n from the Our World in Data graph on global energy substitution \n (https://ourworldindata.org/grapher/global-energy-substitution). \n Note that for years before 1965, the data might show '0 TWh consumption' or 'no data' for some sources.");
}

void TimeSeriesPlots::AddItem_Compare(TString path_base)
{
 TString path = path_base + "/Compare";
 TString page = "jsrootsys/files/Comparison.htm";
 this->serv->CreateItem(path, "Compare two jsons");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", TString("<iframe width='100%' height=1000 src='") + page + "'></iframe>");
 this->serv->SetIcon(path, "rootsys/icons/bld_paste.png");
}

void TimeSeriesPlots::Refresh()
{
 this->Init_Plots();
 this->Init_Registration();
}

#endif
