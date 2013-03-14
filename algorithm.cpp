#include "algorithm.h"

bool PointCompare(QPointF a, QPointF b)
{
    return a.x() * b.y() - a.y() * b.x() > 0;
}

Algorithm::Algorithm()
    : _botleft(100000, 10000)
    , _state(INIT)
{
}

int Algorithm::AddPoint(QPointF point)
{
    if (_state != INIT)
        return (int)_state;
    if (_data.size() == 0)
    {
        _rect = QRectF(point - QPointF(1, 1), point + QPointF(1, 1));
        _botleft = point;
    }
    _rect |= QRectF(point - QPointF(1, 1), point + QPointF(1, 1));
    _data.push_back(point);
    if (_botleft.y() > point.y() || (_botleft.y() == point.y() && _botleft.x() > point.x()))
        _botleft = point;
}

int Algorithm::AddPoint(qreal x, qreal y)
{
    return AddPoint(QPointF(x, y));
}

int Algorithm::Init()
{
    if (_state != INIT)
        return 1;
    if (_data.size() < 3)
        return 2;
    n=_data.size();
    for (int i = 0; i < n; ++i)
        _data[i] -= _botleft;
    std::sort(_data.begin(), _data.end(), PointCompare);
    _i = 3;
    _cur = 2;
    _state = BUILD;

    return 0;
}

int Algorithm::Build()
{
    if (_state != BUILD)
        return 1;
    //GRAHAM
    _flow.push_back(Action(STEPARATOR));
    for (_i = 3; _i < n; ++_i)
    {
        if (Turn(_data[_cur - 1], _data[_cur], _data[_i]) >= 0)
        {
            _flow.push_back(Action(SET_CUR, _cur + 1, _cur));
            ++_cur;

            std::swap(_data[_i], _data[_cur]);
            _flow.push_back(Action(SWAP));
        }
        else
        {
            int temp = _i;
            while(Turn(_data[_cur - 1], _data[_cur], _data[_i]) < 0)
            {
                std::swap(_data[_i], _data[_cur]);
                _flow.push_back(Action(SWAP));

                _flow.push_back(Action(SET_I, _cur, _i));
                _i = _cur;

                _flow.push_back(Action(SET_CUR, _cur - 1, _cur));
                _cur--;
            }
            _flow.push_back(Action(SET_I, temp, _i));
            _i = temp;

            _flow.push_back(Action(SET_CUR, _cur + 1, _cur));
            ++_cur;
        }
        _flow.push_back(Action(SET_I, _i + 1, _i));
        _flow.push_back(Action(STEPARATOR));
    }
    //GRAHAMEND
    _flowit = _flow.size() - 1;
//    SkipBack(); //Makes algo reset after building command list, TODO
    _state = STEP;
    return 0;
}

void Algorithm::SkipForward()
{
    while (!StepForward());
}

void Algorithm::SkipBack()
{
    while (!StepBack());
}

void Algorithm::Reset()
{
    _state = INIT;
    _data.clear();
    _flow.clear();
}

int Algorithm::StepForward()
{
    if (_flow[_flowit]._oper != STEPARATOR)
        return 1;
    if (_flow.size() <= _flowit + 1)
        return 2;
    ++_flowit;
    while(_flow[_flowit]._oper != STEPARATOR)
    {
        switch(_flow[_flowit]._oper)
        {
        case SET_I:
            _i = _flow[_flowit]._fwd;
            break;
        case SET_CUR:
            _cur = _flow[_flowit]._fwd;
            break;
        case SWAP:
            std::swap(_data[_i], _data[_cur]);
            break;
        case STEPARATOR:
            std::cout << "CRITICAL STEPARATION\n" << std::flush;
            break;
        }
        ++_flowit;
    }
    return 0;
}

int Algorithm::StepBack()
{
    if (_flow[_flowit]._oper != STEPARATOR)
        return 1;
    if (_flowit <= 0)
        return 2;

    --_flowit;
    while(_flow[_flowit]._oper != STEPARATOR)
    {
        switch(_flow[_flowit]._oper)
        {
        case SET_I:
            _i = _flow[_flowit]._bck;
            break;
        case SET_CUR:
            _cur = _flow[_flowit]._bck;
            break;
        case SWAP:
            std::swap(_data[_i], _data[_cur]);
            break;
        case STEPARATOR:
            std::cout << "CRITICAL STEPARATION\n" << std::flush;
            break;
        }
        --_flowit;
    }
    return 0;
}

AlgoState Algorithm::GetState()
{
    return _state;
}

QPointF Algorithm::GetBotleft()
{
    return _botleft;
}

QRectF Algorithm::GetRect()
{
    return _rect;
}

qint32 Algorithm::GetStepNum()
{
    return _flow[_flowit]._fwd;
}

qint32 Algorithm::GetStepCount()
{
    return _flow[_flow.size() - 1]._fwd;
}

std::vector<QPointF>* Algorithm::GetVectorR()
{
    return &_data;
}

qint32 Algorithm::GetCur()
{
    return _cur;
}

double Algorithm::Turn(QPointF a, QPointF b, QPointF c)
{
    return (b.x() - a.x()) * (c.y() - a.y()) -
            (b.y() - a.y()) * (c.x() - a.x()) ;
}

Action::Action(Operation a, qint32 fwd, qint32 bck)
    : _oper(a)
    , _fwd(fwd)
    , _bck(bck)
{
}

Action::Action(Operation a)
    : _oper(a)
    , _fwd(0)
    , _bck(0)
{
    static qint32 i(0);
    if (_oper == STEPARATOR)
        _fwd = i++;
}
