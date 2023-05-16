#include "drawtool.h"
#include <QDebug>

DrawTool::DrawTool(int x,int y,int w,int h,QWidget *parent) : QWidget(parent)
{
    this->setGeometry(x,y,w,h);

    int startY = 15;
    int btnw = 50;
    int space = 60;
    ToolButton* btn1 = new ToolButton(0,":/image/btn_1.png",this);
    btn1->setGeometry(0,startY,btnw,btnw);
    btn1->show();
    m_Vector.push_back(btn1);

    ToolButton* btn2 = new ToolButton(1,":/image/btn_0.png",this);
    btn2->setGeometry(0,startY+space,btnw,btnw);
    btn2->show();
    m_Vector.push_back(btn2);

    ToolButton* btn3 = new ToolButton(2,":/image/btn_2.png",this);
    btn3->setGeometry(0,startY+space * 2,btnw,btnw);
    btn3->show();
    m_Vector.push_back(btn3);

    ToolButton* btn4 = new ToolButton(3,":/image/btn_3.png",this);
    btn4->setGeometry(0,startY+space * 3,btnw,btnw);
    btn4->hide();
    m_Vector.push_back(btn4);



    ToolButton* btn5 = new ToolButton(4,"",this);
    btn5->setCheckable(true);
    btn5->setGeometry(space,startY+space - 30,btnw,btnw);
    btn5->show();
    m_Vector.push_back(btn5);

    ToolButton* btn6 = new ToolButton(5,":/image/shape_0.png",this);
    btn6->setCheckable(true);
    btn6->setGeometry(space,startY+space*2 - 30,btnw,btnw);
    btn6->show();
    m_Vector.push_back(btn6);


    m_Picker = new ColorPicker(true,this);
    m_Picker->move(17,2);
    m_Picker->hide();
    connect(m_Picker,SIGNAL(sigSelected(int)),this,SLOT(sltColor(int)));


    m_Shape = new ColorPicker(false,this);
    m_Shape->move(17,62);
    m_Shape->hide();
    connect(m_Shape,SIGNAL(sigSelected(int)),this,SLOT(sltShape(int)));



    for(int i = 0;i < m_Vector.size();++i)
    {
        connect(m_Vector.at(i),SIGNAL(clicked(bool)),this,SLOT(sltSender(bool)));
    }

}

bool DrawTool::isWhiteBoard()
{
    return m_Vector.last()->isChecked();
}

void DrawTool::sltSender(bool flag)
{
    ToolButton* btn = (ToolButton*)sender();
    switch (btn->getId()) {
    case 4:
    {
        m_Shape->hide();
        m_Picker->raise();
        btn->raise();

        m_Picker->setVisible(flag);

    }
        break;
    case 5:
    {
        m_Picker->hide();
        m_Shape->raise();
        btn->raise();

        m_Shape->setVisible(flag);

    }
        break;
    default:
        emit sigTool(btn->getId(),btn->isChecked());
        break;
    }
}

void DrawTool::sltShape(int value)
{
    m_Shape->hide();
    m_Vector.at(5)->setChecked(false);
    m_Vector.at(5)->setPixmalUrl(QString(":/image/shape_%1.png").arg(value));


    emit sigSelectShape(value);
}

void DrawTool::sltColor(int value)
{
    QVector<QColor> colors = m_Picker->getUserColors();

    m_Picker->hide();
    m_Vector.at(4)->setChecked(false);
    m_Vector.at(4)->setColor(colors.at(value));

    emit sigSelectColor(colors.at(value));
}
