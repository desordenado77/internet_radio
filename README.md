internet_radio
==============

This repository contains the code for my arduino yun based internet radio project. More info here: http://how-i-did-that.blogspot.com



Folder Structure
================

Arduino: This has the arduino sketch.
Linino: This folder has the python script that will be run on the linino side. The files yun-radio.py and gs_* should be copied into the folder /root/examples-pygrooveshark/ in the linino root filesystem.
Schematic: This file has the wiring for the LCD and rotary encoder. Other than that, a usb audio interface is also required.



Adding other users, playlists and radios
========================================

There are 3 files that store the users, playlists and radio ids, these are gs_users, gs_playlists and gs_radios. gs_users has one line per user id. Each line should contain the id number and the name of the user. gs_playlists has one line per playlist id. gs_radios has one line per radio id. Each line should contain the id number and the name of the radio.

How to find out the user id number? I found my user id by typing http://grooveshark.com/user-name (replacing user-name with my user name) in my android phone chrome browser, I got redirected to http://grooveshark.com/#!/user/user-name/user-id, where user-id happened to be my id number.

How to find out the playlist id number? Opening the playlist on the PC browser redirected me to the web address http://grooveshark.com/#!/playlist/playlist-name/playlist-id, where playlist-id happened to be the id of the playlist.

How to find out the radio id number? similarly as with the playlist, by going into the genre of the radio you want to play, you will have the radio id in the URL you will be redirected to. 



Requirements
============

On the arduino side, liquid crystal (http://arduino.cc/es/Tutorial/LiquidCrystal) and encoder (http://www.pjrc.com/teensy/td_libs_Encoder.html) libraries are needed for the LCD and rotary encoder.

On the linino side, pygrooveshark (https://github.com/koehlma/pygrooveshark) is required. Linino side also requires mpg123 installed. There are instructions on building and installing mpg123 in http://how-i-did-that.blogspot.com/2014/01/arduino-yun-grooveshark-radio.html


Arduino-linino communication
============================

The communication between the arduino and the linino is done throught the REST interface. The yun-radio.py script gathers all the information about the user radios, playlists and collections and creates key-value pairs for all of them. Once all the key value pairs for the radios, playlists and collections is created, the arduino side can send commands to the linino python script using the keys command, commandData1 and commandData2. There is more info on this in the file rest_interface.txt.


Usage
=====

The menu on the LCD screen is browsed using the rotary encoder. Rotating the shaft is used to move through the menu options, single press is for select and long press is for going back.


