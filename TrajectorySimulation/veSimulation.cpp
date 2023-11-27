#include "stdafx.h"
#include "veSimulation.h"

void veSimulation::executeCalculate(int64_t workingRange, int64_t accTimeTick, int64_t finalSpeed, int64_t responseTime)
{
    double accSpeed = (double)finalSpeed / (accTimeTick - 1);

    int64_t distanceA = finalSpeed * accTimeTick / 2;
    int64_t distanceB = finalSpeed * responseTime / 2;
    int64_t uniformRange = workingRange - distanceA - distanceB;

    if (uniformRange > 0)
    {
        // 在首次往复中达到目标速度
        trapezoidProgram(workingRange, accTimeTick, responseTime, finalSpeed);
    }
    else
    {
        // 需要多次加速

        // 1. 速度曲线构成三角形
        int64_t lastSpeed = triangleProgram(workingRange, responseTime, accSpeed);

        // 2. 继续分类讨论
        int64_t accResponseDistance = lastSpeed * responseTime / 2;
        int64_t tempAccTick = (finalSpeed - lastSpeed) / accSpeed + 1;
        int64_t tempDistance = (lastSpeed + finalSpeed) * tempAccTick / 2;

        if (accResponseDistance + tempDistance + distanceB > workingRange)
        {
            // 仍然需要加速

        }
        else
        {
            // 可以进入匀速模式了
            int64_t tmp = 0;
            QList<int64_t> list1 = speedUP(0, lastSpeed, (double)lastSpeed / responseTime, tmp);
            QList<int64_t> list2 = speedUP(lastSpeed, finalSpeed, accSpeed, tmp);
            QList<int64_t> list3 = speedDown(finalSpeed, responseTime, tmp);
            int64_t uniformRange = workingRange - tmp;
            QList<int64_t> list4 = speedUniform(finalSpeed, uniformRange);
            addChartList(false, { list1 ,list2 ,list4 ,list3 });
        }
    }
}

QList<int64_t> veSimulation::speedUP(int64_t initSpeed, int64_t dstSpeed, double accSpeed, int64_t& count)
{
    QList<int64_t> list;
    double tmp = initSpeed;
    while (tmp < dstSpeed) {
        count += qRound64(tmp);
        list << qRound64(tmp);
        tmp += accSpeed;
    }
    if (dstSpeed - tmp > accSpeed / 2)
    {
        list << dstSpeed;
        count += dstSpeed;
    }
    return list;
}

QList<int64_t> veSimulation::speedDown(int64_t speed, int64_t accTime, int64_t& count)
{
    QList<int64_t> list;
    double accSpeed = (double)speed / (accTime - 1);
    double tmp;

    if (speed % (accTime - 1) == 0)
    {
        tmp = speed;
    }
    else
    {
        tmp = speed - accSpeed;
    }

    while (tmp > 0) {
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
    autoInterpolation(list, remainder);

    return list;
}

void veSimulation::autoInterpolation(QList<int64_t>& list, int64_t remainder)
{
    int64_t points = list.count();
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
}

void veSimulation::addChartList(bool direction, std::initializer_list<QList<int64_t>> tableList)
{
    for (auto list : tableList) {
        for (auto var : list) {
            if (direction)
            {
                m_positon += var;
                m_velocityList << QPointF(m_positonCount, var);
                m_postionList << QPointF(m_positonCount++, m_positon);
            }
            else
            {
                m_positon -= var;
                m_velocityList << QPointF(m_positonCount, var);
                m_postionList << QPointF(m_positonCount++, m_positon);
            }
        }
    }
}

void veSimulation::trapezoidProgram(int64_t range, int64_t speedUPTime, int64_t speedDownTime, int64_t finalSpeed)
{
    int64_t tmp = 0;
    QList<int64_t> list1 = speedUP(0, finalSpeed, (double)finalSpeed / (double)speedUPTime, tmp);
    QList<int64_t> list2 = speedDown(finalSpeed, speedDownTime, tmp);
    int64_t uniformRange = range - tmp;
    QList<int64_t> list3 = speedUniform(finalSpeed, uniformRange);

    addChartList(true, { list1, list3, list2 });
}

int64_t veSimulation::triangleProgram(int64_t workingRange, int64_t responseTime, double accSpeed)
{
    /**
    * int64_t distanceA = dstSpeed * accTimeTick / 2;
    * int64_t distanceB = dstSpeed * responseTime / 2;
    *
    * distanceA + distanceB = workingRange
    * dstSpeed * (accTimeTick + responseTime) = 2 * workingRange
    * (accTimeTick - 1) * accSpeed * (accTimeTick + responseTime) = 2 * workingRange
    * (accTimeTick - 1) * (accTimeTick + responseTime) = 2 * workingRange / accSpeed
    * accTimeTick² + accTimeTick * (responseTime - 1) - responseTime = 2 * workingRange / accSpeed
    * accTimeTick² + accTimeTick * (responseTime - 1) - responseTime - 2 * workingRange / accSpeed = 0
    *
    */

    double A = 1;
    double B = responseTime - 1;
    double C = -responseTime - 2.0 * workingRange / accSpeed;
    double delta = qPow(B, 2) - 4.0 * A * C;
    double thisAccTime = (-B + qSqrt(delta)) / (2 * A);

    int64_t tmp = 0;
    int64_t destSpeed = thisAccTime * accSpeed;
    QList<int64_t> list1 = speedUP(0, destSpeed, (double)destSpeed / (double)thisAccTime, tmp);
    QList<int64_t> list2 = speedDown(destSpeed, responseTime, tmp);
    QList<int64_t> list3;
    int64_t remaining = workingRange - tmp;

    if (remaining < 0)
    {
        remaining += list1.last();
        list1.pop_back();
    }

    if (remaining > list1.count() && remaining >= destSpeed * list2.count())
    {
        list3 = speedUniform(destSpeed, remaining);
    }
    else
    {
        autoInterpolation(list1, remaining);
    }

    addChartList(true, { list1 ,list3 ,list2 });
    return destSpeed;
}

