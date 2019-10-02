#include "ribbonapplication.hpp"

RibbonApplication::RibbonApplication(HWND hWnd):
cRef(1),
hWnd(hWnd)
{
	this->controlsEnabled.fill(FALSE); //0. Project, 1. Object
}

RibbonApplication::~RibbonApplication()
{
}

STDMETHODIMP_(ULONG) RibbonApplication::Release()
{
	LONG cRef = InterlockedDecrement(&this->cRef);

	if(cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP RibbonApplication::QueryInterface(_IN_(IID & iid), _OUT_(VOID** ppv))
{
	HRESULT hResult = RibbonFunctions::QueryInterface(iid, ppv, *this);

	if(SUCCEEDED(hResult))
	{
		this->AddRef();
	}

	return hResult;
}

STDMETHODIMP RibbonApplication::OnViewChanged(UINT32 nViewID, UI_VIEWTYPE typeID, IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode)
{
	UNREFERENCED_PARAMETER(nViewID);
	UNREFERENCED_PARAMETER(pView);
	UNREFERENCED_PARAMETER(uReasonCode);

	HRESULT hResult = E_NOTIMPL;

	if(UI_VIEWTYPE_RIBBON == typeID)
	{
		switch (verb)
		{
			case UI_VIEWVERB_SIZE:
				{
					hResult = SendMessage(this->hWnd, WM_SIZE, 0, 0);
				}
				break;
		}
	}

	return hResult;
}

//for each control create its own handler
STDMETHODIMP RibbonApplication::OnCreateUICommand(UINT32 nCmdID, UI_COMMANDTYPE typeID, IUICommandHandler** ppCommandHandler)
{ 
	UNREFERENCED_PARAMETER(typeID);

	HRESULT hResult = E_NOTIMPL;

	switch(nCmdID)
	{
		case cmdMenuNewProject:
		case cmdMenuOpenProject:
		case cmdMenuExit:
		case cmdNewProject:
		case cmdOpenProject:
		case cmdShowShortcuts:
		case cmdShowLogFile:
		case cmdDatabaseConnect:
		case cmdHelp:
			{
				hResult = RibbonApplication::AddButton(ppCommandHandler, this->hWnd, TRUE);
			}
			break;

		case cmdNodeRemove:
		case cmdSubtraction:
		case cmdIntersection:
		case cmdUnion:
		case cmdDatabaseDisconnect:
		case cmdDatabaseUpload:
		case cmdDatabaseDownload:
			{
				hResult = RibbonApplication::AddButton(ppCommandHandler, this->hWnd, FALSE);
			}
			break;

		case cmdMenuSaveAsProject:
		case cmdMenuSaveProject:
		case cmdSaveAsProject:
		case cmdSaveProject:
		case cmdObjectAdd:
		case cmdTreeDelete:
		case cmdCsgCalculate:
			{
				hResult = RibbonApplication::AddButton(ppCommandHandler, this->hWnd, this->controlsEnabled[0]);
			}
			break;

		case cmdObjectClone:
		case cmdObjectDelete:
		case cmdTextureAdd:
		case cmdNodeAdd:
			{
				hResult = RibbonApplication::AddButton(ppCommandHandler, this->hWnd, this->controlsEnabled[1]);
			}
			break;
	//-----
		case cmdShowAxis:
		case cmdShowGrid:
			{
				hResult = RibbonApplication::AddCheckBox(ppCommandHandler, this->hWnd, this->controlsEnabled[0], TRUE);
			}
			break;
	//-----
		case cmdObjectName:
		case cmdObjectValues:
			{
				std::vector<Property> propertySet;
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("-")}, UI_COLLECTION_INVALIDINDEX, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));

				hResult = RibbonApplication::AddComboBox(ppCommandHandler, this->hWnd, this->controlsEnabled[0], propertySet);
			}
			break;

		case cmdSceneModeType:
			{
				std::vector<Property> propertySet;
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Wire mode")}, 0, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("[1] wire")}, 0, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Color mode")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("[2] color")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Light mode")}, 2, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("[3] lighting")}, 2, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Texture mode")}, 3, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("[4] texture")}, 3, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Csg mode")}, 4, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("[5] boolean operations")}, 4, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));

				hResult = RibbonApplication::AddComboBox(ppCommandHandler, this->hWnd, this->controlsEnabled[0], propertySet, 1);
			}
			break;

		case cmdTransformationType:
			{
				std::vector<Property> propertySet;
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("translate")}, UI_COLLECTION_INVALIDINDEX, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("scale")}, UI_COLLECTION_INVALIDINDEX, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("rotate")}, UI_COLLECTION_INVALIDINDEX, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));

				hResult = RibbonApplication::AddComboBox(ppCommandHandler, this->hWnd, this->controlsEnabled[0], propertySet);
			}
			break;

		case cmdLightMaterialValues:
			{
				std::vector<Property> propertySet;
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Light and material")}, 0, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 0, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Light")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("r=1.00 g=1.00 b=1.00 a=1.00 (ambient)")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("r=1.00 g=1.00 b=1.00 a=1.00 (diffuse)")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("r=1.00 g=1.00 b=1.00 a=1.00 (specular)")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Material")}, 2, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));

				hResult = RibbonApplication::AddComboBox(ppCommandHandler, this->hWnd, this->controlsEnabled[0], propertySet);
			}
			break;

		case cmdLightPosition:
			{
				std::vector<Property> propertySet;
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 0, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Light")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("x=0.0000 y=0.0000 z=0.0000")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));

				hResult = RibbonApplication::AddComboBox(ppCommandHandler, this->hWnd, this->controlsEnabled[0], propertySet);
			}
			break;

		case cmdCsgTreeNames:
			{
				std::vector<Property> propertySet;
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("default")}, UI_COLLECTION_INVALIDINDEX, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::ITEM));

				hResult = RibbonApplication::AddComboBox(ppCommandHandler, this->hWnd, this->controlsEnabled[0], propertySet);
			}
			break;

		case cmdCsgTreeNodes:
			{
				hResult = RibbonApplication::AddComboBox(ppCommandHandler, this->hWnd, FALSE, std::vector<Property>{});
			}
			break;
	//-----
		case cmdCsgAlgorithm:
		case cmdDepthAlgorithm:
		case cmdOffscreenType:
		case cmdOptimization:
			{
				hResult = RibbonApplication::AddDropDownButton(ppCommandHandler, this->controlsEnabled[0]);
			}
			break;

		case cmdObjectType:
			{
				std::vector<Property> propertySet;
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("3D Objects")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmd3Dcube, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmd3Dsphere, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmd3Dcylinder, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmd3Dcone, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmd3Dpyramid, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmd3Dtorus, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));

				hResult = RibbonApplication::AddDropDownGallery(ppCommandHandler, this->controlsEnabled[0], propertySet);
			}
			break;

		case cmdLightMaterial:
			{
				std::vector<Property> propertySet;
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Light")}, 0, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 0, cmdAmbientLight, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 0, cmdDiffuseLight, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 0, cmdSpecularLight, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("Material")}, 1, 0, UI_COMMANDTYPE_UNKNOWN, Property::CONTENTTYPE::CATEGORY));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmdAmbientMaterial, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmdDiffuseMaterial, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmdSpecularMaterial, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));
				ExceptionHandler::push_back<Property>(propertySet, Property(std::basic_string<TCHAR>{_T("")}, 1, cmdEmissiveMaterial, UI_COMMANDTYPE_BOOLEAN, Property::CONTENTTYPE::COMMAND));

				hResult = RibbonApplication::AddDropDownGallery(ppCommandHandler, this->controlsEnabled[0], propertySet);
			}
			break;
	//-----
		case cmdTransformationStep:
			{
				hResult = RibbonApplication::AddSpinner(ppCommandHandler, this->hWnd, this->controlsEnabled[0], 1, 100, 1);
			}
			break;

		case cmdLightSource:
			{
				hResult = RibbonApplication::AddSpinner(ppCommandHandler, this->hWnd, FALSE, 1, 1, 1);
			}
			break;
	//-----
		case cmd3Dcube:
		case cmd3Dsphere:
		case cmd3Dcylinder:
		case cmdAutomatic:
		case cmdGoldfeather:
		case cmdOcclusionQuery:
		case cmdDepthComplexitySampling:
		case cmdFrameBufferObject:
		case cmdPBuffer:
		case cmdFrameBufferObjectARB:
		case cmdFrameBufferObjectEXT:
		case cmdOptimizationDefault:
		case cmdOptimizationOn:
			{
				hResult = RibbonApplication::AddToggleButton(ppCommandHandler, this->hWnd, TRUE, FALSE);
			}
			break;

		case cmd3Dcone:
		case cmd3Dpyramid:
		case cmd3Dtorus:
			{
				hResult = RibbonApplication::AddToggleButton(ppCommandHandler, this->hWnd, FALSE, FALSE);
			}
			break;

		case cmdTransformationXAxis:
			{
				hResult = RibbonApplication::AddToggleButton(ppCommandHandler, this->hWnd, this->controlsEnabled[0], TRUE);
			}
			break;

		case cmdTransformationYAxis:
		case cmdTransformationZAxis:
			{
				hResult = RibbonApplication::AddToggleButton(ppCommandHandler, this->hWnd, this->controlsEnabled[0], FALSE);
			}
			break;

		case cmdAmbientLight:
		case cmdDiffuseLight:
		case cmdSpecularLight:
			{
				hResult = RibbonApplication::AddToggleButton(ppCommandHandler, this->hWnd, this->controlsEnabled[0], FALSE);
			}
			break;

		case cmdAmbientMaterial:
		case cmdDiffuseMaterial:
		case cmdSpecularMaterial:
		case cmdEmissiveMaterial:
			{
				hResult = RibbonApplication::AddToggleButton(ppCommandHandler, this->hWnd, this->controlsEnabled[1], FALSE);
			}
			break;

		case cmdSCS:
		case cmdNoDepthComplexitySampling:
		case cmdAutomaticOffscreenType:
		case cmdOptimizationOff:
			{
				hResult = RibbonApplication::AddToggleButton(ppCommandHandler, this->hWnd, TRUE, TRUE);
			}
			break;
	}

	return hResult;
}

STDMETHODIMP RibbonApplication::OnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler* pCommandHandler)
{
	UNREFERENCED_PARAMETER(commandId);
	UNREFERENCED_PARAMETER(typeID);
	UNREFERENCED_PARAMETER(pCommandHandler);

	return E_NOTIMPL; 
}

HRESULT RibbonApplication::AddButton(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled)
{
	IUICommandHandler* handler = NULL;

	return RibbonApplication::AddControl(ppCommandHandler, RButtonHandler::CreateInstance(&handler, hWnd, enabled), &handler);
}

HRESULT RibbonApplication::AddCheckBox(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled, BOOL defValue)
{
	IUICommandHandler* handler = NULL;

	return RibbonApplication::AddControl(ppCommandHandler, RCheckBoxHandler::CreateInstance(&handler, hWnd, enabled, defValue), &handler);
}

HRESULT RibbonApplication::AddComboBox(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled, _IN_(std::vector<Property> & propertySet), USHORT selected)
{
	IUICommandHandler* handler = NULL;

	return RibbonApplication::AddControl(ppCommandHandler, RComboBoxHandler::CreateInstance(&handler, hWnd, enabled, propertySet, selected), &handler);
}

HRESULT RibbonApplication::AddDropDownButton(_OUT_(IUICommandHandler** ppCommandHandler), BOOL enabled)
{
	IUICommandHandler* handler = NULL;

	return RibbonApplication::AddControl(ppCommandHandler, RDropDownButtonHandler::CreateInstance(&handler, enabled), &handler);
}

HRESULT RibbonApplication::AddDropDownGallery(_OUT_(IUICommandHandler** ppCommandHandler), BOOL enabled, _IN_(std::vector<Property> & propertySet))
{
	IUICommandHandler* handler = NULL;

	return RibbonApplication::AddControl(ppCommandHandler, RDropDownGalleryHandler::CreateInstance(&handler, enabled, propertySet), &handler);
}

HRESULT RibbonApplication::AddSpinner(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled, FLOAT minValue, FLOAT maxValue, FLOAT increment)
{
	IUICommandHandler* handler = NULL;

	return RibbonApplication::AddControl(ppCommandHandler, RSpinnerHandler::CreateInstance(&handler, hWnd, enabled, minValue, maxValue, increment), &handler);
}

HRESULT RibbonApplication::AddToggleButton(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled, BOOL defValue)
{
	IUICommandHandler* handler = NULL;

	return RibbonApplication::AddControl(ppCommandHandler, RToggleButtonHandler::CreateInstance(&handler, hWnd, enabled, defValue), &handler);
}

HRESULT RibbonApplication::AddControl(_OUT_(IUICommandHandler** ppCommandHandler), HRESULT createResult, _OUT_(IUICommandHandler** control))
{
	HRESULT hResult = createResult;

	if(SUCCEEDED(hResult))
	{
		if(*control == NULL)
		{
			return E_POINTER;
		}

		hResult = (*control)->QueryInterface(IID_PPV_ARGS(ppCommandHandler));

		(*control)->Release();
	}

	return hResult;
}