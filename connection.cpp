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
#include "connection.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QDebug>
#include <mainwindow.h>
#include <QString>
#include <QAxObject>
#include "objbase.h"
#include <qdir.h>

connection::connection()
{

}

/***s: import data of CSV file to sqlite*/
bool connection::createConnection(QString inputFilepath,QString outputFilepath,QString tableName)
{
    QFile file(inputFilepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
          std::cerr << "Cannot open file for reading: "
                    << qPrintable(file.errorString()) << std::endl;
          return false;
     }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(outputFilepath);
    if(!db.open())
    {
        QMessageBox::critical(0 , "Can not open database",
                                  "Unable to establish a database connection.",QMessageBox::Cancel);
        std::cerr<<"stop!";
        return false;
    }

    QSqlQuery query;
    query.exec("create table "+ tableName + "(stuName varchar(50),idNum varchar(50),"
                                         "stuID varchar(50),stuCollege varchar(50),"
                                         "stuClass varchar(50),projName varchar(50),"
                                         "projAmount varchar(50),projDate varchar(50),"
                                         "projIssuer varchar(50), primary key(projName,projDate,idNum))");
    QStringList list;
    list.clear();
    QTextStream in(&file);  //QTextStream reading data
    while(!in.atEnd())
    {
       QString fileLine = in.readLine();//from 1st line to next
       list = fileLine.split(",", QString::SkipEmptyParts);

       qDebug()<<list.at(0)<<list.at(1);

       query.prepare("INSERT INTO " + tableName + " (stuName,idNum,stuID,stuCollege,stuClass,projName,projAmount,projDate,projIssuer)"
                                 " VALUES (:stuName,:idNum,:stuID,:stuCollege,:stuClass,:projName,:projAmount,:projDate,:projIssuer)");

       query.bindValue(":stuName", list.at(0)); //bind data
       query.bindValue(":idNum", list.at(1));
       query.bindValue(":stuID", list.at(2));
       query.bindValue(":stuCollege", list.at(3));
       query.bindValue(":stuClass", list.at(4));
       query.bindValue(":projName", list.at(5));
       query.bindValue(":projAmount", list.at(6));
       query.bindValue(":projDate", list.at(7));
       query.bindValue(":projIssuer", list.at(8));

       query.exec();
    }
    if(!db.commit()){
        qDebug()<<"ERROR commit";
    }
    query.clear();
    db.close();
    return true;
}

/***Function: save the the path of output file*/
bool  connection::saveUserData(int pathtype,QString filePathName,QString outputFilepath){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(outputFilepath);
    if(!db.open())
    {
        QMessageBox::critical(0 , "Can not open database",
                                  "Unable to establish a database connection.",QMessageBox::Cancel);
        std::cerr<<"stop!";
        return false;
    }
    QSqlQuery query;
    if(pathtype == scholarshipFileType){
        query.exec("DELETE FROM userScholarShipData");

        query.exec("create table userScholarShipData(scholarshipPath varchar primary key)");

        query.prepare("INSERT INTO userScholarShipData (scholarshipPath)"
                            " VALUES (:scholarshipPath)");
       query.bindValue(":scholarshipPath", filePathName);
       query.exec();
       query.clear();
       db.close();
    }
    else if(pathtype == stipendFileType){
        query.exec("DELETE FROM userStipendData");
        query.exec("create table userStipendData(stipendPath varchar primary key)");

        query.prepare("INSERT INTO userStipendData (stipendPath)"
                            " VALUES (:stipendPath)");
        query.bindValue(":stipendPath", filePathName);
        //qDebug()<<"stipend";
        query.exec();
        query.clear();
        db.close();
    }
    else if(pathtype == scholarshipCSVPath){
        query.exec("DELETE FROM userScholarShipCSVData");
        query.exec("create table userScholarShipCSVData(scholarshipCSVPath varchar primary key)");

        query.prepare("INSERT INTO userScholarShipCSVData (scholarshipCSVPath)"
                            " VALUES (:scholarshipCSVPath)");
        query.bindValue(":scholarshipCSVPath", filePathName);
        query.exec();
        query.clear();
        db.close();
    }else if(pathtype == stipendCSVPath){
        query.exec("DELETE FROM userStipendCSVData");
        query.exec("create table userStipendCSVData(stipendCSVPath varchar primary key)");

        query.prepare("INSERT INTO userStipendCSVData (stipendCSVPath)"
                            " VALUES (:stipendCSVPath)");
        query.bindValue(":stipendCSVPath", filePathName);
        query.clear();
        db.close();
    }
    else{
        QMessageBox::critical(0 , "Can not save your DATA",
                                  "Unable to establish a database connection.",QMessageBox::Cancel);
        std::cerr<<"stop!";
        return false;
    }

}

/***Function: transfer excel file to CSV*/
bool connection::tranExcelToCSV(QString InputExcel,QString OutputCSV){

    //Using QAxObject in a background thread must be initialized first
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false); //NO show
    excel.setProperty("DisplayAlerts", false);//NO warning
    QAxObject * workBooks = excel.querySubObject("WorkBooks");

    workBooks->dynamicCall("Open(const QString&)", QString(InputExcel));
    QAxObject *workbook = excel.querySubObject("ActiveWorkBook");

    QAxObject * worksheet = workbook->querySubObject("Worksheets(int)", 1);
    QAxObject *first_sheet = worksheet->querySubObject("Rows(int)", 1);//delete the first row
    first_sheet->dynamicCall("delete");
    myProgressValue = 50;
    worksheet->dynamicCall("SaveAs(const QString&,int)",QDir::toNativeSeparators(OutputCSV),6);
    workbook->dynamicCall("Close (Boolean)", false); //close file
    excel.dynamicCall("Quit(void)");//close excel
    myProgressValue = 100;
    return true;
}
