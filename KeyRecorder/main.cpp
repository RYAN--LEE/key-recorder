#include "KeyRecorder.h"
#include "GloableApp.h"

int main(int argc, char *argv[])
{
    GloableApp a(argc, argv);
    KeyRecorder w;
    w.show();
    return a.exec();
}
