~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Program Description: Microprocessors CA Assignment - The Maze Game
Authors: C21508813 Haroun Kassouri & C21342953 Andrei Voiniciuc
Date: 20/11/2022
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#####################
 Project Description
#####################

The Maze Game is a 2D game project which requires the user/player to navigate a maze-like environment without the on-screen character touching the walls of the maze. The player is instructed to collect 3 coins, which are "scattered" around the maze, in order to allow for the opening of a gate, which leads the player to game completion.

######################
 Project Requirements
######################

The requirements for this project were straightforward but also very open-ended. We were tasked with developing a 2D video game on the STM32F031 microprocessor and to display it using a TFT display. 

We were provided with a basic rubric that outlined the mandatory elements we had to respect, such as sound and additional buttons for interaction but our team exceeded these with additional features and code implementation.

########################
 Project Implementation
########################
----------------
    Hardware
----------------

We began the project with the assembly of the hardware components. We placed the STM32F031 microprocessor on a breadboard and established connection to a laptop for power and data transfer/ IDE connection. 

Through the use of wires we integrated a TFT display for visuals, a speaker for audio output, and buttons for user interaction. With the hardware components in place the next step was to synchronize everything with the software.

----------------
    Software
----------------

The main code for the software of The Maze Game was written in the C programming language. We used the Keil µVision IDE to program and synchronize the microprocessor, as well as all other hardware components.

A unique aspect of this project was the use of Python and CMD commands to convert pixels from bitmap images into color codes that the STM32F031 microprocessor could interpret. This allowed us to create characters, animations, and graphical elements for the game.

Using these tools, we were able to create a couple of sprites that we were able to implement into our game, visible on the TFT display.

To allow these sprites to interact with each other, we made use of functions which we had available in C header files, such as isInside, which would check if a specific x,y point was inside a rectangle defined by x,y, width and height.

We made use of numerous counter integer variables to integrate functionality into the game, such as coin counter variables and a total coin count variable.

Using the GPIOA and GPIOB ports on the microprocessor we implemented button functionality, which would provide a true/false response that we could configure to allow for "walking" in the specific direction, and by including a 5th button we had a new action, which we used specifically for opening a gate at the end of the game by "pulling" a lever.

The maze structure itself was created by filling x,y,w,h rectangles with a specific colour. This was all done inside a function we named DrawMaze. We then used the ifInside function to allow interaction of the character with the maze walls, making it so that if the character touched a maze wall he would be sent back to the starting position.

We also included a HUD at the top of the screen, which showed the current coin count as well as status messages (+1 Coin!) when the appropriate function was triggered.

Other functions we used were delay, which would ensure the correct flow of the game, allowing for breaks between functions being triggered.

To make use of the speaker we implemented onto our breadboard, we had a playNote function, which played a specific note using frequencies defined in the musical_notes.h header file.

We then had a pluscoin function, as well as a minuslife function, which would display an appropriate message in the HUD, and play an ascending/descending triad of notes.

For error checking, we had specific checks for practically every scenario possible. An example would be when the user would attempt to walk through the end gate without flicking the lever, a message would be printed in the HUD telling the player to flick the lever, an "error" chime would be played, and the player's sprite would be returned to be returned to the x,y position right before the lever, giving him another chance to flick the lever.

All these functions implemented and working together allowed us to create the full functionality of the game, allowing it to be played smoothly and providing excellent user experience.

########################################
 What would we add if we had more time:
########################################

If we had more time, one of the major elements we wanted to implement into the game would be multiple levels, allowing for longer playtime and an even more entertaining experience with The Maze Game.

Another feature we would have implemented would have been a lives system, giving the player 3 lives/ 3 chances to bump into the maze walls before calling "Game Over". We believe this would have made for a better playing experience too.

We discussed different themes throughout the development of this game, but ultimately decided on a Retro-Cyberpunk theme for both the character and the maze itself, making use of many neon colours to appropriately represent the theme. If we had more time we would have implemented different themes in the other levels we planned for The Maze Game.

A final thought that was considered was possibly having music playing throughout each level, matching its appropriate theme. In the future we may consider implementing some of the afforementioned features in a game created on a newer platform, and a more object-oriented programming language.

##########################
 Final notes and sources:
##########################

In general, we believe the full development process of this game was an excellent learning experience, in planning out each feature, thinking how to implement each feature, what functions would be required, and debugging/ fixing numerous issues.

We were able to respect the requirements for the project, which were given to us along with the assignment, even going beyond what was neccessary in certain areas.

The majority of the code was written by us, making use of the limited resources we had online for a project on our specific ARM microprocessor, as well as a few fundamental display and port functions provided to us by our lecturer, Frank Duignan.

https://eleceng.tudublin.ie/frank/arm/index.html


