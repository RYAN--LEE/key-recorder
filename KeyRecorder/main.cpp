#include <QtWidgets/QApplication>
#include "KeyRecorder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KeyRecorder w;
    w.show();
    return a.exec();
}
