//
// Created by Tigermouthbear on 8/18/20.
//

#include "ConfigWidget.h"

#include <QtWidgets/QGridLayout>

ConfigWidget::ConfigWidget(Config* config): QWidget() {
    this->config = config;

    setWindowTitle("TigerCapture - Config");

    // create layout
    auto* layout = new QGridLayout(this);

    // add elements
    shouldMinimizeCheckbox = new QCheckBox("Close window on capture", this);
    shouldMinimizeCheckbox->setChecked(config->shouldMinimize());
    layout->addWidget(shouldMinimizeCheckbox, 0, 0, 1, 2, Qt::AlignHCenter);

    uploadersLabel = new QLabel("File Uploader", this);
    layout->addWidget(uploadersLabel, 1, 0);

    uploadersDropdown = new QComboBox(this);
    uploadersDropdown->addItem("None");
    uploadersDropdown->addItem("kinda epic");
    uploadersDropdown->addItem("very epic");
    layout->addWidget(uploadersDropdown, 1, 1);
}

void ConfigWidget::closeEvent(QCloseEvent *event) {
    config->setShouldMinimize(shouldMinimizeCheckbox->isChecked());
}
