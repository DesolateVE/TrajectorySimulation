#include "stdafx.h"
#include "veSimulation.h"

void veSimulation::executeCalculate(int64_t workingRange, int64_t accTimeTick, int64_t finalSpeed, int64_t responseTime)
{
    int64_t distanceA = finalSpeed * (accTimeTick + 1) / 2;
    int64_t distanceB = finalSpeed * (responseTime + 1) / 2;
    int64_t uniformRange = workingRange - distanceA - distanceB;

    if (uniformRange > 0)
    {
        // 在首次往复中达到目标速度
        trapezoidProgram(workingRange, accTimeTick, responseTime, finalSpeed);
    }
    else
    {
        // 需要再次加速

    }
}

QList<int64_t> veSimulation::speedUP(int64_t speed, int64_t accTime, int64_t& count)
{
    double accSpeed = (double)speed / accTime;
    QList<int64_t> list;
    double tmp = 0;
    while (tmp < speed)
    {
        count += qRound64(tmp);
        list << qRound64(tmp);
        tmp += accSpeed;
    }
    if (list.last() != speed)
    {
        list << speed;
        count += speed;
    }
    return list;
}

QList<int64_t> veSimulation::speedDown(int64_t speed, int64_t accTime, int64_t& count)
{
    double accSpeed = (double)speed / accTime;
    QList<int64_t> list;
    double tmp = speed;
    while (tmp > 0)
    {
        count += qRound64(tmp);
        list << qRound64(tmp);
        tmp -= accSpeed;
    }
    if (list.last() != 0)
    {
        list << 0;
    }
    return list;
}

QList<int64_t> veSimulation::speedUniform(int64_t speed, int64_t range)
{
    int64_t points = range / speed;
    int64_t remainder = range % speed;
    QList<int64_t> list(points, speed);

    if (remainder > points)
    {
        for (size_t i = 0; i < points; i++) {
            list[i] += remainder / points;
        }
        for (size_t i = 0; i < remainder % points; i++) {
            list[i]++;
        }
    }
    else
    {
        for (size_t i = 0; i < remainder; i++) {
            list[i]++;
        }
    }

    return list;
}

void veSimulation::trapezoidProgram(int64_t range, int64_t speedUPTime, int64_t speedDownTime, int64_t finalSpeed)
{
    int64_t tmp = 0;
    QList<int64_t> list1 = speedUP(finalSpeed, speedUPTime, tmp);
    QList<int64_t> list2 = speedDown(finalSpeed, speedDownTime, tmp);
    int64_t uniformRange = range - tmp;
    QList<int64_t> list3 = speedUniform(finalSpeed, uniformRange);

    qreal i = 0;
    int64_t count = 0;
    for (auto n : list1) {
        count += n;
        m_velocityList << QPointF{i, (qreal)n};
        m_postionList << QPointF{i++, (qreal)count};
    }
    for (auto n : list3) {
        count += n;
        m_velocityList << QPointF{i, (qreal)n};
        m_postionList << QPointF{i++, (qreal)count};
    }
    for (auto n : list2) {
        count += n;
        m_velocityList << QPointF{i, (qreal)n};
        m_postionList << QPointF{i++, (qreal)count};
    }
}

