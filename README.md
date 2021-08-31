# fancurve

This application controls the fan speed of AMD GPUs according to a set fan curve.

**Build:**
* `mkdir release && cd release`
* `cmake -DCMAKE_BUILD_TYPE=Release ..`
* `make`

**Install:**
* `sudo make install`

**Usage:**  
* Start the executable with `sudo ./fancurve` or `sudo service fancurve start` as a systemd service

**Configuration:**
* Create a file `/etc/fancurve/curve`
* Make your own fan curve by appending lines with the format: `temperatureC fanspeed%`
* An example config can be found [here](https://github.com/Mantas-2155X/fancurve/blob/master/curve)

**Systemd service:**
* `sudo cp fancurve.service /etc/systemd/system`
* `sudo chmod 644 /etc/systemd/system/fancurve.service`

**Uninstall:**
* Remove `/usr/local/bin/fancurve`
* If installed service, remove `/etc/systemd/system/fancurve.service`
