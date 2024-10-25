## HeatMaps

![HeatMaps Visualization Screenshot1](Screenshot_HeatMaps_demo1.png?raw=true)  
![HeatMaps Visualization Screenshot2](Screenshot_HeatMaps_demo2.png?raw=true)

### Overview

The **HeatMaps** folder includes tools for creating heat maps from fictitious trading data,
 highlighting how MarketPrice compares to IntrinsicValue on a log scale.
 This data is shaped by customizable user functions.

### Features
- Basic visualization : static heat-map data interacting by GUI in the page "heatmap_default.json" and "heatmap_default_3d.json" .
- **Static Visualization:**
  - **Files:** `heatmap_default.json`, `heatmap_default_3d.json`
  - Delivers static heat maps that illustrate the spread between MarketPrice and IntrinsicValue,
    enhanced by an interactive GUI for closer examination.

- **Dynamic Movie Player:**
  - **File:** `PlayLayers`
  - This feature allows users to view heat maps as they evolve, displaying various scenarios or data layers dynamically.

- **Simulation Tools:**
  - **Files:** `SimulationPanel` and `LoadSimPlot`
  - A framework where users can input custom functions to simulate and visualize different market conditions on heat maps.

### Examples

![HeatMaps Visualization Example1](Screenshot_HeatMaps_example1.png?raw=true) 
![HeatMaps Visualization Example2](Screenshot_HeatMaps_example2.png?raw=true) 
![HeatMaps Visualization Example3](Screenshot_HeatMaps_example3.png?raw=true)


### Usage

- **Interaction:** Users can manipulate views and delve into data layers through the GUI with ease.
- **Customization:** The ability to edit user-defined functions (UDFs) lets users personalize the visualization to fit their analytical needs.
