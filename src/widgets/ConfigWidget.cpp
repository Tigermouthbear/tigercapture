//
// Created by Tigermouthbear on 8/18/20.
//

#include "ConfigWidget.h"

#include <QtWidgets/QGridLayout>
#include <iostream>
#include <filesystem>
#include "../FileUtils.h"

ConfigWidget::ConfigWidget(Config* config): QWidget() {
    this->config = config;

    setWindowTitle("Config");

    // create layout
    auto* layout = new QGridLayout(this);

    // add elements
    shouldMinimizeCheckbox = new QCheckBox("Close window on capture", this);
    shouldMinimizeCheckbox->setChecked(config->shouldMinimize());
    layout->addWidget(shouldMinimizeCheckbox, 1, 0, 1, 2, Qt::AlignHCenter);

    uploadersLabel = new QLabel("File Uploader", this);
    layout->addWidget(uploadersLabel, 2, 0);

    uploadersDropdown = new QComboBox(this);
    uploadersDropdown->addItem("None");
    uploadersDropdown->setCurrentText("None");
    for(const auto& entry: std::filesystem::directory_iterator(FileUtils::getUploadersDirectory())) {
        std::string filename = entry.path().filename();
        uploadersDropdown->addItem(filename.c_str());
        if(config->getUploaderLoc() == filename) uploadersDropdown->setCurrentText(filename.c_str());
    }
    layout->addWidget(uploadersDropdown, 2, 1);

    saveButton = new QPushButton("Save", this);
    layout->addWidget(saveButton, 3, 0, 1, 2, Qt::AlignHCenter);
    connect(saveButton, SIGNAL (released()), this, SLOT (save()));
}

void ConfigWidget::save() {
    config->setShouldMinimize(shouldMinimizeCheckbox->isChecked());
    if(!config->setUploader(uploadersDropdown->currentText().toStdString())) uploadersDropdown->setCurrentText("None");
}
