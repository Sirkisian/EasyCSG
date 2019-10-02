#pragma once
//-----
#include "ribbon.hpp"
//----
#define CMDARRAYSIZE(x) sizeof(x)/sizeof(UINT)
//---
//--
class RibbonControlGroup
{
	public:
		static HRESULT SetObjectTypeControls(IUIFramework* framework, UINT & cmd);
		static HRESULT SetAxisControls(IUIFramework* framework, UINT cmd);
		static HRESULT SetLightControls(IUIFramework* framework, BOOL value);
		static HRESULT SetLightMaterialControls(IUIFramework* framework, UINT cmd);
		static HRESULT SetObjectCsgControls(IUIFramework* framework, UINT cmd);
		static HRESULT SetCsgAlgorithmControls(IUIFramework* framework, UINT & cmd);
		static HRESULT SetCsgDepthAlgorithmControls(IUIFramework* framework, UINT & cmd);
		static HRESULT SetCsgOfscreenTypeControls(IUIFramework* framework, UINT & cmd);
		static HRESULT SetCsgOptimizationControls(IUIFramework* framework, UINT & cmd);
		static HRESULT SetDatabaseControls(IUIFramework* framework, BOOL value);

		static HRESULT EnableDisableCommands(IUIFramework* framework, const UINT* commands, USHORT count, BOOL value);

		static const UINT projectControls[];
		static const BYTE projectControlsSize;
		static const UINT objectControls[];
		static const BYTE objectControlsSize;

	private:
		static HRESULT SetToggleButtonGroup(IUIFramework* framework, UINT* commands, BYTE count, UINT & command, BOOL radioButton);
};