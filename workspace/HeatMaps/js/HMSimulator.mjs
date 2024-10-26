import { httpRequest, openFile, draw, redraw, cleanup, isNodeJs } from "/jsrootsys/modules/main.mjs";
import { gStyle } from "/jsrootsys/modules/core.mjs";

const hmsimulator = 
{
 folder_funcs: "/HeatMaps_Plots/func/",
 folder_simulator: "/HeatMaps_Plots/ClientSimulation/simulator/",
 file_defaulfuncs: "Base_Functions_2D.cfg",
 
 get_dtnow: function()
 {
  const now = new Date();
  const options = { 
   year: "numeric", month: "short", day: "2-digit", 
   hour: "2-digit", minute: "2-digit", 
   timeZoneName: "short", hour12: false, timeZone: "Asia/Taipei"
  };
  const formatter = new Intl.DateTimeFormat("en-US", options);
  const parts = formatter.formatToParts(now);
  const result = parts.reduce(
   (acc, part) =>
   {
    switch(part.type) {
     case "year": acc.year = part.value; break;
     case "month": acc.month = part.value; break;
     case "day": acc.day = part.value.padStart(2, "0"); break;
     case "hour": acc.hour = part.value; break;
     case "minute": acc.minute = part.value; break;
     case "timeZoneName": acc.zone = part.value; break;
    }
    return acc;
   },
   {}
  );
  const { year, month, day, hour, minute, zone } = result;
  return `${year}${month}${day}_${hour}${minute}[${zone}]`;
 },

 gen_userplotname: function(user, file)
 {
  const username = user.value || user.placeholder;
  const outname = file.value || file.placeholder;
  user.value = username;
  file.value = outname;
  return `${user.value}/${file.value}.json`;
 },

 gen_userplotname3d: function(user, file)
 {
  const username = user.value || user.placeholder;
  const outname = file.value || file.placeholder;
  user.value = username;
  file.value = outname;
  return `${user.value}/${file.value}_3d.json`;
 },

 fill_basefuncs: async function(container)
 {
  const file = this.folder_funcs + this.file_defaulfuncs;
  const response = await fetch(file);
  if (!response.ok)
  {
   console.error(`Failed to load ${file}. HTTP status_code= ${response.status}`);
  }
  else
  {
   const text = await response.text();
   if (container && container.tagName === "TEXTAREA")
    container.value = text;
   else
    console.error("Not a Textarea element");
  }
 },

 submit_task: async function(plot, file, user, udf, log)
 {
  if (!user || !udf || !file || !log)
   throw new Error("Missing required parameters for submit_task.");

  const username = user.value;
  const outname = file.value;
  // console.log(udf.value);
  let response = await httpRequest(`/HeatMaps_Plots/ClientSimulation/simulator/root.websocket?user=${username}&out=${outname}&outplot=${plot}`, "post", udf.value);
  console.log(response);
  log.value = response;
 }
};

const wshandler = 
{
 socket: null,
 cnt: 0,

 call: function(eleid)
 {
  let path = `wss://${window.location.host}${hmsimulator.folder_simulator}root.websocket`;
  console.log("Starting socket " + path);
  this.socket = new WebSocket(path);

  this.socket.onopen = this.onopen.bind(this, eleid);
  this.socket.onmessage = this.onmessage.bind(this, eleid);
  this.socket.onclose = this.onclose.bind(this, eleid);
  this.socket.onerror = this.onerror.bind(this, eleid);
 },

 onopen: function(eleid)
 {
  this.show(eleid, "WebSocket initialized");
  this.socket._ready = true;
  this.socket.send("First message from client");
 },

 onmessage: function(eleid, e)
 {
   let msg = e.data;
   if (typeof msg !== "string") {
     return console.log("Unsupported message kind: " + (typeof msg));
   }
   this.show(eleid, "Received: " + msg);
 },

 onclose: function(eleid, event)
 {
  this.show("WebSocket closed. Reason: " + (event.reason || "No reason provided"));
  this.socket._ready = false;
 },

 onerror: function(eleid, error)
 {
  this.socket._ready = false;
  this.show("WebSocket error: " + (error.message || "Unknown error"));
 },

 show: function(eleid, str)
 {
  document.getElementById(eleid).insertAdjacentHTML("beforeend", `<text>${str}</text><br/>`);
  console.log(str);
 }
};

export { hmsimulator, wshandler };
