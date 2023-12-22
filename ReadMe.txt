Programming with C/C++ 4: Computer Games in 2D 5SD814 54825 HT2023
Student: Gustaf Styf

For this assignment we were tasked with creating a remake/demake of a 2D arcade like game. We were given free choice of which one to pick, so I settled for a Bomberman-like game.

The game features 4 kinds of collision detection, all of which can be tested in a "Test Room" accessed from the Title screen. The game also features Parallax scrolling in two of the Screens, and a highscore system. For the most part this game reuses most code for update and collision to be used for in-game entities of different kinds, and have the resoluition of the collision be determined by the specific entitykind.

How to play:
This game is played with WASD or the Arrow Keys for movement
Backspace is used for opening the menu which is navigated with the movement keys,
Enter is used for menu selection while the menu is open,
Enter or Space Key during gameplay will place bombs.

Tips:
You can't place bombs on top of walls or enemies. They need to be placed in the ground where there is space. There is a red dot projection to show where you are placing the bomb.
You can kick bombs by running into them. This saves you from walking up to the wall or enemy you're trying to blow up.
Bombs detonate on impact with enemies and destructible walls, making for faster clearning.
Bomb blasts hurt the player too, as does walking into enemies. Don't worry, you'll notice when a bomb is about to explode.