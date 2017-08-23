#include "kstage.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    KStage w;
    w.show();

    return a.exec();

}
