#pragma once

class veSimulation
{
public:
    veSimulation() = default;

public:
    void executeCalculate(int64_t workingRange, int64_t accTimeTick, int64_t finalSpeed, int64_t responseTime);

public:
    QList<QPointF> m_velocityList;
    QList<QPointF> m_postionList;

private:
    int64_t m_positon = 0;
    int64_t m_positonCount = 0;

    int64_t m_timeTick;
    int64_t m_distance;
    bool m_isSmooth;

    int64_t m_curTimeTick = 0;
    int64_t m_curDistance = 0;

private:
    QList<int64_t> speedUP(int64_t initSpeed, int64_t dstSpeed, double accSpeed, int64_t& count);
    QList<int64_t> speedDown(int64_t speed, int64_t accTime, int64_t& count);
    QList<int64_t> speedUniform(int64_t speed, int64_t range);

    void autoInterpolation(QList<int64_t>& list, int64_t remainder);
    void addChartList(bool direction, std::initializer_list<QList<int64_t>> tableList);

    void trapezoidProgram(int64_t range, int64_t speedUPTime, int64_t speedDownTime, int64_t finalSpeed);
    int64_t triangleProgram(int64_t workingRange, int64_t responseTime, double accSpeed);


private:
    const int64_t CURRENT_LOOP_FREQ = 16000;
    const int64_t encoder_4ab = 131072;
    const int64_t PS_CTL_SNUM = 1;
};

