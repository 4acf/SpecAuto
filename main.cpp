//#include "spectrogramautomation.h"
#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //SpectrogramAutomation w;
    Dialog w;
    w.show();
    return a.exec();
}
