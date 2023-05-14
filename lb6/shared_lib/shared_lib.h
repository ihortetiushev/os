// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SHAREDLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SHAREDLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SHAREDLIB_EXPORTS
#define SHAREDLIB_API __declspec(dllexport)
#else
#define SHAREDLIB_API __declspec(dllimport)
#endif

// This class is exported from the dll
class SHAREDLIB_API Csharedlib {
public:
	Csharedlib(void);
	// TODO: add your methods here.
};

extern SHAREDLIB_API int nsharedlib;

SHAREDLIB_API int fnsharedlib(void);
