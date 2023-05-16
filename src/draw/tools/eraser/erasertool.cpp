// Copyright(c) 2017-2018 Alejandro Sirgo Rica & Contributors
//
// This file is part of Flameshot.
//
//     Flameshot is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     Flameshot is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with Flameshot.  If not, see <http://www.gnu.org/licenses/>.

#include "erasertool.h"
#include <QPainter>

EraserTool::EraserTool(QObject *parent) : AbstractPathTool(parent) {

}

QIcon EraserTool::icon(const QColor &background, bool inEditor) const {
    Q_UNUSED(inEditor);
    return QIcon(iconPath(background) + "pencil.svg");
}
QString EraserTool::name() const {
    return tr("Pencil");
}

QString EraserTool::nameID() {
    return "";
}

QString EraserTool::description() const {
    return tr("Sets the Pencil as the paint tool");
}

CaptureTool* EraserTool::copy(QObject *parent) {
    return new EraserTool(parent);
}

void EraserTool::process(QPainter &painter, const QPixmap &pixmap, bool recordUndo) {
    if (recordUndo) {
        updateBackup(pixmap);
    }
    painter.setPen(QPen(m_color, m_thickness));
    painter.drawPolyline(m_points.data(), m_points.size());
}

void EraserTool::paintMousePreview(QPainter &painter, const CaptureContext &context) {
    painter.setPen(QPen(context.color, context.thickness + 2));
    painter.drawLine(context.mousePos, context.mousePos);
}

void EraserTool::drawStart(const CaptureContext &context) {
    m_color = context.color;
    m_thickness = context.thickness + 2;
    m_points.append(context.mousePos);
    m_backupArea.setTopLeft(context.mousePos);
    m_backupArea.setBottomRight(context.mousePos);
}

void EraserTool::pressed(const CaptureContext &context) {
    Q_UNUSED(context);
}
