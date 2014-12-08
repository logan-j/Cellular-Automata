Project 2 part 1
Logan Jewett
Readme File
11/18/13
UPDATED: 12/16/13

*******************************************************************
About this Document:

Readme that contains relevant information regarding the alive.cc
life_gui.cc and life.cc program files. Builds the life.exe and
life_gui.exe executables.
*******************************************************************

TABLE OF CONTENTS:

1.	alive.cc

	CLASSES AND STRUCTS
	.1	Point
	.2	Range
	.3	RGB
	.4	LGrid
	.5	Terrain : LGrid
	.6	Window : LGrid
	.7	Chars
	.8	Colors
	.9	Life
	.10	Switch
	
	EXCEPTIONS
	.11	iS
	.12	tFA
	.13	mA
	.14	iA
	.15	fNN
	.16	tMA
	.17	iR
	.18	mFI
	.19	mD

2.	life_gui.cc

	CLASSES AND STRUCTS
	.1	Gui//defunct as of 12/16/13
	.2	main
	
3.	life.cc
	
	CLASSES AND STRUCTS
	.1	main

**added 12/16/13
4.	gui.cc
	
	CLASSES AND STRUCTS
	
	

*******************************************************************

							alive.cc

DESCRIPTION: mostly a collection of utilities for usage by other
classes. Does 99% of the heavy lifting for both life_gui and life.
design implementation was to create a Life object with the input
file as a string that you would call methods onto to manipulate
terrain and window dimensions, as well as calculate generations.

Also contains the Switch object, which just takes the command line
input and parses it/makes it useful.

life and life_gui use both Life and Switch objects to do most of
relevant calculation and data storage.
*******************************************************************

CLASSES AND STRUCTS
	.1	Point -	class for a representation of a cartesian pair. Has
				constructors for both a string input (roughly "(x,y)")
				as well as explicit declarations. Also has a toString()
				method.
				
	.2	Range -	class for a representation of a range with a "low" and
				"high" value. Has constructors for both a string input
				(roughly "l..h") and explicit values. Also has getter
				and setter methods, a bool inRange(), a getSize() method
				which returns the size of the range, and a toString() 
				method.
				
	.3	RGB -	class for representing an RGB value. Has a constructor
				for just a string input (roughly "(R,G,B)") as well as
				as toString() method.
				
	.4	Grid -	struct for a representation of a 2-dimensional grid.
				implemented by Terrain and Window. Contains an x-range
				and a y-range as well as generic getter and setter methods.
				
	.5	Terrain : Grid
				implements the grid struct. Has constructors for both a
				string input (the Terrain struct from the input file) as well
				as an explicit constructor. Also has a toString() method which
				implements the ranges' toString() method. Also a method 
				inTerrain(Point param) which returns a boolean representation
				of whether specified point param is in the grid.
						
	.6	Window : Grid
				implements the grid struct. Has a constructor for both a
				string input (the Window struct from the input file) as well
				as a more variable explicit constructor. The explicit
				constructor can check the bounds of a specified Terrain
				objects and set the updated window within those bounds.
				Also contains a toString() method similar to Terrain.
				
	.7	Chars -	class for containing information on which character 
				represents alive or dead cells for the program life.
				One constructor that takes a string input (the Chars
				struct in the given input file), as well as a toString()
				method that returns a string representation of the
				Chars struct.
				
	.8	Colors -similar to chars. Contains information regarding which
				colors are used to represent alive and dead cells in
				the life_gui program. Takes a string input in the constructor
				(the Colors struct in the given input file) and makes
				slash stores the information into RGB elements. Also
				has a toString() method that returns a string
				representation of the Colors struct.
					
	.9	Life -	Takes a string input into the constructor (a string
				of the input file) and parses it, and divvies it out
				to the relevant constructors, which are stored in the
				class. There are a ton of getter and setter methods
				that are utilized throughout both life and life_gui
				for setting/getting the terrain and window. The method
				next() is for calculating the next generation based on
				the current state information regarding alive and dead
				cells. Uses a vector<Point> to store all the point
				representations of the currently alive cells and uses
				next() to update the vector. Constructor has both a
				life and life_gui mode which dictates which structs
				it will and will not allow to be missing from the
				input file.
				
	.10	Switch -Takes the int argc and the char *argv[] values from the
				main files implemented in both life and life_gui and
				parses/interprets it into relevant information.
				Also sets all the switches to their initial values.
	
EXCEPTIONS - NOTE: very limited argument testing.

	.11	iS -	Invalid switch. Thrown when attempting to use a switch
				that isn't supported or recognized.
				
	.12	tFA -	NOW DEFUNCT: thrown if not enough arguments are given.
	
	.13	mA -	Missing Argument. Thrown if there are no more arguments
				available from the command after the given switch (needs
				an additional argument to do something, either a range
				or an integer, etc.)
				
	.14	iA -	Illegal Argument. Thrown if a switch that needs an
				argument is proceeded by another switch or something 
				that's been utilized already.
				
	.15	fNN -	File not found. Will be thrown if the only non-switch
				slash parameter argument doesn't open a file.
				
	.16	tMA -	Too many arguments. Thrown if there are more than one
				arguments from the shell that could potentially be file
				names and aren't switches or their parameters.
				(random characters that weren't weeded out).
				
	.17	iR	-	Thrown by Range if the specified "low" is greater than
				"high" or if the input into the constructor doesn't
				meet the specified format.
				
	.18	mFI -	Missing file info. A relevant struct or other information
				is absent from the input file. Is dependent on which 
				program is utilized.
				
	.19	mD -	Thrown if there are multiple struct declarations in the
				input file (multiple Colors, etc.);



*******************************************************************

							life_gui.cc

DESCRIPTION: Graphical implementation of the game of life. Takes an
input file of a specified format about terrain, window, display and
state information and then output a graphical representation based
on Conway's game of life. Also takes a variety of command line
arguments as various options.

12/16/13 - mostly stands as an implementation of gui.cc, which does
most of the heavy lifting in terms of updating the life object and
outputting the display info. A lot of code was moved to gui.cc
*******************************************************************

CLASSES AND STRUCTS
	.1	Gui -	//defunct//Utilized by main to draw the window. Takes all the
				relevant starting information into the constructor
				(like vector<Point> storing all the alive cells)
				and then interprets that into a graphical
				representation. Also has a method for setting the
				terrain and window boundaries.//defunct//
				
	.2	main -	Stitches together the classes Gui, Life, and Switch
				in order to take in an input file and variety of
				options regarding the terrain, window, and generation, 
				construct the Life object, perform generation
				enumeration, and then passes that info into 
				the Gui class so that it can output a graphical
				representation of the cells in the game of life.


*******************************************************************

							life.cc

DESCRIPTION: Shell implementation of the game of life. Takes an
input file of a specified format about terrain, window, display and
state information and then outputs it in a specified format (either
an ASCII visual or a formatted text file).
*******************************************************************

CLASSES AND STRUCTS
	.1	main -	Stitches together the classes Life and Switch in
				order to take in an input file and a variety of
				options regarding the terrain, window, and generation,
				construct the Life object, perform generation enumeration,
				and then either output that information into the shell
				as either a ASCII representation (which is calculated
				in main) or output a life struct text representation of
				the state info.
				
				
*******************************************************************

							gui.cc

DESCRIPTION: Class for handling all of the graphical updating and
controls for the life_gui class. Originally gui.cc was part of the
life_gui class, but as it was expanded to accommodate controls and
dynamic updating, the class was separated. Also was necessary to make
use of custom slots or actions. Basically just encapsulates all of
the required stuff for the gui and associated controls.
*******************************************************************				

FUNCTIONS
	.1 delay - delays the computer by an input number of milliseconds
	
SLOTS
	.1 restart -	for usage by the restart button. reverts the Life
					to its initial state
	.2 play	-		just loops through utilizing delay and a bool to
					start and stop
	.3 changeSize -	linked with the size slider. Just updates the
					graphical info based on the value.
	.4 step - 		increments the Life struct one step. pauses playing
	

CLASSES AND STRUCTS
	.1 Gui - 		constructs a gui object. does most of the graphical
					heavy lifting in terms of displaying the controls.
					Takes in a life object as well as height and offset
					information regarding the window and terrain so that
					things will display properly


	