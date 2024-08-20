import { httpRequest, openFile, draw } from '/jsrootsys/modules/main.mjs';

const gpsplotter = 
{
 folder_plot: "/GPS_Plots/json_plot/",

 drawjson: async function(eleid, filename)
 {
  let obj = await httpRequest(this.folder_plot+filename, "object");
  draw(eleid, obj);
 },

 drawroot: async function(eleid, filename, objname)
 {
  let file = await openFile(this.folder_plot+filename);
  let obj = await file.readObject(objname);
  draw(eleid, obj);
 }
};

export { gpsplotter };
