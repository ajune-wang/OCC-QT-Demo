# OCC-QT-Demo

## 1.环境安装
1.0 VS2015/VS2017/VS2019  

1.1 QT安装  
QT5.12.7 https://download.qt.io/archive/qt/5.12/5.12.7/qt-opensource-windows-x86-5.12.7.exe  
QT for VS 插件 https://download.qt.io/official_releases/vsaddin/2.5.2/qt-vsaddin-msvc2015-2.5.2-rev.01.vsix  

1.2 OCCT  
OpenCascade7.5.0已经安装并且编译好，无其他三方库依赖（精简版本）  

## 2.VS项目启动
2.1 vs2015  
启动  projects/VS2015/msvc_new.bat  

2.2 QT Creator+VC14（VS2015编译器）  
启动  projects/QtCreator_MSVC/msvc_pro.bat  

2.3 QT Creator+MinGW(安装QT时可安装)  
启动  projects/QtCreator_MINGW/mingw_pro.bat  

2.4 旧的方式  
旧的启动方式请参照README_OLD.md  
*安装设置OCCT然后修改env.bat*  
启动  projects/VS2015/_msvc_old.bat  