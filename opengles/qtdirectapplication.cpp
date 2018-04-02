#include "qtdirectapplication.h"
#include "qtdirectupdateevent.h"
#include "qtdirectinitevent.h"
#include "qtdirectscreen.h"


static QtDirectScreen * mScreen = 0;
static bool initialized = false;

QtDirectApplication::QtDirectApplication(int &argc, char **argv) : QCoreApplication(argc, argv)
{

}

QtDirectApplication::~QtDirectApplication()
{

}

bool QtDirectApplication::event(QEvent *e)
{
    if (e->type() == QtDirectUpdateEvent::type()) {
        if (mScreen != 0)
            mScreen->update();
        postEvent(QtDirectApplication::instance(), new QtDirectUpdateEvent());
        return true;
    }
    if (e->type() == QtDirectInitEvent::type()) {
        if ((mScreen != 0)&&(!initialized)) {
            mScreen->init();
            initialized = false;
        }
        postEvent(QtDirectApplication::instance(), new QtDirectUpdateEvent());
    }
    return false;
}

int QtDirectApplication::exec(QtDirectScreen *screen)
{
    mScreen = screen;
    postEvent(QtDirectApplication::instance(), new QtDirectInitEvent());
    return QCoreApplication::exec();
}

void QtDirectApplication::exit(int returnCode)
{
    if (mScreen != 0)
        mScreen->clear();
    mScreen = 0;
    QCoreApplication::exit(returnCode);
}
