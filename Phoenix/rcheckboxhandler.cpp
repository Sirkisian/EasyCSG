#include "rcheckboxhandler.hpp"

RCheckBoxHandler::RCheckBoxHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(BOOL & defValue)):
RToggleControlHandler(hWnd, enabled, defValue)
{
}

RCheckBoxHandler::~RCheckBoxHandler()
{
}