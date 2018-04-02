#ifndef QTDIRECTAPPLICATION_H
#define QTDIRECTAPPLICATION_H
#include <QCoreApplication>

class QtDirectScreen;
class QtDirectApplication : public QCoreApplication
{
    Q_OBJECT
public:
    QtDirectApplication(int &argc, char **argv);
    ~QtDirectApplication();
    bool event(QEvent * e);
    static int exec(QtDirectScreen * screen);
    static void exit(int returnCode = 0);
private:
};

#endif // QTDIRECTAPPLICATION_H
