//
// Created by Tigermouthbear on 8/31/20.
//

#ifndef TIGERCAPTURE_UPLOADEDFILEWIDGET_H
#define TIGERCAPTURE_UPLOADEDFILEWIDGET_H

#include <QMimeDatabase>
#include <QtWidgets/QToolButton>
#include <QMenu>
#include <QContextMenuEvent>

class UploadedFileWidget: public QToolButton {
Q_OBJECT
private:
    QMimeDatabase mime_database;

    std::string url;
    std::string loc;

    QMenu* menu;
    QAction* copyURLAction;
    QAction* copyFileLocationAction;

public:
    UploadedFileWidget(QWidget* widget, std::pair<std::string, std::string> pair);

private slots:
    void copyURL();
    void copyFileLocation();

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
};


#endif //TIGERCAPTURE_UPLOADEDFILEWIDGET_H
