# zxvis

a data visualization library and application based on vulkan

data visualization of Scientometrics (papers)

### primitives

* object
  
  * [x] rectangle
  
  * [x] circle
  
  * [x] pie
  
  * [ ] spherical
  
  * [ ] cube

* lines
  
  * [ ] bezier

* text rendering

  
### interaction
  
* [ ] imgui
* [ ] mouse and keyboard event
* [ ] set primitives size and choose items
* [ ] import excel from path

### Refactor

* [ ] CPU raster
* [ ] GPU vector rendering
* [ ] 3D object rendering

### build:

we only test code on windows using visual studio

to build the project on windows, using:

```null
replace "E:/vulkansdk/Include" and "E:/vulkansdk/Lib" in "CMakeLists.txt" with your own path of vulkan

cd zxvis
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
```
