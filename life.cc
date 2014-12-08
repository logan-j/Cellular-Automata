/*
 * life.cc
 *
 * author: Logan Jewett
 *
 */


#include "alive.h"

main(int argc, char *argv[]){
	try{
		Switch switches(argc, argv);
		
		if(switches.h){
			string help = "\n\n\tDESCRIPTION\n\n\tlife is an implementation of \"Conway's Game of Life\" from the 1970s.\
						 \n\tIt takes in a specially formatted life struct with state information about the array\
						 \n\tas well as a variety of options to change display type, size, and format, etc. Stdin is used\
						 \n\twhen no file name is specified. Usage is as follows:\
						 \n\n\t\t$ ./life [switches] [file]\
						 \n\n\tOPTIONS\n\n\n\t-h\t\tPrint a help message and exit.\
						 \n\n\n\t-f\t\tOutput current state information as a life struct\
						 \n\n\n\t-v\t\tOutput current state visually\
						 \n\n\n\t-g  n\t\tSpecify the desired generation number. Default is n = 0\
						 \n\n\n\t-tx l..h\tSet the x range of the terrain; overrides values specified in the input file\
						 \n\n\n\t-ty l..h\tSet the y range of the terrain; overrides values specified in the input file\
						 \n\n\n\t-wx l..h\tSet the x range of the window; overrides values specified in the input file\
						 \n\n\n\t-wy l..h\tSet the y range of the window; overrides values specified in the input file\n\n";
			cerr << help;
			return 0;
		}
		
		//decided to just handle whether the switches were null or not in here
		//because the compiler really didn't like the idea of NULL range pointers
		//being fed to methods.
		
		ifstream f;
		f.open(switches.fileName.c_str());
		string toParse, upper;
		if(f.is_open()){
			stringstream buffer;
			buffer << f.rdbuf();
			toParse = buffer.str();
			
			f.close();
		}else{
			char c;
			while(cin.get(c)){
				toParse += c;
			}
		}
		upper = stripCommAndUpper(toParse);
		int iType = getType(upper);
		if(iType == 0)throw tF();//if there are no appropriate automata structs
		if(iType == 4)throw tM();//too many automata structs
		Life out(upper, true, iType);
		
		Range *tx, *ty, *wx, *wy;
		
		//Checks if there were range parameters given as a switch, if no parameter was
		//given then the value in switches is NULL and it sets it to the corresponding 
		//value set from the input file.
		(switches.tx == NULL) ? tx = new Range(out.getTXrange()) : tx = new Range(*(switches.tx));
		(switches.ty == NULL) ? ty = new Range(out.getTYrange()) : ty = new Range(*(switches.ty));
		(switches.wx == NULL) ? wx = new Range(out.getWXrange()) : wx = new Range(*(switches.wx));
		(switches.wy == NULL) ? wy = new Range(out.getWYrange()) : wy = new Range(*(switches.wy));
		
		//This sets the terrain. Will override values given in the input file.
		//The window is bounded by the edges of the terrain.
		out.setTerrainAndWindow(*tx, *ty, *wx, *wy);
		
		for(int i = 0; i < switches.g; i++)out.next();
		
		if(switches.f && !switches.v){
			cout << out.toString(); 
		}
		
		else if(switches.v && !switches.f){
			//opted for this 2D array business in here because I'm still having trouble
			//passing malloc'd memory between methods/classes. Computer doesn't like it.
			
			int width = out.getWidth();
			int height = out.getHeight();
			int xOff = out.getXoff();
			int yOff = out.getYoff();
			int type = out.getType();
			char alive = out.getAliveC();
			char dead = out.getDeadC();
			char wire, tail;
			if(type == 3){
				wire = out.getWireC();
				tail = out.getTailC();
			}
			
			int **arr;
			arr = (int **) malloc(height * sizeof(int*));
			for(int i = 0; i < height; i++) arr[i] = (int *) calloc(width, sizeof(int));
			
			//creates an int array with relevant info (similar in gui.cc) for keeping track of which cell is which.
			if(type == 1 || type == 2){
				for(vector<Point>::iterator point = out.alive.begin(); point != out.alive.end(); ++point){
					int xP = (*point).x - xOff;
					int yP = height - (*point).y + yOff - 1;
					arr[yP][xP] = 1;
				}
			}
			else {
				for(vector<Point>::iterator point = out.alive.begin(); point != out.alive.end(); ++point){
					int xP = (*point).x - xOff;
					int yP = height - (*point).y + yOff - 1;
					arr[yP][xP] = 3;
				}
				for(vector<Point>::iterator point = out.wire.begin(); point != out.wire.end(); ++point){
					int xP = (*point).x - xOff;
					int yP = height - (*point).y + yOff - 1;
					arr[yP][xP] = 1;
				}
				for(vector<Point>::iterator point = out.tail.begin(); point != out.tail.end(); ++point){
					int xP = (*point).x - xOff;
					int yP = height - (*point).y + yOff - 1;
					arr[yP][xP] = 2;
				}
			}
			cout << "\n";
			
			//takes the recently created int array and iterates it, pumping out chars into cout
			for(int i = 0; i < height; i++){
				bool line = false;
				for(int k = 0; k < width; k++){
					if(out.getWXrange().inRange(k + xOff, true) && out.getWYrange().inRange(i + yOff, true)){//checks if it's in the window.
						line = true;
						if(type == 1 || type == 2){
							if(arr[i][k] == 1) cout << alive;
							else cout << dead;
						}
						else if(type == 3){
							if(arr[i][k] == 1) cout << wire;
							else if(arr[i][k] == 2) cout << tail;
							else if(arr[i][k] == 3) cout << alive;
							else cout << dead;
						}
					}
				}
				if(line)cout << "\n";
			}
		}
		
		//Visual or file output exclusive
		else{
			cerr << "Select either -v or -f for output.";
		}
		
	}
	
	//throwing strings caused status access violations. Decided to just
	//do this. Generic exception handling lost the error message.
	catch (iS ex){cerr << ex.what();}
	catch (tFA ex){cerr << ex.what();}
	catch (mA ex){cerr << ex.what();}
	catch (iA ex){cerr << ex.what();}
	catch (fNN ex){cerr << ex.what();}
	catch (tMA ex){cerr << ex.what();}
	catch (iR ex){cerr << ex.what();}
	catch (mFI ex){cerr << ex.what();}
	catch (tF ex){cerr << ex.what();}
	catch (tM ex){cerr << ex.what();}
}