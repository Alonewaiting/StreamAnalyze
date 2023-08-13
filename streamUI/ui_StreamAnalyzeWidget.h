/********************************************************************************
** Form generated from reading UI file 'StreamAnalyzeWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STREAMANALYZEWIDGET_H
#define UI_STREAMANALYZEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StreamAnalyzeWidgetClass
{
public:
    QAction *actionopen;
    QAction *actionclose;
    QAction *actionsave;
    QAction *actionquit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonPlayer;
    QPushButton *pushButtonStop;
    QLabel *label;
    QLineEdit *lineEditLength;
    QLabel *label_2;
    QLineEdit *lineEditFilePath;
    QLabel *label_3;
    QComboBox *comboBoxStreamType;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout;
    QTreeView *treeViewH26X;
    QTableView *tableViewNALU;
    QTreeView *treeViewHead;
    QHBoxLayout *horizontalLayout;
    QTableView *tableViewBit;
    QTextEdit *textEditBit;
    QTextEdit *textEditMessage;
    QMenuBar *menuBar;
    QMenu *menuMain;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *StreamAnalyzeWidgetClass)
    {
        if (StreamAnalyzeWidgetClass->objectName().isEmpty())
            StreamAnalyzeWidgetClass->setObjectName("StreamAnalyzeWidgetClass");
        StreamAnalyzeWidgetClass->resize(1236, 977);
        actionopen = new QAction(StreamAnalyzeWidgetClass);
        actionopen->setObjectName("actionopen");
        actionclose = new QAction(StreamAnalyzeWidgetClass);
        actionclose->setObjectName("actionclose");
        actionsave = new QAction(StreamAnalyzeWidgetClass);
        actionsave->setObjectName("actionsave");
        actionquit = new QAction(StreamAnalyzeWidgetClass);
        actionquit->setObjectName("actionquit");
        centralWidget = new QWidget(StreamAnalyzeWidgetClass);
        centralWidget->setObjectName("centralWidget");
        centralWidget->setEnabled(true);
        centralWidget->setLayoutDirection(Qt::LeftToRight);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        pushButtonPlayer = new QPushButton(centralWidget);
        pushButtonPlayer->setObjectName("pushButtonPlayer");

        horizontalLayout_2->addWidget(pushButtonPlayer);

        pushButtonStop = new QPushButton(centralWidget);
        pushButtonStop->setObjectName("pushButtonStop");

        horizontalLayout_2->addWidget(pushButtonStop);

        label = new QLabel(centralWidget);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        lineEditLength = new QLineEdit(centralWidget);
        lineEditLength->setObjectName("lineEditLength");

        horizontalLayout_2->addWidget(lineEditLength);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName("label_2");
        label_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_2->setLayoutDirection(Qt::RightToLeft);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        lineEditFilePath = new QLineEdit(centralWidget);
        lineEditFilePath->setObjectName("lineEditFilePath");

        horizontalLayout_2->addWidget(lineEditFilePath);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName("label_3");

        horizontalLayout_2->addWidget(label_3);

        comboBoxStreamType = new QComboBox(centralWidget);
        comboBoxStreamType->addItem(QString());
        comboBoxStreamType->addItem(QString());
        comboBoxStreamType->setObjectName("comboBoxStreamType");

        horizontalLayout_2->addWidget(comboBoxStreamType);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 1);
        horizontalLayout_2->setStretch(2, 1);
        horizontalLayout_2->setStretch(3, 1);
        horizontalLayout_2->setStretch(4, 1);
        horizontalLayout_2->setStretch(5, 2);
        horizontalLayout_2->setStretch(8, 6);

        verticalLayout->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName("gridLayout");
        treeViewH26X = new QTreeView(centralWidget);
        treeViewH26X->setObjectName("treeViewH26X");

        gridLayout->addWidget(treeViewH26X, 1, 1, 1, 1);

        tableViewNALU = new QTableView(centralWidget);
        tableViewNALU->setObjectName("tableViewNALU");
        tableViewNALU->setMinimumSize(QSize(800, 0));
        tableViewNALU->setDragEnabled(true);

        gridLayout->addWidget(tableViewNALU, 0, 0, 2, 1);

        treeViewHead = new QTreeView(centralWidget);
        treeViewHead->setObjectName("treeViewHead");

        gridLayout->addWidget(treeViewHead, 0, 1, 1, 1);

        gridLayout->setRowStretch(0, 1);
        gridLayout->setRowStretch(1, 3);
        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 3);

        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        tableViewBit = new QTableView(centralWidget);
        tableViewBit->setObjectName("tableViewBit");
        tableViewBit->setMinimumSize(QSize(700, 0));

        horizontalLayout->addWidget(tableViewBit);

        textEditBit = new QTextEdit(centralWidget);
        textEditBit->setObjectName("textEditBit");

        horizontalLayout->addWidget(textEditBit);

        textEditMessage = new QTextEdit(centralWidget);
        textEditMessage->setObjectName("textEditMessage");

        horizontalLayout->addWidget(textEditMessage);

        horizontalLayout->setStretch(0, 4);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(2, 2);

        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 10);
        verticalLayout->setStretch(2, 3);
        StreamAnalyzeWidgetClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(StreamAnalyzeWidgetClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1236, 29));
        menuMain = new QMenu(menuBar);
        menuMain->setObjectName("menuMain");
        menuView = new QMenu(menuBar);
        menuView->setObjectName("menuView");
        StreamAnalyzeWidgetClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(StreamAnalyzeWidgetClass);
        mainToolBar->setObjectName("mainToolBar");
        StreamAnalyzeWidgetClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(StreamAnalyzeWidgetClass);
        statusBar->setObjectName("statusBar");
        StreamAnalyzeWidgetClass->setStatusBar(statusBar);

        menuBar->addAction(menuMain->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuMain->addAction(actionopen);
        menuMain->addAction(actionclose);
        menuMain->addAction(actionsave);
        menuMain->addSeparator();
        menuMain->addAction(actionquit);

        retranslateUi(StreamAnalyzeWidgetClass);

        QMetaObject::connectSlotsByName(StreamAnalyzeWidgetClass);
    } // setupUi

    void retranslateUi(QMainWindow *StreamAnalyzeWidgetClass)
    {
        StreamAnalyzeWidgetClass->setWindowTitle(QCoreApplication::translate("StreamAnalyzeWidgetClass", "StreamAnalyzeWidget", nullptr));
        actionopen->setText(QCoreApplication::translate("StreamAnalyzeWidgetClass", "open", nullptr));
        actionclose->setText(QCoreApplication::translate("StreamAnalyzeWidgetClass", "close", nullptr));
        actionsave->setText(QCoreApplication::translate("StreamAnalyzeWidgetClass", "save", nullptr));
        actionquit->setText(QCoreApplication::translate("StreamAnalyzeWidgetClass", "quit", nullptr));
        pushButtonPlayer->setText(QCoreApplication::translate("StreamAnalyzeWidgetClass", "Player", nullptr));
        pushButtonStop->setText(QCoreApplication::translate("StreamAnalyzeWidgetClass", "Stop", nullptr));
        label->setText(QCoreApplication::translate("StreamAnalyzeWidgetClass", "\350\257\273\345\217\226NalUnit \351\225\277\345\272\246", nullptr));
        label_2->setText(QCoreApplication::translate("StreamAnalyzeWidgetClass", "\346\226\207\344\273\266\350\267\257\345\276\204", nullptr));
        label_3->setText(QCoreApplication::translate("StreamAnalyzeWidgetClass", "\347\240\201\346\265\201\347\261\273\345\236\213", nullptr));
        comboBoxStreamType->setItemText(0, QCoreApplication::translate("StreamAnalyzeWidgetClass", "H265", nullptr));
        comboBoxStreamType->setItemText(1, QCoreApplication::translate("StreamAnalyzeWidgetClass", "H264", nullptr));

        menuMain->setTitle(QCoreApplication::translate("StreamAnalyzeWidgetClass", "\345\274\200\345\247\213", nullptr));
        menuView->setTitle(QCoreApplication::translate("StreamAnalyzeWidgetClass", "\350\247\206\345\233\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StreamAnalyzeWidgetClass: public Ui_StreamAnalyzeWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STREAMANALYZEWIDGET_H
