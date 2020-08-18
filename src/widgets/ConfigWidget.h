//
// Created by Tigermouthbear on 8/18/20.
//

#ifndef TIGERCAPTURE_CONFIGWIDGET_H
#define TIGERCAPTURE_CONFIGWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include "../Config.h"

class ConfigWidget: public QWidget {
Q_OBJECT
private:
    Config* config;

    QCheckBox* shouldMinimizeCheckbox;
    QLabel* uploadersLabel;
    QComboBox* uploadersDropdown;

public:
    ConfigWidget(Config* config);

protected:
    void closeEvent(QCloseEvent* event) override;
};


#endif //TIGERCAPTURE_CONFIGWIDGET_H
