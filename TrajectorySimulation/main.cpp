#include "stdafx.h"
#include "TrajectorySimulation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrajectorySimulation w;
    w.show();
    return a.exec();
}
