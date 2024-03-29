//
// Created by Tigermouthbear on 8/21/20.
//

#include "UploadsExplorerWidget.h"

#include "../../TigerCapture.hpp"

UploadsExplorerWidget::UploadsExplorerWidget(QWidget* parent): QScrollArea(parent) {
    central = new QWidget(this);
    layout = new QGridLayout(central);
    QGridLayout(this).addWidget(central);
    setWidget(central);
    setWidgetResizable(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout->setAlignment(Qt::AlignTop);

    updateUploads();

    show();
}

void UploadsExplorerWidget::updateScrollArea() {
    // remove all previous elements
    QLayoutItem *child;
    while((child = layout->takeAt(0)) != 0) {
        layout->removeItem(child);
        delete child;
    }

    // add all widgets to scroll area
    if(compact) {
        int num = uploadedFileWidgets.size() - 1;
        for(UploadedFileWidget* uploadedFileWidget: uploadedFileWidgets) {
            layout->addWidget(uploadedFileWidget, (int) (num / 2), num % 2);
            num--;
        }
    } else {
        int columns = (width() - layout->horizontalSpacing() - verticalScrollBar()->sizeHint().width()) / (116 + layout->horizontalSpacing()); columns = columns > 1 ? columns : 1; // how many widgets are in one row
        int num = uploadedFileWidgets.size() - 1;
        for(UploadedFileWidget* uploadedFileWidget: uploadedFileWidgets) {
            layout->addWidget(uploadedFileWidget, (int) (num / columns), num % columns);
            num--;
        }
    }
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

    updateScrollArea();
    update();
}

void UploadsExplorerWidget::resizeEvent(QResizeEvent* event) {
    updateScrollArea();
    QScrollArea::resizeEvent(event);
}

UploadsExplorerWidget::~UploadsExplorerWidget() {
    for(UploadedFileWidget* uploadedFileWidget: uploadedFileWidgets)
        delete uploadedFileWidget;
}

void UploadsExplorerWidget::setCompact(bool compactIn) {
    compact = compactIn;
}
