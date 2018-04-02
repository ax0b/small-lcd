#include "qtdirectupdateevent.h"

static QEvent::Type mEventType = QEvent::None;

QtDirectUpdateEvent::QtDirectUpdateEvent() : QEvent(QtDirectUpdateEvent::type())
{

}

QtDirectUpdateEvent::~QtDirectUpdateEvent()
{

}

QEvent::Type QtDirectUpdateEvent::type()
{
    if (mEventType == QEvent::None)
    {
        mEventType = static_cast<QEvent::Type>(QEvent::registerEventType());
    }
    return mEventType;
}


