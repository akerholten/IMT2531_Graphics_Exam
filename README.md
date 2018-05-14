# IMT2531_Graphics_Exam
*Graphics exam done in OpenGL for IMT2531 at NTNU*

### Author
- [Nikolai Åkerholt](https://github.com/akerholten) (473184) - nikolaaa


<img src="https://github.com/akerholten/IMT2531_Graphics_Exam/blob/master/images/3rdperson.png" height="300" width="540">


### To build:
- Clone repository
- Run CMake
- Set solution properties to single startup project "GraphicExam" (Visual Studio)
- Build with compiler (Debug, Win32)

*Note: CMake probably only works on Windows, and I have only built with Visual Studio with CMake*

### To run from .exe:
- Go to folder Executable -> Release
- Run GraphicsExam.exe

### Keybindings

#### Camera
- <kbd>-</kbd> Cycles between camera modes (Glider, First-Person Glider, Free-Roam, and Restricted-Cam)
- <kbd>I</kbd> Forward (Free-Roam) or Z movement (Restricted-Cam)
- <kbd>K</kbd> Backwards (Free-Roam) or -Z movement (Restricted-Cam)
- <kbd>J</kbd> Left (Free-Roam) or X movement (Restricted-Cam)
- <kbd>L</kbd> Right (Free-Roam) or -X movement (Restricted-Cam)
- <kbd>Y</kbd> Y movement (Restricted-Cam)
- <kbd>H</kbd> -Y movement (Restricted-Cam)
- Zoom in and out with scroll-wheel
- Move mouse around to change view orientation

#### Glider
- <kbd>W</kbd> Pitch Up
- <kbd>S</kbd> Pitch Down
- <kbd>A</kbd> Roll Left
- <kbd>D</kbd> Roll Right
- <kbd>,</kbd> Increase speed
- <kbd>.</kbd> Decrease speed
- <kbd>R</kbd> Reset to original position
- <kbd>F</kbd> Teleport to new random position

#### Seasonal Change
- <kbd>1</kbd> Toggles 'Winter'
- <kbd>2</kbd> Toggles 'Spring'
- <kbd>3</kbd> Toggles 'Summer'
- <kbd>4</kbd> Toggles 'Autumn'
- <kbd>5</kbd> Pauses seasonal change
- <kbd>O</kbd> Toggles contour lines
- <kbd>&uarr;</kbd> Increase lerp on zones
- <kbd>&darr;</kbd> Decrease lerp on zones

#### Time Change
- <kbd>6</kbd> Toggles 'Night'
- <kbd>7</kbd> Toggles 'Morning'
- <kbd>8</kbd> Toggles 'Noon'
- <kbd>9</kbd> Toggles 'Afternoon'
- <kbd>0</kbd> Pauses time change


### Feature List

#### Heightmap/Terrain
- Loading heightmap with usage of stb_image
- Showing proper vegetation with smooth transitions between zones (water - grass - mud - snow)
- Contour lines can be toggled

#### Camera
- 3rd person Glider mode
- First person Glider mode
- Free-Roam mode
- Restricted-Movement mode
- Zoom in/out


#### Seasonal / Time-of-Day change
- Seasons cycle automatically between Winter - Spring - Summer - Autumn
- Season and month is printed on top left corner
- Time-of-day cycles between Night - Morning - Noon - Afternoon
- Time-of-day is printed as a 24 hour clock on top right corner
- Rising water levels in Spring

#### Glider
- Glider model loaded with .obj (extracted with blender from the .blend file)
- WASD for controls (Pitch and roll)
- Speed increase with ',' and decrease with '.'
- 3rd person and First-Person perspective, change with '-'
- 'F' changes the current position of the glider to a random position
- 'R' resets the glider position to it's original position
- Glider showing proper specular lighting

#### Some additional
- 'Skybox' rendered around everything
- 'Up' and 'Down' arrows changes the interpolation value between zones


### Images

<img src="https://github.com/akerholten/IMT2531_Graphics_Exam/blob/master/images/FirstPerson.png" height="300" width="540">


<img src="https://github.com/akerholten/IMT2531_Graphics_Exam/blob/master/images/FreeRoam.png" height="300" width="540">


### [To-Do List](TODO-LIST.md)

### [Known-Bugs List](KNOWN-BUGS.md)


#### Sources:

*Heightmap Loading*
https://www.3dgep.com/multi-textured-terrain-in-opengl/#The_Terrain::LoadHeightmap_Method

*Heightmap Loading*
https://stackoverflow.com/questions/48286572/opengl-heightmap-terrain-and-normals-with-low-resolution

*Heightmap Loading*
http://www.bluemoonproductions.nl/tutorials/3d-terrain-using-a-heightmap/

*Skybox*
https://learnopengl.com/Advanced-OpenGL/Cubemaps

*Assimp Materials*
http://assimp.sourceforge.net/lib_html/materials.html

*Calculating 'up' vector from rotation*
https://stackoverflow.com/a/1121731

*Blending for transparency*
https://learnopengl.com/Advanced-OpenGL/Blending
