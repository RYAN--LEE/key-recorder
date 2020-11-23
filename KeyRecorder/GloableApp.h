#pragma once

#include <QObject>
#include <QtWidgets/QApplication>
#include <QEvent>

class GloableApp : public QApplication
{
public:
    GloableApp(int& argc, char** argv);
    bool notify(QObject*, QEvent*);

};

