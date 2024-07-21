# Modbus-RTU-Listen
ESP32 yaml and c++ include header to passively listen to Modbus comms between a Growatt Inverter and Chint DDSU666 Power Meter
This code passively listens to the continuous Modbus traffic between a Growatt 3000
inverter and its Chint DDSU666 power meter. The aim is to get almost realtime data
into HA to be used in automations to optimize the use of power in a battery-less
solar system. The data is sent from the esp32 device
to Home Assistant via HA's API, updating once per second, and MQTT is not required!
The data is read as strings of hex bytes, and converted into single precision float values
by the float_from_8hex () c++ function in the include file, then passed by global variables
into template sensors, so becoming entities that are available and up-to-date in HA itself.
Other data values for frequency, power factor, reactive power and grid frequency
(boring!, always >4.9 and <50.1 in Europe) could be easily added by adapting the code
which I've tried to keep as short as possible.
 
The other sensor is just an ads1115 analogue to digital converter that gives an overall voltage 
value from an an array of 8 miniature 0.5 volt, 80 ma solar panels connected in series into a
load resistor to give a max output of 4 volts, 80ma into a load resistor.  The mini panels are
scattered among the big panels to allow for partial shading, and the voltage across the load 
resistor, once calibrated, should give an estimate of the power the big panels could produce 
at any moment if the inverter isn't throttling the output because it is has nowhere to send it.
Lines 59-68 and 71-77 marked with ***s can be deleted if only the power meter data is wanted.

To use the code:
1) Copy the code from the Float-from-8hex.h file and paste it into a file you create
   with that name in your ESPHome directory.
2) Paste the yaml code here into your esp32 device's yaml code in ESPHome and install.

Hardware required: ![WhatsApp Image 2024-07-21 at 14 59 47_7ad4e137](https://github.com/user-attachments/assets/73fedef3-6c71-41e0-83b3-ce1474e2504b)

The image shows hardware I used. The Modbus comms are read by ESP32 GPIO pins 16 and 17 after passing through a RS485 to TTL converter. There is no need for a load resistor if the esp32 is within a few metres of the inverter. For some reason I don't understand, perhaps to do with the differential nature of the signal with RS485 comms, I can only read data easily from the 
Chint DDSU666 power meter at the inverter end of the cable, with the orange wire from A+ on the UART<>TTl converter piggy backing onto pin 5 of the Growatt inverter's syscom port, and the brown wire from B- onto pin 3. Pin 5 is one of the pair for the power meter signal, but pin 3 is one of the pair for the inverter's MinShineBus or Third party monitoring equipment, so this seems weird and I would welcome enlightenment. Anyway it works well for me and does not affect the continual comms requests and responses on the RS485 connection, just passively listening.

The voltage of the mini solar panel array is read by the adc board and passed to pins GPIO 21 and 22 on the esp32.
For some reason I don't understand, I can only get a clear signal when 
