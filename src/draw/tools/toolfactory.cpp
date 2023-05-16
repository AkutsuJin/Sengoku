#include "toolfactory.h"
#include "arrow/arrowtool.h"
#include "circle/circletool.h"
#include "line/linetool.h"
#include "marker/markertool.h"
#include "pencil/penciltool.h"
#include "rectangle/rectangletool.h"
#include "selection/selectiontool.h"
#include "blur/blurtool.h"
#include "text/texttool.h"
#include "eraser/erasertool.h"

ToolFactory::ToolFactory(QObject *parent) : QObject(parent) {

}

CaptureTool* ToolFactory::CreateTool(
        ESHAPETYPE t,
        QObject *parent)
{
    CaptureTool *tool;
    switch (t) {
    case TYPE_ARROW:
        tool = new ArrowTool(parent);
        break;
    case TYPE_CIRCLE:
        tool = new CircleTool(parent);
        break;
    case TYPE_DRAWER:
        tool = new LineTool(parent);
        break;
    case TYPE_MARKER:
        tool = new MarkerTool(parent);
        break;
    case TYPE_PENCIL:
        tool = new PencilTool(parent);
        break;
    case TYPE_RECTANGLE:
        tool = new RectangleTool(parent);
        break;
    case TYPE_SELECTION:
        tool = new SelectionTool(parent);
        break;
    case TYPE_BLUR:
        tool = new BlurTool(parent);
        break;
    case TYPE_TEXT:
        tool = new TextTool(parent);
        break;
//    case TYPE_ERASER:
//        tool = new EraserTool(parent);
//        break;
    default:
        tool = nullptr;
        break;
    }
    return tool;
}
