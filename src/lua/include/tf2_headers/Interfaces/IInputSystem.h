#if !defined(IntfIInputSystem_Included)
#define IntfIInputSystem_Included

#include "Misc/IAppSystem.h"
#include "Misc/AnalogCode.h"
#include "Misc/ButtonCode.h"
#include "Misc/InputEnums.h"
// #include "../Steam/ISteamController.h"

typedef struct IInputSystem IInputSystem;

typedef struct IInputSystem_VMT
{
	IAppSystem_VMT vmt_IAppSystem;

	void (*AttachToWindow)(IInputSystem*, void* hWnd);
	void (*DetachFromWindow)(IInputSystem*);
	void (*EnableInput)(IInputSystem*, bool bEnable);
	void (*EnableMessagePump)(IInputSystem*, bool bEnable);
	void (*PollInputState)(IInputSystem*);
	int (*GetPollTick)(IInputSystem*); // const
	bool (*IsButtonDown)(IInputSystem*, ButtonCode_t code); // const
	int (*GetButtonPressedTick)(IInputSystem*, ButtonCode_t code); // const
	int (*GetButtonReleasedTick)(IInputSystem*, ButtonCode_t code); // const
	int (*GetAnalogValue)(IInputSystem*, AnalogCode_t code); // const
	int (*GetAnalogDelta)(IInputSystem*, AnalogCode_t code); // const
	int (*GetEventCount)(IInputSystem*); // const
	const InputEvent_t* (*GetEventData)(IInputSystem*); // const
	void (*PostUserEvent)(IInputSystem*, const InputEvent_t* event);
	int (*GetJoystickCount)(IInputSystem*); // const
	void (*EnableJoystickInput)(IInputSystem*, int nJoystick, bool bEnable);
	void (*EnableJoystickDiagonalPOV)(IInputSystem*, int nJoystick, bool bEnable);
	void (*SampleDevices)(IInputSystem*);
	void (*SetRumble)(IInputSystem*, float fLeftMotor, float fRightMotor, int userId);
	void (*StopRumble)(IInputSystem*);
	void (*ResetInputState)(IInputSystem*);
	void (*SetPrimaryUserId)(IInputSystem*, int userId);
	const char* (*ButtonCodeToString)(IInputSystem*, ButtonCode_t code); // const
	const char* (*AnalogCodeToString)(IInputSystem*, AnalogCode_t code); // const
	ButtonCode_t (*StringToButtonCode)(IInputSystem*, const char* pString); // const
	AnalogCode_t (*StringToAnalogCode)(IInputSystem*, const char* pString); // const
	void (*SleepUntilInput)(IInputSystem*, int nMaxSleepTimeMS);
	ButtonCode_t (*VirtualKeyToButtonCode)(IInputSystem*, int nVirtualKey); // const
	int (*ButtonCodeToVirtualKey)(IInputSystem*, ButtonCode_t code); // const
	ButtonCode_t (*ScanCodeToButtonCode)(IInputSystem*, int lParam); // const
	int (*GetPollCount)(IInputSystem*); // const
	void (*SetCursorPosition)(IInputSystem*, int x, int y);
	void* (*GetHapticsInterfaceAddress)(IInputSystem*); // const
	void (*SetNovintPure)(IInputSystem*, bool bPure);
	bool (*GetRawMouseAccumulators)(IInputSystem*, int* accumX, int* accumY);
	void (*SetConsoleTextMode)(IInputSystem*, bool bConsoleTextMode);
	// ISteamController* (*SteamControllerInterface)(IInputSystem*);
	// uint32 (*GetNumSteamControllersConnected)(IInputSystem*);
	// bool (*IsSteamControllerActive)(IInputSystem*);
	// bool (*IsSteamControllerConnected)(IInputSystem*);
	// int (*GetSteamControllerIndexForSlot)(IInputSystem*, int nSlot);
	// bool (*GetRadialMenuStickValues)(IInputSystem*, int nSlot, float& fX, float& fY);
	// void (*ActivateSteamControllerActionSetForSlot)(IInputSystem*, uint64 nSlot, GameActionSet_t eActionSet);
	// ControllerActionSetHandle_t (*GetActionSetHandle)(IInputSystem*, GameActionSet_t eActionSet);
	// ControllerActionSetHandle_t (*GetActionSetHandle)(IInputSystem*, const char* szActionSet);
	// EControllerActionOrigin (*GetSteamControllerActionOrigin)(IInputSystem*, const char* action, GameActionSet_t action_set);
	// EControllerActionOrigin (*GetSteamControllerActionOrigin)(IInputSystem*, const char* action, ControllerActionSetHandle_t action_set_handle);
	// const wchar_t* (*GetSteamControllerFontCharacterForActionOrigin)(IInputSystem*, EControllerActionOrigin origin);
	// const wchar_t* (*GetSteamControllerDescriptionForActionOrigin)(IInputSystem*, EControllerActionOrigin origin);
	// void (*SetSkipControllerInitialization)(IInputSystem*, bool bSkip);
} IInputSystem_VMT;

struct IInputSystem
{
	IInputSystem_VMT* vmt;
};

#define IInputSystem_REFL "IInputSystem"
#define IInputSystem_SIGNATURE "InputSystemVersion001"
#define IInputSystem_MODULE "inputsystem"

#endif
