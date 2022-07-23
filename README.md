# crossy-forest
A C++ endless runner game modeled after the mobile game Crossy Road.

Specifications:
1. The player is controlled by all four arrow keys. 
2. In the dead state (before the player touches any keys), the avatar is centered at the bottom of the screen.
3. The last lane of the screen disappears at a constant rate, and all other lanes move down while a new lane is generated at the top. If the avatar is on the lane disappearing, the avatar dies. 
4. Still obstacles and collectable items are randomly generated as the player moves across the screen.
5. If the player goes off the side of the screen or bottom of the screen, the player dies.
6. Moving obstacles that cross the screen are randomly generated as the player moves across the screen, and if the player gets hit by a moving obstacle, the player dies.
7. Collectable items are tracked and the count is displayed at the top of the screen.
8. After the player dies, a Game Over screen is displayed with the final score. The final score is determined by the number of lanes crossed by the player plus a bonus amount from the collectable items.
9. One of the collectable items is a game booster. If the avatar collects it, the last lane will disappear at a slower rate for 5 seconds.
10. To return to the dead state from the Game Over screen, click the mouse.
