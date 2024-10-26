import { httpRequest, openFile, draw, redraw, cleanup } from "/jsrootsys/modules/main.mjs";
import { gStyle } from "/jsrootsys/modules/core.mjs";

const hmtexter =
{
 create_txtinput: function(container_id, placeholder="")
 {
  const container = document.getElementById(container_id);
  if (!container)
  {
   console.error(`Container with id ${container_id} not found.`);
   return null;
  }
  const input = document.createElement("input");
  input.setAttribute("type", "text");
  input.id = container_id + "_inp";
  input.placeholder = placeholder;
  container.appendChild(input);
  return input;
 },

 create_txtoutput: function(container_id, msg)
 {
  const container = document.getElementById(container_id);
  if (!container)
  {
   console.error(`Container with id ${container_id} not found.`);
   return null;
  }
  const output = document.createElement("input");
  output.defaultValue = msg;
  output.disabled = true;
  output.readOnly = true;
  container.appendChild(output);
  return output;
 },

 create_txtareainput: function(container_id)
 {
  const container = document.getElementById(container_id);
  if (!container)
  {
   console.error(`Container with id ${container_id} not found.`);
   return null;
  }
  const input = document.createElement("textarea");
  input.id = container_id + "_inp";
  input.rows = "40";
  input.cols = "200";

  function insert_tab(event)
  {
   if (event.key === 'Tab')
   {
    event.preventDefault();
    const textarea = event.target;
    const start = textarea.selectionStart;
    const end = textarea.selectionEnd;
    textarea.value = textarea.value.substring(0, start) + "\t" + textarea.value.substring(end);
    textarea.selectionStart = textarea.selectionEnd = start + 1;
   }
  }
  input.addEventListener("keydown", insert_tab);
  container.appendChild(input);
  return input;
 },

 create_txtareaoutput: function(container_id)
 {
  const container = document.getElementById(container_id);
  if (!container)
  {
   console.error(`Container with id ${container_id} not found.`);
   return null;
  }
  const output = document.createElement("textarea");
  output.rows = "10";
  output.cols = "200";
  container.appendChild(output);
  return output;
 }
}

const hmplotter =
{
 idx_plotcycle: 0,    // Current iteration index within the cycle

 N_layers: 7,    // Number of layers to play through

 t_page_ms: 3000,    // Time for displaying each page in milliseconds

 // Filename format: <folder_plot> + "layers/" + <filegroup_layer> + "_" + <idx_plotcycle> + ".root"
 filegroup_layer: "default",    // Prefix for layer filenames
 folder_plot: "/HeatMaps_Plots/json_plot/",    // Folder path for plot data

 timerid_layercycle: 0,    // ID of the timer for cycling through layers

 drawjson: async function(eleid, filename, bredraw=false)
 {
  try
  {
   const obj = await httpRequest(this.folder_plot+filename, "object");
   gStyle.fOptStat = 0;
   if (bredraw)
    await redraw(eleid, obj);
   else
    await draw(eleid, obj);
  }
  catch (error)
  {
   console.error(`Failed to draw JSON for ${filename}:`, error);
  }
 },

 playlayers: async function(eleid, nameid, prefix, N_layers, opt)
 {
  this.idx_plotcycle = this.idx_plotcycle % N_layers;
  // Note: Implementing display for JSON files across various 2D and 3D drawing options can be complicated.
  const fname = `${this.folder_plot}layers/${prefix}_${(this.idx_plotcycle+1)}.root`;

  document.getElementById(nameid).innerHTML = `${fname}    drawopt=${opt}`;
  const file = await openFile(fname, "object");
  const h2d = await file.readObject("h_data");
  const fline = await file.readObject("f_line");
  await redraw(eleid, h2d, `${opt}nostat`);

  if (!opt.startsWith("e") && !opt.startsWith("lego") & !opt.startsWith("surf"))
   await draw(eleid, fline, "SAME");

  this.idx_plotcycle++;
 },

 create_drawopts: function(container_id)
 {
  const container = document.getElementById(container_id);
  if (!container)
  {
   console.error(`Container with id ${container_id} not found.`);
   return null;
  }
  let select = document.createElement('select');
  select.id = container_id + "_sel";
  const options = [
   "colz", "box",
   "candle1", "candle2", "candle3", "candle4",
   "candley1", "candley2", "candley3", "candley4",
   "violin1", "violin2", "violiny1", "violiny2",
   "cont", "cont1", "cont2", "cont3",
   "surf", "surf1", "surf2", "surf4", "surf6",
   "lego0", "lego1", "lego2", "lego3", "lego4",
   "e",
   ""
  ];
  options.forEach(option => {
    let opt = document.createElement('option');
    opt.text = option;
    select.appendChild(opt);
  });

  // Append the select to the container
  container.appendChild(select);
  return select;
 },

 create_cycler: function(eleid, nameid, sel, t_set)
 {
  cleanup(eleid);
  const opt = new URLSearchParams(window.location.search).get("opt") || (sel && sel.value) || "colz";
  if (t_set && t_set.value>0)
  {
   const duration = parseInt(t_set.value);
   if (!isNaN(duration) && duration > 0)
    this.t_page_ms = duration;
   else
    console.warn('Invalid input for page duration, using default.');
  }
  if (this.timerid_layercycle)
   clearTimeout(this.timerid_layercycle);

  const cycler = () =>
  {
   this.playlayers(eleid, nameid, this.filegroup_layer, this.N_layers, opt);
   this.timerid_layercycle = setTimeout(cycler, this.t_page_ms);
  }
  this.idx_plotcycle = 0;
  cycler();
 }
};

export { hmtexter, hmplotter };
