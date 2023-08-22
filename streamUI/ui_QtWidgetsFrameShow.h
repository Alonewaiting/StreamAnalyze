/********************************************************************************
** Form generated from reading UI file 'QtWidgetsFrameShow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTWIDGETSFRAMESHOW_H
#define UI_QTWIDGETSFRAMESHOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QColumnView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWidgetsFrameShowClass
{
public:
    QAction *actionSave_to_mp4;
    QAction *actionSave_to_mov;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QColumnView *columnView;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtWidgetsFrameShowClass)
    {
        if (QtWidgetsFrameShowClass->objectName().isEmpty())
            QtWidgetsFrameShowClass->setObjectName("QtWidgetsFrameShowClass");
        QtWidgetsFrameShowClass->resize(1075, 789);
        actionSave_to_mp4 = new QAction(QtWidgetsFrameShowClass);
        actionSave_to_mp4->setObjectName("actionSave_to_mp4");
        actionSave_to_mov = new QAction(QtWidgetsFrameShowClass);
        actionSave_to_mov->setObjectName("actionSave_to_mov");
        centralWidget = new QWidget(QtWidgetsFrameShowClass);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        columnView = new QColumnView(centralWidget);
        columnView->setObjectName("columnView");

        verticalLayout->addWidget(columnView);

        widget = new QWidget(centralWidget);
        widget->setObjectName("widget");

        verticalLayout->addWidget(widget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName("pushButton_3");

        horizontalLayout->addWidget(pushButton_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 4);
        QtWidgetsFrameShowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtWidgetsFrameShowClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1075, 29));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        QtWidgetsFrameShowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtWidgetsFrameShowClass);
        mainToolBar->setObjectName("mainToolBar");
        QtWidgetsFrameShowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtWidgetsFrameShowClass);
        statusBar->setObjectName("statusBar");
        QtWidgetsFrameShowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionSave_to_mp4);
        menuFile->addAction(actionSave_to_mov);

        retranslateUi(QtWidgetsFrameShowClass);

        QMetaObject::connectSlotsByName(QtWidgetsFrameShowClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtWidgetsFrameShowClass)
    {
        QtWidgetsFrameShowClass->setWindowTitle(QCoreApplication::translate("QtWidgetsFrameShowClass", "QtWidgetsFrameShow", nullptr));
        actionSave_to_mp4->setText(QCoreApplication::translate("QtWidgetsFrameShowClass", "Save to mp4", nullptr));
        actionSave_to_mov->setText(QCoreApplication::translate("QtWidgetsFrameShowClass", "Save to mov", nullptr));
        pushButton->setText(QCoreApplication::translate("QtWidgetsFrameShowClass", "PushButton", nullptr));
        pushButton_2->setText(QCoreApplication::translate("QtWidgetsFrameShowClass", "PushButton", nullptr));
        pushButton_3->setText(QCoreApplication::translate("QtWidgetsFrameShowClass", "PushButton", nullptr));
        menuFile->setTitle(QCoreApplication::translate("QtWidgetsFrameShowClass", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtWidgetsFrameShowClass: public Ui_QtWidgetsFrameShowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTWIDGETSFRAMESHOW_H
