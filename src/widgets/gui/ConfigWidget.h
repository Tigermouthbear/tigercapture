//
// Created by Tigermouthbear on 8/18/20.
//

#ifndef TIGERCAPTURE_CONFIGWIDGET_H
#define TIGERCAPTURE_CONFIGWIDGET_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

class ConfigWidget;
#include "../../TigerCapture.h"
class TigerCapture;

class ConfigWidget: public QDialog {
Q_OBJECT
private:
    TigerCapture* tigerCapture;

    QLineEdit* delayInput;
    QCheckBox* shouldMinimizeCheckbox;
    QCheckBox* shouldKeepAboveCheckbox;
    QCheckBox* shouldClipboardCheckbox;
    QLabel* label;
    QComboBox* uploadersDropdown;
    QPushButton* saveButton;

public:
    explicit ConfigWidget(TigerCapture* tigerCapture);

private slots:
    void save();
};


#endif //TIGERCAPTURE_CONFIGWIDGET_H
