# NB_YPVS_driver
Better and cheaper alternative for Yamaha Power Valve System in Yamaha DT125.


### Project Specs
* MCU - STM32F103c8t6
* Build System - Platformio
* Framework - STMcube HAL
* Test framework - Unity



### System Features
* Smooth regulation of the power valve depending on the engine speed
* Changing the engine characteristics by uploading different maps
* Full separation of logic from hardware
* TDD on desktop - development without access to hardware
* WiFi diagnostic tool

###  WiFI diagnostic tool repo: 
> https://github.com/detmot5/YpvsWifiDiagnosticTool.git

### Kicad Schematics and PCB
> https://github.com/detmot5/nb_YPVS_Sch


### Installing, building and uploading to device 
> pip install platformio

> pio run -t upload

## Desktop testing 
> pio test -e native


