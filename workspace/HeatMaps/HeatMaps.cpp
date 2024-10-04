#ifndef HeatMaps_cpp
#define HeatMaps_cpp

HeatMaps::HeatMaps(std::string sv, THttpServer *serv)
{
 this->sv = sv;
 this->serv = serv;
 this->b_Reg_HeatMaps_Plots = false;
 this->Init_Plots();
 this->Init_Registration();
}

HeatMaps::~HeatMaps()
{
 if (this->h_data)
  this->h_data->Delete();
}

std::vector<TString> HeatMaps::Get_Filenames()
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

void HeatMaps::Init_Plots()
{
 //always redraw
 this->Init_FakeData_Plots();

 auto filenames = this->Get_Filenames();
 if (filenames.size() <= 0)
  std::cout << "default plot is missing\n";

 this->Nfiles = filenames.size();    // 1: if only default
 this->HeatMapsFiles.resize(this->Nfiles);
 for(int ifile=0; ifile<this->Nfiles; ifile++)
  this->HeatMapsFiles[ifile] = filenames[ifile];
}

void HeatMaps::Init_FakeData_Plots()
{
 // via SV internal methods
 this->LaunchProc_WholeGeneration();
}

void HeatMaps::Init_Registration()
{
 TString base_path = "/HeatMaps_Plots";
 TString page_heatmap = "jsrootsys/files/Show_HeatMaps.htm";
 TString reg_path = "";

 if(!this->b_Reg_HeatMaps_Plots)
 {
  this->serv->CreateItem(base_path, "dir_HeatMaps_Plots");
  this->AddItems(base_path);
  this->serv->AddLocation(TString(base_path)+"/json_plot/", this->dir_plot);
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

void HeatMaps::AddItems(TString path_base)
{
 this->AddItem_FakeDataInfo(path_base);
 this->AddItem_PlayLayers(path_base);
}

void HeatMaps::AddItem_FakeDataInfo(TString path_base)
{
 TString cmd = TString::Format("cat %s", this->file_FakeData_Info.Data());
 TString txt = gSystem->GetFromPipe(cmd);
 TString path = path_base + "/FakeData_Info";
 this->serv->CreateItem(path, "FakeData Info");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", txt.Data());
}

void HeatMaps::AddItem_PlayLayers(TString path_base)
{
 TString path = path_base + "/PlayLayers";
 TString page = "jsrootsys/files/Play_Layers.htm";
 this->serv->CreateItem(path, "Play simulated layers");
 this->serv->SetItemField(path, "_kind", "Text");
 this->serv->SetItemField(path, "value", TString("<iframe width='100%' height=1000 src='") + page + "'></iframe>");
 this->serv->SetIcon(path, "rootsys/icons/bld_paste.png");
}

void HeatMaps::Refresh()
{
 this->Init_Plots();
 this->Init_Registration();
}

void HeatMaps::LaunchProc_WholeGeneration()
{
 bool b_chk = this->Test_DefaultFuncs();
 if (b_chk)
 {
  this->Gen_DefaultHeatMap();
  this->Gen_DefaultLayers();
 }
}

bool HeatMaps::Test_DefaultFuncs()
{
 TString cmd = this->sc_test_f2d;
 int status = gSystem->Exec(cmd);
 return status == 0;
}

void HeatMaps::Gen_DefaultHeatMap()
{
 TString cmd = this->sc_gen_heatmap + " " + this->cfg_basefunc + " " + this->out_fn_default;
 int status = gSystem->Exec(cmd);
}

void HeatMaps::Gen_DefaultLayers()
{
 TString cmd = this->sc_gen_layers + " " + this->cfg_basefunc + " " + this->out_prefix_layer;
 int status = gSystem->Exec(cmd);
}

#endif
