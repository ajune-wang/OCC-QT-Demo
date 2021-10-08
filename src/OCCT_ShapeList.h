#pragma once

#include <NCollection_List.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

typedef class NCollection_List<TopoDS_Shape>          OCCT_ShapeList;
typedef class NCollection_List<TopoDS_Solid>          OCCT_SolidList;
typedef class NCollection_List<TopoDS_Face>           OCCT_FaceList;
typedef class NCollection_List<TopoDS_Edge>           OCCT_EdgeList;
typedef class NCollection_List<TopoDS_Vertex>         OCCT_VertexList;

inline OCCT_ShapeList & operator+=(OCCT_ShapeList & ShapeList, const TopoDS_Shape & Shape)
{
	ShapeList.Append(Shape);
	return ShapeList;
}
inline OCCT_ShapeList & operator-=(OCCT_ShapeList & ShapeList, const TopoDS_Shape & Shape)
{
	ShapeList.Remove(Shape);
	return ShapeList;
}
inline OCCT_ShapeList & operator+=(OCCT_ShapeList & ShapeList, const OCCT_ShapeList & Other)
{
	for (OCCT_ShapeList::Iterator Itor(Other); Itor.More(); Itor.Next())
		ShapeList.Append(Itor.Value());
	return ShapeList;
}
inline OCCT_ShapeList & operator-=(OCCT_ShapeList & ShapeList, const OCCT_ShapeList & Other)
{
	for (OCCT_ShapeList::Iterator Itor(Other); Itor.More(); Itor.Next())
		ShapeList.Remove(Itor.Value());
	return ShapeList;
}

inline OCCT_SolidList & operator+=(OCCT_SolidList & SolidList, const TopoDS_Solid & Solid)
{
	SolidList.Append(Solid);
	return SolidList;
}
inline OCCT_SolidList & operator-=(OCCT_SolidList & SolidList, const TopoDS_Solid & Solid)
{
	SolidList.Remove(Solid);
	return SolidList;
}
inline OCCT_SolidList & operator+=(OCCT_SolidList & SolidList, const OCCT_SolidList & Other)
{
	for (OCCT_SolidList::Iterator Itor(Other); Itor.More(); Itor.Next())
		SolidList.Append(Itor.Value());
	return SolidList;
}
inline OCCT_SolidList & operator-=(OCCT_SolidList & SolidList, const OCCT_SolidList & Other)
{
	for (OCCT_SolidList::Iterator Itor(Other); Itor.More(); Itor.Next())
		SolidList.Remove(Itor.Value());
	return SolidList;
}

inline OCCT_FaceList & operator+=(OCCT_FaceList & FaceList, const TopoDS_Face & Face)
{
	FaceList.Append(Face);
	return FaceList;
}
inline OCCT_FaceList & operator-=(OCCT_FaceList & FaceList, const TopoDS_Face & Face)
{
	FaceList.Remove(Face);
	return FaceList;
}
inline OCCT_FaceList & operator+=(OCCT_FaceList & FaceList, const OCCT_FaceList & Other)
{
	for (OCCT_FaceList::Iterator Itor(Other); Itor.More(); Itor.Next())
		FaceList.Append(Itor.Value());
	return FaceList;
}
inline OCCT_FaceList & operator-=(OCCT_FaceList & FaceList, const OCCT_FaceList & Other)
{
	for (OCCT_FaceList::Iterator Itor(Other); Itor.More(); Itor.Next())
		FaceList.Remove(Itor.Value());
	return FaceList;
}

inline OCCT_EdgeList & operator+=(OCCT_EdgeList & EdgeList, const TopoDS_Edge & Edge)
{
	EdgeList.Append(Edge);
	return EdgeList;
}
inline OCCT_EdgeList & operator-=(OCCT_EdgeList & EdgeList, const TopoDS_Edge & Edge)
{
	EdgeList.Remove(Edge);
	return EdgeList;
}
inline OCCT_EdgeList & operator+=(OCCT_EdgeList & EdgeList, const OCCT_EdgeList & Other)
{
	for (OCCT_EdgeList::Iterator Itor(Other); Itor.More(); Itor.Next())
		EdgeList.Append(Itor.Value());
	return EdgeList;
}
inline OCCT_EdgeList & operator-=(OCCT_EdgeList & EdgeList, const OCCT_EdgeList & Other)
{
	for (OCCT_EdgeList::Iterator Itor(Other); Itor.More(); Itor.Next())
		EdgeList.Remove(Itor.Value());
	return EdgeList;
}

inline OCCT_VertexList & operator+=(OCCT_VertexList & VertexList, const TopoDS_Vertex & Vertex)
{
	VertexList.Append(Vertex);
	return VertexList;
}
inline OCCT_VertexList & operator-=(OCCT_VertexList & VertexList, const TopoDS_Vertex & Vertex)
{
	VertexList.Remove(Vertex);
	return VertexList;
}
inline OCCT_VertexList & operator+=(OCCT_VertexList & VertexList, const OCCT_VertexList & Other)
{
	for (OCCT_VertexList::Iterator Itor(Other); Itor.More(); Itor.Next())
		VertexList.Append(Itor.Value());
	return VertexList;
}
inline OCCT_VertexList & operator-=(OCCT_VertexList & VertexList, const OCCT_VertexList & Other)
{
	for (OCCT_VertexList::Iterator Itor(Other); Itor.More(); Itor.Next())
		VertexList.Remove(Itor.Value());
	return VertexList;
}

inline OCCT_ShapeList & operator<<(OCCT_ShapeList & List, const TopoDS_Shape & Shape)
{
	List.Append(Shape);
	return List;
}
inline OCCT_SolidList & operator<<(OCCT_SolidList & List, const TopoDS_Solid & Shape)
{
	List.Append(Shape);
	return List;
}
inline OCCT_FaceList & operator<<(OCCT_FaceList & List, const TopoDS_Face & Shape)
{
	List.Append(Shape);
	return List;
}
inline OCCT_EdgeList & operator<<(OCCT_EdgeList & List, const TopoDS_Edge & Shape)
{
	List.Append(Shape);
	return List;
}
inline OCCT_VertexList & operator<<(OCCT_VertexList & List, const TopoDS_Vertex & Shape)
{
	List.Append(Shape);
	return List;
}
