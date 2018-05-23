|----------------------------------------------|
|	
|	A simple WebGL viewer  
|
|	Authors: 
|		Alex Carruesco Llorens (main code)
|		Carlos Andujar (minor updates)
|		
|----------------------------------------------|
	
The viewer uses HTML, Javascript and WebGL. 

Launch the program by just opening the "main.html" file.

Tested with 
- Gogle Chrome v42+ (Recommended browser)
- Mozilla Firefox v37+
but it work with any recent browser with WebGL enabled. 

# Folders and files:
- GLV/ -> GLViewer (javascript and CSS files)
- libs/ -> Folder with the external .js libs.
- Models/ -> Folder with some 3D models (.obj format)
- main.html -> The main file.

The main code of the required functionalities for this project
are meant to be implemented in 
	
	GLV/DisplaySurface.js
	GLV/CAVE.js
	GLV/shaders/cave.js
	
The rest of the files are from the base GLViewer.
