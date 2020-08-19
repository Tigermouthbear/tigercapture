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
    shouldUpload = new QCheckBox("Upload", this);
    shouldUpload->setChecked(config->shouldUpload());
    layout->addWidget(shouldUpload, 0, 0, 1, 2, Qt::AlignHCenter);

    shouldMinimizeCheckbox = new QCheckBox("Close window on capture", this);
    shouldMinimizeCheckbox->setChecked(config->shouldMinimize());
    layout->addWidget(shouldMinimizeCheckbox, 1, 0, 1, 2, Qt::AlignHCenter);

    uploadersLabel = new QLabel("File Uploader", this);
    layout->addWidget(uploadersLabel, 2, 0);

    uploadersDropdown = new QComboBox(this);
    uploadersDropdown->addItem("None");
    uploadersDropdown->addItem("kinda epic");
    uploadersDropdown->addItem("very epic");
    layout->addWidget(uploadersDropdown, 2, 1);
}

void ConfigWidget::closeEvent(QCloseEvent *event) {
    config->setShouldUpload(shouldUpload->isChecked());
    config->setShouldMinimize(shouldMinimizeCheckbox->isChecked());
}
