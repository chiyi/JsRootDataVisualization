#ifndef LaunchSimulator_cpp
#define LaunchSimulator_cpp

#include "LaunchSimulator.h"

LaunchSimulator::LaunchSimulator(
   const char *name, const char *title,
   const TString &html,
   const TString &dir_work, const TString &dir_func, const TString &dir_plot,
   const TString &sc_test_f2d, const TString &sc_gen_heatmap
  )
   : THttpWSHandler(name, title), html(html), dir_work(dir_work), dir_func(dir_func+"/user"), dir_plot(dir_plot+"/user"), sc_test_f2d(sc_test_f2d), sc_gen_heatmap(sc_gen_heatmap)
{
 this->Clean_UserDir();
}

// load custom HTML page when open correspondent address
TString LaunchSimulator::GetDefaultPageContent()
{
 return this->html;
}

Bool_t LaunchSimulator::ProcessWS(THttpCallArg *arg)
{
 if (!arg) return true;

 TString subject_svlog = "ProcessWS";
 TDatime now;

 if (arg->IsMethod("WS_CONNECT"))
  return fWSId == 0;
 
 if (arg->IsMethod("WS_READY"))
 {
  fWSId = arg->GetWSId();
  return true;
 }
 
 if (arg->IsMethod("WS_CLOSE"))
 {
  fWSId = 0;
  return true;
 }
 
 if (arg->IsMethod("WS_DATA"))
 {
  TString str((const char *)arg->GetPostData(), arg->GetPostDataLength());
  this->print_svlog(subject_svlog, TString("Client msg: ")+str);
  now = TDatime();
  SendCharStarWS(arg->GetWSId(), TString::Format("LaunchSimulator replies:%s server counter:%d", now.AsString(), fServCnt++));
  return true;
 }

 if (arg->IsMethod("POST"))
  return process_postdata(arg);

 return false;
}

void LaunchSimulator::Clean_UserDir()
{
 int status = gSystem->Exec(this->sc_cleandir);
}

Bool_t LaunchSimulator::process_postdata(THttpCallArg* arg)
{
 TString subject_svlog = "process_postdata";
 TString data((const char *) arg->GetPostData(), arg->GetPostDataLength());
 TUrl tmpurl(TString("http://localhost/dummy.htm?") + arg->GetQuery());
 TString user(tmpurl.GetValueFromOptions("user"));
 TString outname(tmpurl.GetValueFromOptions("out"));
 TString outjson(tmpurl.GetValueFromOptions("outplot"));

 // DEBUG: Uncomment the following line if you want to log received POST data
 this->print_svlog(subject_svlog, TString("Received POST data: ")+data);
 this->print_svlog(subject_svlog, TString("ArgQuery(): ")+arg->GetQuery());
 this->print_svlog(subject_svlog, TString("user: ")+user);
 this->print_svlog(subject_svlog, TString("outname: ")+outname);

 //TEST and CREATE
 TString path_udf = write_udfs(user, outname, data);
 TString path_log = path_udf + ".log";
 int status1 = test_udfs(path_udf, path_log);
 this->print_svlog(subject_svlog, TString("status1(test_udf)=")+status1);
 if (path_udf.Length() > 0 && status1 == 0)
 {
  this->print_svlog(subject_svlog, "To Create SimData from UDFs");
  int status2 = create_simdata(outjson, user, path_udf, path_log);
  if (status2 == 0)
   this->print_svlog(subject_svlog, TString("user:")+user+" file:"+outjson+" is completed.");
  else
   this->print_svlog(subject_svlog, TString("simulation from ")+outname+" is failed.");
 }
 else
  this->print_svlog(subject_svlog, TString("Test User-Defined Functions Failed  ")+outname);

 TString log = get_log(path_log);
 arg->SetContentType("application/text");
 arg->SetContent(log);
 return true;
}

void LaunchSimulator::print_svlog(const TString &subject, const TString &log)
{
 TString title = TString::Format("[LaunchSimulator::%s]:", subject.Data());
 std::cout << title << " " << log << std::endl;
}

void LaunchSimulator::print_sverr(const TString &subject, const TString &err)
{
 TString title = TString::Format("[LaunchSimulator::%s]:", subject.Data());
 std::cerr << title << " " << err << std::endl;
}


TString LaunchSimulator::gen_cmd(const std::vector<TString> &exe_args)
{
 TString res;
 for (const auto item: exe_args)
  res += " " + item;
 res.Remove(0, 1);
 return res;
}

TString LaunchSimulator::write_udfs(const TString &user, const TString &outname, TString data)
{
 TString res="";

 TString subject_svlog = "write_udfs";
 TString cmd = this->gen_cmd(std::vector<TString>({this->sc_mkdir, this->dir_func, user}));
 int status = gSystem->Exec(cmd);
 res = this->dir_work + "/" + this->dir_func + "/" + user + "/" + outname;
 FILE *fw = fopen(res, "w");
 if (fw == NULL)
 {
  res = "";
  this->print_sverr(subject_svlog, TString("Failed to open file for writting"));
 }
 else
 {
  data.Puts(fw);
  fclose(fw);
 }

 return res;
}

int LaunchSimulator::test_udfs(TString path_udf, TString path_log)
{
 if (path_udf.BeginsWith(this->dir_work + "/"))
  path_udf = path_udf(this->dir_work.Length()+1, path_udf.Length());
 TString cmd = TString::Format("%s %s > %s 2>&1", this->sc_test_f2d.Data(), path_udf.Data(), path_log.Data());
 int status = gSystem->Exec(cmd);
 return status;
}

TString LaunchSimulator::get_log(TString path_log)
{
 TString cmd = TString::Format("cat %s", path_log.Data());
 return gSystem->GetFromPipe(cmd);
}

int LaunchSimulator::create_simdata(TString outjson, TString user, TString path_udf, TString path_log)
{
 TString outdir = this->dir_plot + "/" + user;
 TString cmd = this->gen_cmd(std::vector<TString>({this->sc_mkdir, this->dir_plot, user}));
 gSystem->Exec(cmd);

 if (path_udf.BeginsWith(this->dir_work + "/"))
  path_udf = path_udf(this->dir_work.Length()+1, path_udf.Length());
 cmd = TString::Format(
  "%s >> %s 2>&1",
  this->gen_cmd(std::vector<TString>({this->sc_gen_heatmap, path_udf, this->dir_plot+"/"+outjson})).Data(),
  path_log.Data()
 );
 int status = gSystem->Exec(cmd);
 return status;
}

#endif
