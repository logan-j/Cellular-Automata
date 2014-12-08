/*
 * life_gui.cc
 *
 * author: Logan Jewett
 *
 */


#include "alive.h"
#include "gui.h"

//moved gui to separate class

int main(int argc, char *argv[]){
	try{
		Switch switches(argc, argv);
		
		if(switches.h){//help message
			string help = "\n\n\tDESCRIPTION\n\n\tlife_gui is a graphical version of \"Conway's Game of Life\" from the 1970s.\
						 \n\tIt takes in a specially formatted life struct with state information about the array\
						 \n\tas well as a variety of options to change size and format, etc. Stdin is used\
						 \n\twhen no file name is specified. Output is a graphical representation that utilizes Qt.\
						 \n\tUsage is as follows:\
						 \n\n\t\t$ ./life_gui [switches] [file]\
						 \n\n\tOPTIONS\n\n\n\t-h\t\tPrint a help message and exit.\
						 \n\n\n\t-s  n\t\tSpecify the size, in pixels, of each square in the grid. Default is n = 10\
						 \n\n\n\t-g  n\t\tSpecify the desired generation number. Default is n = 0\
						 \n\n\n\t-c\t\tSpawn the control panel for life_gui\
						 \n\n\n\t-tx l..h\tSet the x range of the terrain; overrides values specified in the input file\
						 \n\n\n\t-ty l..h\tSet the y range of the terrain; overrides values specified in the input file\
						 \n\n\n\t-wx l..h\tSet the x range of the window; overrides values specified in the input file\
						 \n\n\n\t-wy l..h\tSet the y range of the window; overrides values specified in the input file\n\n";
			cerr << help;
			return 0;
		}
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
		Life out(upper, false, iType);
		
		//recalculating window info from specified switches/what's visible
		Range *tx, *ty, *wx, *wy;
		(switches.tx == NULL) ? tx = new Range(out.getTXrange()) : tx = new Range(*(switches.tx));
		(switches.ty == NULL) ? ty = new Range(out.getTYrange()) : ty = new Range(*(switches.ty));
		(switches.wx == NULL) ? wx = new Range(out.getWXrange()) : wx = new Range(*(switches.wx));
		(switches.wy == NULL) ? wy = new Range(out.getWYrange()) : wy = new Range(*(switches.wy));
		out.setTerrainAndWindow(*tx, *ty, *wx, *wy);
		
		//initializes the life object to the stated generation
		for(int i = 0; i < switches.g; i++)out.next();
		
		int width = out.getWidth();
		int height = out.getHeight();
		int xOff = out.getXoff();
		int yOff = out.getYoff();
		
		int xL = out.getXL() - xOff;
		int xH = out.getXH() - xOff + 1;
		int yL = out.getYL() - yOff;
		int yH = out.getYH() - yOff + 1;
		
		QApplication app(argc, argv);
		
		Gui window(out, height, width, xOff, yOff, switches.s);
		window.wSet(xL, xH, yL, yH);
		window.move(300,300);
		
		QString name = QString(switches.fileName.c_str());
		window.setWindowTitle(name);
		window.show();
		
		//as you can see, gui.cc pretty well encapsulated the controls.
		if(switches.c == true){

			window.control();
			window.show();
				
		}
		
		return app.exec();
		
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