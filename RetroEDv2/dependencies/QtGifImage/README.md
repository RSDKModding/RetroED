Documentation: http://qtgifimage.debao.me

Qt Gif Image is a Qt library that can read and write Gif files.
 
## Getting Started

### Usage(1): Use Qt Gif Image as Qt5's addon module

* Download the source code.

* Put the source code in any directory you like. At the toplevel directory run

**Note**: Perl is needed.

```
    qmake
    make
    make install
```

The library, the header files, and others will be installed to your system.

* Add following line to your qmake's project file:

```
    QT += gifimage
```

* Then, using Qt Gif Image in your code

### Usage(2): Use source code directly

The package contains a **qtgifimage.pri** file that allows you to integrate the component into applications that use qmake for the build step.

* Download the source code.

* Put the source code in any directory you like. For example, 3rdparty:

```
    |-- project.pro
    |-- ....
    |-- 3rdparty\
    |     |-- qtgifimage\
    |     |
```

* Add following line to your qmake project file:

```
    include(3rdparty/qtgifimage/src/gifimage/qtgifimage.pri)
```

* Then, using Qt Gif Image in your code

## References

* http://giflib.sourceforge.net/intro.html
