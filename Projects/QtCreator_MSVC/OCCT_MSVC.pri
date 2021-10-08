#设置

OCCIncludePath =    $${PWD}/../../OCC-Lib/inc
OCCLibPath =        $${PWD}/../../OCC-Lib/MSVC_win64_vc14/lib

INCLUDEPATH += $$OCCIncludePath

win32:  LIBS += -L$$OCCLibPath/ -lTKBin
win32:  LIBS += -L$$OCCLibPath/ -lTKBinL
win32:  LIBS += -L$$OCCLibPath/ -lTKG3d
win32:  LIBS += -L$$OCCLibPath/ -lTKService
win32:  LIBS += -L$$OCCLibPath/ -lTKVRML
win32:  LIBS += -L$$OCCLibPath/ -lTKGeomAlgo
win32:  LIBS += -L$$OCCLibPath/ -lTKShHealing
win32:  LIBS += -L$$OCCLibPath/ -lTKXCAF
win32:  LIBS += -L$$OCCLibPath/ -lTKBinTObj
win32:  LIBS += -L$$OCCLibPath/ -lTKGeomBase
win32:  LIBS += -L$$OCCLibPath/ -lTKStdL
win32:  LIBS += -L$$OCCLibPath/ -lTKXDEIGES
win32:  LIBS += -L$$OCCLibPath/ -lTKBinXCAF
win32:  LIBS += -L$$OCCLibPath/ -lTKHLR
win32:  LIBS += -L$$OCCLibPath/ -lTKStd
win32:  LIBS += -L$$OCCLibPath/ -lTKXDESTEP
win32:  LIBS += -L$$OCCLibPath/ -lTKBool
win32:  LIBS += -L$$OCCLibPath/ -lTKIGES
win32:  LIBS += -L$$OCCLibPath/ -lTKSTEP209
win32:  LIBS += -L$$OCCLibPath/ -lTKXMesh
win32:  LIBS += -L$$OCCLibPath/ -lTKBO
win32:  LIBS += -L$$OCCLibPath/ -lTKLCAF
win32:  LIBS += -L$$OCCLibPath/ -lTKSTEPAttr
win32:  LIBS += -L$$OCCLibPath/ -lTKXmlL
win32:  LIBS += -L$$OCCLibPath/ -lTKBRep
win32:  LIBS += -L$$OCCLibPath/ -lTKMath
win32:  LIBS += -L$$OCCLibPath/ -lTKSTEPBase
win32:  LIBS += -L$$OCCLibPath/ -lTKXml
win32:  LIBS += -L$$OCCLibPath/ -lTKCAF
win32:  LIBS += -L$$OCCLibPath/ -lTKMesh
win32:  LIBS += -L$$OCCLibPath/ -lTKSTEP
win32:  LIBS += -L$$OCCLibPath/ -lTKXmlTObj
win32:  LIBS += -L$$OCCLibPath/ -lTKCDF
win32:  LIBS += -L$$OCCLibPath/ -lTKMeshVS
win32:  LIBS += -L$$OCCLibPath/ -lTKSTL
win32:  LIBS += -L$$OCCLibPath/ -lTKXmlXCAF
win32:  LIBS += -L$$OCCLibPath/ -lTKernel
win32:  LIBS += -L$$OCCLibPath/ -lTKOffset
win32:  LIBS += -L$$OCCLibPath/ -lTKTObj
win32:  LIBS += -L$$OCCLibPath/ -lTKXSBase
win32:  LIBS += -L$$OCCLibPath/ -lTKFeat
win32:  LIBS += -L$$OCCLibPath/ -lTKOpenGl
win32:  LIBS += -L$$OCCLibPath/ -lTKTopAlgo
win32:  LIBS += -L$$OCCLibPath/ -lTKFillet
win32:  LIBS += -L$$OCCLibPath/ -lTKPrim
win32:  LIBS += -L$$OCCLibPath/ -lTKV3d
win32:  LIBS += -L$$OCCLibPath/ -lTKG2d
win32:  LIBS += -L$$OCCLibPath/ -lTKRWMesh
win32:  LIBS += -L$$OCCLibPath/ -lTKVCAF

INCLUDEPATH += $$OCCLibPath
DEPENDPATH += $$OCCLibPath

#gl_lib_path = lib/
#LIBS += -lEGL
