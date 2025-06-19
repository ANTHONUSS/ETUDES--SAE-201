#include "notepad/notepad.h"
#include <ui_Notepad.h>
#include "etape/Etape.h"

int main(int argc, char* argv[]) {
	QApplication EditorApp(argc, argv);

    //test classe Etape
    int d=45;
    int m=37;
    int s=16;
    QChar NS='E';
    Etape e;
    e.setLongitude(d,m,s,NS);
    e.setLatitude(d,m,s,'N');
//    std::cout<<e.getLongitude()<<std::endl;
//    std::cout<<e.getCoordonnee().toStdString()<<std::endl;


	Notepad Editor;
	Editor.show();
	//bonjour
	return EditorApp.exec();

}
