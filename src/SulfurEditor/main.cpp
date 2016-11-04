#include "Windows/sfEditorWindow.hpp"
#include <QtWidgets/QApplication>
#include "Reflection/sfReflection.hpp"
#include "Utils/sfFileUtils.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    char *theme = (char*)Sulfur::FileUtils::ReadFile("Themes/Darcula.set");
    if (theme != nullptr)
    {
      a.setStyleSheet(theme);
      delete[] theme;
    }

    Sulfur::EditorWindow w;
    w.show();

    while (w.isVisible())
    {
      w.Frame();
      a.processEvents();
    }

    return 0;
}
