# Severity explained:

level 1: Balance issues or minor visual issues.

level 2: A nuisance for the user, either from a technical perspective or mechanical one.

level 3: Program breaking in nature. Will probably have the same origin as a level 2 bug.


###### Bug template:
- Description: *Mandatory*
- Possible solution: *Optional*
- Cause: *Optional* *guessing is allowed*


## LEVEL 3:



## LEVEL 2:

###### Rendering transparent meshes, looks through incorrect
- Description: Rendering transparent meshes makes you look through other meshes that are drawn
- Possible solution: Make sure to render transparent meshes last, and preferably the furthest away transparent meshes first
- Cause: This is due to how rendering transparent objects are done, they have to be drawn last, and also the transparent objects furthest has to be drawn before those that are closer

###### Not able to load highest resolution/pixel dense heightmap
- Description: Loading heightmap "height100" and "height50" works, but "height20" and "height10" causes a crash.
- Possible solution: Either convert the project to 64-bit, or possibly loading the heightmap data straight into the VBO/EBO.
- Cause: This is due to the vectors not getting enough space allocated, and also as we are running and compiling on 32-bit, we
are allocated less memory than on 64-bit.


## LEVEL 1:

###### Last pixel when loading heightmap has incorrect value
- Description: When loading the heightmap, the last pixel loaded is of a slightly to high value (should be >= 1, but is ~1.17)
- Possible solution: Change for-loop for indexing?
- Cause: Guess: Might be indexing pixels incorrect

###### Contour line on water is incorrect in some seasons
- Description: Displaying contour line in winter season does some time show as too huge.
- Possible solution: Draw a smaller contour line, or change the check if the contour line should be drawn
- Cause: Guess: Due to that the position of alot of the water is in a certain range of the terrain, it causes it to draw larger lines than intended.

###### Skybox not always loading into appropriate places
- Description: Sometimes the position of front and back textures are flipped on certain skybox cubemaps
- Possible solution: Make sure it loads correctly depending on the type of cubemap files
- Cause:

###### Moving mouse after going out of plane-flight mode results in view "flipping" to last cam pos
- Description: When going from flight-mode to free-roam camera, the camera "flips" over to the last cam pos before flight-mode started
- Possible solution: Calculate new yaw/pitch values, and apply them to the camera after flightmode.
- Cause: Guess: When leaving flight-mode, the camera calulates it's rotation with "yaw" and "pitch" values that have not been calculated while flying

###### Dashboard not rendering totally correct in first person
- Description: It seems like some textures have some opacity or are supposed to be drawn with glass on top maybe, so they do not render correctly in the first-person mode
- Possible solution: Change proper render distance, position, or handle which meshes to draw in which order more correct
- Cause: Guess: They have glass above them, that makes the opacity draw a bit weird at times?
