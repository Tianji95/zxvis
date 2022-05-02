# zxvis
a data visualization library and application based on vulkan

data visualization of Scientometrics (papers)





- primitives

  - [x] rectangle
  - [ ] circle
  - [ ] pie
  - [ ] spherical
  - [ ] cube
  - [ ] 

- lines
  - [ ] bezier



### build：

we only test code on windows using visual studio

to build the project on windows, using:

```
replace "E:/vulkansdk/Include" and "E:/vulkansdk/Lib" in "CMakeLists.txt" with your own path of vulkan

cd zxvis
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
```

