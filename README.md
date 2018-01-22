# Playing with Conics and OpenGL on OS X

Welcome.  These are code sketches relating to conic sections and OpenGL.  It's all very humble and intentionally non-exemplary.  I just want to get some ideas out there that have been on my mind for a long time.  We can do better with our math textbooks, don't you think?


It would be nice to have a couple different views on the same thing.  Maybe 2D in one window and 3D in the other.

![alt tag](view/img/hello-subWindows.png)

Oh, some text would be good too.  For now, I'm happy to get bitmapped text rendering.  When I'm ready to be fancy, I'll switch to strocked fonts.

![alt tag](view/img/hello-text.png)

A window that maintains overall aspect ratio even when it is resized would be useful, especially with math where we don't want to distort our rendered curves.

![alt tag](view/img/hello-ar-golden.png)

![alt tag](view/img/hello-ar-golden-big.png)

Make it easy to change the aspect ratio, too.

![alt tag](view/img/hello-ar-2to1.png)

And it would be nice to leverage some OpenGL primitives (like glMap1f() and glEvalCoord1f()) to render an actual conic:

![alt tag](view/img/hello-parabola.png)

Render a 2D and 3D parabola.  Illumination obviously needs some work, but incrementally interesting.

![alt tag](view/img/hello-split-parabolas.png)
