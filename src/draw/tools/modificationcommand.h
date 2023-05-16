#ifndef MODIFICATIONCOMMAND_H
#define MODIFICATIONCOMMAND_H


#include <QObject>
#include <QUndoCommand>
#include "capturetool.h"

class ModificationCommand : public QUndoCommand {
public:
    ModificationCommand(QPixmap *, CaptureTool *);

    virtual void undo() override;
    virtual void redo() override;

private:
    QPixmap *m_pixmap;
    QScopedPointer<CaptureTool> m_tool;
};


#endif // MODIFICATIONCOMMAND_H
