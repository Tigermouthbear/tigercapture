//
// Created by Tigermouthbear on 8/21/20.
//

#include "UploadsExplorerWidget.h"

#include "../FileUtils.h"

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
    // create upload elements
    std::ifstream file;
    file.open(FileUtils::getUploadsLogFile());
    for(std::pair<std::string, std::string> pair;
        std::getline(file, pair.first, ',') && std::getline(file, pair.second);) {
        uploadedFileWidgets.push_back(new UploadedFileWidget(this, pair));
    }
    file.close();

    // remove all previous elements
    QLayoutItem* item;
    while((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // add all from map
    int num = uploadedFileWidgets.size() - 1;
    for(UploadedFileWidget* uploadedFileWidget: uploadedFileWidgets) {
        layout->addWidget(uploadedFileWidget, (int) (num / 2), num % 2);
        num--;
    }

    update();
}
