//
// Created by Tigermouthbear on 8/18/20.
//

#include "ConfigWidget.h"

#include <QtWidgets/QGridLayout>
#include <iostream>
#include "../FileUtils.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <experimental/filesystem>
#else
#include <filesystem>
#endif

ConfigWidget::ConfigWidget(TigerCapture* tigerCapture): QWidget() {
    this->tigerCapture = tigerCapture;

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle("TigerCapture");
    setWindowFlags(Qt::WindowStaysOnTopHint);

    // create layout
    auto* layout = new QGridLayout(this);

    // add elements

    uploadersLabel = new QLabel("Close window on capture", this);
    layout->addWidget(uploadersLabel, 1, 0);

    shouldMinimizeCheckbox = new QCheckBox(this);
    shouldMinimizeCheckbox->setChecked(tigerCapture->shouldMinimize());
    layout->addWidget(shouldMinimizeCheckbox, 1, 1, 1, 1, Qt::AlignHCenter);

    clipboardLabel = new QLabel("Copy image before uploading", this);
    layout->addWidget(clipboardLabel, 2, 0);

    shouldClipboardCheckbox = new QCheckBox(this);
    shouldClipboardCheckbox->setChecked(tigerCapture->shouldClipboard());
    layout->addWidget(shouldClipboardCheckbox, 2, 1, 1, 1, Qt::AlignHCenter);


    uploadersLabel = new QLabel("Delay (ms)", this);
    layout->addWidget(uploadersLabel, 3, 0);

    delayInput = new QLineEdit(this);
    delayInput->setText(std::to_string(tigerCapture->getDelay()).c_str());
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
        if(tigerCapture->getUploaderLoc() == filename) uploadersDropdown->setCurrentText(filename.c_str());
    }
    layout->addWidget(uploadersDropdown, 4, 1);


    saveButton = new QPushButton("Save and Close", this);
    layout->addWidget(saveButton, 5, 0, 1, 2, Qt::AlignHCenter);
    connect(saveButton, SIGNAL (released()), this, SLOT (save()));
}

void ConfigWidget::save() {
    tigerCapture->setDelay(std::stoi(delayInput->text().toStdString()));
    tigerCapture->setShouldMinimize(shouldMinimizeCheckbox->isChecked());
    tigerCapture->setShouldClipboard(shouldClipboardCheckbox->isChecked());
    if(!tigerCapture->setUploader(uploadersDropdown->currentText().toStdString())) uploadersDropdown->setCurrentText("None");
    close();
}

ConfigWidget::~ConfigWidget() {
    delete delayInput;
    delete shouldMinimizeCheckbox;
    delete shouldClipboardCheckbox;
    delete uploadersLabel;
    delete clipboardLabel;
    delete uploadersDropdown;
    delete saveButton;
}
