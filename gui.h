/*
 * gui.h
 *
 * author: Logan Jewett
 *
 */


#ifndef GUI_H
#define GUI_H
#include "alive.h"

#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QApplication>
#include <QWidget>
#include <QString>
#include <QTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

namespace Ui{
	class Gui;
}

//A lot of the gui stuff is a heavily modified example I found online. I tried but I just could
//not get it to draw in the main class. 
class Gui : public QWidget{
	Q_OBJECT
	
	public:
		//RGB info for alive and dead colors, plus terrain dimensions, a pointer for cell locations,
		//and square size in pixels.
		int aR, aG, aB, dR, dG, dB, wR, wG, wB, tR, tG, tB, type, **array, h, w, es;
		
		bool playing;
		//window dimensions
		int xL, xH, yL, yH;
		Life *life;		
		
		Gui(Life &input, int height, int width, int xOff, int yOff, int s, QWidget *parent = 0);
		
		void updateW();
		void paintEvent(QPaintEvent *event);
		
		//set window boundaries
		void wSet(int xL, int xH, int yL, int yH);
		void control();
		QLabel *num;
		QPushButton *restartButton, *playButton, *stepButton;
		QSpinBox *delaySpin, *sizeSpin;
		
	public slots:
		void restart();
		void play();
		void step();
		void changeSize();//links the size slider updating with refreshing the display
		
		
};



#endif