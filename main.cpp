#include "recordwidget.h"
#include "autosizewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RecordWidget w;
//    AutoSizeWidget w;
    w.show();

    return a.exec();
}
