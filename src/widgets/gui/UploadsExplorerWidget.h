//
// Created by Tigermouthbear on 8/21/20.
//

#ifndef TIGERCAPTURE_UPLOADSEXPLORERWIDGET_H
#define TIGERCAPTURE_UPLOADSEXPLORERWIDGET_H

#include <QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>

#include "UploadedFileWidget.h"

class UploadsExplorerWidget: public QScrollArea {
public:
    explicit UploadsExplorerWidget(QWidget* parent);
    ~UploadsExplorerWidget();

public slots:
    void updateUploads();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    std::vector<UploadedFileWidget*> uploadedFileWidgets;

    QWidget* central;
    QGridLayout* layout;

    void updateScrollArea();
};


#endif //TIGERCAPTURE_UPLOADSEXPLORERWIDGET_H
