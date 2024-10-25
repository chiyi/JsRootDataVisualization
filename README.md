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
* Heat Map Visualization : [HeatMaps](workspace/HeatMaps)  
  **HeatMaps** features tools for creating heat maps from hypothetical trading data, displaying the relationship between MarketPrice and IntrinsicValue.
  It includes static visualizations, dynamic layers, and simulation capabilities.  
  ![HeatMaps Visualization Screenshot1](workspace/HeatMaps/Screenshot_HeatMaps_demo1.png?raw=true)  
  ![HeatMaps Visualization Screenshot2](workspace/HeatMaps/Screenshot_HeatMaps_demo2.png?raw=true)
  
This repository serves as a curated collection of visualization templates, providing both functional tools and inspirational examples for showcasing interesting data through JSROOT visualizations.




## Notes

* **Specialization Over Aggregation:** This repository combines various templates each tailored to specific types of data visualization. Unlike a general-purpose server, each template here focuses on distinct characteristics to serve particular needs effectively.
* **Multilingual Implementation:** Examples within this repo demonstrate the integration of C++, JavaScript, and shell scripts, showcasing how to customize a JSROOT server. For practical applications, users might prefer focusing on one primary technology stack for simplicity.
* **User Management:** While this repository does not implement member handling directly, the systems can leverage operating system-level account management for user authentication and authorization.
