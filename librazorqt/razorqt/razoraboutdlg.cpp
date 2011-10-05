/* BEGIN_COMMON_COPYRIGHT_HEADER
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2010-2011 Razor team
 * Authors:
 *   Petr Vanek <petr@scribus.info>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef RAZORABOUTDLG_CPP
#define RAZORABOUTDLG_CPP
#include "razoraboutdlg.h"
#include "ui_aboutdlg.h"
#include "libtranslate.h"

/**
 * @brief prepares the data to show and fills the form, then shows.
 */
class RazorAboutDLGPrivate: public Ui_about, public QDialog
{
public:
    RazorAboutDLGPrivate();
};


//
RazorAboutDLGPrivate::RazorAboutDLGPrivate()
{
    libTranslate("librazorqt");
    setupUi(this);
    QString content("<html><body>"
    "<h1>Razor Desktop Toolbox<img src=\"images://razor_logo.png\" align=\"right\" /></h1>"
    "<p>Version: " + QString(RAZOR_VERSION) + "</p>"
    "<p>Homepage: <a href=\"http://razor-qt.org\">razor-qt.org</a></p>"
    "<h2>License: GPL2</h2>"
    "<h2>Authors</h2>"
    "<p>The razor-qt Crew consists of (in alphabetical order):</p>"
    "<ul>"
    "<li>Christopher 'VdoP' Regali</li>"
    "<li>Alexander Sokolov &lt;<a href='mailto:sokoloff.a@gmail.com'>sokoloff.a@gmail.com</a>&gt;</li>"
    "<li>Petr Vanek &lt;<a href='mailto:petr@scribus.info'>petr@scribus.info</a>&gt;</li>"
    "</ul>"
    "<p>If you want to contribute, just visit <a href=\"http://razor-qt.org\">razor-qt.org</a>.</p>"
    "<h2>Special thanks to</h2>"
    "<ul>"
    "<li>Andy Fitzsimon (logo/icon)</li>"
    "<li>Eugene Pivnev (QtDesktop)</li>"
    "<li>Paul Gnther (for desktop backgrounds)</li>"
    "<li>Manuel Meier (for ideas)</li>"
    "<li>Alexander Zakher (for the name)</li>"
    "<li>and KDE (http://www.kde.org)</li>"
    "</body></html>");
    
    QTextDocument * doc = new QTextDocument(this);
    doc->addResource(QTextDocument::ImageResource, QUrl("images://razor_logo.png"), QPixmap(QString(SHARE_DIR) + "/graphics/razor_logo.png"));
    doc->setHtml(content);

    this->setAttribute(Qt::WA_DeleteOnClose);
    textBrowser->setDocument(doc);
    show();
}


RazorAboutDLG::RazorAboutDLG()
{
    d_ptr = new RazorAboutDLGPrivate();
}




#endif
