all: life life_gui

clean:
	rm -f alive.o life.o life_gui.o gui.o moc_gui.* life_guimake life.exe life_gui.exe life_gui.pro life_guimake

tarball: proj2p2.tar.gz

proj2p2.tar.gz: makefile readme.txt alive.h gui.h gui.cc life.cc life_gui.cc alive.cc
	tar -czf proj2p2.tar.gz makefile readme.txt alive.h gui.h gui.cc life.cc life_gui.cc alive.cc

life: life.o alive.o
	g++ -o life life.o alive.o

life_gui: life.o alive.o gui.h
	qmake-qt4 -project -nopwd -o life_gui.pro life_gui.cc gui.cc alive.cc
	qmake-qt4 -o life_guimake life_gui.pro
	make -f life_guimake

life.o: life.cc alive.h
	g++ -c life.cc

alive.o: alive.cc alive.h
	g++ -c alive.cc

