/*
 * alive.h
 *
 * author: Logan Jewett
 *
 * this is all the header info for all the structs in alive.cc. See alive.cc for more info
 */

 
#ifndef ALIVE_HH
#define ALIVE_HH

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <locale>
#include <exception>
#include <stdlib.h> //added for atoi
#define MIN(X,Y) (((X - Y) > 0) ? (X) : (Y))//macros for setting window range
#define MAX(X,Y) (((X - Y) < 0) ? (X) : (Y))

using namespace std;

//my build of cygwin wouldn't let me use to_string, so I borrowed this code from
//stack overflow as a work around.

template<typename T>
string to_string(T value){
	ostringstream os;
	os << value;
	return os.str();
}

int getType(string fileName);
string stripCommAndUpper(string input);

class Point{
	public:
		int x;
		int y;
		Point(string param);
		Point(int xIn, int yIn);
		
		string toString();
};

class Range{
	public:
		int low;
		int high;
		
		Range(string param);
		Range(int l, int h);
		
		bool inRange(int param, bool mode = false);
		void setL(int param);
		void setH(int param);
		int getL();
		int getH();
		int getSize();
		bool equals(Range param);
		
		string toString();
};

class RGB{
	public:
		int R;
		int G;
		int B;
		
		RGB(string param);
		
		string toString();
};

struct Grid{
	public:
		Range *Xrange;
		Range *Yrange;
		Range getYrange();
		Range getXrange();
		void setXrange(Range x);
		void setYrange(Range y);
		
		
};

class Terrain: public Grid{
	public:
		Terrain(string param);
		Terrain(Range x, Range y);
		bool inTerrain(Point param);
		string toString();
};

class Window: public Grid{
	public:
		Window(string param);
		Window(Range x, Range y, bool mode = true, Terrain *terr = NULL);

		string toString();
};

class Chars{
	public:
		//uses alive and dead for one/head and zero/empty respectively
		int aliveChar, deadChar, wireChar, tailChar, type;
		
		Chars(string param, int aType);
		
		string toString();
};

class Colors{
	public:
		//uses alive and dead for one/head and zero/empty respectively
		RGB *aliveRGB, *deadRGB, *wireRGB, *tailRGB;
		int type;
		
		Colors(string param, int aType);
		
		string toString();
};

class Life{
	public:
		string name;
		Terrain *terrain;
		Window *window;
		Colors *colors;
		Chars *chars;
		bool mode;
		int type;
		int generation;
		//again, alive for head/one
		vector<Point> alive, wire, tail, AL, WI, TA;//all the points; alive, wire, and tail are calculated, AL, WI, and TA are initial
		
		Life(string fileName, bool mode, int aType);
		void next();
		void aName(string param);//set name
		void points(string param);//parse initial struct and make points
		string initial();//output the initial struct for tosSring
		string toString();
		void setTerrainAndWindow(Range tX, Range tY, Range wX, Range wY);
		void setWindow(Range x, Range y);//vary window size; limit at terrain
		
		void revert(){//dunno if I had to do this in here.
			generation = 0;
			alive.clear();
			wire.clear();
			tail.clear();
			
			for(vector<Point>::iterator point = AL.begin(); point != AL.end(); ++point)alive.push_back(*point);
			for(vector<Point>::iterator point = WI.begin(); point != WI.end(); ++point)wire.push_back(*point);
			for(vector<Point>::iterator point = TA.begin(); point != TA.end(); ++point)tail.push_back(*point);
		}
		
		int getWidth();
		int getHeight();
		
		//just decided to do a lot of inline getter functions
		inline int getGen(){return generation;}
		inline int getType(){return type;}
		
		inline int getXoff(){return (*terrain).getXrange().getL();}
		inline int getYoff(){return (*terrain).getYrange().getL();}
		
		inline Range getTXrange(){return (*terrain).getXrange();}
		inline Range getTYrange(){return (*terrain).getYrange();}
		inline Range getWXrange(){return (*window).getXrange();}
		inline Range getWYrange(){return (*window).getYrange();}
		
		inline int getXL(){return (*window).getXrange().getL();}
		inline int getXH(){return (*window).getXrange().getH();}
		inline int getYL(){return (*window).getYrange().getL();}
		inline int getYH(){return (*window).getYrange().getH();}
		
		inline char getAliveC(){return (*chars).aliveChar;}
		inline char getDeadC(){return (*chars).deadChar;}
		inline char getWireC(){return (*chars).wireChar;}
		inline char getTailC(){return (*chars).tailChar;}
		
		inline RGB getAliveRGB(){return *((*colors).aliveRGB);}
		inline RGB getDeadRGB(){return *((*colors).deadRGB);}
		inline RGB getWireRGB(){return *((*colors).wireRGB);}
		inline RGB getTailRGB(){return *((*colors).tailRGB);}
};

class Switch{
	public:
		string fileName;
		bool f;
		bool h;
		bool v;
		bool c;
		int g;
		int s;
		
		
		Range *tx;
		Range *ty;
		Range *wx;
		Range *wy;
		
		
		Switch(int argc, char* argv[]);
		bool isSwitch(char *param);
		inline string getName(){return fileName;}
};

//exceptions

struct iS : exception{ const char* what() {return "Invalid Switch!\n";}};
struct tFA : exception{ const char* what() {return "Too Few Arguments!\n";}};
struct mA : exception{ const char* what() {return "Missing Argument!\n";}};
struct iA : exception{ const char* what() {return "Invalid Argument!\n";}};
struct fNN : exception{ const char* what() {return "File Not Found!\n";}};
struct tMA : exception{ const char* what() {return "Too Many Arguments!\n";}};
struct iR : exception{ const char* what() {return "Invalid Range Argument!\n";}};
struct mFI : exception{ const char* what() {return "Missing File Info!\n";}};
struct mD : exception{ const char* what() {return "Multiple Struct Declarations!\n";}};
struct tF : exception{ const char* what() {return "Missing Life/Elementary/WireWorld Struct!\n";}};
struct tM : exception{ const char* what() {return "Multiple Life/Elementary/WireWorld Structs!\n";}};

#endif

