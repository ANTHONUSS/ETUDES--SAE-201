#include "notepad/notepad.h"
#include <ui_Notepad.h>

int main(int argc, char* argv[]) {
	QApplication EditorApp(argc, argv);
	Notepad Editor;
	Editor.show();
	//bonjour
	return EditorApp.exec();
}
