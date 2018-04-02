#ifndef MYSCREEN_H
#define MYSCREEN_H

#include "triangle.h"
#include "qtdirectscreen.h"
#include <QObject>



class MyScreen : public QtDirectScreen 
{
    Q_OBJECT
public:
    explicit MyScreen(QObject *parent = 0);
    ~MyScreen();
signals:

public slots:
public:
    void virtual init();
    void virtual update();
    void virtual clear();
private:
    void initOpenGL();
    void resetModel();
    void initModelProj();
    void updateModel();
    void redrawScene();
    GLfloat incAndClipDistance(GLfloat distance, GLfloat distance_inc);
    GLfloat incAndWrapAngle(GLfloat angle, GLfloat angle_inc);
    bool loadTexImages();
    void initTextures();
    void onExit();
    CUBE_STATE_T * m_state;
};

#endif // MYSCREEN_H
