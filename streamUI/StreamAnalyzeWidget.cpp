#include "StreamAnalyzeWidget.h"
#include "FileStream.h"
#include "NALParse.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QFileDialog>
#include <queue>
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
    m_model = new QStandardItemModel(this->ui.tableViewNALU);
    m_model->setHorizontalHeaderItem(TABLELINE_NUM, new QStandardItem("No."));
    m_model->setHorizontalHeaderItem(TABLELINE_OFFSET, new QStandardItem("Offset"));
    m_model->setHorizontalHeaderItem(TABLELINE_LEN, new QStandardItem("Length"));
    m_model->setHorizontalHeaderItem(TABLELINE_START_CODE, new QStandardItem("Start Code"));
    m_model->setHorizontalHeaderItem(TABLELINE_NLU_TYPE, new QStandardItem("NALU Type"));
    m_model->setHorizontalHeaderItem(TABLELINE_INFO, new QStandardItem("Info"));
    this->ui.lineEditLength->setText(QString::number(DEFUAL_NALU_SIZE));
    this->ui.tableViewNALU->setModel(m_model);
    this->ui.tableViewNALU->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableViewNALU->horizontalHeader()->setStretchLastSection(true);
    ui.tableViewNALU->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(this->ui.actionopen,&QAction::triggered,this, &StreamAnalyzeWidget::openFile);
    connect(this->ui.tableViewNALU,&QTableView::clicked,this , &StreamAnalyzeWidget::lineClickedParser);
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
        m_NALUnits.push_back(nalUnit);
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
void StreamAnalyzeWidget::parserNal(const NALUnit& nal){
    if (nal.isNULL()) {
        return;
    }
    auto type = nal.getStreamType();
    if (!m_nalParser) {
        m_nalParser = std::make_shared<NALParser>(type);
    }
    m_nalParser->parseNALU(nal);
    auto tree = m_nalParser->getRootTree();
    if (!tree) {
        return;
    }
    showNALUTree(tree);
    showNALUBits(nal);
}
void StreamAnalyzeWidget::showNALUTree(const std::shared_ptr<TreeList<ParameterDescription>>& tree){
    //BFS
    std::deque<std::shared_ptr<TreeNode<ParameterDescription>>> treeList;
    std::deque<int> treeLayer;
    auto treeRoot = tree->getRoot();
    treeList.push_back(tree->getRoot());
    int layer = 0;
    int currentLayer = 0;
    treeLayer.push_back(layer);

    QStandardItemModel* model = new QStandardItemModel(ui.treeViewH26X);

    model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Parameter") << QStringLiteral("Value"));
    //root
    QStandardItem* rootItem = new QStandardItem(QString::fromStdString(treeRoot->m_treeName));
    model->appendRow(rootItem);
    model->setItem(model->indexFromItem(rootItem).row(), 1, new QStandardItem(QStringLiteral("")));

    while (!treeList.empty())
    {
        auto node = treeList.front();
        auto layer = treeLayer.front();
        if (currentLayer == layer) {
            //same layer just append data.
            for (const auto& val : node->m_element.m_pareterDesciption) {
                QStandardItem* rootFolder = new QStandardItem(QString::fromStdString(val.first));
                rootItem->appendRow(rootFolder);
                rootItem->setChild(rootFolder->index().row(), 1, new QStandardItem(QStringLiteral("")));
            }
        }
        else {
            //different layer create new item
            auto preRootItem = rootItem;
            rootItem = new QStandardItem(QString::fromStdString(node->m_treeName));
            preRootItem->appendRow(rootItem);
            for (const auto& val : node->m_element.m_pareterDesciption) {
                QStandardItem* rootFolder = new QStandardItem(QString::fromStdString(val.first));
                rootItem->appendRow(rootFolder);
                rootItem->setChild(rootFolder->index().row(), 1, new QStandardItem(QStringLiteral("")));
            }
            currentLayer++;
        }

        treeList.pop_front();
        treeLayer.pop_front();
        if (!node->m_children.empty()) {
            layer++;
            for (const auto& val : node->m_children) {
                treeList.push_back(val);
                treeLayer.push_back(layer);
            }
        }
    }


    ui.treeViewH26X->setModel(model);
    ui.treeViewH26X->header()->setSectionResizeMode(QHeaderView::ResizeToContents);


}
void StreamAnalyzeWidget::showNALUBits(const NALUnit& nal) {
     QStandardItemModel* model = new QStandardItemModel(this->ui.tableViewBit);
     auto startCode = nal.getStartCode();
     auto nalUnit = nal.getNALUnit();
     auto showResult = startCode;
     showResult.insert(showResult.end(),nalUnit.begin(),nalUnit.end());
     for (int i = 0; i < showResult.size(); ++i) {
         auto show = QString("%1").arg(showResult[i], 2, 16, QLatin1Char('0')).toUpper();
         if (i % 16 == 0) {
             auto showList = QString("%1").arg(i, 8, 16, QLatin1Char('0')).toUpper();
             model->setItem(i / 16, 0, new QStandardItem(showList));
             model->item(i / 16, 0)->setTextAlignment(Qt::AlignCenter);
         }
         model->setItem(i / 16, (i % 16) + 1, new QStandardItem(show));
     }
     this->ui.tableViewBit->setModel(model);
     this->ui.tableViewBit->horizontalHeader()->setVisible(false);
     this->ui.tableViewBit->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
void StreamAnalyzeWidget::openFile() {
     m_filePath = QFileDialog::getOpenFileName(
        this,
        u8"Open File",
        ".",
        "h2645 files(*.h26*);;"
        "mov files(*.mov);;"
        "mp4 files(*.mp4);;"
        "All Files (*);;"
    );
    this->ui.lineEditFilePath->setText(m_filePath);
    int size =  this->ui.lineEditLength->text().toInt();
    fillNALUTable(size);
}
void StreamAnalyzeWidget::lineClickedParser(const QModelIndex& index) {
    auto indexNalu = index.row();
    if (indexNalu >= m_NALUnits.size()) {
        return;
    }
    parserNal(m_NALUnits[indexNalu]);
  
    return;
}
