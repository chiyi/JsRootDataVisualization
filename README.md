# JsRootDataVisualization
**JsRootDataVisualization** is my collection of experienced patterns and examples for data visualization using [JSROOT](https://github.com/root-project/jsroot/blob/master/docs/JSROOT.md). This repository aims to provide practical insights and reusable templates for various data visualization needs.

## Getting Started

To get started, you'll need to:

* build docker image from [rootproject/root:6.32.02-ubuntu24.04](image)
  ```
  cd image
  ./build.sh
  ```
* run JSROOT server 
  ```
  ./run_container.sh
  Launcher/StartServer.sh
  ```


## Collection of templates
* GPS Plot Style : [GPSPlots](workspace/GPSPlots)  
  **GPSPlots** offers a dynamic demonstration of data visualization for GPS trajectory plotting.  
  ![GPSPlots Visualization Screenshot](workspace/GPSPlots/Screenshot_GPSPlots_style.png?raw=true)
* Time Series Plot Style : [TimeSeries](workspace/TimeSeries)  
  **TimeSeries** provides a dynamic demonstration of time series data visualization, focusing on energy consumption trends and finacial market data.  
  ![TimeSeries Visualization Screenshot1](workspace/TimeSeries/Screenshot_TimeSeries_style1.png?raw=true)  
  ![TimeSeries Visualization Screenshot2](workspace/TimeSeries/Screenshot_TimeSeries_style2.png?raw=true)

This repository continues to grow with new visualization techniques and templates, offering a comprehensive resource for anyone interested in data visualization with JSROOT.

