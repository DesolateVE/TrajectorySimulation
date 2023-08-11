#pragma once

#include <QtWidgets/QWidget>
#include "ui_TrajectorySimulation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TrajectorySimulationClass; };
QT_END_NAMESPACE

class TrajectorySimulation : public QWidget
{
    Q_OBJECT

public:
    TrajectorySimulation(QWidget *parent = nullptr);
    ~TrajectorySimulation();

private:
    Ui::TrajectorySimulationClass *ui;
};
