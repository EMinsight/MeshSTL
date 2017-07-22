Purpose
=======

This program is designed to convert unstructured surface triangulations
into volumetric hexahedral meshes for finite element analysis. The input
file format can be either ASCII or binary STL and the output file format
is Abaqus’s INP.

Disclaimer
==========

This program was an educational project, and is no longer maintained. The 
code is amateur at best, and there is no guarentee as to the validity of
the output. 

Directions
==========

This program was purposely designed to be relatively simple to use. To
compile and use this code, the user will need the GNU g++ compiler and
Make capability. The directions are written for a Linux system.

1.  Download “MeshSTL.tar.gz” and unpack it.

2.  Generate the executable “meshstl” by using the make command and
    execute it.

3.  Type the full path of the STL file you would like to convert and
    press enter. Make sure that you include the “.stl” suffix.

4.  Type the number of hexes you would like per axis and press enter.
    The main body of the program will now execute. If the STL file is
    particularly large or you requested a large number of hexes, this
    will take some time.

5.  The program has finished when it displays the total elapsed time.

6.  The resulting INP file is written to the directory that the
    executable was stored in. Later versions will support writing to
    different directories.

Methodology
===========

Please see the user guide for a detailed descripton of the methodology.
