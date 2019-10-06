# EasyCSG

EasyCSG is a Windows operating system program focusing on Constructive Solid Geometry (CSG).

CSG is a solid modeling technique, which allows to create complex objects by using boolean operations (subtraction, intersection, union) between simple objects, called primitives (cube, sphere, cylinder, cone, pyramid, torus). There are two ways to achieve a CSG result: CSG rendering or CSG calculating. EasyCSG implements both of them. The first one uses the graphics card to get a CSG result visually. It is faster than the calculating part, which is time consuming because it generates the vertices, normals and textures of the final complex object.

The goal of EasyCSG was to create an easy to use, fast, robust and lightweight program.

It is written in C/C++ and uses:

[]()|[]()
------|------
Application programming interface | Windows API (Win32)
Graphical user interface | Windows Ribbon, OpenGL
OpenGL loading library | [glew](http://glew.sourceforge.net/)
CSG rendering | [OpenCSG](http://opencsg.org/)
CSG calcualting | [Carve](https://github.com/folded/carve/)
Mathematical library | [GLM](https://glm.g-truc.net/)
Image library | [STB](https://github.com/nothings/stb/)
Font library | [FreeType](https://freetype.org/)
Memory leak detection | [Visual Leak Detector](https://kinddragon.github.io/vld/)
Icons | [icons8](https://icons8.com/)
Web server | [miniweb](http://miniweb.sourceforge.net/)
[]()|[]()

EasyCSG is developed in Visual Studio 2019 Community. The project contains all files and libraries which are needed for futher development. The Code is divided into two main parts: cross platform and Windows platform.

## Gallery
![](https://raw.githubusercontent.com/Sirkisian/EasyCSG/master/EasyCSG%20images/2objects.png)
[1] Primitives in EasyCSG

![](https://raw.githubusercontent.com/Sirkisian/EasyCSG/master/EasyCSG%20images/3light_texture.png)
[2] Lights/Materials and textures

![](https://raw.githubusercontent.com/Sirkisian/EasyCSG/master/EasyCSG%20images/4opencsg2.png)
[3] CSG rendering result

![](https://raw.githubusercontent.com/Sirkisian/EasyCSG/master/EasyCSG%20images/5carve.png)
[4] CSG calculating result

### To-do
- Choose and implement a file format for export/import
- Add call stack for errors
- Add xml output for logfile
- Add more primitives (cone, pyramid, torus)
- Add action to ribbon buttons in menu (open, save, save as, logfile)
- Add lighting and texturing for carve objects
- Add shader for drawing of carve objects
- Implement triangulation or tessellation for carve objects
- Add database upload and download for projects
- Add code comments
