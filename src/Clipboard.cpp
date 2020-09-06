//
// Created by cook on 9/5/20.
//

#include "Clipboard.h"

#include <QApplication>
#include <QThread>
#include <QClipboard>
#include <QEvent>
#include "../clip/clip.h"

/// Note: QT Must have an open window for these to work

void Clipboard::copyToClipboard(const std::string& text) {
    auto cstr = text.c_str();
    clip::set_text(cstr);
}

void Clipboard::copyToClipboard(const QPixmap& pixmap) {
    auto image = pixmap.toImage();

    clip::image_spec spec = {
            (unsigned long) image.width(),
            (unsigned long) image.height(),
            (unsigned long) image.depth(),
            (unsigned long) image.bytesPerLine(),
            0xff, // red
            0xff00, // green
            0xff0000, // blue
            0xff000000,
            0,
            8,
            16,
            24
    };
    clip::image img(image.data_ptr(), spec);
    clip::set_image(img);
}

void Clipboard::clearClipboard() {
    clip::clear();
}
