import { httpRequest, openFile, draw } from "/jsrootsys/modules/main.mjs";
import { gStyle } from "/jsrootsys/modules/core.mjs";

const gpsplotter = 
{
 folder_plot: "/GPS_Plots/json_plot/",

 drawjson: async function(eleid, filename)
 {
  let obj = await httpRequest(this.folder_plot+filename, "object");
  gStyle.fOptStat = 0;
  draw(eleid, obj);
 },

 drawroot: async function(eleid, filename, objname)
 {
  let file = await openFile(this.folder_plot+filename);
  let obj = await file.readObject(objname);
  gStyle.fOptStat = 0;
  draw(eleid, obj);
 }
};

export { gpsplotter };
