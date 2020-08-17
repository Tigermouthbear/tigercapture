//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_MAINWINDOW_H
#define TIGERCAPTURE_MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>

class MainWindow: public QMainWindow {
Q_OBJECT
private:
    QCheckBox* shouldMinimizeButton;

    QPushButton* fullButton;
    QPushButton* areaButton;
    QPushButton* pinButton;

public:
    void Create();

private slots:
    void handleFullScreenshot();
    void fullScreenShot();

    void handleAreaScreenshot();
    void areaScreenshot();

    void handlePinArea();
    void pinArea();
};


#endif //TIGERCAPTURE_MAINWINDOW_H
