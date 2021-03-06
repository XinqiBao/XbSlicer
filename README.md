# XbSlicer

A program for slicing 3D models in STL format to GCode file.

Some optimizations have been implemented, check [XbSlicer paper](https://xinqibao.xyz/file/AnOptimizedSlicerFor3DPrinting.pdf) for detail.

Yet this software itself is not the best, but it does something. It's a personal project for practice, helped by professor Dov Kruger, from Department of Electrical & Computer Engineering at Stevens Institute of Technology.

## How to run

run `make` to get the executable file.

`make MODE=release` get the release version.

run `xbslicer` or `xbslicer-staging` with input filename, otherwise it'll take a default file.

```
./xbslicer cube.stl
```

Input file has to be stored in `stl_files/`, output gcode file will be in `gcode_files/`.

It can take STL file both in ASCII and Binary format.

ASCII format file with extension `.stl`, Binary format file has to be with extension `.stlb`.

## TODO

Complicated models will require SupportMaterial to print actually stuff, XbSlicer is not support for this.

Still it can print some simple models with the GCode files generated by XbSlicer.

## To improve

For now, the config file is like hard coded, every time make some change in config file, it requires to build the whole project to make it work. It is really frustrating, config file need to be read-in on running time.

Also, the input and output file path is not flexible, as long as file extension, especially for Binary STL file.

For a good command running experience, it need to have support like parser, user can pass in arguments to modify something.
