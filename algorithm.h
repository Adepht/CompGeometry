#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QString>

#include <vector>
#include <algorithm>
#include <iostream>

enum Operation {
    STEPARATOR,
    SET_I,
    SET_CUR,
    SWAP
};

struct Action{
    Action(Operation a, qint32 fwd, qint32 bck);
    Action(Operation a);
    Operation _oper;
    qint32 _fwd;
    qint32 _bck;
};

enum AlgoState {
    INIT,
    BUILD,
    STEP
};

class Algorithm : public QObject
{
    Q_OBJECT
public:
    Algorithm();

    int AddPoint(QPointF point);
    int AddPoint(qreal x, qreal y);
    int Init();
    int Build();
    void Reset();


    AlgoState GetState();
    void ResetToAdd();
    QPointF GetBotleft();
    QRectF GetRect();
    qint32 GetStepNum();
    qint32 GetStepCount();

    std::vector<QPointF>* GetVectorR();
    qint32 GetCur();

signals:
//    void error(QString message);
public slots:
    void SkipForward();
    void SkipBack();
    int StepForward();
    int StepBack();

private:
    double Turn(QPointF a, QPointF b, QPointF c);

    std::vector<QPointF> _data;
    QPointF _botleft;
    QRectF _rect;
    qint32 _i, _cur;
    qint32 n;

    AlgoState _state; //next action

    std::vector<Action> _flow;
    qint32 _flowit;

};

#endif // ALGORITHM_H
