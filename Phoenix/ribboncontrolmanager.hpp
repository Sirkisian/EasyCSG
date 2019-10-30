#pragma once
//-----
#include "ribbon.hpp"
//----
//---
//--
namespace RibbonControlManager
{
	HRESULT EnableDisableCommands(IUIFramework* framework, _IN_(std::vector<UINT> & commands), BOOL value);

	HRESULT SetToggleGroup(IUIFramework* framework, _IN_(std::vector<UINT> & commands), _IN_(UINT & command), BOOL radioButton = TRUE);

	inline HRESULT SetToggleControl(IUIFramework* framework, UINT command)
	{
		return RibbonControlManager::SetToggleGroup(framework, std::vector<UINT>{command}, command);
	}

	HRESULT GetSelectedControl(IUIFramework* framework, _IN_(std::vector<UINT> & commands), _OUT_(UINT & command));

	HRESULT GetStringValue(IUIFramework* framework, UINT command, _OUT_(std::basic_string<TCHAR> & string));

	HRESULT GetDecimalValue(IUIFramework* framework, UINT command, _OUT_(FLOAT & value));
	HRESULT SetDecimalValue(IUIFramework* framework, UINT command, _IN_(FLOAT & value));

	class ComboBoxManager
	{
		public:
			enum class ACTION : BYTE {ADD, REPLACE};

			static inline HRESULT AddEntry(IUIFramework* framework, UINT command, std::basic_string<TCHAR> value, UINT category = 0, SHORT position = -1)
			{
				return RibbonControlManager::ComboBoxManager::SetEntry(framework, command, value, category, position, RibbonControlManager::ComboBoxManager::ACTION::ADD);
			}

			static HRESULT ReplaceEntry(IUIFramework* framework, UINT command, std::basic_string<TCHAR> value, UINT category, BYTE position, BOOL invalidate = FALSE);
			static HRESULT RemoveEntry(IUIFramework* framework, UINT command, BYTE position);
			static HRESULT ClearContent(IUIFramework* framework, UINT command);

			static inline HRESULT Invalidate(IUIFramework* framework, UINT command)
			{
				return framework->InvalidateUICommand(command, UI_INVALIDATIONS_PROPERTY, &UI_PKEY_SelectedItem);
			}

			static HRESULT SelectedEntry(IUIFramework* framework, UINT command, _OUT_(UINT & position));
			static HRESULT SelectEntry(IUIFramework* framework, UINT command, USHORT position);

		private:
			static HRESULT SetEntry(IUIFramework* framework, _IN_(UINT & command), _IN_(std::basic_string<TCHAR> & value), _IN_(UINT & category), SHORT position, RibbonControlManager::ComboBoxManager::ACTION action);
			static HRESULT ChangePre(IUIFramework* framework, _IN_(UINT & command), _OUT_(IUICollection** collection));
			static VOID ChangePost(IUICollection* collection, RPropertySet* propertySet);
	};
};