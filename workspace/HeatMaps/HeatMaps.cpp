#ifndef HeatMaps_cpp
#define HeatMaps_cpp

HeatMaps::HeatMaps(std::string sv, THttpServer *serv)
{
 this->sv = sv;
 this->serv = serv;
 this->b_Reg_HeatMaps_Plots = false;
 this->init_plots();
 this->init_registration();
}

HeatMaps::~HeatMaps()
{
 if (this->h_data)
  this->h_data->Delete();
}

std::vector<TString> HeatMaps::get_filenames()
{
 std::vector<TString> res;
 TString cmd = TString::Format("ls %s/%s/*.json | awk -F '/' '{print $NF}'", this->dir_work.Data(), this->dir_plot.Data());
 TString filestrs = gSystem->GetFromPipe(cmd);
 TString tok;
 Ssiz_t from = 0;
 while (filestrs.Tokenize(tok, from, "\n"))
  res.push_back(tok);

 return res;
}

void HeatMaps::init_plots()
{
 //always redraw
 this->init_fakedataplots();

 auto filenames = this->get_filenames();
 if (filenames.size() <= 0)
  std::cout << "default plot is missing\n";

 this->Nfiles = filenames.size();    // 1: if only default
 this->HeatMapsFiles.resize(this->Nfiles);
 for(int ifile=0; ifile<this->Nfiles; ifile++)
  this->HeatMapsFiles[ifile] = filenames[ifile];
}

void HeatMaps::init_fakedataplots()
{
 // via SV internal methods
 this->launch_wholegeneration();
}

void HeatMaps::init_registration()
{
 TString base_path = "/HeatMaps_Plots";
 TString page_heatmap = "jsrootsys/files/Show_HeatMaps.htm";
 TString reg_path = "";

 if(!this->b_Reg_HeatMaps_Plots)
 {
  this->serv->CreateItem(base_path, "dir_HeatMaps_Plots");
  this->additems(base_path);
  this->serv->AddLocation(TString(base_path)+"/json_plot/", this->dir_work + "/" + this->dir_plot);
  this->serv->AddLocation(TString(base_path)+"/func/", this->dir_work + "/" + this->dir_func);
  this->b_Reg_HeatMaps_Plots = true;
 }
    
 for( int ifile=0; ifile<this->Nfiles; ifile++)
 {
  if(this->HeatMapsFiles[ifile])
  {
   reg_path = base_path + "/" + this->HeatMapsFiles[ifile];
   this->serv->CreateItem(reg_path, "heatmap_page");
   this->serv->SetItemField(reg_path, "_kind", "Text");
   this->serv->SetItemField(reg_path, "value", TString("<iframe width='100%' height=1000 src='") + page_heatmap + "?json=" + this->HeatMapsFiles[ifile] + "'></iframe>");
   this->serv->SetIcon(reg_path, "rootsys/icons/htmlfile.gif");
 }}
}

void HeatMaps::additems(TString path_base)
{
 this->additem_fakedatainfo(path_base);
 this->additem_playlayers(path_base);
 this->additem_clientsimulation(path_base);
 this->additem_loadplot(path_base);
}

void HeatMaps::additem_fakedatainfo(TString path_base)
{
 TString cmd = TString::Format("cat %s", this->file_FakeData_Info.Data());
 TString txt = gSystem->GetFromPipe(cmd);
 TString path = path_base + "/FakeData_Info";
 this->serv->CreateItem(path, "FakeData Info");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", txt.Data());
}

void HeatMaps::additem_playlayers(TString path_base)
{
 TString path = path_base + "/PlayLayers";
 TString page = "jsrootsys/files/Play_Layers.htm";
 this->serv->CreateItem(path, "Play simulated layers");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", TString("<iframe width='100%' height=1000 src='") + page + "'></iframe>");
 this->serv->SetIcon(path, "rootsys/icons/bld_paste.png");
}

void HeatMaps::additem_clientsimulation(TString path_base)
{
 TString path = path_base + "/ClientSimulation";
 this->serv->CreateItem(path, "Simulation from Client's UDF");
 this->serv->Register(path, this->simulator);
 this->serv->Hide(path);

 path = path_base + "/SimulationPanel";
 TString page = "jsrootsys/files/UDF_Simulation.htm";
 this->serv->CreateItem(path, "Simulation Panel");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", TString("<iframe width='100%' height=1000 src='") + page + "'></iframe>");
 this->serv->SetIcon(path, "rootsys/icons/bld_paste.png");
}

void HeatMaps::additem_loadplot(TString path_base)
{
 TString path = path_base + "/LoadSimPlot";
 TString page = "jsrootsys/files/Show_Simulation.htm";
 this->serv->CreateItem(path, "Show simulated plos");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", TString("<iframe width='100%' height=1000 src='") + page + "'></iframe>");
 this->serv->SetIcon(path, "rootsys/icons/bld_paste.png");
}

void HeatMaps::Refresh()
{
 this->init_plots();
 this->init_registration();
 this->simulator->Clean_UserDir();
}

void HeatMaps::launch_wholegeneration()
{
 bool b_chk = this->test_defaultfuncs();
 if (b_chk)
 {
  this->gen_defaultheatmap();
  this->gen_defaultlayers();
 }
}

bool HeatMaps::test_defaultfuncs()
{
 TString cmd = TString::Format("%s %s", this->sc_test_f2d.Data(), this->cfg_basefunc.Data());
 int status = gSystem->Exec(cmd);
 return status == 0;
}

void HeatMaps::gen_defaultheatmap()
{
 TString cmd = TString::Format("%s %s %s", this->sc_gen_heatmap.Data(), this->cfg_basefunc.Data(), this->out_fn_default.Data());
 int status = gSystem->Exec(cmd);
}

void HeatMaps::gen_defaultlayers()
{
 TString cmd = TString::Format("%s %s %s", this->sc_gen_layers.Data(), this->cfg_basefunc.Data(), this->out_prefix_layer.Data());
 int status = gSystem->Exec(cmd);
}

#endif
