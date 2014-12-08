/*
 * alive.cc
 *
 * author: Logan Jewett
 *
 * This stuff is pretty straightforward. A lot of it is getter/setter methods and stuff for parsing
 * input text files into something useful, at least for the colors/chars/point/etc. classes
 */

#include "alive.h"

//Static functions
	string stripCommAndUpper(string toParse){//strips comments and sets it to uppercase. no "ignore's case" as far as I know.
		locale loc;
		string upper;
		int flip = 0;
		
		for(int i = 0; i < toParse.length(); i++){
			char toadd = toParse[i];
			if(flip == 0 && toadd == '"')flip++;
			else if(flip == 1 && toadd == '"')flip--;
			
			if(flip == 0 && toadd == '#') upper += "!#";
			else if(flip == 0) upper += toupper(toadd, loc);
			else upper += toadd;
		}
				
		//decided to just double the #s for comments outside quotes and use that
		//for deleting comments. So it works so long as '!#' isn't in the filename.
		int numI = upper.find("!#"), nl;
		while(numI != string::npos){
			nl = upper.find("\n", numI);
			upper.erase(numI, nl - numI);
			numI = upper.find("!#");
		}
		
		return upper;
	}

	int getType(string upper){//after stripping comments checks for one of these types. Throws exceptions if none found or more than one.
			
			int life, wire, elem;
			life = upper.find("LIFE");
			wire = upper.find("WIREWORLD");
			elem = upper.find("ELEMENTARY");
			
			if(life == string::npos && elem == string::npos && wire == string::npos)return 0;
			else if(wire == string::npos && elem == string::npos)return 1;
			else if(life == string::npos && wire == string::npos)return 2;
			else if(life == string::npos && elem == string::npos)return 3;
			else return 4;
	}


//Switches
	//constructors
		//this is kind of straight forward. Switch just takes the input arguments and looks through them, keeping track of which ones
		//it's seen. Will pair a switch with it's associated info (int or range)
		Switch::Switch(int argc, char* argv[]){
			int *params;
			params = (int *) calloc(argc, sizeof(int));
			f = false;
			h = false;
			v = false;
			c = false;
			g = 0;
			s = 10;
			tx = NULL;
			ty = NULL;
			wx = NULL;
			wy = NULL;
			
			
			for(int i = 0; i < argc - 1; i++){
				if(isSwitch(argv[i + 1]) && params[i] == 0){
					params[i]++;
					
					switch(toupper(argv[i + 1][1])){
						case 'F'://file output
							f = true;
							break;
						case 'H'://help
							h = true;
							break;
						case 'V'://visual output
							v = true;
							break;
						case 'C'://control window (does nothing in life program)
							c = true;
							break;
						case 'G'://generation. Stored as an int
							if(i + 2 >= argc) throw mA();
							if(params[i + 1] != 0) throw iA();
							else g = atoi(argv[i + 2]);
							if(g < 0)throw iA();
							params[i + 1]++;
							
							break;
						case 'S'://pixel size
							if(i + 2 >= argc) throw mA();
							if(params[i + 1] != 0) throw iA();
							else s = atoi(argv[i + 2]);
							if(s < 1)throw iA();
							params[i + 1]++;
							
							break;
						case 'T'://terrain ranges (x and then y)
							if(i + 2 >= argc) throw mA();
							if(params[i + 1] != 0) throw iA();
							
							if(toupper(argv[i + 1][2]) == 'X'){
								tx = new Range(string(argv[i + 2]));
							}else if(toupper(argv[i + 1][2]) == 'Y'){
								ty = new Range(string(argv[i + 2]));
							}else throw 3;
							params[i + 1]++;
							break;
						case 'W'://window ranges (x and then y)
							if(i + 2 >= argc) throw mA();
							if(params[i + 1] != 0) throw iA();
							
							if(toupper(argv[i + 1][2]) == 'X'){
								wx = new Range(string(argv[i + 2]));
							}else if(toupper(argv[i + 1][2]) == 'Y'){
								wy = new Range(string(argv[i + 2]));
							}else throw iS();
							params[i + 1]++;
							break;
						
						default :
							throw iS();//invalid switch if it has '-' and doesn't match one of those
							break;
						
					}
				}
				
			}
			
			int index = -1;
			//checks to see how many arguments have been addressed/checked.
			for(int i = 0; i < argc - 1; i++){
				if(params[i] == 0 && index < 0) index = i;
				else if(params[i] == 0 && index >= 0) throw tMA();
			}
			ifstream f;
			if(index >= 0){
				f.open(argv[index + 1]);
				if(!f.is_open()) throw fNN();//file not found (bad input file)
				else fileName = string(argv[index + 1]);
				f.close();
			}else{
				fileName = " ";//if the file for the index didn't open this will cause the program to switch to stdin for input later on.
			}
		}
		
		bool Switch::isSwitch(char *param){
			return param[0] == '-';
		}

//point
	//constructors
		Point::Point(string param){
			int xb, yb;
			xb = param.find("(");
			yb = param.find(",");
			
			if(xb == string::npos || yb == string::npos) throw mFI();
			
			x = atoi(param.substr(xb + 1).c_str());
			y = atoi(param.substr(yb + 1).c_str());
		}
		
		Point::Point(int xIn, int yIn){
			x = xIn;
			y = yIn;
		}
	
	//methods
		string Point::toString(){
			return "(" + to_string(x) + "," + to_string(y) + ")";
		}
		
//range
	//constructors
		Range::Range(string param){
			//first attempted a few different methods from C++ but couldn't get stringstreams to work
			//decided to use atoi instead since I know how it works.
			int m = param.find("..");
			if(m == string::npos) throw iR();
			param.replace(m, 2, "  ");
			int l = atoi(param.c_str());
			string sub = param.substr(m + 1);
			int h = atoi(sub.c_str());
			if(l > h)throw iR();
			else{
				low = l;
				high = h;
			}
		}
		
		Range::Range(int l, int h){
			if(l > h)throw iR();
			else{
				low = l;
				high = h;
			}
		}
		
	//methods
		
		bool Range::inRange(int param, bool mode){ //array mode is false, terrain mode is true (upperbound exclusivity)
			if(mode){
				return (param >= low && param <= high);
			}else return (param >= low && param < high);
		}
		
		void Range::setL(int param){
			low = param;
		}
		
		void Range::setH(int param){
			high = param;
		}
		
		int Range::getL(){
			return low;
		}
		
		int Range::getH(){
			return high;
		}
		
		int Range::getSize(){
			return (high - low + 1);
		}
		
		string Range::toString(){
			return to_string(low) + ".." + to_string(high);
		}
		
		bool Range::equals(Range param){
			return(low == param.low && high == param.high);
		}
		
//rgb
	//constructors
		RGB::RGB(string param){
			int rb, gb, bb;
			rb = param.find("(");
			gb = param.find(",", rb);
			bb = param.find(",", gb + 1);
			
			if(rb == string::npos || gb == string::npos || bb == string::npos) throw mFI();
			
			R = atoi(param.substr(rb + 1).c_str());
			G = atoi(param.substr(gb + 1).c_str());
			B = atoi(param.substr(bb + 1).c_str());
		}
		
	//methods
		string RGB::toString(){
			return "(" + to_string(R) + "," + to_string(G) + "," + to_string(B) + ")";
		}
//grid
	//methods
		Range Grid::getXrange(){
			return *Xrange;
		}
		
		Range Grid::getYrange(){
			return *Yrange;
		}
		
		void Grid::setXrange(Range x){
			Xrange = new Range(x.getL(), x.getH());
		}
		
		void Grid::setYrange(Range y){
			Yrange = new Range(y.getL(), y.getH());
		}
		
//terrain
	//constructors
		Terrain::Terrain(string param){
			int xb, yb, xe, ye;
			xb = param.find("XRANGE");
			xb = param.find("=", xb);
			yb = param.find("YRANGE");
			yb = param.find("=", yb);
			xe = param.find(";", xb);
			ye = param.find(";", yb);
			
			if(xb == string::npos || yb == string::npos || xe == string::npos || ye == string::npos) throw mFI();
			
			Xrange = new Range(param.substr(xb + 1, xe - xb - 1));
			Yrange = new Range(param.substr(yb + 1, ye - yb - 1));
			
		}
		
		Terrain::Terrain(Range x, Range y){
			setXrange(x);
			setYrange(y);
		}
		
		
	//methods
		string Terrain::toString(){
			return "Terrain={Xrange=" + (*Xrange).toString() + ";Yrange=" + (*Yrange).toString() + ";};";
		}
		
		bool Terrain::inTerrain(Point param){
			int x = param.x;
			int y = param.y;
			bool bx = (*Xrange).inRange(x, true);
			bool by = (*Yrange).inRange(y, true);
			return(bx && by);
		}

//window
	//constructors
		Window::Window(string param){
			int xb, yb, xe, ye;
			xb = param.find("XRANGE");
			xb = param.find("=", xb);
			yb = param.find("YRANGE");
			yb = param.find("=", yb);
			xe = param.find(";", xb);
			ye = param.find(";", yb);
			
			if(xb == string::npos || yb == string::npos || xe == string::npos || ye == string::npos) throw mFI();
			
			Xrange = new Range(param.substr(xb + 1, xe - xb - 1));
			Yrange = new Range(param.substr(yb + 1, ye - yb - 1));
			
		}
		
		Window::Window(Range x, Range y, bool mode, Terrain *terrain){
			if(mode){//normal mode
				
				setXrange(x);
				setYrange(y);
			}else{ //set window within bounds of terrain
				int xL, yL, xH, yH;
				
					xL = MIN(x.getL(), (*terrain).getXrange().getL());
					xH = MAX(x.getH(), (*terrain).getXrange().getH());
				
					yL = MIN(y.getL(), (*terrain).getYrange().getL());
					yH = MAX(y.getH(), (*terrain).getYrange().getH());
				
				
				Xrange = new Range(xL, xH);
				Yrange = new Range(yL, yH);
			}
			
		}
		
	//methods
		string Window::toString(){
			return "Window={Xrange=" + (*Xrange).toString() + ";Yrange=" + (*Yrange).toString() + ";};";
		}

//chars
	//constructors
		Chars::Chars(string param, int aType){
			type = aType;
			int ab, db, wb, tb;
			if(type == 1){
				ab = param.find("ALIVE");
				ab = param.find("=", ab);
				db = param.find("DEAD");
				db = param.find("=", db);
			}
			if(type == 2){
				ab = param.find("ONE");
				ab = param.find("=", ab);
				db = param.find("ZERO");
				db = param.find("=", db);
			}
			if(type == 3){
				ab = param.find("HEAD");
				ab = param.find("=", ab);
				db = param.find("EMPTY");
				db = param.find("=", db);
			
				wb = param.find("WIRE");
				wb = param.find("=", wb);
				tb = param.find("TAIL");
				tb = param.find("=", tb);
			}
			
			
			if(ab == string::npos || db == string::npos) throw mFI();
			aliveChar = atoi(param.substr(ab + 1).c_str());
			deadChar = atoi(param.substr(db + 1).c_str());
			if(type == 3){
				if(wb == string::npos || tb == string::npos) throw mFI();
				wireChar = atoi(param.substr(wb + 1).c_str());
				tailChar = atoi(param.substr(tb + 1).c_str());
			}
		}
		
	//methods
		string Chars::toString(){
			string out = "Chars={";
			if(type == 1) out += "Alive=" + to_string(aliveChar) + ";Dead=" + to_string(deadChar) + ";};";
			if(type == 2) out += "One=" + to_string(aliveChar) + ";Zero=" + to_string(deadChar) + ";};";
			if(type == 3) out += "Head=" + to_string(aliveChar) + ";Wire=" + to_string(wireChar) + ";Tail=" + to_string(tailChar) + ";Empty=" + to_string(deadChar) + ";};";
			
			return out;
		}
		
//colors
	//constructors
		Colors::Colors(string param, int aType){
			type = aType;
			int ab, db, ae, de, wb, tb, we, te;
			if(type == 1){
				ab = param.find("ALIVE");
				ab = param.find("=", ab);
				ae = param.find(";", ab);
			
				db = param.find("DEAD");
				db = param.find("=", db);
				de = param.find(";", db);
			}
			
			if(type == 2){
				ab = param.find("ONE");
				ab = param.find("=", ab);
				ae = param.find(";", ab);
			
				db = param.find("ZERO");
				db = param.find("=", db);
				de = param.find(";", db);
			}
			if(type == 3){
				ab = param.find("HEAD");
				ab = param.find("=", ab);
				ae = param.find(";", ab);
			
				db = param.find("EMPTY");
				db = param.find("=", db);
				de = param.find(";", db);
				
				wb = param.find("WIRE");
				wb = param.find("=", wb);
				we = param.find(";", wb);
			
				tb = param.find("TAIL");
				tb = param.find("=", tb);
				te = param.find(";", tb);
			}
			
			
			
			if(ab == string::npos || db == string::npos || ae == string::npos || de == string::npos) throw mFI();
			aliveRGB = new RGB(param.substr(ab + 1, ae - ab - 1));
			deadRGB = new RGB(param.substr(db + 1, de - db - 1));
			
			if(type == 3){
				if(wb == string::npos || we == string::npos || tb == string::npos || te == string::npos) throw mFI();
				wireRGB = new RGB(param.substr(wb + 1, we - wb - 1));
				tailRGB = new RGB(param.substr(tb + 1, te - tb - 1));
			}
			
		}
		
	//methods
		string Colors::toString(){
			//only dependent on whether it's the life program
			//since it's the one that outputs the file info.
			string a, d, w, t, out = "Colors={";
			a = (*aliveRGB).toString();
			d = (*deadRGB).toString();
			if(type == 3){
				w = (*wireRGB).toString();
				t = (*tailRGB).toString();
			}
			
			
			//length checking is just to see if there's a color value
			if(a.length() > 3 && d.length() > 3){
				if(type == 1){
					out += "Alive=" + a + ";Dead=" + d + ";";
				}
				if(type == 2){
					out += "One=" + a + ";Zero=" + d + ";";
				}
				if(type == 3 && w.length() > 3 && t.length() > 3){
					out += "Head=" + a + ";Wire=" + w + ";Tail=" + t + ";Empty=" + d + ";";
				}
			}
			out += "};";
			return out;
		}
		
//life
	//constructors
		Life::Life(string upper, bool iMode, int aType){
			generation = 0;
			mode = iMode; //true for life, false for life_gui
			type = aType;
			
				string tags[] = {"NAME", "TERRAIN", "CHARS", "COLORS", "INITIAL", "WINDOW"};
				locale loc;
				
				//uses the tags to check for the location of the structs then parses each section
				//and sends it to the appropriate constructors.
				for(int i = 0; i < 6; i++){
					int index = upper.find(tags[i]);
					if(index != string::npos){
						int end, error;
						error = upper.find(tags[i], index + 1);
						if(error != string::npos) throw mD();
						switch(i){
							case 0:
								end = upper.find(";", index);
								aName(upper.substr(index, end - index + 1));
								
								break;
							case 1:
								end = upper.find("};", index);
								terrain = new Terrain(upper.substr(index, end - index + 2));
								
								break;
							case 2:
								end = upper.find("};", index);
								chars = new Chars(upper.substr(index, end - index + 2), type);
								
								break;
							case 3:
								end = upper.find("};", index);
								colors = new Colors(upper.substr(index, end - index + 2), type);
								
								break;
							case 4:
								end = upper.find("};", index);
								points(upper.substr(index, end - index + 2));
								
								break;
							case 5:
								end = upper.find("};", index);
								window = new Window(upper.substr(index, end - index + 2));
								
								break;						
						}
					}else{
						if(i == 5){
							Range x((*terrain).getXrange().getL(), (*terrain).getXrange().getH());
							Range y((*terrain).getYrange().getL(), (*terrain).getYrange().getH());
							window = new Window(x, y);
						}
						else if(i == 0){
							name = " ";
							name.clear();
						}
						else if(i != 4 && ((mode && i == 2) || (!mode && i == 3))){
							throw mFI();
						}
					}
				}
			
		}
		
	//methods
		//calculates the next generation based on the current state for a variety of types.
		void Life::next(){
			generation++;
			int x = (*terrain).getXrange().getSize();
			int y = (*terrain).getYrange().getSize();
			int xOff = (*terrain).getXrange().getL();
			int yOff = (*terrain).getYrange().getL();
			int arr[y][x], aalive[y][x];
			
			//initialize the arrays
			for(int i = 0; i < y; i++){
				for(int k = 0; k < x; k++){
					arr[i][k] = 0;
					aalive[i][k] = 0;
				}
			}
			Range xR(0, x);
			Range yR(0, y);
			
			//calculates the next generation for life type
			if(type == 1){
				for(vector<Point>::iterator point = alive.begin(); point != alive.end(); ++point){
					int ex	= (*point).x - xOff;
					int why = (*point).y - yOff;
					aalive[why][ex]++;
					for(int i = -1; i < 2; i ++){
						for(int k = -1; k < 2; k++){
							if(xR.inRange(ex + k) && yR.inRange(why + i) && !(k == 0 && i == 0))arr[why + i][ex + k] += 1;
						}
					}
				}
				alive.clear();
				for(int i = 0; i < y; i++){
					for(int k = 0; k < x; k++){
						if(arr[i][k] == 3){
							alive.push_back(*(new Point((k + xOff), (i + yOff))));
						}
						else if(arr[i][k] == 2 && aalive[i][k] != 0){
							alive.push_back(*(new Point((k + xOff), (i + yOff))));
						}
					}
				}
			}
			
			//calculate the next() step for elementary type
			if(type == 2){
				for(vector<Point>::iterator point = alive.begin(); point != alive.end(); ++point){
					int ex = (*point).x - xOff;
					int why = (*point).y - yOff;
					aalive[why][ex]++;
				}
				vector<Point> temp;
				int one, two, three, count = 0;
				for(int i = 0; i < y - 1; i++){
					for(int k = 0; k < x; k++){
						two = aalive[i][k];
						if(k != 0) one = aalive[i][k - 1];
						else one = 0;
						if(k != (x - 1)) three = aalive[i][k + 1];
						else three = 0;
						int test = (4 * one) + (2 * two) + three;
						if(test > 0 && test < 5){
							if(xR.inRange(k) && yR.inRange(i - 1)) temp.push_back(*(new Point((k + xOff), (i + yOff - 1))));
						}
					}
				}
				//added this for handling duplicate points.
				alive.clear();
				for(vector<Point>::iterator point = AL.begin(); point != AL.end(); ++point)alive.push_back(*point);
				for(vector<Point>::iterator point = temp.begin(); point != temp.end(); ++point)alive.push_back(*point);
			}
			
			//calculate the next() step for wireworld type
			if(type == 3){
				int ex, why;
				for(vector<Point>::iterator point = alive.begin(); point != alive.end(); ++point){
					ex = (*point).x - xOff;
					why = (*point).y - yOff;
					arr[why][ex] = 3;
					for(int i = -1; i < 2; i ++){
						for(int k = -1; k < 2; k++){
							if(xR.inRange(ex + k) && yR.inRange(why + i) && !(k == 0 && i == 0))aalive[why + i][ex + k] += 1;
						}
					}
				}
				for(vector<Point>::iterator point = wire.begin(); point != wire.end(); ++point){
					ex = (*point).x - xOff;
					why = (*point).y - yOff;
					arr[why][ex] = 1;
				}
				for(vector<Point>::iterator point = tail.begin(); point != tail.end(); ++point){
					ex = (*point).x - xOff;
					why = (*point).y - yOff;
					arr[why][ex] = 2;
				}
				//this is for updating all of the new points. hard to check for duplicates so I just cleared them and put
				//in all the new ones.
				alive.clear();
				wire.clear();
				tail.clear();
				for(int i = 0; i < y; i++){
					for(int k = 0; k < x; k++){
						if(arr[i][k] == 2)wire.push_back(*(new Point((k + xOff), (i + yOff))));
						else if(arr[i][k] == 3)tail.push_back(*(new Point((k + xOff), (i + yOff))));
						else if(arr[i][k] == 1){
							if(aalive[i][k] == 1 || aalive[i][k] == 2){
								alive.push_back(*(new Point((k + xOff), (i + yOff))));
							}
							else wire.push_back(*(new Point((k + xOff), (i + yOff))));
						}
					}
				}
			}
		}
		
		//initial is for outputting the display info for all of the relevant points. Added type checking
		//so that the appropriate info is displayed.
		string Life::initial(){
			string out = "Initial={";
			if(type == 1 || type == 2){
				for(vector<Point>::iterator point = alive.begin(); point != alive.end(); ++point){
					if(point == alive.begin() && type == 1)out += "Alive=";
					else if(point == alive.begin() && type == 2)out += "One=";
					
					if(point != alive.begin()) out += ",";
					out += (*point).toString();
				}
				if(out.length() > 15)out+=";";
			}
			
			if(type == 3){
				string h, w, t;
				for(vector<Point>::iterator point = alive.begin(); point != alive.end(); ++point){
					if(point == alive.begin())h += "Head=";
					if(point != alive.begin()) h += ",";
					h += (*point).toString();
				}
				if(h.length() > 7) h += ";";
				for(vector<Point>::iterator point = wire.begin(); point != wire.end(); ++point){
					if(point == wire.begin())w += "Wire=";
					if(point != wire.begin()) w += ",";
					w += (*point).toString();
				}
				if(w.length() > 7) w += ";";
				for(vector<Point>::iterator point = tail.begin(); point != tail.end(); ++point){
					if(point == tail.begin())t += "Tail=";
					if(point != tail.begin()) t += ",";
					t += (*point).toString();
				}
				if(t.length() > 7) t += ";";
				out += (h + w + t);
			}
			out += "};";
			
			return out;
		}
	
		void Life::setTerrainAndWindow(Range tX, Range tY, Range wX, Range wY){
			terrain = new Terrain(tX, tY);
			vector<Point> nAlive, nWire, nTail;
			
			for(vector<Point>::iterator point = alive.begin(); point != alive.end(); ++point){
				if((*terrain).inTerrain(*point))nAlive.push_back(*point);
			}
			alive.clear();
			for(vector<Point>::iterator point = nAlive.begin(); point != nAlive.end(); ++point){
				alive.push_back(*point);
			}
			if(type == 3){
				for(vector<Point>::iterator point = wire.begin(); point != wire.end(); ++point){
					if((*terrain).inTerrain(*point))nWire.push_back(*point);
				}
				wire.clear();
				for(vector<Point>::iterator point = nWire.begin(); point != nWire.end(); ++point){
					wire.push_back(*point);
				}
				for(vector<Point>::iterator point = tail.begin(); point != tail.end(); ++point){
					if((*terrain).inTerrain(*point))nTail.push_back(*point);
				}
				tail.clear();
				for(vector<Point>::iterator point = nTail.begin(); point != nTail.end(); ++point){
					tail.push_back(*point);
				}
			}
			
			setWindow(wX, wY);
		}
		
		
		void Life::setWindow(Range x, Range y){
			
			window = new Window(x, y, false, terrain);
		}
	
		void Life::aName(string param){
			
			int beg, end;
			beg = param.find("\"");
			end = param.find("\"", beg + 1);
			name = param.substr(beg + 1, end - beg - 1);
		}
		
		//this just up the struct for points in "initial" and creates the classes vectors
		//AL, WI, and TA are for storing the set of initial points for reverting.
		void Life::points(string param){
			//wireworld type
			if(type == 3){
				int b, e, ps, pe;
				string sub;
				
				//head points
				b = param.find("HEAD");
				if(b != string::npos){
					e = param.find(";", b);
					sub = param.substr(b, e - b + 1);
					ps = sub.find("(");
					while(ps != string::npos){
						pe = sub.find(")", ps);
						if(pe != string::npos){
							alive.push_back(*(new Point(sub.substr(ps, pe - ps))));
							ps = sub.find("(", ps + 1);
						}
					}
				}
				for(vector<Point>::iterator point = alive.begin(); point != alive.end(); ++point)AL.push_back(*point);
		
				//wire points
				b = param.find("WIRE");
				if(b != string::npos){
					e = param.find(";", b);
					sub = param.substr(b, e - b + 1);
					ps = sub.find("(");
					while(ps != string::npos){
						pe = sub.find(")", ps);
						if(pe != string::npos){
							wire.push_back(*(new Point(sub.substr(ps, pe - ps))));
							ps = sub.find("(", ps + 1);
						}
					}
				}
				
				for(vector<Point>::iterator point = wire.begin(); point != wire.end(); ++point)WI.push_back(*point);
		
				//tail points
				b = param.find("TAIL");
				if(b != string::npos){
					e = param.find(";", b);
					sub = param.substr(b, e - b + 1);
					ps = sub.find("(");
					while(ps != string::npos){
						pe = sub.find(")", ps);
						if(pe != string::npos){
							tail.push_back(*(new Point(sub.substr(ps, pe - ps))));
							ps = sub.find("(", ps + 1);
						}
					}
				}
				
				for(vector<Point>::iterator point = tail.begin(); point != tail.end(); ++point)TA.push_back(*point);
				
				
			}else{//Life and Elem automata types
				int ps, pe;
				ps = param.find("(");
				while(ps != string::npos){
					pe = param.find(")", ps);
					if(pe != string::npos){
						alive.push_back(*(new Point(param.substr(ps, pe - ps))));
						ps = param.find("(", ps + 1);
					}
				}
				for(vector<Point>::iterator point = alive.begin(); point != alive.end(); ++point)AL.push_back(*point);
			}
			
		}
		
		
		int Life::getWidth(){
			return (*terrain).getXrange().getSize();
		}
		
		int Life::getHeight(){
			return (*terrain).getYrange().getSize();
		}
		
		//outputs display to string
		string Life::toString(){
			string header;
			if(type == 1) header = "Life";
			if(type == 2) header = "Elementary";
			if(type == 3) header = "WireWorld";
			return header + "={Name=\"" + name + "\";" + (*terrain).toString() + (*window).toString() +
				(*chars).toString() + (*colors).toString() + initial() + "};";
		}