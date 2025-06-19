#include "notepad/notepad.h"
#include <ui_Notepad.h>
#include "etape/Etape.h"

int main(int argc, char* argv[]) {
	QApplication EditorApp(argc, argv);
	Notepad Editor;
	Editor.show();
	return EditorApp.exec();

}
