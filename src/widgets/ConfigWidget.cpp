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
    setWindowFlags(Qt::WindowStaysOnTopHint);

    // create layout
    auto* layout = new QGridLayout(this);

    // add elements

    uploadersLabel = new QLabel("Close window on capture", this);
    layout->addWidget(uploadersLabel, 1, 0);

    shouldMinimizeCheckbox = new QCheckBox(this);
    shouldMinimizeCheckbox->setChecked(config->shouldMinimize());
    layout->addWidget(shouldMinimizeCheckbox, 1, 1, 1, 1, Qt::AlignHCenter);

    clipboardLabel = new QLabel("Copy image before uploading", this);
    layout->addWidget(clipboardLabel, 2, 0);

    shouldClipboardCheckbox = new QCheckBox(this);
    shouldClipboardCheckbox->setChecked(config->shouldClipboard());
    layout->addWidget(shouldClipboardCheckbox, 2, 1, 1, 1, Qt::AlignHCenter);


    uploadersLabel = new QLabel("Delay (ms)", this);
    layout->addWidget(uploadersLabel, 3, 0);

    delayInput = new QLineEdit(this);
    delayInput->setText(std::to_string(config->getDelay()).c_str());
    delayInput->setValidator(new QIntValidator(0, 20000, this));
    layout->addWidget(delayInput, 3, 1, 1, 1, Qt::AlignHCenter);

    uploadersLabel = new QLabel("File Uploader", this);
    layout->addWidget(uploadersLabel, 4, 0);

    uploadersDropdown = new QComboBox(this);
    uploadersDropdown->addItem("None");
    uploadersDropdown->setCurrentText("None");
    for(const auto& entry: std::filesystem::directory_iterator(FileUtils::getUploadersDirectory())) {
        std::string filename = entry.path().filename();
        uploadersDropdown->addItem(filename.c_str());
        if(config->getUploaderLoc() == filename) uploadersDropdown->setCurrentText(filename.c_str());
    }
    layout->addWidget(uploadersDropdown, 4, 1);


    saveButton = new QPushButton("Save and Close", this);
    layout->addWidget(saveButton, 5, 0, 1, 2, Qt::AlignHCenter);
    connect(saveButton, SIGNAL (released()), this, SLOT (save()));
}

void ConfigWidget::save() {
    config->setDelay(std::stoi(delayInput->text().toStdString()));
    config->setShouldMinimize(shouldMinimizeCheckbox->isChecked());
    config->setShouldClipboard(shouldClipboardCheckbox->isChecked());
    if(!config->setUploader(uploadersDropdown->currentText().toStdString())) uploadersDropdown->setCurrentText("None");
    close();
}
