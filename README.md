ondragc
=======
This is a C++ library whhat implements an incremental mark&sweep garbage collector. It's designated for old C++ compilers
(only namespaces are needed). References between collected objects are kept by smart pointers. The collector is written
above standard runtime memory heap, hence it can be freely mixed with another allocation models. The collector supports
C++ exceptions including ones thrown from object constructors.

The library is distributed under the LGPL 3.0 license. Hence, it can be used in commercial projects. However, changes
and fixes in the library self must be publicly accessible.
