#include "StreamAnalyzeWidget.h"
#include "FileStream.h"
#include "NALParse.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <queue>
#include <sstream>
#define DEFUAL_NALU_SIZE 100
#define SETTINGS_NAME "settings.ini"
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
    connect(this->ui.pushButtonStop ,&QPushButton::clicked, this , &StreamAnalyzeWidget::stopButtonClick);
    connect(this->ui.pushButtonPlayer, &QPushButton::clicked, this, &StreamAnalyzeWidget::playButtonClick);
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
    videoinfo_t info;
    m_nalParser->getVideoInfo(&info);
    //show info.
    if (info.type.get())
    {
        //// profile类型
        //switch (info.profile_idc.get())
        //{
        //case PROFILE_NONE:
        //    strProfileInfo.Format(_T("None"));
        //    break;
        //case PROFILE_MAIN:
        //    strProfileInfo.Format(_T("Main"));
        //    break;
        //case PROFILE_MAIN10:
        //    strProfileInfo.Format(_T("Main10"));
        //    break;
        //case PROFILE_MAINSTILLPICTURE:
        //    strProfileInfo.Format(_T("Main Still Picture"));
        //    break;
        //case PROFILE_MAINREXT:
        //    strProfileInfo.Format(_T("Main RExt"));
        //    break;
        //case PROFILE_HIGHTHROUGHPUTREXT:
        //    strProfileInfo.Format(_T("High Throughput RExt"));
        //    break;
        //default:
        //    strProfileInfo.Format(_T("Unkown"));
        //    break;
        //}
        //switch (m_cVideoInfo.level_idc)
        //{
        //case LEVEL_NONE:
        //    strLevelInfo.Format(_T("none(%d)"), LEVEL_NONE);
        //    break;
        //case LEVEL1:
        //    strLevelInfo.Format(_T("1(%d)"), LEVEL1);
        //    break;
        //case LEVEL2:
        //    strLevelInfo.Format(_T("2(%d)"), LEVEL2);
        //    break;
        //case LEVEL2_1:
        //    strLevelInfo.Format(_T("2.1(%d)"), LEVEL2_1);
        //    break;
        //case LEVEL3:
        //    strLevelInfo.Format(_T("3(%d)"), LEVEL3);
        //    break;
        //case LEVEL3_1:
        //    strLevelInfo.Format(_T("3.1(%d)"), LEVEL3_1);
        //    break;
        //case LEVEL4:
        //    strLevelInfo.Format(_T("4(%d)"), LEVEL4);
        //    break;
        //case LEVEL4_1:
        //    strLevelInfo.Format(_T("4.1(%d)"), LEVEL4_1);
        //    break;
        //case LEVEL5:
        //    strLevelInfo.Format(_T("5(%d)"), LEVEL5);
        //    break;
        //case LEVEL5_1:
        //    strLevelInfo.Format(_T("5.1(%d)"), LEVEL5_1);
        //    break;
        //case LEVEL5_2:
        //    strLevelInfo.Format(_T("5.2(%d)"), LEVEL5_2);
        //    break;
        //case LEVEL6:
        //    strLevelInfo.Format(_T("6(%d)"), LEVEL6);
        //    break;
        //case LEVEL6_1:
        //    strLevelInfo.Format(_T("6.1(%d)"), LEVEL6_1);
        //    break;
        //case LEVEL6_2:
        //    strLevelInfo.Format(_T("6.2(%d)"), LEVEL6_2);
        //    break;
        //case LEVEL8_5:
        //    strLevelInfo.Format(_T("8.5(%d)"), LEVEL8_5);
        //    break;
        //default:
        //    strLevelInfo.Format(_T("Unkown"));
        //    break;
        //}
        //switch (m_cVideoInfo.tier_idc)
        //{
        //case 1:
        //    strTierInfo.Format(_T("Tier High"));
        //    break;
        //case 0:
        //default:
        //    strTierInfo.Format(_T("Tier Main"));
        //    break;
        //}
    }
    else // h264
    {
        // profile类型
        switch (info.profile_idc.get())
        {
        case 66:
            m_strProfileInfo = "Baseline";
            break;
        case 77:
            m_strProfileInfo ="Main";
            break;
        case 88:
            m_strProfileInfo = "Extended";
            break;
        case 100:
            m_strProfileInfo = "High";
            break;
        case 110:
            m_strProfileInfo = "High 10";
            break;
        case 122:
            m_strProfileInfo = "High 422";
            break;
        case 144:
            m_strProfileInfo = "High 444";
            break;
        default:
            m_strProfileInfo = "Unkown";
            break;
        }
        m_strTierInfo.clear();
        m_strTierInfo = QString("%1").arg(info.level_idc.get());
    }
    // common
    // bit depth
    m_strBitDepth= QString("Luma bit: %1 \r\n Chroma bit: %2").arg(info.bit_depth_luma.get()).arg(info.bit_depth_chroma.get());

    // chroma format
    const QString str[] = { QString("monochrome"), QString("YUV420") ,QString("YUV422") ,QString("YUV444") };
    if (info.chroma_format_idc.get() < 0 || info.chroma_format_idc.get() > 3) {
        m_strVideoFormat = "Unkown";
    }
    else {
        m_strVideoFormat = str[info.chroma_format_idc.get()];
    }
    std::stringstream ss;
    std::string typeStr = info.type.get() ? "H.265/HEVC" : "H.264/AVC";
    std::string encodingType = info.encoding_type.get() ? "CABAC" : "CAVLC";
    ss << typeStr << "File Information\r\n\r\n" <<
    "Picture Size      \t:" << info.width.get() << "x" << info.height.get() << "\r\n" <<
    "  - Cropping Left \t: " << info.crop_left.get() << "\r\n" <<
    "  - Cropping Right \t: " << info.crop_right.get() << "\r\n" <<
    "  - Cropping Top \t: " << info.crop_top.get() << "\r\n" <<
    "  - Cropping Bottom \t: " << info.crop_bottom.get() << "\r\n" <<
    "Video Format \t: "<< m_strVideoFormat.toStdString() << "\r\n" <<
    m_strBitDepth.toStdString() << "\r\n" <<
    "Stream Type \t: " << m_strProfileInfo.toStdString() << "Profile @ Level " << m_strLevelInfo.toStdString() << m_strTierInfo.toStdString() << "\r\n" <<
    "Encoding Type \t: "<< encodingType << "\r\n" <<
    "Max fps \t\t: " << info.max_framerate <<"\r\n" <<
    "Frame Count \t: " << "xxx" << "\r\n";
    QString showEditor = QString::fromStdString(ss.str());
    ui.textBrowserHead->setPlainText(showEditor);
    

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
    QString str = QCoreApplication::applicationDirPath();
    QString iniPath = str + "/" + SETTINGS_NAME;
    QSettings* settings = new QSettings(iniPath, QSettings::IniFormat);
    settings->beginGroup("PATH");
    QString filePath = settings->value("OpenFilePath", ".").toString();
    settings->endGroup();
     m_filePath = QFileDialog::getOpenFileName(
        this,
        u8"Open File",
        filePath,
        "h2645 files(*.h26*);;"
        "mov files(*.mov);;"
        "mp4 files(*.mp4);;"
        "All Files (*);;"
    );
     settings->beginGroup("PATH");
     settings->setValue("OpenFilePath", m_filePath);
     settings->endGroup();

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
void StreamAnalyzeWidget::stopButtonClick() {
    QMessageBox::warning(this, "warning", "Don't click me, I Just a usless button, SB!", QMessageBox::Ok);
    return;
}
void StreamAnalyzeWidget::playButtonClick() {
    QMessageBox::warning(this, "warning", "Don't click me, I Just a usless button, SB!", QMessageBox::Ok);
    return;
}
