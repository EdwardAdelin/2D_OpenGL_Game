# Bucharest the Nightmare Trafffic

A 2D concept game prototype where players navigate chaotic traffic in Bucharest. Built using pure OpenGL.

## Table of Contents

- [Instructions](#instructions)
- [Gameplay](#gameplay)
- [Logic Approach](#logicapproach)
- [Features](#features)
- [Controls](#controls)
- [Installation](#installation)
- [Technical Details](#technical-details)
- [Future Plans](#future-plans)
- [Credits](#credits)
- [License](#license)

## Instructions

Build and run the game.
Press ENTER in order to start the game.
Press Right Mouse button or Left Mouse button to steer right or left.
Press W or S to drive faster or slower.
Crashes will be reported in the console. Game closes in 3 seconds after a collision.

Collision algorithm:

- knowing the size of a car, we track the center point of that car
- center point is located at 1/2 of height, width of the car
- collision occurs if:
  distance between middle points of two cars <= ( car1.middle_point radius / 2 ) + ( car2.middle_point radius / 2)
- colission algorithm keeps track of the area of the car, being more efficient than keeping track of all the points of the car

## Gameplay

In _Bucharest the Nightmare Trafffic_, players control a vehicle from an overhead view as they attempt to maneuver through
traffic jams, avoid obstacles, and reach their destination.
The game captures the intense and unpredictable nature of Bucharest's traffic.

## Logic Approach

The game wants to simulate real life horror scenarios of traffic. The overhead view shows 4 traffic lanes. Cars drive forward/upward on the right side of the road (like in Romania).
In order to spawn cars comming from the left side of the road, we spawn a car from the top side of the screen and constantly reduce the "y" coordonates.
In order to spawn cars comming from the right side of the road, we also spawn a car from the top side of the screen, but we take into consideration "optical illusions".

The road also flows in the same way (lane spllitting segments flow from the upper part of the screen, to the low part of the screen).
If cars driving on the left side of the road drive faster than the cars on the right side of the road, due to relativity to our car (wich seems to be always driving forward
due to the flow of the road segments), the cars driving faster seem to be "faster" not because of higher speed, but because they drive in the opposite direction.

Having a road that moves on Y axis, a player`s car that only moves on X axis, and traffic cars that move on X and Y axis, we simulate real life scenarios.
This optical illusions give us the ocasion of creating a fast game, that does not consume resources but gives the impression of an open world map, with infinite length.

## Features

- Overhead view with a realistic representation of traffic flow
- Dynamic obstacles and unpredictable events
- Simple but challenging mechanics to keep players engaged
- 2D graphics created using pure OpenGL

## Controls

- Enter: Start the game
- Mouse Buttons: Lef and Right turn
- Arrow Keys: Throttle and Brake

## Installation

Install MVS 2022 from web. Open the .SLN file located in your game folder. Build and run the project via MVS 2022.

## Technical Details

This project is built with:

- **Language:** C++
- **Graphics:** Pure OpenGL for 2D rendering
- **Libraries:** GLFW for window and input handling
- **Platform:** Tested on Windows

## Future Plans

- Implement 3D graphics
- Add more levels with varying difficulty
- Improve traffic physics and collision handling
- Polish graphics and add UI elements

## Credits

- Developed by: [Jercau Hadasa Stefana][Finichiu Eduard Adelin]
- Art and assets by: [Aria by Opera]
- OpenGL tutorials that helped: [ACG Lectures and Labs, UPB, FILS, CTI-(E)2024]
