import { httpRequest, openFile, draw, redraw } from "/jsrootsys/modules/main.mjs";
import { gStyle } from "/jsrootsys/modules/core.mjs";

function get_arrmax(arr)
{
 const numarr = arr.filter(val => typeof val === "number" && !isNaN(val));
 return Math.max.apply(Math, numarr);
}

function create_input(container_id, placeholder)
{
 const container = document.getElementById(container_id);
 if (!container)
 {
  console.error(`Container with id ${container_id} not found.`);
  return;
 }
 const input = document.createElement("INPUT");
 input.type = "text";
 input.className = "text-input";
 input.id = container_id + "_inp";
 input.placeholder = placeholder;
 container.appendChild(input);
}

const tsplotter = 
{
 folder_plot: "/TimeSeries_Plots/json_plot/",

 drawjson: async function(eleid, filename)
 {
  try
  {
   const obj = await httpRequest(this.folder_plot+filename, "object");
   gStyle.fOptStat = 0;
   await draw(eleid, obj);
  }
  catch (error)
  {
   console.error(`Failed to draw JSON for ${fileName}:`, error);
  }
 },

 compare: async function(eleid, json1, json2)
 {
  try
  {
   const [obj1, obj2] = await Promise.all([
    httpRequest(this.folder_plot+json1, "object"),
    httpRequest(this.folder_plot+json2, "object")
   ]);
   gStyle.fOptStat = 0;
   const max1 = get_arrmax(obj1.fArray);
   const max2 = get_arrmax(obj2.fArray);
   if (max1>=max2)
   {
    await redraw(eleid, obj1, "LFT2");
    await draw(eleid, obj2, "LFT2SAME");
   }
   else
   {
    await redraw(eleid, obj2, "LFT2");
    await draw(eleid, obj1, "LFT2SAME");
   }
  }
  catch (error)
  {
   console.error("Comparison failed:", error);
  }
 }
};

export { create_input, tsplotter };
