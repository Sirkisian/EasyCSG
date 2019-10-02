#include "rtogglebuttonhandler.hpp"

RToggleButtonHandler::RToggleButtonHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(BOOL & defValue)):
RToggleControlHandler(hWnd, enabled, defValue)
{
}

RToggleButtonHandler::~RToggleButtonHandler()
{
}