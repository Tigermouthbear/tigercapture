//
// Created by Tigermouthbear on 8/21/20.
//

#include "UploadsExplorerWidget.h"

#include "../../TigerCapture.hpp"

UploadsExplorerWidget::UploadsExplorerWidget(QWidget* parent, int width, int height): QWidget(parent) {
    scrollArea = new QScrollArea(this);
    central = new QWidget(this);
    layout = new QGridLayout(central);
    scrollArea->setFixedSize(width, height);
    scrollArea->setWidget(central);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    updateUploads();

    scrollArea->show();
}

void UploadsExplorerWidget::updateUploads() {
    uploadedFileWidgets.clear();

    // create upload elements
    std::ifstream file;
    file.open(TC::Files::getUploadsLogFile());
    for(std::pair<std::string, std::string> pair;
        std::getline(file, pair.first, ',') && std::getline(file, pair.second);) {
        uploadedFileWidgets.push_back(new UploadedFileWidget(this, pair));
    }
    file.close();

    // remove all previous elements
    QLayoutItem *child;
    while((child = layout->takeAt(0)) != 0) {
        layout->removeItem(child);
        delete child;
    }

    // add all from map
    int num = uploadedFileWidgets.size() - 1;
    for(UploadedFileWidget* uploadedFileWidget: uploadedFileWidgets) {
        layout->addWidget(uploadedFileWidget, (int) (num / 2), num % 2);
        num--;
    }

    update();
}
