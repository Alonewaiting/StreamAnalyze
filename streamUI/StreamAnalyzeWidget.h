#pragma once

#include <QMainWindow>
#include <QString>
#include <vector>
#include "NALUnit.h"
#include "ui_StreamAnalyzeWidget.h"
class FileStream;
class QStandardItemModel;
class NALParser;
template<class T>
class TreeList;
struct ParameterDescription;
class StreamAnalyzeWidget : public QMainWindow
{
    Q_OBJECT

public:
    StreamAnalyzeWidget(QWidget *parent = nullptr);
    ~StreamAnalyzeWidget();


private slots:
    void openFile();
    void lineClickedParser(const QModelIndex& index);

private:
    void fillNALUTable(int size);
    void addTableData(int64_t offset, int64_t length, const std::vector<uint8_t>& startCode,const QString& NALType,const QString &info);
    void parserNal(const NALUnit& nal);
    void showNALUTree(const  std::shared_ptr<TreeList<ParameterDescription>> & tree);

private:
    Ui::StreamAnalyzeWidgetClass ui;
    std::shared_ptr<FileStream> m_fstream;
    std::vector<NALUnit> m_NALUnits;
    std::shared_ptr<NALParser> m_nalParser;
    QStandardItemModel* m_model;
    QString m_filePath;
};
