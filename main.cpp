/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
//#include <QtCore/QDir>
#include <QtQuick/QQuickView>
//#include <QtQml/QQmlEngine>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
//#include <iostream>
#include <QQmlContext>
//#include <QQmlApplicationEngine>
#include "model.h"
#include <QtWidgets/QMessageBox>
#include <QFileDialog>
QStringList resortStringList (QStringList longitudeList , int longitude){
    QStringList resortedList;
    if (longitude %2 ==0){ // if even longitude resort list
    for (int i = (longitudeList.size())-1 ; i>=0 ; i--){
        resortedList.append(longitudeList[i]);
    }
    }
    else {
        return longitudeList;
    }
    return resortedList;
}
void Write (QString filename,QStringList outputList){
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    QTextStream out(&file);
    out << "import QtQuick 2.1"<<endl;
    out << "Item {"<<endl;
    out <<"\tproperty alias model: dataModel"<<endl;
    out << "\tListModel {"<<endl;
    out << "\t\tid: dataModel"<<endl;
    for (int i =0 ; i< outputList.size() ; i++){
      out <<  "\t\tListElement{ longitude: \"" << outputList[i].split(",")[0] <<"\""<<
              "; latitude: \"" << outputList[i].split(",")[1] <<"\""<<
              "; height: \"" << outputList[i].split(",")[2].trimmed() << "\"; }"<<endl;
    }
    out << "\t}"<<endl;
    out << "}"<<endl;
    file.flush();
    file.close();
}
QStringList Read (QString filename){
    //std::cout << filename.toStdString() << std::endl;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(file.errorString());
        msgBox.exec();
        //std::cout << file.errorString().toStdString() << std::endl;
    }
    QStringList wordList;
    QTextStream in(&file);
     while (!in.atEnd()) {
         QString line = file.readLine();
         wordList.append(line);
        }

       QStringList finalList;
       QStringList longitudeList;
       QStringList resortedList;
     for (int index =0 , longitude =0; index< wordList.size() ;index++){
         if ( wordList[index].split(";").first().toInt()== longitude){
             longitudeList.append(wordList[index]);
         }
         else  {
             //std::cout << longitudeList[0].toStdString() << std::endl;
             //resortedList=resortStringList(longitudeList,longitude);
             //finalList.append(resortedList);
             finalList.append(longitudeList);
             longitudeList.clear();
             longitudeList.append(wordList[index]);
             longitude++;
         }
         if (index ==wordList.size()-1){
             //resortedList=resortStringList(longitudeList,longitude);
             //finalList.append(resortedList);
              finalList.append(longitudeList);
         }


     }
     for (int i =0; i<finalList .size() ;i++){
         finalList[i].replace(";", ",");
     }
     //std::cout << finalList[0].toStdString() << std::endl;
     return finalList;
}
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":geology.png"));
    app.setOrganizationName("Hanna");
    app.setOrganizationDomain("Hanna");
    QString inputFile;
    inputFile =QFileDialog::getOpenFileName(nullptr,"Select txt file contains data","C://","*.txt");
    QStringList outputList;
    int counter=0;
    //QString inputFile =QGuiApplication::applicationDirPath() + QString("/a.txt");
    //QString outputFile = "C:\\Qt\\Examples\\Qt-5.15.0\\datavisualization\\qmlsurface\\qml\\qmlsurface\\Data.qml";
    while(!(QFileInfo(inputFile).exists()) && counter < 1){
        counter++;
        QMessageBox msgBox;
        msgBox.setText("Please select txt file that contains data");
        msgBox.exec();
        inputFile =QFileDialog::getOpenFileName(nullptr,"Select txt file contains data","C://","*.txt");
    }
    if (!(QFileInfo(inputFile).exists())){
        QMessageBox msgBox;
        msgBox.setText("Please prepare .txt file that contains data and try again\n Bye Bye");
        msgBox.exec();
        return -1;}
    outputList=Read(inputFile);
    //Write(outputFile,outputList);
    //std::cout << inputFile.toStdString()<< std::endl;


    AnimalModel model;

    for ( int i =0;i<outputList.size() ; i++){

        model.addAnimal(Animal(outputList[i].split(",")[0].toInt(), outputList[i].split(",")[1].toInt(),outputList[i].split(",")[2].toFloat()));
    }
    //
    //model.addAnimal(Animal(2,4,5));
    QQuickView viewer;
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    viewer.rootContext()->setContextProperty("model", &model);
    //viewer.setInitialProperties({{"model", QVariant::fromValue(&model)}});
    //viewer.engine()->addImportPath(extraImportPath.arg(QGuiApplication::applicationDirPath(),QString::fromLatin1("qmlsurface")));
    viewer.setSource(QUrl("qrc:/qml/qml/qmlsurface/main.qml"));
    viewer.setTitle(QStringLiteral("Egyptolict "));
    viewer.show();

    return app.exec();
}
