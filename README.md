# fancurve

This application controls the fan speed of AMD GPUs according to a set fan curve.

**Usage:**  
* Start the executable with `sudo ./fancurve`.

**Configuration:**
* Create a file `/etc/fancurve/curve`.
* Make your own fan curve by appending lines with the format: `temperatureC fanspeed%`.
* An example config can be found [here](https://github.com/Mantas-2155X/fancurve/blob/master/curve).
