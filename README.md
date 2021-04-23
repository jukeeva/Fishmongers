# Fishmongers
## Sensors unit 1 drives IR lasers with a current mirror circuit controlled by mosfet transistors.
## Sensors unit 1 sends IR data that is received by the sensors unit 2. 
## Sensor unit 2 generates PWM signal that is filtered with a low-pass filter and fed into a bipolar junction transistor which enables current flow and the signal logic “1” is generated. 
## When there is a fish preventing IR laser beams from reaching the receiver sensors, the signal logic is “0”.

<img width="736" alt="fishmongers" src="https://user-images.githubusercontent.com/44908342/115925035-35802f00-a489-11eb-8153-35be9169afa9.PNG"> 
## Below is the PWD filtering circuit for civerting presence of fish to logic "1" and absence of the fish to logic "0"

<img width="471" alt="irreceivercircuit" src="https://user-images.githubusercontent.com/44908342/115925171-65c7cd80-a489-11eb-85fa-cc51ffe658f2.PNG">
