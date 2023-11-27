#include "stdafx.h"
#include "TrajectorySimulation.h"
#include <QtCharts>
#include <QLineSeries>
#include "veSimulation.h"
#include "TestClass.h"

TrajectorySimulation::TrajectorySimulation(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TrajectorySimulationClass())
{
    ui->setupUi(this);

    QChart* chart;

    chart = new QChart();
    ui->velocityGraphicsView->setChart(chart);
    ui->velocityGraphicsView->setRenderHint(QPainter::Antialiasing);

    chart = new QChart();
    ui->postionGraphicsView->setChart(chart);
    ui->postionGraphicsView->setRenderHint(QPainter::Antialiasing);

    connect(ui->renderButton, &QPushButton::clicked, [this] {

        veSimulation sim;
        QLineSeries* series;

        sim.executeCalculate(
            ui->workDistanceBox->value(),
            ui->accTimeBox->value(),
            ui->dstSpeedBox->value(),
            ui->responseBox->value()
        );

        TestClass test;

        ui->postionGraphicsView->chart()->removeAllSeries();
        ui->velocityGraphicsView->chart()->removeAllSeries();

        series = new QLineSeries();
        series->setName("速度曲线");
        /*series->replace(sim.m_velocityList);*/
        series->replace(test.m_list2);
        ui->velocityGraphicsView->chart()->addSeries(series);
        ui->velocityGraphicsView->chart()->createDefaultAxes();

        series = new QLineSeries();
        series->setName("位移曲线");
        /*series->replace(sim.m_postionList);*/
        series->replace(test.m_list1);
        ui->postionGraphicsView->chart()->addSeries(series);
        ui->postionGraphicsView->chart()->createDefaultAxes();
        });
}

TrajectorySimulation::~TrajectorySimulation()
{
    delete ui;
}
