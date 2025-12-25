#if !defined(MiscCommonMacros_Included)
#define MiscCommonMacros_Included

#if (defined(__linux__))
	#define VMT_DTOR_PADDING(ID) void* __linux_vmt_padding##ID;
#elif (defined(_WIN32) || defined(_WIN64))
	#define VMT_DTOR_PADDING(ID)
#endif

#if (defined(__linux__))
	#define VSTDLIB_DEFINE "libvstdlib"
#elif (defined(_WIN32) || defined(_WIN64))
	#define VSTDLIB_DEFINE "vstdlib"
#endif

#endif