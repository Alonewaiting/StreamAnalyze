// CMakeProject1.cpp: ����Ӧ�ó������ڵ㡣
#include <QtWidgets/QApplication>
#include "StreamAnalyzeWidget.h"
int main(int argc, char* argv[]) {   
    QApplication a(argc, argv);
    StreamAnalyzeWidget sw;
    sw.show();
    return a.exec();
}
