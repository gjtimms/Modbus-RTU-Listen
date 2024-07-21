# Modbus-RTU-Listen
ESP32 yaml and c++ include header to passively listen to Modbus comms between a Growatt Inverter and Chint DDSU666 Power Meter, b
This code passively listens to the continuous Modbus traffic between a Growatt 3000 inverter and its Chint DDSU666 power meter, but should be easily adaptable to listen to any other Modbus RTU traffic. The timeout in the debug listening sequence detects the end of a Modbus message (flagged by a 3.5 chacter pause rather than any termination character) and separates them neatly, and the UARTDebug::log_hex(direction, bytes, ','); lambda code logs each message as a std::vector< uint8_t > string of hex chars.
The 8 byte strings are requests for data are ignored and only the 37 bytes (reading Bank 2000H) and 29 bytes (reading bank 4000H) are chopped into 4 byte (8 hex chars) pieces to be converted into float values by the float_from_8hex (ints_4) function in the float_from_8hex.h include file.
The aim is to get almost realtime power data into HA for use automations to optimize the use of power in a battery-less solar system. The data is sent from the esp32 device to Home Assistant via HA's API, updating once per second, and MQTT is not required! The data is read as strings of hex bytes, and converted into single precision float values by the float_from_8hex () c++ function in the include file, then passed by global variables into template sensors, so becoming entities that are available and kept up-to-date in HA itself.
Other data values for frequency, power factor, reactive power and grid frequency -- boring!, always >4.9 and <50.1 in Europe -- can be easily added by adapting the code which I've tried to keep as short as possible.
 ![DDSU666 Registers](https://github.com/user-attachments/assets/6f7b64b9-af53-4e76-8789-1f61dc141637)
!!! The 2 entries in the fifth column count registers and signify 4 bytes which is correct for single precision float values !!!

The other sensor is just an ads1115 analogue to digital converter that gives an overall voltage value from an an array of 8 miniature 0.5 volt, 80 ma solar panels connected in series into a load resistor to give a max output of 4 volts, 80ma into a load resistor.  The mini panels are scattered among the big panels to allow for partial shading, and the voltage across the load resistor, once calibrated, should give an estimate of the power the big panels could produce at any moment if the inverter were not throttling the output because it is has nowhere to send it.
Lines 59-68 and 71-77 marked with ***s can be deleted if it is only the power meter data that is wanted.
The voltage of the mini solar panel array is read by the adc board and passed to pins GPIO 21 and 22 on the esp32.

To use the code:
Put the Float-from-8hex.h file into your ESPHome directory and flash your esp32 use the yaml code in ESPHome and install. 

Hardware required: ![Hardware for Modbus RTU Listen](https://github.com/user-attachments/assets/1309c0f7-3d30-482d-9756-e128a27ce672)
Try swapping the Rx and Tx 

The image shows hardware I used. The Modbus comms are read by ESP32 GPIO pins 16 and 17 after passing through a RS485 to TTL converter. There is no need for a load resistor if the esp32 is within a few metres of the inverter. For some reason I don't understand, perhaps to do with the differential nature of the signal with RS485 comms, I can only read data easily from the 
Chint DDSU666 power meter at the inverter end of the cable, with the orange wire from A+ on the UART<>TTl converter piggy backing onto pin 5 of the Growatt inverter's syscom port, and the brown wire from B- onto pin 3. Pin 5 is one of the pair for the power meter signal, but pin 3 is one of the pair for the inverter's MinShineBus or Third party monitoring equipment, so this seems weird and I would welcome enlightenment. Anyway it works well for me and does not affect the continual comms requests and responses on the RS485 connection, it just passively listens.

