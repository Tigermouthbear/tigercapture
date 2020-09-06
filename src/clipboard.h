//
// Created by cook on 9/5/20.
//

#ifndef TIGERCAPTURE_CLIPBOARD_H
#define TIGERCAPTURE_CLIPBOARD_H


#include <string>
#include <QPixmap>

class Clipboard {
public:
    static void copyToClipboard(const std::string& text);
    static void copyToClipboard(const QPixmap& pixmap);
    static void clearClipboard();
};


#endif //TIGERCAPTURE_CLIPBOARD_H
