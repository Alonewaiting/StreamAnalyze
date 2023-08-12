// CMakeProject1.cpp: 定义应用程序的入口点。
#include <QtWidgets/QApplication>
#include "StreamAnalyzeWidget.h"
int main(int argc, char* argv[]) {   
    QApplication a(argc, argv);
    StreamAnalyzeWidget sw;
    sw.show();
    return a.exec();
}
