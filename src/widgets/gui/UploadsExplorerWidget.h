//
// Created by Tigermouthbear on 8/21/20.
//

#ifndef TIGERCAPTURE_UPLOADSEXPLORERWIDGET_H
#define TIGERCAPTURE_UPLOADSEXPLORERWIDGET_H

#include <QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>

#include "UploadedFileWidget.h"

class UploadsExplorerWidget: public QWidget {
public:
    explicit UploadsExplorerWidget(QWidget* parent, int width, int height);

public slots:
    void updateUploads();

private:
    std::vector<UploadedFileWidget*> uploadedFileWidgets;

    QScrollArea* scrollArea;
    QWidget* central;
    QGridLayout* layout;
};


#endif //TIGERCAPTURE_UPLOADSEXPLORERWIDGET_H
