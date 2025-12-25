#if !defined(MiscIColorCorrectionSystem_Included)
#define MiscIColorCorrectionSystem_Included

#include "Misc/ImageFormat.h"
#include "Misc/BaseTypes.h"

typedef unsigned int ColorCorrectionHandle_t;
typedef struct ShaderColorCorrectionInfo_t ShaderColorCorrectionInfo_t;

typedef struct IColorCorrectionSystem IColorCorrectionSystem;

typedef struct IColorCorrectionSystem_VMT
{
	void (*Init)(IColorCorrectionSystem*);
	void (*Shutdown)(IColorCorrectionSystem*);
	ColorCorrectionHandle_t (*AddLookup)(IColorCorrectionSystem*, const char* pName);
	bool (*RemoveLookup)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle);
	void (*SetLookupWeight)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle, float flWeight);
	float (*GetLookupWeightHandle)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle);
	float (*GetLookupWeight)(IColorCorrectionSystem*, int i);
	void (*LockLookup)(IColorCorrectionSystem*);
	void (*LockLookupHandle)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle);
	void (*UnlockLookup)(IColorCorrectionSystem*);
	void (*UnlockLookupHandle)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle);
	void (*SetLookup)(IColorCorrectionSystem*, RGBX5551_t inColor, color24 outColor);
	void (*SetLookupHandle)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle, RGBX5551_t inColor, color24 outColor);
	color24 (*GetLookup)(IColorCorrectionSystem*, RGBX5551_t inColor);
	color24 (*GetLookupHandle)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle, RGBX5551_t inColor);
	void (*LoadLookup)(IColorCorrectionSystem*, const char* pLookupName);
	void (*LoadLookupHandle)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle, const char* pLookupName);
	void (*CopyLookup)(IColorCorrectionSystem*, const color24* pSrcColorCorrection);
	void (*CopyLookupHandle)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle, const color24* pSrcColorCorrection);
	void (*ResetLookupHandle)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle);
	void (*ResetLookup)(IColorCorrectionSystem*);
	void (*ReleaseTextures)(IColorCorrectionSystem*);
	void (*RestoreTextures)(IColorCorrectionSystem*);
	void (*ResetLookupWeights)(IColorCorrectionSystem*);
	int (*GetNumLookups)(IColorCorrectionSystem*);
	color24 (*ConvertToColor24)(IColorCorrectionSystem*, RGBX5551_t inColor);
	void (*SetResetable)(IColorCorrectionSystem*, ColorCorrectionHandle_t handle, bool bResetable);
	void (*EnableColorCorrection)(IColorCorrectionSystem*, bool bEnable);
	void (*GetCurrentColorCorrection)(IColorCorrectionSystem*, ShaderColorCorrectionInfo_t* pInfo);
} IColorCorrectionSystem_VMT;

struct IColorCorrectionSystem
{
	IColorCorrectionSystem_VMT* vmt;
};

#endif