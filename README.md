# 441-FP - There And Back Again
## Guild - ?

Guild consists of Elijah Mt. Castle, Shane Cranor, Jonathan Woolf, and Benjamin Wagley.

## Description

The night is dark and there are obstacles ahead! Play as a small horse running through a desolated city at night. Jump over abandoned structures that block your path. Position yourself using A and D, pressing W to leap over the towers. Be careful, though; if you hit a tower you'll loose one of your valuable 4 lives. If you run out of lives you'll have to start all over. There's a score in the top right that tracks how far you've made it. How far do you think you can go?

Outside of general gameply, there a several controls that alter the world and interaction. Pressing P will pause obstacles (and the score), while pressing O will spawn extra obstacles. If you hold down SHIFT while pressing P, however, the background will freeze in place. Pressing O while holding SHIFT will also spawn extra background objeects. Finally, pressing 0 (zero) will exit gameplay in put you in free camera mode where you can move around the world. Pressing WASD will change where you look, and SPACE or SPACE + SHIFT will move you forwards or backwards respectively.

## Compilation

Compile using CMake. You may need to alter the CMakeLists.txt path's for the include and lib if your include and lib directories are in a different location than the default we've been using for the class. Also, make sure to set the working directory to that of the projects, not the build directory. Afterwhich, everything should run fine.

## Bugs
None that we know of.

## Responsibilites
We each contributed all over the place, so there was a good deal of overlap, but below are areas where we each focused more specifically.
### Elijah
### Shane
### Benjamin
### Jonathan
Created the Blinn Phong shader used for general lighting. Shader allows for multiple light sources and uses of different materials. \
Created billboard shader that uses a geometry shader to produce billboards. Billboards are used for hearts and scoreboard. \
Added lives and scoreboard that grows with progress, along with automatic respawning. \
Added dynamic spotlight that moves with player and changes color on collision, along with player flickering.

## Controls
### In Game Controls
| Key       | Description           |
| --------- | --------------------- |
| P         | Pause Obstacles       |
| SHIFT + P | Pause Background      |
| O         | Spawn Obstacles       |
| SHIFT + O | Spawn Background Wave |
| 0         | Toggle Free Cam       |
| W         | Jump                  |
| A         | Move Left             |
| D         | Move Right            |

### Free Cam Controls
| Key           | Description  |
| ------------- | ------------ |
| W             | Look Up      |
| S             | Look Down    |
| A             | Look Left    |
| D             | Look Right   |
| SPACE         | Move Forward |
| SPACE + SHIFT | Move Forward |

## Other
| Person   | Time Spent | Lab Preparedness | Funometer |
| -------- | ---------- | ---------------- | --------- |
| Elijah   | 8hrs       | 7/10             | 9/10      |
| Jonathan | 9hrs       | 8/10             | 8/10      |
| Benjamin | ?hrs       | ?/10             | ?/10      |
| Shane    | ?hrs       | ?/10             | ?/10      |

## Credit
| Name              | Github                                       | Email               |
| ----------------- | -------------------------------------------- | ------------------- |
| Elijah Mt. Castle | @[Elijah1111](https://github.com/Elijah1111) | someemail@email.com |
| Jonathan Woolf    | @[jlwoolf](https://github.com/jlwoolf)       | jlwoolf@mines.edu   |
| Shane Cranor      | @[bwagley](https://github.com/bwagley)       | someemail@email.com |
| Benjamin Wagley   | @[jlwoolf](https://github.com/jlwoolf)       | someemail@email.com |




