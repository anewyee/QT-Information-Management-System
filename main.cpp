#include "mainwindow.h"
#include <QApplication>


#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>
#include <QTextStream>
#include <QDebug>

#include "DarkStyle.h"
#include "framelesswindow.h"

#include <QFile>
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

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    a.setStyle(new DarkStyle);
    FramelessWindow framelessWindow;
    MainWindow *mainWindow = new MainWindow;
    framelessWindow.setContent(mainWindow);
    framelessWindow.show();
    return a.exec();
}
