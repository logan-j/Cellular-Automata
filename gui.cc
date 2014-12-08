/*
 * gui.cc
 *
 * author: Logan Jewett
 *
 */


#include "gui.h"
#include "alive.h"

//the following are the custom slots.

//little function I found while poking around online
//for Qt to get a reliable delay.
void delay(int s){
    QTime dieTime= QTime::currentTime().addMSecs(s);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);    
}

//calls revert and updates. The life class has vectors for storing
//the initial states.
void Gui::restart(){
	if(playing) playing = false;
	life->revert();
	this->updateW();
	num->setText(QString(to_string(life->getGen()).c_str()));//this ended up being a pain to get something qstring would like
	this->show();
}

//just toggles play on and off with the incorporated delay.
void Gui::play(){
	if(playing) playing = false;
	else{
		playing = true;
		int timer = delaySpin->value();
		while(playing){
			delay(timer);
			life->next();
			this->updateW();
			num->setText(QString(to_string(life->getGen()).c_str()));
			this->show();	
		}
	}
}

//sets "es" or size from the spinBox value and stops the animation and updates the window.
void Gui::changeSize(){
	playing = false;
	es = sizeSpin->value();
	this->updateW();
	this->show();
}

//turn off the animation, increment life once, and update.
void Gui::step(){
	if(playing) playing = false;
	life->next();
	this->updateW();
	num->setText(QString(to_string(life->getGen()).c_str()));
	this->show();
}

//height and width of the terrain and the offsets for the boundaries
Gui::Gui(Life &input, int height, int width, int xOff, int yOff, int s, QWidget *parent) : QWidget(parent){
	playing = false;
	life = &input;
	
	//sets all the color info in gui. Although instead of ints I guess I could've just added RGBs to the class.
	aR = input.getAliveRGB().R;
	aG = input.getAliveRGB().G;
	aB = input.getAliveRGB().B;
	dR = input.getDeadRGB().R;
	dG = input.getDeadRGB().G;
	dB = input.getDeadRGB().B;
	type = input.getType();
	if(type == 3){
		wR = input.getWireRGB().R;
		wG = input.getWireRGB().G;
		wB = input.getWireRGB().B;
		tR = input.getTailRGB().R;
		tG = input.getTailRGB().G;
		tB = input.getTailRGB().B;
	}
	
	//max display size of 25 pixels. felt like just doing that to fix the issue with setting pixel size larger
	//than the max value set on the slider. This seemed about the max size in the example.
	if (s > 25) es = 25;
	else es = s;
	h = height;
	w = width;
	
	array = (int **) malloc(height * sizeof(int*));
	for(int i = 0; i < height; i++) array[i] = (int *) calloc(width, sizeof(int));
	
	//uses alive to represent head/one/alive, with 3/1/1 to represent it respectively
	for(vector<Point>::iterator point = input.alive.begin(); point != input.alive.end(); ++point){
		int xP = (*point).x - xOff;
		int yP = (*point).y - yOff;
		if(type != 3) array[yP][xP] = 1;
		else array[yP][xP] = 3;
	}
	
	//sets the wire and tail info from the input life object for wireworld.
	if(type == 3){
		for(vector<Point>::iterator point = input.wire.begin(); point != input.wire.end(); ++point){
			int xP = (*point).x - xOff;
			int yP = (*point).y - yOff;
			array[yP][xP] = 1;
		}
		for(vector<Point>::iterator point = input.tail.begin(); point != input.tail.end(); ++point){
			int xP = (*point).x - xOff;
			int yP = (*point).y - yOff;
			array[yP][xP] = 2;
		}
	}
}

void Gui::control(){
			QWidget *controls = new QWidget();
			controls->setWindowTitle("Life_Gui Control Panel");
			
			//buttons
			restartButton = new QPushButton(QString("Restart"), this);
			playButton = new QPushButton(QString("|>"), this);
			stepButton = new QPushButton(QString("Step"), this);
			QPushButton *quitButton = new QPushButton(QString("Quit"));
	
			
			
			//grid and sliders
			QLabel *grid = new QLabel("Grid Size:");
			sizeSpin = new QSpinBox;
			sizeSpin -> setRange(1, 25);
			QSlider *sizeSlider = new QSlider(Qt::Horizontal);
			sizeSlider -> setRange(1, 25);
			sizeSlider -> setValue(es);
			sizeSpin -> setValue(es);
			
			//delay and sliders
			QLabel *del = new QLabel("Delay (ms):");
			delaySpin = new QSpinBox;
			delaySpin -> setRange(100, 3000);
			QSlider *delaySlider = new QSlider(Qt::Horizontal);
			delaySlider -> setRange(100, 3000);
			
			
			//generations
			QLabel *gen = new QLabel("Generation:");
			num = new QLabel();
			num->setText(QString(to_string(life->getGen()).c_str()));
			//buttons 
			
			
			
			//do stuff (add actions to buttons and slider/spinboxes)
			QObject::connect(sizeSpin, SIGNAL(valueChanged(int)), sizeSlider, SLOT(setValue(int)));
			QObject::connect(sizeSlider, SIGNAL(valueChanged(int)), sizeSpin, SLOT(setValue(int)));
			QObject::connect(delaySpin, SIGNAL(valueChanged(int)), delaySlider, SLOT(setValue(int)));
			QObject::connect(delaySlider, SIGNAL(valueChanged(int)), delaySpin, SLOT(setValue(int)));
			QObject::connect(sizeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeSize()));//linking slider and changeSize
			
			QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
			QObject::connect(quitButton, SIGNAL(clicked()), controls, SLOT(close()));
			
			QObject::connect(stepButton, SIGNAL(clicked()), this, SLOT(step()));
			QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(play()));
			QObject::connect(restartButton, SIGNAL(clicked()), this, SLOT(restart()));
			
			
			//add stuff to layout. Each line is its own QHBox that's then added to the QVBox
			QHBoxLayout *grids = new QHBoxLayout;
			QHBoxLayout *delays = new QHBoxLayout;
			QHBoxLayout *buttons = new QHBoxLayout;
			QHBoxLayout *generations = new QHBoxLayout;
			QVBoxLayout *layout = new QVBoxLayout;
			grids->addWidget(grid);
			grids->addWidget(sizeSpin);
			grids->addWidget(sizeSlider);
			
			delays->addWidget(del);
			delays->addWidget(delaySpin);
			delays->addWidget(delaySlider);
			
			generations->addWidget(gen);
			generations->addWidget(num);
			
			buttons->addWidget(quitButton);
			buttons->addWidget(restartButton);
			buttons->addWidget(playButton);
			buttons->addWidget(stepButton);
			
			layout->addLayout(grids);
			layout->addLayout(delays);
			layout->addLayout(generations);
			layout->addLayout(buttons);
			controls->setLayout(layout);
			
			//display
			controls->show();
}

//initially added for being able to move the window about, but it didn't show up in the program specs
//so I didn't add it.
void Gui::wSet(int iXL, int iXH, int iYL, int iYH){
	
	xL = iXL;
	xH = iXH;
	yL = iYL;
	yH = iYH;
}

//this just re-puts out graphical info based on the current generation of the
//stored life object.
void Gui::updateW(){
	array = (int **) malloc(h * sizeof(int*));
	for(int i = 0; i < h; i++) array[i] = (int *) calloc(w, sizeof(int));
	int xOff = (*life).getXoff();
	int yOff = (*life).getYoff();
	
	for(vector<Point>::iterator point = (*life).alive.begin(); point != (*life).alive.end(); ++point){
		int xP = (*point).x - xOff;
		int yP = (*point).y - yOff;
		if(type != 3) array[yP][xP] = 1;
		else array[yP][xP] = 3;
	}
	if(type == 3){
		for(vector<Point>::iterator point = (*life).wire.begin(); point != (*life).wire.end(); ++point){
			int xP = (*point).x - xOff;
			int yP = (*point).y - yOff;
			array[yP][xP] = 1;
		}
		for(vector<Point>::iterator point = (*life).tail.begin(); point != (*life).tail.end(); ++point){
			int xP = (*point).x - xOff;
			int yP = (*point).y - yOff;
			array[yP][xP] = 2;
		}
	}
}

//paints the box and updates everything.
void Gui::paintEvent(QPaintEvent *event){
	Q_UNUSED(event);
	QPainter painter(this);
	if(es > 3)painter.setPen(QColor("#000000"));
	else painter.setPen(Qt::NoPen);
	this->resize(((xH - xL) * es), ((yH - yL) * es));
	for(int i = yL; i < yH; ++i){
		for(int k = xL; k < xH; ++k){
			if(array[i][k] == 1 && (type == 1 || type == 2)) painter.setBrush(QBrush(QColor(aR, aG, aB)));
			else if(array[i][k] == 1 && type == 3) painter.setBrush(QBrush(QColor(wR, wG, wB)));
			else if(array[i][k] == 2) painter.setBrush(QBrush(QColor(tR, tG, tB)));
			else if(array[i][k] == 3) painter.setBrush(QBrush(QColor(aR, aG, aB)));
			else painter.setBrush(QBrush(QColor(dR, dG, dB)));
			painter.drawRect(((k - xL) *es), ((yH - (i + 1)) *es), es, es);
		}
	}
	update();
}