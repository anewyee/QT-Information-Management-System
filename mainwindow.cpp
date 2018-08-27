/*
###############################################################################
#                                                                             #
# The MIT License                                                             #
#                                                                             #
# Copyright (c) 2018 Peng Yuxing(pyuxing@163.com)                             #
# >> https://github.com/pyuxing                                               #
#                                                                             #
###############################################################################
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QPalette>
#include <QTimer>
#include <qdatetime.h>
#include <QTextCodec>
#include <QMessageBox>
#include "mymessagebox.h"
#include <QFile>
#include<QDesktopServices>
#include<QUrl>
#include <connection.h>
#include <QDir>
#include <QFileDialog>
#include <QSqlRecord>
#include <QScreen>
#include <QPixmap>
#include "xlsxdocument.h"
#include <QMenu>

unsigned int myProgressValue;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //time update
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);
    //initial UI
    on_btn_Setting_pressed();
    ui->toolbtn_scholar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->toolbtn_stipend->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->frameInfoDetail->setVisible(false);
    btnExportExcelsetting();
    //read user's data
    if(!readUserData("userData.db")){
        qDebug()<<"Cannot read userData";
    }
    setWindowIcon(QIcon(":/images/icon_window_close.png"));
}

void MainWindow::timerUpdate(void)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd\nhh:mm:ss\n");
    ui->timer->setText(str);
    ui->progressBar->setValue(myProgressValue);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/***Function: search student data from sqlite*/
void MainWindow::on_btnSearch_clicked()
{
    QString strstuName = ui->lEstuName->text();
    QString stridNum = ui->lEidNum->text();
    QString strstuID = ui->lEstuID->text();
    QString strstuCollege = ui->lEstuCollege->text();
    QString strstuClass = ui->lEstuClass->text();
    QString strprojName = ui->lEprojName->text();
    QString strTableName;
    QObject *ptrTableviewInfo;
    qDebug()<<enumStartMenu;
    if(enumStartMenu == scholarship){
        ptrTableviewInfo = qobject_cast<QObject*>(ui->tableviewScholarshipInfo);
        strTableName = scholarshipTable;
    }
    else if(enumStartMenu == stipend){
        ptrTableviewInfo = qobject_cast<QObject*>(ui->tableviewStipendInfo);
        strTableName = stipendTable;
    }
    if(!strstuName.isEmpty()){
        searchData(ptrTableviewInfo,"stuName",strstuName,strTableName);//Query based on student name
    }
    else if(!stridNum.isEmpty()){
        searchData(ptrTableviewInfo,"idNum",stridNum,strTableName);//Query based on student IDCARD number
    }
    else if(!strstuID.isEmpty()){
        searchData(ptrTableviewInfo,"stuID",strstuID,strTableName);
    }
    else if(!strstuCollege.isEmpty()){
        searchData(ptrTableviewInfo,"stuCollege",strstuCollege,strTableName);
    }
    else if(!strstuClass.isEmpty()){
        searchData(ptrTableviewInfo,"stuClass",strstuClass,strTableName);
    }
    else if(!strprojName.isEmpty()){
        searchData(ptrTableviewInfo,"projName",strprojName,strTableName);
    }
    else{
        searchData(ptrTableviewInfo,"TABLE","NO SEARCH",strTableName);//Did not enter the query content
    }
}

/***Function：filter query data*/
bool MainWindow::searchData(QObject *ptrTableviewInfo,QString strHeadItem,QString strExactItem,QString strTableName){
    QSqlQueryModel *model = new QSqlQueryModel(ptrTableviewInfo);
    if((strHeadItem != "TABLE") && (strExactItem != "NO SEARCH")){
        model->setQuery(QString("select * from " + strTableName + " where %1 = '%2'").arg(strHeadItem).arg(strExactItem));
    }

    else{//Did not enter the query content
        MyMessageBox::showMyMessageBox(this, QStringLiteral("Query Reminder"), QStringLiteral("No data to be queried in the database!"), MESSAGE_WARNNING, BUTTON_OK_AND_CANCEL, true);
    }

    model->setHeaderData(0,Qt::Horizontal,"headstuName");

    setTableHead(model);

    QTableView *ptrTableview = qobject_cast<QTableView *>(ptrTableviewInfo);

    ptrTableview->setModel(model);
    if(!ptrTableview->verticalHeader()->count()){
        MyMessageBox::showMyMessageBox(this, QStringLiteral("Query Reminder"), QStringLiteral("No data to be queried in the database!"), MESSAGE_WARNNING, BUTTON_OK_AND_CANCEL, true);
    }
    return true;
}

/*Function: set headerdata of tableview*/
void MainWindow::setTableHead(QSqlQueryModel *headModel){
    QString headstuName = QString::fromLocal8Bit(" Name ");
    QString headidNum = QString::fromLocal8Bit(" ID number ");
    QString headstuID = QString::fromLocal8Bit(" StudentID ");
    QString headstuCollege = QString::fromLocal8Bit(" College ");
    QString headstuClass = QString::fromLocal8Bit(" Class ");
    if(enumStartMenu == scholarship){
        QString headprojName = QString::fromLocal8Bit(" ScholarshipName ");
        QString headprojAmount = QString::fromLocal8Bit(" ScholarshipAmount ");
        headModel->setHeaderData(5,Qt::Horizontal,headprojName);
        headModel->setHeaderData(6,Qt::Horizontal,headprojAmount);
    }
    else if(enumStartMenu == stipend){
        QString headprojName = QString::fromLocal8Bit(" StipendName ");
        QString headprojAmount = QString::fromLocal8Bit(" StipendAmount ");
        headModel->setHeaderData(5,Qt::Horizontal,headprojName);
        headModel->setHeaderData(6,Qt::Horizontal,headprojAmount);
    }

    QString headprojDate = QString::fromLocal8Bit("IssueDate");
    QString headprojIssuer = QString::fromLocal8Bit("Issuer");

    headModel->setHeaderData(0,Qt::Horizontal,headstuName);
    headModel->setHeaderData(1,Qt::Horizontal,headidNum);
    headModel->setHeaderData(2,Qt::Horizontal,headstuID);
    headModel->setHeaderData(3,Qt::Horizontal,headstuCollege);
    headModel->setHeaderData(4,Qt::Horizontal,headstuClass);
    headModel->setHeaderData(7,Qt::Horizontal,headprojDate);
    headModel->setHeaderData(8,Qt::Horizontal,headprojIssuer);
}

void MainWindow::on_btn_About_clicked()
{
    QUrl url(QString("https://github.com/pyuxing"));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_btnScholarship_clicked()
{
    connection con;
    if(!con.createConnection(scholarshipPath,"scholarship.db",scholarshipTable))
    {
        qDebug() << "Can not create connection";
        MyMessageBox::showMyMessageBox(this, QStringLiteral("Error Reminder"), QStringLiteral("Please make sure the scholarship data\n import is correct!"), MESSAGE_WARNNING, BUTTON_OK_AND_CANCEL, true);
    }

    QSqlTableModel *model;
    model = new QSqlTableModel(this);
    model->setTable(scholarshipTable);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //select the table
    ui->tableviewScholarshipInfo->setModel(model);
    setTableHead(model);
    ui->tableviewScholarshipInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);//Make it uneditable
    ui->tableviewScholarshipInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//Set the column width to automatically adjust according to the text
    ui->tableviewScholarshipInfo->setSelectionBehavior(QAbstractItemView::SelectRows); //Set the selected behavior to select the entire row
    ui->tableviewStipendInfo->setVisible(false);
}

void MainWindow::on_btn_Stipend_clicked()
{
    connection con;
    if(!con.createConnection(stipendPath,"stipend.db",stipendTable))
    {
        qDebug() << "Can not create connection";
        MyMessageBox::showMyMessageBox(this, QStringLiteral("Error Reminder"), QStringLiteral("Please make sure the scholarship data\n import is correct!"), MESSAGE_WARNNING, BUTTON_OK_AND_CANCEL, true);
    }

    QSqlTableModel *model;

    model = new QSqlTableModel(this);
    model->setTable(stipendTable);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    ui->tableviewStipendInfo->setModel(model);
    setTableHead(model);
    ui->tableviewStipendInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableviewStipendInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableviewStipendInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableviewScholarshipInfo->setVisible(false);
}

/***Function: switch left menu*/
void MainWindow::on_btn_Setting_pressed()
{
    enumStartMenu = setting;
    ui->btn_ClearALL->setVisible(true);
    ui->label_tableName->setVisible(false);
    ui->progressBar->setVisible(true);
    ui->group_settingpath->setVisible(true);
    ui->tableviewStipendInfo->setVisible(false);
    ui->tableviewScholarshipInfo->setVisible(false);
    ui->groupBoxSearch->setEnabled(false);
    ui->btnClear->setStyleSheet("color:rgb(166,166,166)");
    ui->btnSearch->setStyleSheet("color:rgb(166,166,166)");

    ui->btn_Setting->setCheckable(true);
    ui->btn_Setting->setChecked(false);
    ui->btn_Setting->setStyleSheet("background-color: rgb(85, 170, 255);color:rgb(255,255,255)");

    ui->btnScholarship->setChecked(false);
    ui->btnScholarship->setStyleSheet("background-color: rgb(255,255,255);color:rgb(0,0,0)");
    ui->btn_Stipend->setChecked(false);
    ui->btn_Stipend->setStyleSheet("background-color: rgb(255,255,255);color:rgb(0,0,0)");
}

void MainWindow::on_btnScholarship_pressed()
{

    enumStartMenu = scholarship;
    ui->btn_ClearALL->setVisible(false);
    ui->label_tableName->setVisible(true);
    ui->label_tableName->setText(QStringLiteral("Scholarship Information"));
    ui->progressBar->setVisible(false);
    ui->group_settingpath->setVisible(false);
    ui->tableviewStipendInfo->setVisible(false);
    ui->tableviewScholarshipInfo->setVisible(true);
    ui->groupBoxSearch->setEnabled(true);
    ui->btnClear->setStyleSheet("color:rgb(0,0,0)");
    ui->btnSearch->setStyleSheet("color:rgb(0,0,0)");

    ui->btnScholarship->setCheckable(true);
    ui->btnScholarship->setChecked(false);
    ui->btnScholarship->setStyleSheet("background-color: rgb(85, 170, 255);color:rgb(255,255,255)");

    ui->btn_Setting->setChecked(false);
    ui->btn_Setting->setStyleSheet("background-color: rgb(255,255,255);color:rgb(0,0,0)");
    ui->btn_Stipend->setChecked(false);
    ui->btn_Stipend->setStyleSheet("background-color: rgb(255,255,255);color:rgb(0,0,0)");
}

void MainWindow::on_btn_Stipend_pressed()
{
    enumStartMenu = stipend;
    ui->btn_ClearALL->setVisible(false);
    ui->label_tableName->setVisible(true);
    ui->label_tableName->setText(QStringLiteral("Stipend Information"));
    ui->progressBar->setVisible(false);
    ui->group_settingpath->setVisible(false);
    ui->tableviewStipendInfo->setVisible(true);
    ui->tableviewScholarshipInfo->setVisible(false);
    ui->groupBoxSearch->setEnabled(true);
    ui->btnClear->setStyleSheet("color:rgb(0,0,0)");
    ui->btnSearch->setStyleSheet("color:rgb(0,0,0)");

    ui->btn_Stipend->setCheckable(true);
    ui->btn_Stipend->setChecked(false);
    ui->btn_Stipend->setStyleSheet("background-color: rgb(85, 170, 255);color:rgb(255,255,255)");

    ui->btn_Setting->setChecked(false);
    ui->btn_Setting->setStyleSheet("background-color: rgb(255,255,255);color:rgb(0,0,0)");
    ui->btnScholarship->setChecked(false);
    ui->btnScholarship->setStyleSheet("background-color: rgb(255,255,255);color:rgb(0,0,0)");
}

///奖学金数据导入
void MainWindow::on_toolbtn_scholar_clicked()
{
    connection conSaveUserData;

    QString strDir = QCoreApplication::applicationDirPath() + QStringLiteral("\\CSV datas\\");
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkdir(strDir);
    }
    QString strFileCSVPath = strDir + "scholarship" + QDateTime::currentDateTime().toString("_yyyyMMddHHmmss") + ".CSV";
    QString directory = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("Import Scholarship Data"),QDir::currentPath(),"EXCEL(*.xlsx)"));
    if(!directory.isEmpty()){
        ui->lEscholarship->setText(directory);
        ui->progressBar->setRange(0,100);
        ui->progressBar->setValue(50);
        conSaveUserData.tranExcelToCSV(directory,strFileCSVPath);
        ui->progressBar->setValue(100);
        ui->lEscholarship->setStyleSheet("background-color: rgb(255,255,255);color: rgb(0, 85, 255);");
        scholarshipPath = strFileCSVPath;
        conSaveUserData.saveUserData(scholarshipFileType,directory,"userData.db");
        conSaveUserData.saveUserData(scholarshipCSVPath,strFileCSVPath,"userData.db");
    }
}


void MainWindow::on_toolbtn_stipend_clicked()
{
    connection conSaveUserData;

    QString strDir = QCoreApplication::applicationDirPath() + QStringLiteral("\\CSV datas\\");
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkdir(strDir);
    }
    QString strFileCSVPath = strDir + "stipend" + QDateTime::currentDateTime().toString("_yyyyMMddHHmmss") + ".CSV";
    QString directory = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("Import Stipend Data"),QDir::currentPath(),"EXCEL(*.xlsx)"));
    if(!directory.isEmpty()){
        ui->lEstipend->setText(directory);
        ui->progressBar->setRange(0,100);
        ui->progressBar->setValue(50);
        conSaveUserData.tranExcelToCSV(directory,strFileCSVPath);
        ui->progressBar->setValue(100);
        ui->lEstipend->setStyleSheet("background-color: rgb(255,255,255);color: rgb(0, 85, 255);");
        stipendPath = strFileCSVPath;
        conSaveUserData.saveUserData(stipendFileType,directory,"userData.db");
        conSaveUserData.saveUserData(stipendCSVPath,strFileCSVPath,"userData.db");
    }
}

void MainWindow::on_btnClear_clicked()
{
    ui->lEstuName->clear();
    ui->lEidNum->clear();
    ui->lEstuID->clear();
    ui->lEstuCollege->clear();
    ui->lEstuClass->clear();
    ui->lEprojName->clear();
    if(enumStartMenu == scholarship){
        on_btnScholarship_clicked();
    }
    else if(enumStartMenu == stipend){
        on_btn_Stipend_clicked();
    }
}

/***Function： display student information card*/
void MainWindow::on_tableviewScholarshipInfo_doubleClicked(const QModelIndex &index)
{
    {
        ui->btnClear->setStyleSheet("color:rgb(166,166,166)");
        ui->btnSearch->setStyleSheet("color:rgb(166,166,166)");
        ui->btn_About->setStyleSheet("color:rgb(166,166,166)");
        ui->btn_Setting->setStyleSheet("color:rgb(166,166,166)");
        if(enumStartMenu == scholarship){
            ui->btn_Stipend->setStyleSheet("color:rgb(166,166,166)");
        }
        else if(enumStartMenu == stipend){
            ui->btnScholarship->setStyleSheet("color:rgb(166,166,166)");

        }

    }
    ui->label_projNameName->setText(QStringLiteral("SchoName:"));
    ui->label_projAmountName->setText(QStringLiteral("Amount:"));
    QModelIndex current_index = ui->tableviewScholarshipInfo->currentIndex();
    QAbstractItemModel* m = (QAbstractItemModel *)current_index.model();
       for(int columnIndex = 0;columnIndex < m->columnCount();columnIndex++){
           QModelIndex x = m->index(current_index.row(),columnIndex);

           QString cellData = x.data().toString();
           switch (columnIndex) {
           case 0:
               ui->label_stuName->setText(cellData);
               fileStuName = cellData;
               break;
           case 1:
               ui->label_idNum->setText(cellData);
               break;
           case 2:
               ui->label_stuID->setText(cellData);
               break;
           case 3:
               ui->label_stuCollege->setText(cellData);
               break;
           case 4:
               ui->label_stuClass->setText(cellData);
               break;
           case 5:
               ui->label_projName->setText(cellData);
               break;
           case 6:
               ui->label_projAmount->setText(cellData);
               break;
           case 7:
               ui->label_projDate->setText(cellData);
               break;
           case 8:
               ui->label_projIssuer->setText(cellData);
               break;
           default:
               break;
           }
       }
    ui->tableviewScholarshipInfo->setEnabled(false);
    ui->groupBoxSearch->setEnabled(false);
    ui->frameMenu->setEnabled(false);
    ui->frameInfoDetail->setVisible(true);
}

void MainWindow::on_tableviewStipendInfo_doubleClicked(const QModelIndex &index)
{
    {
        ui->btnClear->setStyleSheet("color:rgb(166,166,166)");
        ui->btnSearch->setStyleSheet("color:rgb(166,166,166)");
        ui->btn_About->setStyleSheet("color:rgb(166,166,166)");
        ui->btn_Setting->setStyleSheet("color:rgb(166,166,166)");
        if(enumStartMenu == scholarship){
            ui->btn_Stipend->setStyleSheet("color:rgb(166,166,166)");
        }
        else if(enumStartMenu == stipend){
            ui->btnScholarship->setStyleSheet("color:rgb(166,166,166)");
        }
    }
    ui->label_projNameName->setText(QStringLiteral("StiName:"));
    ui->label_projAmountName->setText(QStringLiteral("Amount:"));
    QModelIndex current_index = ui->tableviewStipendInfo->currentIndex();
    QAbstractItemModel* m = (QAbstractItemModel *)current_index.model();
       for(int columnIndex = 0;columnIndex < m->columnCount();columnIndex++){
           QModelIndex x = m->index(current_index.row(),columnIndex);

           QString cellData = x.data().toString();
           switch (columnIndex) {
           case 0:
               ui->label_stuName->setText(cellData);
               fileStuName = cellData;
               break;
           case 1:
               ui->label_idNum->setText(cellData);
               break;
           case 2:
               ui->label_stuID->setText(cellData);
               break;
           case 3:
               ui->label_stuCollege->setText(cellData);
               break;
           case 4:
               ui->label_stuClass->setText(cellData);
               break;
           case 5:
               ui->label_projName->setText(cellData);
               break;
           case 6:
               ui->label_projAmount->setText(cellData);
               break;
           case 7:
               ui->label_projDate->setText(cellData);
               break;
           case 8:
               ui->label_projIssuer->setText(cellData);
               break;
           default:
               break;
           }
       }
    ui->tableviewStipendInfo->setEnabled(false);
    ui->frameInfoDetail->raise();
    ui->groupBoxSearch->setEnabled(false);
    ui->frameMenu->setEnabled(false);
    ui->frameInfoDetail->setVisible(true);

}

/***Function: save cars as images*/
void MainWindow::on_btn_SaveScreenShot_clicked()
{
    QString strDir = QCoreApplication::applicationDirPath() + QStringLiteral("\\StuInfoCard\\");
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkdir(strDir);
    }
    QString strFile = strDir + fileStuName + QDateTime::currentDateTime().toString("_yyyyMMddHHmmss") + ".png";
    ui->btn_confirmCloseInfoCard->setVisible(false);
    ui->btn_SaveScreenShot->setVisible(false);
    ui->line_btwSaveConfirm->setVisible(false);
    QPixmap pix = QPixmap::grabWidget(ui->frameInfoDetail);
    ui->btn_confirmCloseInfoCard->setVisible(true);
    ui->btn_SaveScreenShot->setVisible(true);
    ui->line_btwSaveConfirm->setVisible(true);
    QString fileName = QFileDialog::getSaveFileName(this,QStringLiteral("SaveAsImages"),strFile,"PNG (*.png);;BMP (*.bmp);;JPEG (*.jpg *.jpeg)");
    if (!fileName.isNull())
    {
        pix.save(fileName);
    }

}

void MainWindow::on_btn_confirmCloseInfoCard_clicked()
{
    ui->frameInfoDetail->setVisible(false);
    qDebug()<<enumStartMenu;
    if(enumStartMenu == stipend){
        ui->tableviewStipendInfo->setEnabled(true);
    }
    else if(enumStartMenu == scholarship){
        ui->tableviewScholarshipInfo->setEnabled(true);
        qDebug()<<"TRUE";
    }
    ui->groupBoxSearch->setEnabled(true);
    ui->frameMenu->setEnabled(true);
    {
        ui->btnClear->setStyleSheet("color:rgb(0,0,0)");
        ui->btnSearch->setStyleSheet("color:rgb(0,0,0)");
        ui->btn_About->setStyleSheet("color:rgb(0,0,0)");
        ui->btn_Setting->setStyleSheet("color:rgb(0,0,0)");
        if(enumStartMenu == scholarship){
            ui->btn_Stipend->setStyleSheet("color:rgb(0,0,0)");
        }
        else if(enumStartMenu == stipend){
            ui->btnScholarship->setStyleSheet("color:rgb(0,0,0)");
        }

    }
}

void MainWindow::onExportMenuTriggered(QAction *action){
    QXlsx::Document xlsx;
    xlsx.write("A1", QStringLiteral("StudentName"));
    xlsx.write("B1", QStringLiteral("IDNumber"));
    xlsx.write("C1", QStringLiteral("StudentID"));
    xlsx.write("D1", QStringLiteral("College"));
    xlsx.write("E1", QStringLiteral("Class"));
    xlsx.write("H1", QStringLiteral("IssueDate"));
    xlsx.write("I1", QStringLiteral("Issuer"));
    QString modelFileName;
    if (action == exportMenu_action_scholarship){
        xlsx.write("F1", QStringLiteral("ScholarshipName"));
        xlsx.write("G1", QStringLiteral("ScholarshipAmount"));
        QString strModelFile = QStringLiteral("\\ScholarshipModel") + ".xlsx";
        modelFileName = QFileDialog::getSaveFileName(this,QStringLiteral("ScholarshipModel"),strModelFile,"EXCEL (*.xlsx)");
    }
    else if (action == exportMenu_action_stipend){
        xlsx.write("F1", QStringLiteral("StipendName"));
        xlsx.write("G1", QStringLiteral("StipenAmount"));
        QString strModelFile = QStringLiteral("\\StipendModel") + ".xlsx";
        modelFileName = QFileDialog::getSaveFileName(this,QStringLiteral("StipendModel"),strModelFile,"EXCEL (*.xlsx)");
    }
    xlsx.setDocumentProperty("title", QStringLiteral("ExcelModel"));
    xlsx.setDocumentProperty("creator", QStringLiteral("https://github.com/pyuxing"));
    if (!modelFileName.isNull())
    {
        xlsx.saveAs(modelFileName);
    }
}


void MainWindow::on_btnExportExcel_clicked()
{

}

/***Function: export excel model*/
void MainWindow::btnExportExcelsetting(void){
    connect(exportMenu,SIGNAL(triggered(QAction *)),this,SLOT(onExportMenuTriggered(QAction *)));
    exportMenu->addAction(exportMenu_action_scholarship);
    exportMenu->addAction(exportMenu_action_stipend);

    exportMenu_action_scholarship->setText(QStringLiteral("Export Scholarship model"));
    exportMenu_action_stipend->setText(QStringLiteral("Export Stipend model"));

    exportMenu->setStyleSheet("font: 8pt ,'Microsoft YaHei'");

    ui->btnExportExcel->setMenu(exportMenu);

}

/***Function： read data from user's db*/
bool MainWindow::readUserData(QString userDataPath){
    QString conName;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(userDataPath);
        if(!db.open())
        {
            QMessageBox::critical(0 , "Can not open database",
                                      "Unable to establish a database connection.",QMessageBox::Cancel);
            qDebug()<<"FALSE";
            return false;
        }
        QSqlQuery query;
        query.exec("select * from userScholarShipCSVData ");
        while (query.next()) {
            if(query.value(0).toBool()){//not empty
                scholarshipPath = query.value(0).toString();
            }
        }
        query.exec("select * from userStipendCSVData ");
        while (query.next()) {
            if(query.value(0).toBool()){
                stipendPath = query.value(0).toString();
            }
        }

        query.exec("select * from userScholarShipData ");
        while (query.next()) {
            if(query.value(0).toBool()){
                ui->lEscholarship->setText(query.value(0).toString());
                ui->lEscholarship->setStyleSheet("background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);");
            }
        }

        query.exec("select * from userStipendData ");
        while (query.next()) {
            if(query.value(0).toBool()){
                ui->lEstipend->setText(query.value(0).toString());
                ui->lEstipend->setStyleSheet("background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);");
            }
        }
        conName = db.connectionName();
        query.clear();
        db.close();
    }
    QSqlDatabase::removeDatabase(conName);
    if((scholarshipPath.size()) && (stipendPath.size())){
        on_btnScholarship_pressed();
        on_btnScholarship_clicked();
    }
    return true;

}

void MainWindow::on_btn_ClearALL_clicked()
{
    if(QMessageBox::warning(NULL, QStringLiteral("Clear Database Data"), QStringLiteral("Are you sure you want to clear all data from the system?"), QStringLiteral("Cancel"), QStringLiteral("Confirm"))){
        QString conName;
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("userData.db");
            if(!db.open()){
                QMessageBox::critical(0 , "Can not open database",
                                          "Unable to establish a database connection.",QMessageBox::Cancel);
            }
            QSqlQuery query;
            query.exec("DELETE FROM userScholarShipData");//delete all data
            query.exec("DELETE FROM userScholarShipCSVData");
            query.exec("DELETE FROM userStipendData");
            query.exec("DELETE FROM userStipendCSVData");
            query.clear();
            db.close();
        }
        QSqlDatabase::removeDatabase(conName);

        QString conName_scholar;
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("scholarship.db");
            if(!db.open()){
                QMessageBox::critical(0 , "Can not open database",
                                          "Unable to establish a database connection.",QMessageBox::Cancel);
            }
            QSqlQuery query;
            query.exec("DELETE FROM scholarshipTable");
            query.clear();
            db.close();
        }
        QSqlDatabase::removeDatabase(conName_scholar);

        QString conName_stipend;
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("scholarship.db");
            if(!db.open()){
                QMessageBox::critical(0 , "Can not open database",
                                          "Unable to establish a database connection.",QMessageBox::Cancel);
            }
            QSqlQuery query;
            query.exec("DELETE FROM stipendTable");
            query.clear();
            db.close();
        }
        QSqlDatabase::removeDatabase(conName_stipend);
      }
}
