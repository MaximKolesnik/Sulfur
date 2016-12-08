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
    QSplashScreen *splashScreen = new QSplashScreen(QPixmap("images/Splash.png"));
    w.show();
    splashScreen->show();

    QTimer::singleShot(2500, splashScreen, SLOT(close()));

    while (w.isVisible())
    {
      w.Frame();
      a.processEvents();
    }

    return 0;
}
