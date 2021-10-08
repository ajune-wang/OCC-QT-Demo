# OCC-QT-Demo
OpenCacade7.5.0 QT5 示例
## 1.环境安装
1.0 VS2015/VS2017/VS2019  

1.1 下载OpenCascade7.5.0并安装  
官网 （外网可能有点慢）https://old.opencascade.com/content/latest-release  
微云 链接：https://share.weiyun.com/1bM7dI5G 密码：9v2nuf
  
1.2 QT安装  
QT5.12.7 https://download.qt.io/archive/qt/5.12/5.12.7/qt-opensource-windows-x86-5.12.7.exe  
QT for VS 插件 https://download.qt.io/official_releases/vsaddin/2.5.2/qt-vsaddin-msvc2015-2.5.2-rev.01.vsix  

1.3 安装视频  
环境配置可参照视频 https://www.bilibili.com/video/BV12T4y1P7Z3?share_source=copy_web  
## 2.VS项目启动
修改环境变量 *env.bat* 中  
```set "OCCT_7_5_0=E:\OpenCASCADE-7.5.0-vc14-64"```  
然后点击 *msvc.bat* 启动（务必）  
*PS：OpenCascade使用的其他三方库的需要配置，使用批命令（.bat）的方式启动省去了设置这些三方库*  
*OpenCascade安装路径下面的QT只有Release的库，因此只能编写Release版本的QT程序，建议另外安装QT。参照1.2下载地址和1.3安装视频*  
