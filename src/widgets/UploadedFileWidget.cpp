//
// Created by Tigermouthbear on 8/31/20.
//

#include "UploadedFileWidget.h"

#include "../Clipboard.h"

UploadedFileWidget::UploadedFileWidget(QWidget* widget, std::pair<std::string, std::string> pair): QToolButton(widget) {
    url = pair.second;
    loc = pair.first;

    menu = new QMenu();

    // add actions
    copyURLAction = menu->addAction("Copy URL");
    copyFileLocationAction = menu->addAction("Copy File Location");

    // add events
    connect(copyURLAction, SIGNAL(triggered()), this, SLOT(copyURL()));
    connect(copyFileLocationAction, SIGNAL(triggered()), this, SLOT(copyFileLocation()));

    // set icon and add menu to widget
    if(QString::fromStdString(pair.first).endsWith(".png")) {
        setIcon(QIcon(pair.first.c_str()));
        setIconSize({100, 100});
    }
    setMenu(menu);
    show();
}

void UploadedFileWidget::copyURL() {
    Clipboard::copyToClipboard(url);
}

void UploadedFileWidget::copyFileLocation() {
    Clipboard::copyToClipboard(loc);
}

UploadedFileWidget::~UploadedFileWidget() {
    delete copyURLAction;
    delete copyFileLocationAction;
    delete menu;
}
