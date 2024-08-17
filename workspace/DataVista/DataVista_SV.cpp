#ifndef DataVista_SV_cpp
#define DataVista_SV_cpp

DataVista_SV *DATAVISTA_SV;

DataVista_SV::DataVista_SV(std::string ptr, std::string logfile)
{
 this->serv = new THttpServer("http:13931?nocache;rw");
 this->logfile = logfile;
 this->sv = ptr;
 this->Init_DebugInfo();
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
}

#endif
