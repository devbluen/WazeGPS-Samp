# 🌍📍 WazeGPS for SERVER SAMP
This is a system inspired by GTA V where it will create a route to the selected destination on your SAMP server.

## Dependencies
- [@GPS Plugin](https://github.com/kristoisberg/samp-gps-plugin) **Download Version 1.4.0 of the Plugin, as 1.4.1 has a loading Error!**

## How to install?
- Add the WazeGPS include to the **"Pawno > Includes"** folder
- Write **#include \<GPS\>** in your gamemode
- Write **#include \<WazeGPS\>** in your gamemode
- Remember to install the GPS Plugin first and add the **#include \<GPS\> before Waze** so that everything works correctly

## Functions
```pawn
SetPlayerWaze(playerid, Float:x, Float:y, Float:z, color = 0x8A44E4FF);
StopWazeGPS(playerid);
IsValidWazeGPS(playerid);
```

## Print
![Logo do Projeto](https://i.imgur.com/GD9aKcK.png)
