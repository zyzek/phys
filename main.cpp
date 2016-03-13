#include "dialog.h"
#include <QApplication>

#include <cosmogony.h>

int main(int argc, char *argv[])
{
    parsing_main();

    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
