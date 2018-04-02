#ifndef QTDIRECTUPDATEEVENT_H
#define QTDIRECTUPDATEEVENT_H
#include <QEvent>

class QtDirectUpdateEvent : public QEvent
{
public:
    QtDirectUpdateEvent();
    ~QtDirectUpdateEvent();
    static QEvent::Type type();

};

#endif // QTDIRECTUPDATEEVENT_H
