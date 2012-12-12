/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Johannes Zellner <webmaster@nebulon.de>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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

#include "volumepopup.h"

#include "audiodevice.h"

#include <qtxdg/xdgicon.h>

#include <QtGui/QSlider>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QLabel>
#include <QtCore/QProcess>

VolumePopup::VolumePopup(QWidget* parent):
    QWidget(parent,  Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint),
    m_pos(0,0),
    m_anchor(Qt::TopLeftCorner),
    m_device(0)
{
    m_mixerButton = new QLabel(this);
    m_mixerButton->setMargin(5);
    m_mixerButton->setToolTip(tr("Launch mixer"));
    m_mixerButton->setTextFormat(Qt::RichText);
    m_mixerButton->setText(QString("<a href=\"#\">%1</a>").arg(tr("Mixer")));

    m_volumeSlider = new QSlider(Qt::Vertical, this);
    m_volumeSlider->setTickPosition(QSlider::TicksBothSides);
    m_volumeSlider->setTickInterval(10);

    m_muteToggleButton = new QPushButton(this);
    m_muteToggleButton->setIcon(XdgIcon::fromTheme(QStringList() << "audio-volume-muted"));
    m_muteToggleButton->setCheckable(true);
    m_muteToggleButton->setFlat(true);

    QVBoxLayout *l = new QVBoxLayout(this);
    l->setSpacing(0);
    l->setMargin(2);

    l->addWidget(m_mixerButton, 0, Qt::AlignHCenter);
    l->addWidget(m_volumeSlider, 0, Qt::AlignHCenter);
    l->addWidget(m_muteToggleButton, 0, Qt::AlignHCenter);

    connect(m_mixerButton, SIGNAL(linkActivated(QString)), this, SIGNAL(launchMixer()));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(handleSliderValueChanged(int)));
    connect(m_muteToggleButton, SIGNAL(clicked()), this, SLOT(handleMuteToggleClicked()));
}

void VolumePopup::enterEvent(QEvent *event)
{
    emit mouseEntered();
}

void VolumePopup::leaveEvent(QEvent *event)
{
    emit mouseLeft();
}

void VolumePopup::handleSliderValueChanged(int value)
{
    if (!m_device)
        return;

    m_device->setVolume(value);
}

void VolumePopup::handleMuteToggleClicked()
{
    if (!m_device)
        return;

    m_device->toggleMute();
}

void VolumePopup::handleDeviceVolumeChanged(int volume)
{
    m_volumeSlider->setValue(volume);
    updateStockIcon();
}

void VolumePopup::handleDeviceMuteChanged(bool mute)
{
    m_muteToggleButton->setChecked(mute);
    updateStockIcon();
}

void VolumePopup::updateStockIcon()
{
    if (!m_device)
        return;

    QString iconName;
    if (m_device->volume() <= 0 || m_device->mute())
        iconName = "audio-volume-muted";
    else if (m_device->volume() <= 33)
        iconName = "audio-volume-low";
    else if (m_device->volume() <= 66)
        iconName = "audio-volume-medium";
    else
        iconName = "audio-volume-high";

    m_muteToggleButton->setIcon(XdgIcon::fromTheme(iconName));
    emit stockIconChanged(iconName);
}

void VolumePopup::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    realign();
}

void VolumePopup::open(QPoint pos, Qt::Corner anchor)
{
    m_pos = pos;
    m_anchor = anchor;
    realign();
    show();
}

void VolumePopup::handleWheelEvent(QWheelEvent *event)
{
    m_volumeSlider->event(reinterpret_cast<QEvent*>(event));
}

void VolumePopup::setDevice(AudioDevice *device)
{
    if (device == m_device)
        return;

    // disconnect old device
    if (m_device)
        disconnect(m_device);

    m_device = device;

    if (m_device) {
        m_volumeSlider->setValue(m_device->volume());
        m_muteToggleButton->setChecked(m_device->mute());

        connect(m_device, SIGNAL(volumeChanged(int)), this, SLOT(handleDeviceVolumeChanged(int)));
        connect(m_device, SIGNAL(muteChanged(bool)), this, SLOT(handleDeviceMuteChanged(bool)));
    }

    updateStockIcon();
    emit deviceChanged();
}

void VolumePopup::setSliderStep(int step)
{
    m_volumeSlider->setSingleStep(step);
    m_volumeSlider->setPageStep(step * 10);
}

void VolumePopup::realign()
{
    QRect rect;
    rect.setSize(sizeHint());
    switch (m_anchor)
    {
    case Qt::TopLeftCorner:
        rect.moveTopLeft(m_pos);
        break;

    case Qt::TopRightCorner:
        rect.moveTopRight(m_pos);
        break;

    case Qt::BottomLeftCorner:
        rect.moveBottomLeft(m_pos);
        break;

    case Qt::BottomRightCorner:
        rect.moveBottomRight(m_pos);
        break;

    }

    QRect screen = QApplication::desktop()->availableGeometry(m_pos);

    if (rect.right() > screen.right())
        rect.moveRight(screen.right());

    if (rect.bottom() > screen.bottom())
        rect.moveBottom(screen.bottom());

    move(rect.topLeft());
}
