# Space Invaders Infinity
## About the game

Space Invaders Infinity is a 8x16 LED Matrix Arduino game which expands the original Space Invader's mechanics. The main addition is "traveling between levels". What I mean by that is: you beat the bad guys, you travel to the next area by dodging asteroids, debris, comets, etc and repeat. 

It is also called Infinity because after beating the last boss, the player can choose to continue playing until he loses all the lives.

## Game specifications

The game has a scoring system where you get points for destroying enemy ships, bonus score for clearing levels without taking damage, traveling to another level successfully. In Infinity mode, all score gain will be halved.

The difficulty of the game will increase with each level. Enemies will shoot more often and ship's speed will increase when travelling between levels.

First three highscores will be saved in EEPROM, and the game will feature buying lives with score after travelling to another planet.
