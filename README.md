# Playing with Conics and OpenGL on OS X

Welcome.  These are code sketches relating to conic sections and OpenGL.  It's all very humble and intentionally non-exemplary.  I just want to get some ideas out there that have been on my mind for a long time.  We can do better with our math textbooks, don't you think?


It would be nice to have a couple different views on the same thing.  Maybe 2D in one window and 3D in the other.  The following is a stripped down version of some [nice code](http://study.marearts.com/2012/01/sample-source-to-make-subwindow-in.html) from JH Kim:

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

```
    // Configure a map to support downstream evaluation of
    // a 3rd order Bezier curve.

    //          +-- Triggers generation of glVertex3 commands by evaluator.
    //          |
    //          |              +-- min parametric value
    //          |              |
    //          |              |    +-- max parametric value
    //          |              |    |
    //          |              |    |   +-- stride, # of values between gBezierControlPoints.
    //          |              |    |   |   So a 3d control point will have x, y, z
    //          |              |    |   |   values between control point boundaries,
    //          |              |    |   |   or a stride of 3
    //          |              |    |   |
    //          |              |    |   |  +-- number of control points
    //          |              |    |   |  |
    //          |              |    |   |  |   +-- ptr to array of control points
    //          |              |    |   |  |   |

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &gBezierControlPoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);

    ...

   glBegin(GL_LINE_STRIP);
      //
      // Subdivide the curve into steps between the min and max
      // parametric values, evaluating the curve as we go and
      // issuing corresponding glVertex3 calls.
      //
      // Within a GL_LINE_STRIP context, effect is to join points
      // along the curve by straight lines, approximating the
      // overall curve.
      //
      for (i = 0; i <= 30; i++)
         glEvalCoord1f((GLfloat) i/30.0);
   glEnd();
```

Render a 2D and 3D parabola.  Illumination obviously needs some work, but incrementally interesting.

![alt tag](view/img/hello-split-parabola.png)

Better 3D parabola with transparency.

![alt tag](view/img/hello-transparency-1.png)

Start exploring how to mix user-interface elements with OpenGL graphics, courtesy [GLUI](https://github.com/libglui/glui) and Paul Rademacher, et al.

![alt tag](controller/img/hello-checkbox-wire.png)

![alt tag](controller/img/hello-checkbox-solid.png)

Getting libglui to build required digging into [cmake](https://cmake.org).
![alt tag](controller/img/cmake.png)

And here are the rudiments of a nice little user interface, ready to modify.

![alt tag](controller/img/hello-ui-1.png)

Teach a plane to intersect a cone at any angle and offset to make conics sections.

![alt tag](controller/img/hello-conics.png)

Add slider control for variable cone generator angle.

![alt tag](controller/img/hello-conics-1.png)

Get transparency slider working for plane.

![alt tag](controller/img/hello-conics-2.png)

Add pre-set conic section buttons and strip the UI down to essentials.

![alt tag](controller/img/hello-conics-6.png)

Add a clip plane for conic cross-section.

![alt tag](controller/img/hello-conics-7.png)

So I have something I can demo.  But the code needs attention:

* It's mainly old-school C with global variables running around. The code is already prone to side-effects and will be difficult to evolve and I know I'm missing out on the virtues of object oriented languages.

* The windowing and user interface libraries were expedient choices that offered some helpful example code, but they're a bit outdated. It'd be nice to use something more modern and ready for mobile.  I /am/ liking cmake, though, and its portable goodness.

* My primary demo app, hello-conics-7, is structured as one file. I should really think about patterns a bit and split things out across multiple files.

Decisions:

* Make the code more object oriented and C++-isifed.
* Port from GLUT/GLUI to SDL2.
* Split code across files and beef up the build environment a bit.

The other approach would be webgl and js.  I guess that would take me down the webapp / react-native path for mobile.  But I've got some momentum with C/C++ and want to wrangle in that playground awhile.

These look helpful:

[C++](https://www.youtube.com/playlist?list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&disable_polymer=true)

[Abstracting OpenGL into C++ classes](https://www.youtube.com/watch?v=bTHqmzjm2UI)

[Modern OpenGL with C++](https://www.youtube.com/watch?v=ftiKrP3gW3k)

The action has moved [here](https://github.com/zenglenn42/opengl-tutorial-tbb) for the moment.
