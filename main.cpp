#include "recordwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RecordWidget w;
    w.show();

    return a.exec();
}
