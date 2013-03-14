#include "drawwidget.h"

QRectF ToSquare(QRectF a)
{
    QRectF ans = a;
//    PrintQRect(a);
    if (ans.width() > ans.height())
        ans.adjust(0, (ans.height() - ans.width()) / 2,
                   0, (ans.width() - ans.height()) / 2);
    else
        ans.adjust((ans.width() - ans.height()) / 2, 0,
                   (ans.height() - ans.width()) / 2, 0);
    return ans;
}

QRectF Adjust(QRectF a)
{
    return a.adjusted(a.width() * -0.05, a.height() * -0.05,
                      a.width() * 0.05, a.height() * 0.05);
}

void PrintQRect(QRectF a)
{
    std::cout << a.topLeft().x() << ' ' << a.topLeft().y() << std::endl;
    std::cout << a.bottomRight().x() << ' ' << a.bottomRight().y() << std::endl;
}

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(401, 401);
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);


    painter.setFont(QFont("Arial", 10));

    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::black);

    //Draw boundary
    QRect rect = this->rect();
    rect.setBottomRight(rect.bottomRight() - QPoint(1,1));
    painter.drawRect(rect);

    std::vector<QPointF>* vec = _algo->GetVectorR();
    QPointF p = _algo->GetBotleft();
    qint32 n = _algo->GetCur();
    //painter.setRenderHint(QPainter::Antialiasing);

    QRectF r = Adjust(ToSquare(_algo->GetRect()));
//    painter.drawRect(_algo->GetRect());
//    painter.drawRect(r);

    switch(_algo->GetState())
    {
    case INIT:
    case BUILD:
        for (int i = 0; i < vec->size(); ++i)
        {
            QPointF t = PointInRect(r, (*vec)[i]);
            painter.drawEllipse(t, 3, 3);
            painter.drawPoint(t);

        }
        break;
    case STEP:
        QString s;
        QTextStream st(&s);
        st << "Step " << _algo->GetStepNum() << "/" << _algo->GetStepCount();
        painter.drawText(this->rect(), Qt::AlignHCenter | Qt::AlignTop, s);

        painter.setPen(Qt::red);
        for (int i = 0; i < vec->size(); ++i)
        {
            QPointF t = PointInRect(r, (*vec)[i] + p);
            QPointF t1 = PointInRect(r, (*vec)[i + 1] + p);
            QPointF t0 = PointInRect(r, (*vec)[0] + p);

            if (i < n)
                painter.drawLine(t, t1);
            if (i == n)
                painter.drawLine(t, t0);

            if (i > n)
                painter.setPen(Qt::black);
            painter.drawEllipse(t, 3, 3);
            painter.drawPoint(t);

        }
        painter.setPen(Qt::black);
        break;
    }
}

void DrawWidget::setAlgo(Algorithm *a)
{
    _algo = a;
}

QPointF DrawWidget::PointInRect(QRectF r, QPointF p)
{
    QPointF ans;
    ans.setX(this->width() * (p.x() - r.topLeft().x()) / r.width());
    ans.setY(this->height() * (1 - (p.y() - r.topLeft().y()) / r.height()));
    return ans;
}
