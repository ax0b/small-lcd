#ifndef QTDIRECTINITEVENT_H
#define QTDIRECTINITEVENT_H

#include <QEvent>

class QtDirectInitEvent : public QEvent
{
public:
    QtDirectInitEvent();
    ~QtDirectInitEvent();
    static QEvent::Type type();
};

#endif // QTDIRECTINITEVENT_H
