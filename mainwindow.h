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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlTableModel>
#include <QSqlDatabase>

#include <QDebug>

#include <QMenu>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool searchData(QObject *ptrTableviewInfo,QString strHeadItem,QString strExactItem,QString strTableName);
    void MainWindow::setTableHead(QSqlQueryModel *headModel);
    void btnExportExcelsetting(void);
    bool readUserData(QString userDataPath);

private slots:
    void on_btnSearch_clicked();

    void on_btn_Setting_pressed();

    void on_btn_About_clicked();

    void on_btnScholarship_clicked();

    void on_btn_Stipend_clicked();

    void on_btnScholarship_pressed();

    void on_btn_Stipend_pressed();

    void on_toolbtn_scholar_clicked();

    void on_toolbtn_stipend_clicked();

    void on_btnClear_clicked();

    void on_tableviewScholarshipInfo_doubleClicked(const QModelIndex &index);

    void on_tableviewStipendInfo_doubleClicked(const QModelIndex &index);

    void on_btn_confirmCloseInfoCard_clicked();

    void on_btn_SaveScreenShot_clicked();

    void on_btnExportExcel_clicked();

    void on_btn_ClearALL_clicked();

public slots:
    void timerUpdate(void);
    void onExportMenuTriggered(QAction *action);

signals:
    void sendShowInfoSignal();

public:
    QString scholarshipPath;
    QString stipendPath;
    QString fileStuName;

private:
    QMenu *exportMenu = new QMenu();
    QAction *exportMenu_action_scholarship = new QAction(exportMenu);
    QAction *exportMenu_action_stipend = new QAction(exportMenu);

    typedef enum {
        setting,
        scholarship,
        stipend
    }STARTMENU;

    STARTMENU enumStartMenu;

private:
    Ui::MainWindow *ui;

};

//Store file path macro
#define scholarshipFileType (0)
#define stipendFileType (1)
#define scholarshipCSVPath (2)
#define stipendCSVPath (3)

//Table name macro
#define scholarshipTable ("scholarshipTable")
#define stipendTable ("stipendTable")

extern unsigned int myProgressValue;

#endif // MAINWINDOW_H
