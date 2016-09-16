#include "Windows/sfEditorWindow.hpp"
#include <QtWidgets/QApplication>
#include "Reflection/sfReflection.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Sulfur::EditorWindow w;
    w.show();

    while (w.isVisible())
    {
      a.processEvents();
      w.Frame();
    }

    return 0;
}
