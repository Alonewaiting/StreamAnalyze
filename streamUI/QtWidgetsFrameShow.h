#pragma once

#include <QMainWindow>
#include "ui_QtWidgetsFrameShow.h"

class QtWidgetsFrameShow : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsFrameShow(QWidget *parent = nullptr);
    ~QtWidgetsFrameShow();

private:
    Ui::QtWidgetsFrameShowClass ui;
};
