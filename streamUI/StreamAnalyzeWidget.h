#pragma once

#include <QMainWindow>
#include <QString>
#include "ui_StreamAnalyzeWidget.h"
class FileStream;
class QStandardItemModel;
class StreamAnalyzeWidget : public QMainWindow
{
    Q_OBJECT

public:
    StreamAnalyzeWidget(QWidget *parent = nullptr);
    ~StreamAnalyzeWidget();


private slots:
    void openFile();

private:
    void fillNALUTable(int size);
    void addTableData(int64_t offset, int64_t length, const std::vector<uint8_t>& startCode,const QString& NALType,const QString &info);

private:
    Ui::StreamAnalyzeWidgetClass ui;
    std::shared_ptr<FileStream> m_fstream;
    QStandardItemModel* m_model;
    QString m_filePath;
};
