# Temperature display for aqara sensor with xiaomi gateway v2
This is a program for Wemos D1 mini to display information from 
two temperature and humidity sensors Aqara (WSDCGQ11LM). Like this:

![sensor.png](images/sensor.png)

This program works with Xiaomi Gateway V2 in developer mode. 
This program can connect to the hub through a TCP port using WI-FI. 
Then the hub can be connected to the sensor using the ZigBee protocol, 
and you can see the information from the sensor.
You need to configure this program is to find the sids of your sensors.

![board_bb.png](images/board_bb.png)

It can also get information from openweathermap from the Internet.
The display looks like this:

![display.jpg](images/display.jpg)

# How to flash

After downloading this source, rename config.ino.example to config.ino and write your options there.

