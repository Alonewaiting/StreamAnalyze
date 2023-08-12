#include "StreamAnalyzeWidget.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QFileDialog>
#include "FileStream.h"
#define DEFUAL_NALU_SIZE 100

enum 
{
    TABLELINE_NUM=0,
    TABLELINE_OFFSET = 1,
    TABLELINE_LEN,
    TABLELINE_START_CODE,
    TABLELINE_NLU_TYPE,
    TABLELINE_INFO
};
StreamAnalyzeWidget::StreamAnalyzeWidget(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_model = new QStandardItemModel(this);
    m_model->setHorizontalHeaderItem(TABLELINE_NUM, new QStandardItem("No."));
    m_model->setHorizontalHeaderItem(TABLELINE_OFFSET, new QStandardItem("Offset"));
    m_model->setHorizontalHeaderItem(TABLELINE_LEN, new QStandardItem("Length"));
    m_model->setHorizontalHeaderItem(TABLELINE_START_CODE, new QStandardItem("Start Code"));
    m_model->setHorizontalHeaderItem(TABLELINE_NLU_TYPE, new QStandardItem("NALU Type"));
    m_model->setHorizontalHeaderItem(TABLELINE_INFO, new QStandardItem("Info"));
    this->ui.lineEditLength->setText(QString::number(DEFUAL_NALU_SIZE));
    this->ui.tableViewNALU->setModel(m_model);
    connect(this->ui.actionopen,&QAction::triggered,this, &StreamAnalyzeWidget::openFile);
}

StreamAnalyzeWidget::~StreamAnalyzeWidget()
{}
void StreamAnalyzeWidget::fillNALUTable(int size){
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "please select right file", "warning", QMessageBox::Ok | QMessageBox::Cancel);
        return;
    }
    m_fstream = std::make_shared<FileStream>(m_filePath.toStdString());
    int64_t offset = 0;

    while (size--)
    {
        auto nalUnit = m_fstream->getNextNALUnit();
        auto nalData = nalUnit.getNALUnit();
        auto startCode = nalUnit.getStartCode();
        auto streamType = nalUnit.getStreamType();
        startCode.push_back(nalData[0]);
        auto nalTypeName = QString::fromStdString(nalUnit.getNALUTypeName());
        switch (streamType)
        {
        case STREAM_TYPE::STREAM_NONE:{
                
            }
            break;
        case STREAM_TYPE::STREAM_H264:{
                this->ui.comboBoxStreamType->setCurrentIndex(1);
            }
            break;
        case STREAM_TYPE::STREAM_H265:{
                this->ui.comboBoxStreamType->setCurrentIndex(0);
            }
            break;
        default:
            break;
        }



        addTableData(offset,nalUnit.getNALUnitSize(), startCode , nalTypeName,"");
        offset += nalUnit.getNALUnitSize();

    }




}
void StreamAnalyzeWidget::addTableData(int64_t offset, int64_t length,const std::vector<uint8_t>& startCode,const QString& NALType,const QString& info)
{
    int iCurLine = m_model->rowCount();
    m_model->setItem(iCurLine, TABLELINE_NUM, new QStandardItem(QString::number(iCurLine)));
    m_model->item(iCurLine, TABLELINE_NUM)->setTextAlignment(Qt::AlignCenter);
    QString str = QString("%1").arg(offset, 8, 16, QLatin1Char('0')).toUpper();
    m_model->setItem(iCurLine, TABLELINE_OFFSET, new QStandardItem(str));
    m_model->item(iCurLine, TABLELINE_OFFSET)->setTextAlignment(Qt::AlignCenter);

    m_model->setItem(iCurLine, TABLELINE_LEN, new QStandardItem(QString::number(length)));
    m_model->item(iCurLine, TABLELINE_LEN)->setTextAlignment(Qt::AlignCenter);
    QString strStartCode;
    for (const auto& num : startCode) {
        strStartCode += QString("%1").arg(num, 2, 16, QLatin1Char('0')).toUpper();
    }


    m_model->setItem(iCurLine, TABLELINE_START_CODE, new QStandardItem(strStartCode));
    m_model->item(iCurLine, TABLELINE_START_CODE)->setTextAlignment(Qt::AlignCenter);

    m_model->setItem(iCurLine, TABLELINE_NLU_TYPE, new QStandardItem(NALType));
    m_model->item(iCurLine, TABLELINE_NLU_TYPE)->setTextAlignment(Qt::AlignCenter);

    m_model->setItem(iCurLine, TABLELINE_INFO, new QStandardItem(info));
    m_model->item(iCurLine, TABLELINE_INFO)->setTextAlignment(Qt::AlignCenter);



}
void StreamAnalyzeWidget::openFile() {
     m_filePath = QFileDialog::getOpenFileName(
        this,
        u8"Open File",
        ".",
        "h265 files(*.h265);;"
        "h264 files(*.h264);;"
        "mov files(*.mov);;"
        "mp4 files(*.mp4);;"
    );
    this->ui.lineEditFilePath->setText(m_filePath);
    int size =  this->ui.lineEditLength->text().toInt();
    fillNALUTable(size);
}
