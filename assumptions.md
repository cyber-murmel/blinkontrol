# Assumptions
- exact calibration of the ADC will be carried out at a later point
  - the code contains defines for the upper and lower values based on empirical tests
- the logarithmic function of the potentiometer is desired and doesn't need to be corrected (i.e. no calculation of the pot shaft angle from the voltage)
- the ADC task is the only code writing to the control variables for the LED task
  - ADC and LED task communicate via shared variables
  - the ADC task may call LED interface functions directly
