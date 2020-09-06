//
// Created by Tigermouthbear on 8/31/20.
//

#include "UploadedFileWidget.h"

UploadedFileWidget::UploadedFileWidget(QWidget* widget, std::pair<std::string, std::string> pair): QToolButton(
        widget) {
    menu = new QMenu();
    //menu->addAction("item 1");
    //menu->addAction("item 2");
    if(QString::fromStdString(pair.first).endsWith(".png")) {
        setIcon(QIcon(pair.first.c_str()));
        setIconSize({100, 100});
    }
    setMenu(menu);
    show();
}
