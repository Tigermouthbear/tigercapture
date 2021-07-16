//
// Created by Tigermouthbear on 8/18/20.
//

#include "ConfigWidget.h"

#include <QtWidgets/QGridLayout>
#include <iostream>
#include <QtCore/QDir>

ConfigWidget::ConfigWidget(TigerCapture* tigerCapture): QDialog() {
    this->tigerCapture = tigerCapture;

    setWindowTitle("TigerCapture | Config");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // create layout
    auto* layout = new QGridLayout(this);

    // add elements
    label = new QLabel("Close window on capture", this);
    layout->addWidget(label, 1, 0);

    shouldMinimizeCheckbox = new QCheckBox(this);
    shouldMinimizeCheckbox->setChecked(tigerCapture->getConfig()->shouldMinimize());
    layout->addWidget(shouldMinimizeCheckbox, 1, 1, 1, 1, Qt::AlignHCenter);

    label = new QLabel("Keep window on top", this);
    layout->addWidget(label, 2, 0);

    shouldKeepAboveCheckbox = new QCheckBox(this);
    shouldKeepAboveCheckbox->setChecked(tigerCapture->getConfig()->shouldKeepAbove());
    layout->addWidget(shouldKeepAboveCheckbox, 2, 1, 1, 1, Qt::AlignHCenter);

    label = new QLabel("Copy image before uploading", this);
    layout->addWidget(label, 3, 0);

    shouldClipboardCheckbox = new QCheckBox(this);
    shouldClipboardCheckbox->setChecked(tigerCapture->getConfig()->shouldClipboard());
    layout->addWidget(shouldClipboardCheckbox, 3, 1, 1, 1, Qt::AlignHCenter);

    label = new QLabel("Delay (ms)", this);
    layout->addWidget(label, 4, 0);

    delayInput = new QLineEdit(this);
    delayInput->setText(std::to_string(tigerCapture->getConfig()->getDelay()).c_str());
    delayInput->setValidator(new QIntValidator(0, 20000, this));
    layout->addWidget(delayInput, 4, 1, 1, 1, Qt::AlignHCenter);

    label = new QLabel("File Uploader", this);
    layout->addWidget(label, 5, 0);

    uploadersDropdown = new QComboBox(this);
    uploadersDropdown->addItem("None");
    uploadersDropdown->setCurrentText("None");

    // gather files
    QDir directory(TC::Files::getUploadersDirectory().c_str());
    QStringList files = directory.entryList(QStringList() << "*.json" << "*.JSON", QDir::Files);
    for(const auto& file: files) {
        std::string filename = file.toStdString();
        uploadersDropdown->addItem(filename.c_str());
        if(tigerCapture->getConfig()->getUploaderLoc() == filename) uploadersDropdown->setCurrentText(filename.c_str());
    }
    uploadersDropdown->addItem("Imgur");
    if(tigerCapture->getConfig()->getUploaderLoc() == "Imgur") uploadersDropdown->setCurrentText("Imgur");

    layout->addWidget(uploadersDropdown, 5, 1);

    saveButton = new QPushButton("Save", this);
    layout->addWidget(saveButton, 6, 0, 1, 2, Qt::AlignHCenter);
    connect(saveButton, SIGNAL (released()), this, SLOT (save()));
}

void ConfigWidget::save() {
    tigerCapture->getConfig()->setDelay(std::stoi(delayInput->text().toStdString()));
    tigerCapture->getConfig()->setShouldMinimize(shouldMinimizeCheckbox->isChecked());
    tigerCapture->getConfig()->setShouldClipboard(shouldClipboardCheckbox->isChecked());
    if(!tigerCapture->getConfig()->setUploader(uploadersDropdown->currentText().toStdString())) uploadersDropdown->setCurrentText("None");

    bool above = shouldKeepAboveCheckbox->isChecked();
    if(above != tigerCapture->getConfig()->shouldKeepAbove()) {
        tigerCapture->getConfig()->setShouldKeepAbove(above);
        tigerCapture->getMainWindow()->close();
        tigerCapture->openWindow();
    }

    close();
}
