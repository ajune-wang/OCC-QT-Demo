#pragma once

#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>

class OCCT_GraphDriver
{
private:
	OCCT_GraphDriver(void) {}
	~OCCT_GraphDriver(void) {}
public:
	//创建绘图器
	static Handle(V3d_Viewer) CreateViewer();

	//创建创建交互式上下文
	static Handle(AIS_InteractiveContext) CreateAISContext(const Handle(V3d_Viewer) & hViewer);

	static Handle(AIS_InteractiveContext) CreateViewCubeAISContext(const Handle(V3d_Viewer) & hViewer);

	static Handle(AIS_InteractiveContext) CreateXYZAISContext(const Handle(V3d_Viewer) & hViewer);

	//用当前绘图器绘图器创建视窗
	static Handle(V3d_View) CreateView(const Handle(V3d_Viewer) & hViewer);
};