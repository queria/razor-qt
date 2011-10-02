/* BEGIN_COMMON_COPYRIGHT_HEADER
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2010-2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.ru>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;  only version 2 of
 * the License is valid for this program.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * END_COMMON_COPYRIGHT_HEADER */


#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtCore/QDebug>
#include <QtCore/QLibraryInfo>

#include "razorpanelapplication.h"
#include "razorpanel.h"
#include "razortranslate.h"

/*! The razor-panel is the panel of Razor-qt.
  Usage: razor-panel [CONFIG_ID]
    CONFIG_ID      Section name in config file ~/.razor/panel.conf
                   (default main)
 */

int main(int argc, char *argv[])
{
    RazorPanelApplication a(argc, argv);
    a.setWindowIcon(QIcon(QString(SHARE_DIR) + "/graphics/razor_logo.png"));

    TRANSLATE_APP;

    RazorPanel panel;
    a.setPanel(&panel);
    panel.show();

    return a.exec();

}
