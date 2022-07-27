# GLSL Toy
## public notes
**probably important things to mention**
- coding streams for this were recorded and are to be nicely(?) edited for an youtube vid
## roadmap or smth
- [DONE] set up premake5 with includes and shit
- [DONE] code to run opengl window
- [DONE] ESC, F5, R hotkeys, check if they work (output smth to console or close the window for ESC)
- [DONE] sample code to compile and run shader inside string var
- modify window to be run on top of everything (GLFW_FLOATING?)
- (optional but preferred) modify window to be run without borders (not gonna work idk)
- code to read shader from file; moving the shader from string var to file
- add uTime uniform
- code to recompile and run an updated shader, tested with F5 and CTRL+R hotkeys
- can give up here but making a semi-transparent text editor on top of a running shader would be an amazing optimization trip and a quite useful addition for future videos/streams 

## final product
#### the goal
	drawing a compiled-at-runtime shader  
	R for recompiling and reloading the shader on the fly
	
	Q: why?
	A: don't care mate. makes it easier to learn/teach how to work with shaders or smth?

#### improving the idea (if i have some lifeforce left)
	drawing a semi-transparent text editor on top of the running shader
		(could actually use ImGUI for that?) 
	
	H for hiding/showing the text editor itself and revealing/covering the graphics
	
	Q: what's the point of going further?
	A: i want to flex with better application in front of my second ego! 
		(and supposedly show you how it's done 
			(though i have no clue right now...
				well, watching people struggle is quite entertaining nowadays innit))
