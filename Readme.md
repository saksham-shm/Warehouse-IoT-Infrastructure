## Statement
Pharmaceutical companies, food distributors, and logistics warehouses suffer massive losses when temperature-sensitive goods are stored or transported under wrong conditions — and they only discover the damage after the fact. There is no real-time visibility into storage zones, and unauthorized access to restricted inventory goes unrecorded.

This project delivers a fully monitred, access controlled environment for warehouse facility. Continuously logging the sensors data, access and alerts the moment any parameter dwels out of range(gas leaks, Air quality, lockdown if no one is inside). The warehouse can be monitored and accessed remotely via wifi/hotspot (*must be in same network*)


# Day 1

Day 1, I ended the project by integrating LCD display(16x2) and the RFID tags, and servo motor(door) to work in response the RFID access granted or not. The system we will integrate multiple sensors and modueles to maintain **security and monitoring facility**.

## First and for most (Visualize) - LCD 16x2 

I choose this only because it was easy to build and i can get going and develop my pave into complex aspects of the projects.

==Note: == in the code **SDA and SLC** are initialized via `Wire(SDA_port,SLC_port);` and the lcd via `lcd.init()`

First part of building the system. correctly implementing the screen is necessary. All the information we will calculate and measure throught out PIR sensor, UltraSonic Sensors, Gas and smoke, DHT22 will be displayed over the LCD Screen. A function is made, when called prints the passed argument in LCD two lines.
The LCD dusplay has two lines to output to.

![alt text](images\image.png)
Here the components used till now are **ESP 32** and **LCD 16x2**.


## Relay

Relay module is an electirc switch which helps of control the circuit being On/Off via code.
Relay module is used as a switch, the IN of the relay is connected to a pin to a micro-processor or micro-controller.
When the IN pin is high, the Normally-Open circuit performs, When the IN pin is LOW, the Normally-Closed Circuit performs.
--the COM is the common, 3.3v will be enough.

the Relay is controlled via esp32, and its controlling conponents are connected via external battery source.

![alt text](images\image1.png)

## Buzzer

when every a unauthorized access or any alert needs to be passed regarding temparature or humidity changes out of defined range, the buzzer plays.

The buzzeer is a passive buzzer meaning it can be controlled with PWM, with two ways, an easier way - `tone(buzzer,1000)`, `noTone(buzzer)`, which is what i used bacause it recudes the complexity of the system, and logical operations to perform. 

![alt text](images\image2.png)

## RFID

Warehouse has to be secured, controling who can access andn who can't, for this matter we use RFID, thus here RFID is implemented.

Only with the registered RFID tags authorized to access, the employeers can access the warehouse.

![alt text](images\image3.png)

## Servo motor

using servo motor to controll the warehouse door. Servo has three pins, VCC,GND, and PWM. I connected it with relay--the GND to external power source GND, and V+ to the NO(which ultimately completes the curcuit with external power source), and PWM to the ESP32 to send code on how much to open.
![alt text](images\image4.png)

## Lets get Technical

