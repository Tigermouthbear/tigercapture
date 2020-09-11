//
// Created by Tigermouthbear on 8/18/20.
//

#ifndef TIGERCAPTURE_CONFIGWIDGET_H
#define TIGERCAPTURE_CONFIGWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include "../TigerCapture.h"

class ConfigWidget: public QWidget {
Q_OBJECT
private:
    TigerCapture* tigerCapture;

    QLineEdit* delayInput;
    QCheckBox* shouldMinimizeCheckbox;
    QCheckBox* shouldClipboardCheckbox;
    QLabel* uploadersLabel;
    QLabel* clipboardLabel;
    QComboBox* uploadersDropdown;
    QPushButton* saveButton;

public:
    explicit ConfigWidget(TigerCapture* tigerCapture);

private slots:
    void save();
};


#endif //TIGERCAPTURE_CONFIGWIDGET_H
