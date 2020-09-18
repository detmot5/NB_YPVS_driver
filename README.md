# NB_YPVS_driver
Better and cheaper alternative for Yamaha Power Valve System in Yamaha DT125.



## System Features
* Smooth regulation of the power valve depending on the engine speed
* Changing the engine characteristics by uploading different maps
* Full separation of logic from hardware
* TDD on desktop - development without access to hardware


## Installing, building uploading to device 
> pip install platformio
> pio run -t upload

## Desktop testing 
> pio test -e native


