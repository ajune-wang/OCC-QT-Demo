#include "mdichild.h"
#include "ComplainUtf8.h"
#include "OcctView.h"
#include "GUI_Message.h"
#include <QtWidgets>

void MdiChild::Init()
{
	//GUI_Message
	{
		p_MessageDock = new QDockWidget(this);
		p_MessageWidget = new GUI_Message(p_MessageDock);
		p_MessageDock->setMinimumWidth(300);
		p_MessageDock->setWidget(p_MessageWidget);
		p_MessageDock->setWindowTitle(tr("ÏûÏ¢ÊÓÍ¼"));
		p_MessageDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
		p_MessageDock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
		this->addDockWidget(Qt::BottomDockWidgetArea, p_MessageDock);
		p_MessageDock->hide();
	}

	//OCCVIEW
	{
		QWidget * p3DWidget = new QWidget(this);
		QHBoxLayout * pLayout = new QHBoxLayout();
		pLayout->setMargin(0);
		p3DWidget->setLayout(pLayout);
		q3dView = new OcctView(h_MyViewer->getAisContext(), this);
		connect(q3dView, &OcctView::selectionChanged, this, &MdiChild::selectionChanged);
		pLayout->addWidget(q3dView);

		QToolBar * pToolbar = new QToolBar(p3DWidget);
		pToolbar->setOrientation(Qt::Vertical);
		pToolbar->setIconSize(QSize(20, 20));
		pToolbar->addActions(*q3dView->getViewActions());
		pToolbar->addActions(*q3dView->getRaytraceActions());
		pLayout->addWidget(pToolbar);
		this->setCentralWidget(p3DWidget);
	}
}