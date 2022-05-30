# Conway's Game of Life

![A screenshot of the Game of Life simulation](screenshot.png?raw=true "Optional Title")

## Introduction
A basic implementation of John Conway's _Game of Life_, originally written in 2017 for my final-year A Level project. 

## Usage
* _Space_ - Run/Pause the simulation
* _Left Click_ - Insert a cell
* _Right Click_ - Delete/kill a cell

The window title will display the current status of the simulation (running/paused). Additionally, while paused a small red border will appear around the edges of the field. The user can only insert/delete cells while the simulation is paused (the program will pause the simulation on start-up to allow the user to draw an initial set of cells).

While running, the simulation will update the field based on a set of rules (outlined by LifeWiki [here](https://conwaylife.com/wiki/Conway%27s_Game_of_Life#Rules)).

Currently the simulation runs however fast the user's computer can run it, however it is limited by vsync (i.e. it will update 60 times a second if the user is using a 60Hz monitor, and runs faster at higher refresh rates).

## Improvements
There are a few planned features which will make the program more user-friendly:
* Allow user to input an custom field size.
* More control over the visual output, e.g. customisable cell size, arbitrary resolutions, panning/scrolling/zooming the field.
* Implement proper timing and allow the user to adjust the speed of the simulation (rather than lazily locking it to vsync).
* Add option to restart/clear simulation