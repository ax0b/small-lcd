#ifndef QTDIRECTSCREEN_H
#define QTDIRECTSCREEN_H

#include <QObject>

class QtDirectScreen : public QObject 
{
    Q_OBJECT
public:
    explicit QtDirectScreen(QObject *parent = 0);
    ~QtDirectScreen();
signals:

public slots:
public:
    void virtual init();
    void virtual update();
    void virtual clear();
};

#endif // QTDIRECTSCREEN_H
