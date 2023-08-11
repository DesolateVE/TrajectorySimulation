#include "stdafx.h"
#include "TrajectorySimulation.h"
#include <QtCharts>
#include <QLineSeries>
#include "veSimulation.h"

TrajectorySimulation::TrajectorySimulation(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TrajectorySimulationClass())
{
    ui->setupUi(this);

    veSimulation sim;
    sim.executeCalculate(800000, 98, 8000, 37);

    QChart* chart;
    QLineSeries* series;
    chart = new QChart();
    series = new QLineSeries();
    series->setName("速度曲线");
    ui->velocityGraphicsView->setChart(chart);
    ui->velocityGraphicsView->setRenderHint(QPainter::Antialiasing);
    series->replace(sim.m_velocityList);
    chart->addSeries(series);
    chart->createDefaultAxes();

    chart = new QChart();
    series = new QLineSeries();
    series->setName("位移曲线");
    ui->postionGraphicsView->setChart(chart);
    ui->postionGraphicsView->setRenderHint(QPainter::Antialiasing);
    series->replace(sim.m_postionList);
    chart->addSeries(series);
    chart->createDefaultAxes();
}

TrajectorySimulation::~TrajectorySimulation()
{
    delete ui;
}
