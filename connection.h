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
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>

class connection
{
public:
    connection();
    bool createConnection(QString importFilepath, QString outputFilepath, QString tableName);
    bool  connection::saveUserData(int pathtype,QString filePathName,QString outputFilepath);
    bool tranExcelToCSV(QString InputExcel,QString OutputCSV);

};

#endif // CONNECTION_H
