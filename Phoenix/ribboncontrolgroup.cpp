#include "ribboncontrolgroup.hpp"

const UINT RibbonControlGroup::projectControls[] = {cmdMenuSaveAsProject, cmdMenuSaveProject, cmdSaveAsProject, cmdSaveProject,
												    cmdObjectType, cmdObjectName, cmdObjectValues, cmdObjectAdd,
												    cmdSceneModeType, cmdTransformationType, cmdTransformationStep, cmdTransformationXAxis, cmdTransformationYAxis, cmdTransformationZAxis,
												    cmdLightMaterial, cmdLightMaterialValues, cmdLightPosition, cmdLightSource,
												    cmdCsgTreeNames, cmdCsgTreeNodes, cmdTreeClone, cmdTreeDelete,
												    cmdCsgCalculate, cmdObjectFacesSet,
												    cmdCsgAlgorithm, cmdDepthAlgorithm, cmdOffscreenType, cmdOptimization};

const BYTE RibbonControlGroup::projectControlsSize = ARRAYSIZE(projectControls);

const UINT RibbonControlGroup::objectControls[] = {cmdObjectClone, cmdObjectDelete,
												   cmdAmbientMaterial, cmdDiffuseMaterial, cmdSpecularMaterial, cmdEmissiveMaterial, cmdTextureAdd,
												   cmdNodeAdd, cmdNodeRemove,
												   cmdSubtraction, cmdIntersection, cmdUnion};

const BYTE RibbonControlGroup::objectControlsSize = ARRAYSIZE(objectControls);

HRESULT RibbonControlGroup::SetObjectTypeControls(IUIFramework* framework, UINT & cmd)
{
	UINT commands[] = {cmd3Dcube, cmd3Dsphere, cmd3Dcylinder, cmd3Dcone, cmd3Dpyramid, cmd3Dtorus};
	return RibbonControlGroup::SetToggleButtonGroup(framework, commands, CMDARRAYSIZE(commands), cmd, TRUE);
}

HRESULT RibbonControlGroup::SetAxisControls(IUIFramework* framework, UINT cmd)
{
	UINT commands[] = {cmdTransformationXAxis, cmdTransformationYAxis, cmdTransformationZAxis};
	return RibbonControlGroup::SetToggleButtonGroup(framework, commands, CMDARRAYSIZE(commands), cmd, TRUE);
}

HRESULT RibbonControlGroup::SetLightControls(IUIFramework* framework, BOOL value)
{
	UINT commands[] = {cmdAmbientLight, cmdDiffuseLight, cmdSpecularLight};
	return RibbonControlGroup::EnableDisableCommands(framework, commands, CMDARRAYSIZE(commands), value);
}

HRESULT RibbonControlGroup::SetLightMaterialControls(IUIFramework* framework, UINT cmd)
{
	UINT commands[] = {cmdAmbientLight, cmdDiffuseLight, cmdSpecularLight, cmdAmbientMaterial, cmdDiffuseMaterial, cmdSpecularMaterial, cmdEmissiveMaterial};
	return RibbonControlGroup::SetToggleButtonGroup(framework, commands, CMDARRAYSIZE(commands), cmd, TRUE);
}

HRESULT RibbonControlGroup::SetObjectCsgControls(IUIFramework* framework, UINT cmd)
{
	UINT commands[] = {cmdIntersection, cmdSubtraction, cmdUnion};
	return RibbonControlGroup::SetToggleButtonGroup(framework, commands, CMDARRAYSIZE(commands), cmd, TRUE);
}

HRESULT RibbonControlGroup::SetCsgAlgorithmControls(IUIFramework* framework, UINT & cmd)
{
	UINT commands[] = {cmdAutomatic, cmdGoldfeather, cmdSCS};
	return RibbonControlGroup::SetToggleButtonGroup(framework, commands, CMDARRAYSIZE(commands), cmd, TRUE);
}

HRESULT RibbonControlGroup::SetCsgDepthAlgorithmControls(IUIFramework* framework, UINT & cmd)
{
	UINT commands[] = {cmdOcclusionQuery, cmdDepthComplexitySampling, cmdNoDepthComplexitySampling};
	return RibbonControlGroup::SetToggleButtonGroup(framework, commands, CMDARRAYSIZE(commands), cmd, TRUE);
}

HRESULT RibbonControlGroup::SetCsgOfscreenTypeControls(IUIFramework* framework, UINT & cmd)
{
	UINT commands[] = {cmdFrameBufferObject, cmdPBuffer, cmdFrameBufferObjectARB, cmdFrameBufferObjectEXT, cmdAutomaticOffscreenType};
	return RibbonControlGroup::SetToggleButtonGroup(framework, commands, CMDARRAYSIZE(commands), cmd, TRUE);
}

HRESULT RibbonControlGroup::SetCsgOptimizationControls(IUIFramework* framework, UINT & cmd)
{
	UINT commands[] = {cmdOptimizationDefault, cmdOptimizationOn, cmdOptimizationOff};
	return RibbonControlGroup::SetToggleButtonGroup(framework, commands, CMDARRAYSIZE(commands), cmd, TRUE);
}

HRESULT RibbonControlGroup::SetDatabaseControls(IUIFramework* framework, BOOL value)
{
	UINT commands[] = {cmdDatabaseConnect, cmdDatabaseDisconnect, cmdDatabaseUpload, cmdDatabaseDownload, cmdProjectsList};
	return RibbonControlGroup::EnableDisableCommands(framework, commands, CMDARRAYSIZE(commands), value);
}

HRESULT RibbonControlGroup::EnableDisableCommands(IUIFramework* framework, const UINT* commands, USHORT count, BOOL value)
{
	BOOL valueCurrent;
	PROPVARIANT propvarNew;
	PROPVARIANT propvarCurrent;

	HRESULT hResult = UIInitPropertyFromBoolean(UI_PKEY_Enabled, value, &propvarNew);
	IFFAILEDRETURN(hResult);

	for(USHORT i = 0; i < count; i++)
	{
		hResult = framework->GetUICommandProperty(commands[i], UI_PKEY_Enabled, &propvarCurrent);

		if(SUCCEEDED(hResult))
		{
			hResult = UIPropertyToBoolean(UI_PKEY_Enabled, propvarCurrent, &valueCurrent);
			IFFAILEDRETURN(hResult);

			if(value != valueCurrent)
			{
				framework->SetUICommandProperty(commands[i], UI_PKEY_Enabled, propvarNew);
			}
		}
	}

	return S_OK;
}

HRESULT RibbonControlGroup::SetToggleButtonGroup(IUIFramework* framework, UINT* commands, BYTE count, UINT & command, BOOL radioButton)
{
	BOOL value;
	PROPVARIANT propvar;
	PROPVARIANT propvarFalse;

	HRESULT hResult = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, FALSE, &propvarFalse);
	IFFAILEDRETURN(hResult);

	for (BYTE i = 0; i < count; i++)
	{
		hResult = framework->GetUICommandProperty(commands[i], UI_PKEY_BooleanValue, &propvar);
		IFFAILEDRETURN(hResult);

		hResult = UIPropertyToBoolean(UI_PKEY_BooleanValue, propvar, &value);
		IFFAILEDRETURN(hResult);

		if(commands[i] == command)
		{
			if(radioButton)
			{
				if(!value)
				{
					hResult = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, TRUE, &propvar);
					IFFAILEDRETURN(hResult);

					hResult = framework->SetUICommandProperty(commands[i], UI_PKEY_BooleanValue, propvar);
					IFFAILEDRETURN(hResult);
				}
			}

			continue;
		}

		if(value)
		{
			hResult = framework->SetUICommandProperty(commands[i], UI_PKEY_BooleanValue, propvarFalse);
			IFFAILEDRETURN(hResult);
		}
	}

	return hResult;
}