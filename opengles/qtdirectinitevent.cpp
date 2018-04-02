#include "qtdirectinitevent.h"

static QEvent::Type mEventType = QEvent::None;

QtDirectInitEvent::QtDirectInitEvent() : QEvent(QtDirectInitEvent::type())
{

}

QtDirectInitEvent::~QtDirectInitEvent()
{

}

QEvent::Type QtDirectInitEvent::type()
{
    if (mEventType == QEvent::None)
    {
        mEventType = static_cast<QEvent::Type>(QEvent::registerEventType());
    }
    return mEventType;
}
