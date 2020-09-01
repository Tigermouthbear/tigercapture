//
// Created by Tigermouthbear on 8/31/20.
//

#ifndef TIGERCAPTURE_UPLOADEDFILEWIDGET_H
#define TIGERCAPTURE_UPLOADEDFILEWIDGET_H

#include <QtWidgets/QToolButton>
#include <QMenu>

class UploadedFileWidget: public QToolButton {
Q_OBJECT
private:
    QMenu* menu;
public:
    UploadedFileWidget(QWidget* widget, std::pair<std::string, std::string> pair);
};


#endif //TIGERCAPTURE_UPLOADEDFILEWIDGET_H
