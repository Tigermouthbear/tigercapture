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
    std::ifstream file;
    file.open(FileUtils::getUploadsLogFile());
    for(std::pair<std::string, std::string> pair;
        std::getline(file, pair.first, ',') && std::getline(file, pair.second);) {
        map[pair] = new UploadedFileWidget(this, pair);
    }
    file.close();

    // remove all previous elements
    QLayoutItem* item;
    while((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // add all from map
    int num = map.size() - 1;
    std::map<std::pair<std::string, std::string>, UploadedFileWidget*>::iterator it;
    for(it = map.begin(); it != map.end(); it++) {
        layout->addWidget(it->second, (int) (num / 2), num % 2);
        num--;
    }

    update();
}

UploadsExplorerWidget::~UploadsExplorerWidget() {
    delete scrollArea;
    delete central;
    delete layout;
}

