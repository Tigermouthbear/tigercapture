//
// Created by Tigermouthbear on 8/31/20.
//

#include <QFileIconProvider>
#include "UploadedFileWidget.h"

#include "../../TigerCapture.hpp"

UploadedFileWidget::UploadedFileWidget(QWidget* widget, std::pair<std::string, std::string> pair): QToolButton(widget) {
    url = pair.second;
    loc = pair.first;

    // get file info
    QFileInfo file(pair.first.c_str());
    QString filename = file.fileName();
    setToolTip(filename);

    menu = new QMenu();

    // add actions
    copyURLAction = menu->addAction("Copy URL");
    copyFileLocationAction = menu->addAction("Copy File Location");

    // add events
    connect(copyURLAction, SIGNAL(triggered()), this, SLOT(copyURL()));
    connect(copyFileLocationAction, SIGNAL(triggered()), this, SLOT(copyFileLocation()));

    // set icon and add menu to widget
    if(filename.endsWith(".png")) setIcon(QIcon(pair.first.c_str()));
    else {
        QList<QMimeType> mime_types = mime_database.mimeTypesForFileName(filename);
        setIcon(QIcon::fromTheme(mime_types[0].iconName()));
    }
    setIconSize({100, 100});

    setMenu(menu);
    show();
}

void UploadedFileWidget::contextMenuEvent(QContextMenuEvent* event) {
    menu->popup({event->globalX(), event->globalY()});
}

void UploadedFileWidget::copyURL() {
    TC::Clipboard::copyToClipboard(url);
}

void UploadedFileWidget::copyFileLocation() {
    TC::Clipboard::copyToClipboard(loc);
}
