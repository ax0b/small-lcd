#include "qtdirectapplication.h"
#include "myscreen.h"

int main(int argc, char *argv[])
{
    QtDirectApplication a(argc, argv);

    return a.exec(new MyScreen());
}
