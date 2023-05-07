// shared_lib.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "shared_lib.h"


// This is an example of an exported variable
SHAREDLIB_API int nsharedlib=0;

// This is an example of an exported function.
SHAREDLIB_API int fnsharedlib(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Csharedlib::Csharedlib()
{
    return;
}
