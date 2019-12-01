# Schrödinger Quest

Schrödinger Quest is a platformer game consisting of two levels where the player (apart from moving and jumping like any other platform game) can become intangible being able to pass through most platforms like a ghost. However, there are some platforms specifically designed to counter this ability, becoming only tangible when the player is intangible. Also, there are some enemies that will try to chase and kill the player when he is tangible (some even to the extent of chasing him when he becomes intangible). This game has been made by two second year students of the Design and Development of Videogames degree in CITM Terrassa (UPC) using C++ in Visual Studio and with SDL and pugixml libraries, Tiled for the creation of the maps and Brofiler for checking the time spent in functions of the code. 

## Usage

This game is played using these keys in the keyboard and this button of the mouse:

* Movement with WASD.

* Jump with spacebar.

* Change tangibility with left click of mouse.

* Attack with right click of mouse.

* Change volume with +/-.

* Exit the game with Escape.

Apart from these keys, there are some debug keys:

* Start from the first level with F1.

* Start from the second level with F2.

* Start from the beginning of the current level with F3.

* Save the current state with F5.

* Load the previous state with F6.

* Show colliders and pathfinding with F9.

* God Mode with F10.

* Enable/Disable FPS cap to 30 with F11.

## Authors
* Arnau Falgueras Garcia de Atocha
    * Github account: Arnau77(https://github.com/Arnau77)

* Miquel Suau Gonzàlez
    * Github account: MayKoder(https://github.com/MayKoder)

## Github link
https://github.com/DESVJ/Schrodinger_Quest

## Innovations

Here is a list with the innovations done in this delivery:

* Use of right button mouse.

* Animations of enemies loaded with Tiled.

* Added game window/taskbar icon

## Work by members

* Arnau Falgueras Garcia de Atocha

  * State machine of enemies
  
  * Entity System
  
  * Brofiler
  
  * UML

* Miquel Suau Gonzàlez

  * Tiled related with enemies
  
  * Collisions with enemies
  
  * Dt

  * Cap to 30 without vsync

## Credits

In order to make this game, we have used art not owned by us. Below we credit the authors of this art:

* Background & Tilesets:

    * 16x16 Industrial Tileset by Robert:
    	* https://0x72.itch.io/
    
		Downloaded from https://0x72.itch.io/16x16-industrial-tileset

    * Taiga: Asset Pack by vnitti:
    	* https://vnitti.itch.io/
		
		Downloaded from https://vnitti.itch.io/taiga-asset-pack

     * Living Tissue Background by ansimuz:
     	* https://opengameart.org/users/ansimuz
		
		Downloaded from https://opengameart.org/content/living-tissue-background


* Sprites of the player: Pixel Art Adventurer Sprites by Elthen:
    * https://elthen.itch.io/

	Downloaded from https://elthen.itch.io/pixel-art-adventurer-sprites

* Sprites of the enemies (bat): Bat Sprite Pack by Elthen:
    * https://elthen.itch.io/

	Downloaded from https://elthen.itch.io/bat-sprite-pack

* Sprites of the enemies (snake): 2D Pixel Art Cobra Sprites by Elthen:
    * https://elthen.itch.io/

	Downloaded from https://elthen.itch.io/2d-pixel-art-cobra-sprites

* Music: Platformer Game Music Pack by CodeManu:
    * https://opengameart.org/users/codemanu

	Downloaded from	https://opengameart.org/content/platformer-game-music-pack

* SFX: Sounds Effects of Sonic the Hedgehog 2 by Sega, Sonic Team and Sega Technical Institue: 
    * https://www.sega.com/
    * http://www.sonicteam.com/        
    * https://en.wikipedia.org/wiki/Sega_Technical_Institute

	Downloaded from: https://www.sounds-resource.com/genesis_32x_scd/sonicthehedgehog2/sound/2628/

## License

MIT License

Copyright (c) [2019] [Arnau Falgueras Garcia de Atocha, Miquel Suau Gonzàlez]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
