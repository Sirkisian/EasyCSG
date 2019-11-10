#include "winincludes.hpp"

#include <vld.h> //need to be included in a c/c++ file

const std::basic_string<TCHAR> mCLASSNAME{_T("WindowClass")};
const std::basic_string<TCHAR> mMAINWINDOWNAME{_T("EasyCSG (under development)")};
const std::basic_string<TCHAR> mCHILDWINDOWNAME{_T("World")};
const std::basic_string<TCHAR> mSTATUSBAR{_T("Phoenix ...")};
const std::basic_string<TCHAR> mRCNAME{_T("PHOENIX_RIBBON")}; //ribbon .rc file name

WindowsUI* g_windowsUI = NULL;
Ribbon* g_ribbon = NULL;
Project* g_project = NULL;
Mouse g_mouse;

Timer g_timer(112, 3);
PrintScreen* g_printScreen = NULL;
INFOMESSAGE g_infoMessage;
BOOL g_repaint = FALSE;

TRANSFORMATIONVALUES g_transformationValues;

std::basic_string<TCHAR> g_fileName;

INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR pCmdLine, _In_ INT nCmdShow)
{
	VLDEnable();

	if(SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) //COM - Windows Ribbon, Open and Save Dialog
	{
		createUI(hInstance);

		CoUninitialize();

		free();
	}

	return 0;
}

VOID createUI(HINSTANCE hInstance)
{
	std::array<SHORT, 2> position{0, 0};
	std::array<SHORT, 2> size{0, 0};

	WindowsUI::getScreenSize(size);

	if(size[mSIZE::WIDTH] > 0 && size[mSIZE::HEIGHT] > 0)
	{
		g_windowsUI = new WindowsUI;

		if(g_windowsUI != NULL)
		{
			ClassProperty cp(mCLASSNAME, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC, (HBRUSH) COLOR_WINDOW, WindowProcedure);
			WindowProperty wp(mMAINWINDOWNAME, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, position, size, mWINDOW::MAIN, 0, SW_SHOWMAXIMIZED);
			StatusBarProperty sp(mSTATUSBAR, mWINDOW::STATUSBAR, mWINDOW::MAIN, RGB(255, 255, 255));

			g_windowsUI->createUI(hInstance, cp, wp, sp);
		}
	}
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND childWindow = NULL;
	IUIFramework* framework = NULL;

	if(g_windowsUI != NULL)
	{
		childWindow = g_windowsUI->getHwnd(mWINDOW::CHILD);
	}

	if(g_ribbon != NULL)
	{
		framework = g_ribbon->GetFramework();
	}

	if(hWnd == childWindow)
	{
		switch(uMsg)
		{
			case WM_PAINT:
				{
					if(g_project != NULL)
					{
						BOOL antiAlias = !WGLEW_ARB_multisample;

						if(g_project->graphicWorld.getDrawMode() != SceneObject::DRAWMODE::CSG)
						{
							g_project->graphicWorld.drawScene(antiAlias);
						}
						else
						{
							GraphicFunctions::clearScene(GraphicWorld::sceneColor);

							const UINT & shader = g_project->graphicWorld.getShaderProgram();
							const UINT & shaderFixed = g_project->graphicWorld.getShaderProgramFixed();

							g_project->graphicWorld.beginDraw(shader, antiAlias);
							g_project->graphicWorld.beginDraw(shaderFixed, antiAlias, TRUE);

							g_project->csgWorld.generateOpenCsgTrees();

							g_project->csgWorld.drawObjects(shader, shaderFixed);

							g_project->graphicWorld.endDraw(shader);
							g_project->graphicWorld.endDraw(shaderFixed);

							g_project->graphicWorld.drawAxis(shader, antiAlias);
						}

						if(g_timer.valid())
						{
							g_repaint = TRUE;
							g_project->fontManager.drawText(std::basic_string<TCHAR>{_T("verdana")}, g_infoMessage.message, g_infoMessage.position[mCOORDINATE::X], g_infoMessage.position[mCOORDINATE::Y], 1.0f, std::array<FLOAT, 3>{1.0f, 0.0f, 0.0f});
						}

						if(g_windowsUI->moveBack2Front(mWINDOW::CHILD))
						{
							ValidateRect(hWnd, NULL);
						}
					}
				}
				return 0;
		}
	}

	switch(uMsg)
	{
		case WM_HOTKEY:
			{
			}
			break;

		case WM_COMMAND:
			{
				WORD cmd = LOWORD(wParam);

				switch(cmd)
				{
					case ID_ACC_PROJECT_NEW:
						{
							SendMessage(hWnd, WM_RIBBON, cmdNewProject, 0);
						}
						break;
					case ID_ACC_PROJECT_OPEN:
						{
							SendMessage(hWnd, WM_RIBBON, cmdOpenProject, 0);
						}
						break;
					case ID_ACC_PROJECT_SAVE:
						{
							SendMessage(hWnd, WM_RIBBON, cmdSaveProject, 0);
						}
						break;

					case ID_ACC_MODE1:
					case ID_ACC_MODE2:
					case ID_ACC_MODE3:
					case ID_ACC_MODE4:
					case ID_ACC_MODE5:
						{
							BYTE type = (ID_ACC_MODE1 - cmd) * -1;

							RibbonControlManager::ComboBoxManager::SelectEntry(framework, cmdSceneModeType, type);

							SendMessage(hWnd, WM_RIBBON, cmdSceneModeType, type);
						}
						break;

					case ID_ACC_TR_TRANSLATE:
						{
							RibbonControlManager::ComboBoxManager::SelectEntry(framework, cmdTransformationType, 0);

							SendMessage(hWnd, WM_RIBBON, cmdTransformationType, 0);
						}
						break;
					case ID_ACC_TR_SCALE:
						{
							RibbonControlManager::ComboBoxManager::SelectEntry(framework, cmdTransformationType, 1);

							SendMessage(hWnd, WM_RIBBON, cmdTransformationType, 1);
						}
						break;
					case ID_ACC_TR_ROTATE:
						{
							RibbonControlManager::ComboBoxManager::SelectEntry(framework, cmdTransformationType, 2);

							SendMessage(hWnd, WM_RIBBON, cmdTransformationType, 2);
						}
						break;

					case ID_ACC_AXIS_X:
						{
							SendMessage(hWnd, WM_RIBBON, cmdTransformationXAxis, 0);
						}
						break;

					case ID_ACC_AXIS_Y:
						{
							SendMessage(hWnd, WM_RIBBON, cmdTransformationYAxis, 0);
						}
						break;

					case ID_ACC_AXIS_Z:
						{
							SendMessage(hWnd, WM_RIBBON, cmdTransformationZAxis, 0);
						}
						break;

					case ID_ACC_DEL:
						{
							SendMessage(hWnd, WM_RIBBON, cmdObjectDelete, 0);
						}
						break;
				}
			}
			break;

		case WM_CREATE:
			{
				if(g_ribbon == NULL)
				{
					g_ribbon = new Ribbon;

					if(g_ribbon != NULL)
					{
						if(SUCCEEDED(g_ribbon->CreateRibbon(hWnd, mRCNAME.data())))
						{
							return 0;
						}
						else
						{
							return -1;
						}
					}
				}
			}
			break;

		case WM_CLOSE:
			{
				DestroyWindow(hWnd);
			}
			return 0;

		case WM_DESTROY:
				{
					if(hWnd == g_windowsUI->getHwnd(mWINDOW::MAIN))
					{
						if(g_ribbon != NULL)
						{
							g_ribbon->DestroyRibbon();
						}

						OpenCSG::freeResources();

						g_windowsUI->release();

						PostQuitMessage(0);
					}
				}
			return 0;

		case WM_SIZE:
			{
				EnumChildWindows(hWnd, EnumChildProc, (LPARAM) hWnd);
			}
			return 0;

		case WM_MOUSEWHEEL:
			{
				if(hWnd == childWindow)
				{
					CHAR sign = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? -1 : 1;

					if((GET_KEYSTATE_WPARAM(wParam) & MK_SHIFT) == MK_SHIFT)
					{
						g_project->graphicWorld.transformSelectedObjects(g_transformationValues.axisRibbon, g_transformationValues.type, 0.1f * g_transformationValues.step * sign);
					}
					else
					{
						g_project->graphicWorld.camera.setOrbitCamera(0, 0, sign);
					}

					InvalidateRect(hWnd, NULL, TRUE);

					return 0;
				}
			}
			break;

		case WM_LBUTTONDOWN:
			{
				if(hWnd == childWindow)
				{
					if(g_project->graphicWorld.selectedObjectsCount() > 0)
					{
						RECT rect;

						if(GetClientRect(hWnd, &rect))
						{
							mCOORDINATE axis;
							g_transformationValues.set = g_project->graphicWorld.selectTransformationAxis(GET_X_LPARAM(lParam), rect.bottom - GET_Y_LPARAM(lParam), axis);

							if(g_transformationValues.set)
							{
								g_transformationValues.axis = axis;
							}
						}
					}
				}
			} //without break
		case WM_RBUTTONDOWN:
			{
				g_mouse.set(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			return 0;

		case WM_LBUTTONUP:
			{
				g_transformationValues.set = FALSE;
			} //without break
		case WM_RBUTTONUP:
			{
				g_mouse.clear();
			}
			return 0;

		case WM_MOUSEMOVE:
			{
				if(hWnd == childWindow)
				{
					if(wParam == MK_LBUTTON || wParam == MK_RBUTTON)
					{
						g_mouse.move(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

						if(wParam == MK_LBUTTON)
						{
							if(g_transformationValues.set)
							{
								std::array<FLOAT, 3> value{static_cast<FLOAT>(-g_mouse.sign[mCOORDINATE::X]), static_cast<FLOAT>(g_mouse.sign[mCOORDINATE::Y]), static_cast<FLOAT>(-g_mouse.sign[mCOORDINATE::Y])};

								g_project->graphicWorld.transformSelectedObjects(g_transformationValues.axis, g_transformationValues.type, 0.1f * g_transformationValues.step * value[g_transformationValues.axis]);
							}
						}
						else if(wParam == MK_RBUTTON)
						{
							g_project->graphicWorld.camera.setOrbitCamera(g_mouse.sign[mCOORDINATE::X], g_mouse.sign[mCOORDINATE::Y]);
						}

						InvalidateRect(hWnd, NULL, TRUE);

						return 0;
					}
				}
			}
			break;

		case WM_LBUTTONDBLCLK:
			{
				if(hWnd == childWindow)
				{
					RECT rect;

					if(GetClientRect(hWnd, &rect))
					{
						BOOL selected = g_project->graphicWorld.selectedObjectsCount() > 0;

						BOOL clearSelection = selected && !((GET_KEYSTATE_WPARAM(wParam) & MK_CONTROL) == MK_CONTROL);

						if(clearSelection)
						{
							g_project->graphicWorld.clearSelection();
						}

						size_t position = g_project->graphicWorld.selectObjects(GET_X_LPARAM(lParam), rect.bottom - GET_Y_LPARAM(lParam));

						if(position > 0)
						{
							setMaterial(framework, position, selected, clearSelection);

							if(!selected)
							{
								g_project->graphicWorld.drawTransformationAxis = TRUE;

								g_ribbon->EnableControlsOnCreate(TRUE, mCONTROLGROUP::OBJECT);

								RibbonControlManager::EnableDisableCommands(framework, RibbonControlGroups::objectControls, TRUE);
							}
						}
						else
						{
							if(clearSelection)
							{
								removeSelection(framework);
							}
						}

						InvalidateRect(hWnd, NULL, TRUE);
					}

					return 0;
				}
			}
			break;

		case WM_RBUTTONDBLCLK:
			{
				if(hWnd == childWindow)
				{
					RECT rect;

					if(GetClientRect(hWnd, &rect))
					{
						size_t position = g_project->graphicWorld.selectObject(GET_X_LPARAM(lParam), rect.bottom - GET_Y_LPARAM(lParam));

						g_project->graphicWorld.centerObject(position);

						g_project->graphicWorld.camera.setOrbitCamera(0, 0);

						InvalidateRect(hWnd, NULL, TRUE);
					}
				}
			}
			break;

		case WM_TIMER:
			{
				if(wParam == g_timer.id)
				{
					g_timer.incrementTimer();

					if(!g_timer.valid())
					{
						g_timer.killTimer();

						g_infoMessage.message.clear();

						if(g_repaint)
						{
							g_repaint = FALSE;

							InvalidateRect(childWindow, NULL, TRUE);
						}
						else
						{
							if(g_printScreen != NULL)
							{
								g_printScreen->loadScreen();

								PrintScreen::post(TRUE);
							}
						}
					}
				}
			}
			break;

		case WM_RIBBON:
			{
				switch(wParam)
				{
					//TAB [Project]
					case cmdNewProject:
					case cmdMenuNewProject:
						{
							if(g_project != NULL)
							{
								Dialog::MessageDialog dialog;

								if(dialog.show(hWnd, Dialog::MessageDialog::TYPE::M_QUESTION, mMAINWINDOWNAME, std::basic_string<TCHAR>{_T("Do you want to create a new project?")}))
								{
									g_project->reset();

									g_transformationValues = TRANSFORMATIONVALUES();

									resetRibbon(framework);
									removeSelection(framework);

									InvalidateRect(childWindow, NULL, TRUE);
								}
							}
							else
							{
								std::array<SHORT, 2> position{0, 0};
								std::array<SHORT, 2> size{0, 0};

								WindowProperty wp(mCHILDWINDOWNAME, WS_CHILDWINDOW, position, size, mWINDOW::CHILD, mWINDOW::MAIN, SW_SHOW);

								if(g_windowsUI->createGraphicWindow(wp))
								{
									g_project = new Project;

									if(g_project != NULL)
									{
										g_ribbon->EnableControlsOnCreate(TRUE, mCONTROLGROUP::PROJECT);

										RibbonControlManager::EnableDisableCommands(framework, RibbonControlGroups::projectControls, TRUE);

										LPTSTR fontPath;
										if(SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Fonts, 0, NULL, &fontPath)))
										{
											std::basic_string<TCHAR> font{fontPath};

											font.append(_T("\\verdana.ttf"));

											g_project->fontManager.loadFont(font);

											CoTaskMemFree(fontPath);
										}

										g_printScreen = new PrintScreen;

										return SendMessage(hWnd, WM_SIZE, 0, 0);
									}
								}
							}
						}
						break;

					case cmdSaveAsProject:
					case cmdMenuSaveAsProject:
						{
							std::basic_string<TCHAR> name{_T("EasyCSG - ")};
							std::basic_string<TCHAR> fileFormats{_T("*.ecsg")};
							name.append(fileFormats);

							std::vector<COMDLG_FILTERSPEC> types;
							ExceptionHandler::push_back<COMDLG_FILTERSPEC>(types, {name.data(), fileFormats.data()});

							if(SUCCEEDED(Dialog::save(mMAINWINDOWNAME, types, g_fileName)))
							{
								size_t position = g_fileName.rfind(_T("."));

								if(position == std::string::npos)
								{
									g_fileName.append(_T(".ecsg"));
								}
								else
								{
									std::basic_string<TCHAR> extension = g_fileName.substr(position + 1);

									if(extension.compare(_T("ecsg")) != 0)
									{
										g_fileName.append(_T(".ecsg"));
									}
								}

								g_project->save(g_fileName);
							}
						}
						break;

					case cmdSaveProject:
					case cmdMenuSaveProject:
						{
							if(g_fileName.empty())
							{
								SendMessage(hWnd, WM_RIBBON, cmdSaveAsProject, 0);
							}
							else
							{
								g_project->save(g_fileName);
							}
						}
						break;

					case cmdMenuExit:
						{
							return PostMessage(hWnd, WM_CLOSE, 0, 0);
						}
						break;

					case cmdShowShortcuts:
						{
							std::basic_string<TCHAR> html;

							if(getExePath(html))
							{
								html.append(_T("\\shortcuts\\shortcuts.html"));

								if(PathFileExists(html.data()))
								{
									ShellExecute(NULL, _T("open"), html.data(), NULL, NULL, SW_SHOWNORMAL);
								}
							}
						}
						break;

					case cmdShowLogFile:
						{
							std::basic_string<TCHAR> path;

							if(getExePath(path))
							{
								path.append(_T("\\messages"));

								std::basic_string<TCHAR> program = path + _T("\\miniweb.exe");

								if(PathFileExists(program.data()))
								{
									SHELLEXECUTEINFO shExecInfo;
									SecureZeroMemory(&shExecInfo, sizeof(shExecInfo));
									shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
									shExecInfo.lpVerb = _T("open");
									shExecInfo.lpFile = program.data();
									shExecInfo.lpParameters = _T("");
									shExecInfo.nShow = SW_MINIMIZE;

									if(ShellExecuteEx(&shExecInfo))
									{
										//...
									}
								}
							}
						}
						break;

					//Tab [Graphic]
					case cmd3Dcube:
					case cmd3Dsphere:
					case cmd3Dcylinder:
					case cmd3Dcone:
					case cmd3Dpyramid:
					case cmd3Dtorus:
						{
							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::objectTypeControls, wParam);
						}
						break;

					case cmdObjectName:
						{
							if(lParam == STRINGVALUE)
							{
								std::basic_string<TCHAR> objectName;
								if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, wParam, objectName)))
								{
									if(!objectName.empty())
									{
										std::basic_ostringstream<TCHAR> regex;
										regex << _T("^[a-zA-z]{1}[a-zA-Z0-9_]{0,") << GraphicObject::objectNameSize - 1 << _T("}$");

										if(!RegularExpression::match(objectName, regex.str()))
										{
											writeInfoMessage(hWnd, framework, wParam, std::basic_string<TCHAR>{_T("-")}, std::basic_string<TCHAR>{_T("Invalid input!")});
										}
										else
										{
											if(g_project->graphicWorld.objectNameExists(objectName))
											{
												writeInfoMessage(hWnd, framework, wParam, std::basic_string<TCHAR>{_T("-")}, std::basic_string<TCHAR>{_T("Name exists!")});
											}
											else
											{
												RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, wParam, objectName, 0, 0, TRUE);
											}
										}
									}
								}
							}
						}
						break;

					case cmdObjectValues:
						{
							if(lParam == STRINGVALUE)
							{
								UINT objectType = 0;
								std::basic_string<TCHAR> objectValues;

								if(handleFloatListControl(hWnd, framework, wParam, RibbonControlGroups::objectTypeControls, objectValues, objectType, std::basic_string<TCHAR>{_T("-")}, std::basic_string<TCHAR>{_T("No primitive selected!")}))
								{
									BOOL validValues = FALSE;

									std::vector<FLOAT> values(4, 0.0f);
									Input::list2floatArray(objectValues, std::basic_string<TCHAR>{_T(";")}, values);

									switch(objectType)
									{
										case cmd3Dcube:
										case cmd3Dsphere:
										case cmd3Dcylinder:
											{
												std::vector<std::array<FLOAT, 2>> minMax{{0.1f, 100.0f}, {0.1f, 100.0f}, {0.1f, 100.0f}, {0.0f, 0.0f}};
												validValues = Input::checkFloatArrayValues(values, minMax);
											}
											break;
									}

									if(!validValues)
									{
										writeInfoMessage(hWnd, framework, wParam, std::basic_string<TCHAR>{_T("-")}, std::basic_string<TCHAR>{_T("Invalid arguments!")});
									}
									else
									{
										RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, wParam, objectValues, 0, 0, TRUE);
									}
								}
							}
						}
						break;

					case cmdObjectAdd:
						{
							UINT objectType = 0;
							RibbonControlManager::GetSelectedControl(framework, RibbonControlGroups::objectTypeControls, objectType);

							if(objectType == 0)
							{
								writeInfoMessage(hWnd, framework, 0, std::basic_string<TCHAR>{_T("-")}, std::basic_string<TCHAR>{_T("No primitive selected!")});
							}
							else
							{
								std::vector<std::basic_string<TCHAR>> inputErrors;
								ExceptionHandler::push_back<std::basic_string<TCHAR>>(inputErrors, _T("-"));

								std::basic_string<TCHAR> objectName;
								if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, cmdObjectName, objectName)))
								{
									if(std::find(std::begin(inputErrors), std::end(inputErrors), objectName) == std::end(inputErrors))
									{
										std::basic_string<TCHAR> objectValues;
										if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, cmdObjectValues, objectValues)))
										{
											inputErrors.pop_back();

											if(std::find(std::begin(inputErrors), std::end(inputErrors), objectValues) == std::end(inputErrors))
											{
												GraphicObjectG* object = NULL;

												BOOL defaultValues = objectValues.compare(_T("-")) == 0;

												std::vector<FLOAT> values(4, 0.0f);
												if(!defaultValues)
												{
													Input::list2floatArray(objectValues, std::basic_string<TCHAR>{_T(";")}, values);
												}

												switch(objectType)
												{
													case cmd3Dcube:
														{
															if(defaultValues)
															{
																values[0] = Object3D::cubeParameters[0];
																values[1] = Object3D::cubeParameters[1];
																values[2] = Object3D::cubeParameters[2];
																values[3] = 0;
															}

															object = g_project->graphicWorld.objectManager.createPrimitive(Object::OBJECTTYPE::CUBE, objectName, values);
														}
														break;

													case cmd3Dsphere:
														{
															if(defaultValues)
															{
																values[0] = Object3D::sphereParameters[0];
																values[1] = Object3D::sphereParameters[1];
																values[2] = Object3D::sphereParameters[2];
																values[3] = 0;
															}

															object = g_project->graphicWorld.objectManager.createPrimitive(Object::OBJECTTYPE::SPHERE, objectName, values);
														}
														break;

													case cmd3Dcylinder:
														{
															if(defaultValues)
															{
																values[0] = Object3D::cylinderParameters[0];
																values[1] = Object3D::cylinderParameters[1];
																values[2] = Object3D::cylinderParameters[2];
																values[3] = 0;
															}

															object = g_project->graphicWorld.objectManager.createPrimitive(Object::OBJECTTYPE::CYLINDER, objectName, values);
														}
														break;
												}

												if(object != NULL)
												{
													std::array<FLOAT, 4> material{0.7f, 0.7f, 0.7f, 1.0f};
													object->material.setMaterialValues(material, Material::MATERIALTYPE::EMISSIVE);

													g_project->graphicWorld.addGraphicObject(object);

													RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, cmdObjectName, std::basic_string<TCHAR>{_T("-")}, 0, 0, TRUE);

													if(!defaultValues)
													{
														RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, cmdObjectValues, std::basic_string<TCHAR>{_T("-")}, 0, 0, TRUE);
													}

													InvalidateRect(childWindow, NULL, TRUE);
												}
											}
										}
									}
								}
							}
						}
						break;

					case cmdObjectClone:
						{
							GraphicObject* clone = NULL;

							const std::vector<GraphicObject*> & objects = g_project->graphicWorld.getSelectedObjects();

							for(std::vector<GraphicObject*>::const_iterator i = objects.begin(), j = objects.end(); i != j; i++)
							{
								clone = (*i)->clone();

								if(clone != NULL)
								{
									g_project->graphicWorld.objectManager.setObjectId(clone);

									std::basic_ostringstream<TCHAR> cloneName;
									cloneName << _T("clone#") << clone->getObjectId();
									clone->setObjectName(cloneName.str());

									g_project->graphicWorld.addGraphicObject(clone);
								}
							}
						}
						break;

					case cmdObjectDelete:
						{
							if(g_project->graphicWorld.selectedObjectsCount() > 0)
							{
								BOOL deleted = FALSE;

								Dialog::MessageDialog dialog;

								if(dialog.show(hWnd, Dialog::MessageDialog::TYPE::M_QUESTION, mMAINWINDOWNAME, std::basic_string<TCHAR>{_T("Do you want to delete the selected object-s?\n(Nodes will not be deleted.)")}))
								{
									std::vector<GraphicObject*> objects = g_project->graphicWorld.getSelectedObjects(); //get copy of selected objects

									for(std::vector<GraphicObject*>::iterator i = objects.begin(), j = objects.end(); i != j; i++)
									{
										if(!g_project->csgWorld.isObjectCsgNode(*i))
										{
											deleted = TRUE;

											g_project->graphicWorld.deleteGraphicObject((*i)->getObjectId());

											*i = NULL;
										}
									}

									if(deleted)
									{
										size_t size = g_project->graphicWorld.selectedObjectsCount();

										if(size == 0)
										{
											removeSelection(framework);
										}
										else
										{
											setMaterial(framework, size, TRUE, size == 1);
										}

										InvalidateRect(childWindow, NULL, TRUE);
									}
								}
							}
						}
						break;

					case cmdSceneModeType:
						{
							if(g_project != NULL)
							{
								BYTE selected = static_cast<BYTE>(lParam);

								g_ribbon->SetMutableControl(_T("sceneModeType"), std::vector<UnionValue>{UnionValue(selected)});

								g_project->graphicWorld.setDrawMode(SceneObject::convertDrawMode(selected + 1));

								InvalidateRect(childWindow, NULL, TRUE);
							}
						}
						break;

					case cmdTransformationType:
						{
							BYTE selected = static_cast<BYTE>(lParam);

							g_ribbon->SetMutableControl(_T("transformationType"), std::vector<UnionValue>{UnionValue(selected)});

							g_transformationValues.type = Transformation::convertType(selected + 1);
						}
						break;

					case cmdTransformationStep:
						{
							g_transformationValues.step = static_cast<FLOAT>(lParam);
						}
						break;

					case cmdTransformationXAxis:
						{
							g_transformationValues.axisRibbon = mCOORDINATE::X;
							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::axisControls, wParam);
							g_ribbon->SetMutableControl(_T("transformationAxis"), std::vector<UnionValue>{UnionValue(wParam)});
						}
						break;

					case cmdTransformationYAxis:
						{
							g_transformationValues.axisRibbon = mCOORDINATE::Y;
							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::axisControls, wParam);
							g_ribbon->SetMutableControl(_T("transformationAxis"), std::vector<UnionValue>{UnionValue(wParam)});
						}
						break;

					case cmdTransformationZAxis:
						{
							g_transformationValues.axisRibbon = mCOORDINATE::Z;
							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::axisControls, wParam);
							g_ribbon->SetMutableControl(_T("transformationAxis"), std::vector<UnionValue>{UnionValue(wParam)});
						}
						break;

					case cmdAmbientLight:
					case cmdDiffuseLight:
					case cmdSpecularLight:
					case cmdAmbientMaterial:
					case cmdDiffuseMaterial:
					case cmdSpecularMaterial:
					case cmdEmissiveMaterial:
						{
							std::vector<UINT> lightMaterialControls = RibbonControlGroups::lightControls;
							lightMaterialControls.insert(lightMaterialControls.end(), RibbonControlGroups::materialControls.begin(), RibbonControlGroups::materialControls.end());
							RibbonControlManager::SetToggleGroup(framework, lightMaterialControls, wParam);
						}
						break;

					case cmdLightMaterialValues:
						{
							if(lParam == STRINGVALUE)
							{
								UINT lightMaterialType = 0;
								std::basic_string<TCHAR> lightMaterialValues;

								std::vector<UINT> lightMaterialControls = RibbonControlGroups::lightControls;
								lightMaterialControls.insert(lightMaterialControls.end(), RibbonControlGroups::materialControls.begin(), RibbonControlGroups::materialControls.end());

								if(handleFloatListControl(hWnd, framework, wParam, lightMaterialControls, lightMaterialValues, lightMaterialType, std::basic_string<TCHAR>{_T("")}, std::basic_string<TCHAR>{_T("No light/material selected!")}))
								{
									std::vector<FLOAT> values(4, 0.0f);

									Input::list2floatArray(lightMaterialValues, std::basic_string<TCHAR>{_T(";")}, values);

									std::vector<std::array<FLOAT, 2>> minMax{{0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f}};

									if(!Input::checkFloatArrayValues(values, minMax))
									{
										writeInfoMessage(hWnd, framework, wParam, std::basic_string<TCHAR>{_T("")}, std::basic_string<TCHAR>{_T("Invalid arguments!")});
									}
									else
									{
										std::array<FLOAT, 4> arrayValues;
										std::copy(values.begin(), values.end(), arrayValues.begin());

										std::basic_string<TCHAR> string;

										//for light
										if(lightMaterialType == cmdAmbientLight || lightMaterialType == cmdDiffuseLight || lightMaterialType == cmdSpecularLight)
										{
											Light::LIGHTTYPE lightType = Light::LIGHTTYPE::NOLIGHTTYPE;

											switch(lightMaterialType)
											{
												case cmdAmbientLight:
													{
														lightType = Light::LIGHTTYPE::AMBIENT;
													}
													break;

												case cmdDiffuseLight:
													{
														lightType = Light::LIGHTTYPE::DIFFUSE;
													}
													break;

												case cmdSpecularLight:
													{
														lightType = Light::LIGHTTYPE::SPECULAR;
													}
													break;
											}

											FLOAT lightSource = 0.0f;
											if(SUCCEEDED(RibbonControlManager::GetDecimalValue(framework, cmdLightSource, lightSource)))
											{
												g_project->graphicWorld.setLightValues(GL_LIGHT0 + (static_cast<BYTE>(lightSource) - 1), arrayValues, lightType);

												BYTE lightTypeNum = static_cast<BYTE>(lightType);

												formatLightMaterialString(arrayValues, string, lightTypeNum);

												RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, wParam, string, 1, lightTypeNum);
											}
										}
										else
										{
											//for material
											Material::MATERIALTYPE materialType = Material::MATERIALTYPE::NOMATERIALTYPE;

											switch(lightMaterialType)
											{
												case cmdAmbientMaterial:
													{
														materialType = Material::MATERIALTYPE::AMBIENT;
													}
													break;

												case cmdDiffuseMaterial:
													{
														materialType = Material::MATERIALTYPE::DIFFUSE;
													}
													break;

												case cmdSpecularMaterial:
													{
														materialType = Material::MATERIALTYPE::SPECULAR;
													}
													break;

												case cmdEmissiveMaterial:
													{
														materialType = Material::MATERIALTYPE::EMISSIVE;
													}
													break;
											}

											g_project->graphicWorld.setMaterialValues(arrayValues, materialType);

											BYTE materialTypeNum = static_cast<BYTE>(materialType);

											formatLightMaterialString(arrayValues, string, materialTypeNum);

											if(g_project->graphicWorld.selectedObjectsCount() > 1)
											{
												string.insert(0, _T("*"));
											}

											RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, wParam, string, 2, 3 + materialTypeNum);
										}

										RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, wParam, std::basic_string<TCHAR>{_T("")}, 0, 0, TRUE);

										SceneObject::DRAWMODE mode = g_project->graphicWorld.getDrawMode();

										if(mode == SceneObject::DRAWMODE::LIGHTING || mode == SceneObject::DRAWMODE::TEXTURE || mode == SceneObject::DRAWMODE::CSG )
										{
											InvalidateRect(childWindow, NULL, TRUE);
										}
									}
								}
							}
						}
						break;

					case cmdLightPosition:
						{
							if(lParam == STRINGVALUE)
							{
								std::basic_string<TCHAR> lightPosition;
								if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, wParam, lightPosition)))
								{
									if(!lightPosition.empty())
									{
										if(!RegularExpression::match(lightPosition, std::basic_string<TCHAR>{_T("^[+-]?\\d{1,3}(\\.\\d{1,2})?(;[+-]?\\d{1,3}(\\.\\d{1,2})?){0,2}$")}))
										{
											writeInfoMessage(hWnd, framework, wParam, std::basic_string<TCHAR>{_T("")}, std::basic_string<TCHAR>{_T("Invalid input!")});
										}
										else
										{
											std::vector<FLOAT> values(3, 0.0f);
											Input::list2floatArray(lightPosition, std::basic_string<TCHAR>{_T(";")}, values);

											std::vector<std::array<FLOAT, 2>> minMax{{-100.0f, 100.0f}, {-100.0f, 100.0f}, {-100.0f, 100.0f}};

											if(!Input::checkFloatArrayValues(values, minMax))
											{
												writeInfoMessage(hWnd, framework, wParam, std::basic_string<TCHAR>{_T("")}, std::basic_string<TCHAR>{_T("Invalid arguments!")});
											}
											else
											{
												FLOAT lightSource = 0.0f;
												if(SUCCEEDED(RibbonControlManager::GetDecimalValue(framework, cmdLightSource, lightSource)))
												{
													std::array<FLOAT, 3> arrayValues;
													std::copy(values.begin(), values.end(), arrayValues.begin());

													g_project->graphicWorld.setLightPosition(GL_LIGHT0 + (static_cast<BYTE>(lightSource) - 1), arrayValues);

													std::basic_string<TCHAR> string;
													formatLightPosition(arrayValues, string);

													RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, wParam, string, 1, 1);

													SceneObject::DRAWMODE mode = g_project->graphicWorld.getDrawMode();

													if(mode == SceneObject::DRAWMODE::LIGHTING || mode == SceneObject::DRAWMODE::TEXTURE || mode == SceneObject::DRAWMODE::CSG )
													{
														InvalidateRect(childWindow, NULL, TRUE);
													}
												}

												RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, wParam, std::basic_string<TCHAR>{_T("")}, 0, 0, TRUE);
											}
										}
									}
								}
							}
						}
						break;

					case cmdTextureAdd:
						{
							std::basic_string<TCHAR> file;

							std::basic_string<TCHAR> name{_T("Images - ")};
							std::basic_string<TCHAR> fileFormats{_T("*.jpg;*.jpeg;*.png;*.bmp")};
							name.append(fileFormats);

							std::vector<COMDLG_FILTERSPEC> types;
							ExceptionHandler::push_back<COMDLG_FILTERSPEC>(types, {name.data(), fileFormats.data()});

							if(SUCCEEDED(Dialog::open(mMAINWINDOWNAME, types, file)))
							{
								UINT texture = g_project->textureManager.textureLoaded(file);

								if(texture == 0)
								{
									texture = g_project->textureManager.loadTexture(file);
								}

								if(texture > 0)
								{
									g_project->graphicWorld.setTexture(texture);

									SceneObject::DRAWMODE mode = g_project->graphicWorld.getDrawMode();

									if(mode == SceneObject::DRAWMODE::TEXTURE || mode == SceneObject::DRAWMODE::CSG )
									{
										InvalidateRect(childWindow, NULL, TRUE);
									}
								}
							}
						}
						break;

					case cmdShowAxis:
						{
							g_project->graphicWorld.drawSceneAxis = static_cast<BOOL>(lParam);

							InvalidateRect(childWindow, NULL, TRUE);
						}
						break;

					case cmdShowGrid:
						{
							g_project->graphicWorld.drawSceneGrid = static_cast<BOOL>(lParam);

							InvalidateRect(childWindow, NULL, TRUE);
						}
						break;

					case cmdCsgTreeNames:
						{
							std::basic_string<TCHAR> treeName;
							if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, wParam, treeName)))
							{
								if(lParam == STRINGVALUE)
								{
									if(!treeName.empty())
									{
										if(g_project->csgWorld.addCsgTree(treeName))
										{
											RibbonControlManager::ComboBoxManager::AddEntry(framework, wParam, treeName);
										}
									}
								}

								RibbonControlManager::ComboBoxManager::ClearContent(framework, cmdCsgTreeNodes);

								RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::objectCsgControls, 0);
								RibbonControlManager::EnableDisableCommands(framework, RibbonControlGroups::objectCsgControls, FALSE);

								if(g_project->csgWorld.isCsgTreeEmpty(treeName))
								{
									RibbonControlManager::EnableDisableCommands(framework, std::vector<UINT>{cmdCsgTreeNodes, cmdNodeRemove}, FALSE);
								}
								else
								{
									RibbonControlManager::EnableDisableCommands(framework, std::vector<UINT>{cmdCsgTreeNodes, cmdNodeRemove}, TRUE);

									const std::vector<CsgNode>* nodes = g_project->csgWorld.getCsgNodes(treeName);

									if(nodes != NULL)
									{
										for(std::vector<CsgNode>::const_iterator i = nodes->begin(), j = nodes->end(); i != j; i++)
										{
											std::basic_ostringstream<TCHAR> string;

											if(i != nodes->begin())
											{
												string << _T("op(");

												switch(i->operation)
												{
													case CsgNode::OPERATION::SUBTRACTION:
														{
															string << _T("S");
														}
														break;

													case CsgNode::OPERATION::INTERSECTION:
														{
															string << _T("I");
														}
														break;

													case CsgNode::OPERATION::UNION:
														{
															string << _T("U");
														}
														break;
												}

												string << _T(") ");
											}

											string << i->object->getObjectName();

											RibbonControlManager::ComboBoxManager::AddEntry(framework, cmdCsgTreeNodes, string.str());
										}
									}
								}
							}
						}
						break;

					case cmdCsgTreeNodes:
						{
							if(lParam == 0)
							{
								RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::objectCsgControls, 0);
								RibbonControlManager::EnableDisableCommands(framework, RibbonControlGroups::objectCsgControls, FALSE);
							}
							else
							{
								std::basic_string<TCHAR> treeName;
								if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, cmdCsgTreeNames, treeName)))
								{
									UINT position = 0;
									if(SUCCEEDED(RibbonControlManager::ComboBoxManager::SelectedEntry(framework, cmdCsgTreeNodes, position)))
									{
										CsgNode::OPERATION operation;
										if(g_project->csgWorld.getNodeOperation(treeName, position, operation))
										{
											UINT command = 0;

											switch(operation)
											{
												case CsgNode::OPERATION::SUBTRACTION:
													{
														command = cmdSubtraction;
													}
													break;

												case CsgNode::OPERATION::INTERSECTION:
													{
														command = cmdIntersection;
													}
													break;

												case CsgNode::OPERATION::UNION:
													{
														command = cmdUnion;
													}
													break;
											}

											RibbonControlManager::EnableDisableCommands(framework, RibbonControlGroups::objectCsgControls, TRUE);

											RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::objectCsgControls, command);
										}
									}
								}
							}
						}
						break;

					case cmdNodeAdd:
						{
							std::basic_string<TCHAR> treeName;
							if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, cmdCsgTreeNames, treeName)))
							{
								BOOL empty = g_project->csgWorld.isCsgTreeEmpty(treeName);

								if(empty)
								{
									RibbonControlManager::EnableDisableCommands(framework, std::vector<UINT>{cmdCsgTreeNodes, cmdNodeRemove}, TRUE);
								}

								const std::vector<GraphicObject*> & objects = g_project->graphicWorld.getSelectedObjects();

								for(std::vector<GraphicObject*>::const_iterator i = objects.begin(), j = objects.end(); i != j; i++)
								{
									if(g_project->csgWorld.addCsgNode(treeName, *i))
									{
										std::basic_ostringstream<TCHAR> string;

										if((empty && i != objects.begin()) || !empty)
										{
											string << _T("op(U) ");
										}

										string << (*i)->getObjectName();

										RibbonControlManager::ComboBoxManager::AddEntry(framework, cmdCsgTreeNodes, string.str());
									}
								}

								g_project->csgWorld.setGenerate(treeName);

								RibbonControlManager::ComboBoxManager::Invalidate(framework, cmdCsgTreeNodes);
							}
						}
						break;

					case cmdNodeRemove:
						{
							std::basic_string<TCHAR> treeName;
							if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, cmdCsgTreeNames, treeName)))
							{
								UINT position = 0;
								if(SUCCEEDED(RibbonControlManager::ComboBoxManager::SelectedEntry(framework, cmdCsgTreeNodes, position)))
								{
									if(g_project->csgWorld.removeCsgNode(treeName, position))
									{
										g_project->csgWorld.setGenerate(treeName);

										RibbonControlManager::ComboBoxManager::RemoveEntry(framework, cmdCsgTreeNodes, position);

										if(g_project->csgWorld.isCsgTreeEmpty(treeName))
										{
											RibbonControlManager::EnableDisableCommands(framework, std::vector<UINT>{cmdCsgTreeNodes, cmdNodeRemove}, FALSE);
										}
										else
										{
											if(position == 0)
											{
												RibbonControlManager::ComboBoxManager::SelectEntry(framework, cmdCsgTreeNodes, 0);

												std::basic_string<TCHAR> string;
												if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, cmdCsgTreeNodes, string)))
												{
													try
													{
														std::basic_string<TCHAR> objectName = string.substr(6);

														RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, cmdCsgTreeNodes, objectName, 0, 0);
													}
													catch(const std::out_of_range &)
													{
													}
												}
											}
										}

										RibbonControlManager::ComboBoxManager::Invalidate(framework, cmdCsgTreeNodes);

										RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::objectCsgControls, 0);
										RibbonControlManager::EnableDisableCommands(framework, RibbonControlGroups::objectCsgControls, FALSE);

										if(g_project->graphicWorld.getDrawMode() == SceneObject::DRAWMODE::CSG)
										{
											InvalidateRect(childWindow, NULL, TRUE);
										}
									}
								}
							}
						}
						break;

					case cmdSubtraction:
					case cmdIntersection:
					case cmdUnion:
						{
							std::basic_string<TCHAR> treeName;
							if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, cmdCsgTreeNames, treeName)))
							{
								UINT position = 0;
								if(SUCCEEDED(RibbonControlManager::ComboBoxManager::SelectedEntry(framework, cmdCsgTreeNodes, position)))
								{
									if(position > 0)
									{
										CsgNode::OPERATION operation;
										std::basic_string<TCHAR> letter;

										switch(wParam)
										{
											case cmdSubtraction:
												{
													letter = _T("S");
													operation = CsgNode::OPERATION::SUBTRACTION;
												}
												break;

											case cmdIntersection:
												{
													letter = _T("I");
													operation = CsgNode::OPERATION::INTERSECTION;
												}
												break;

											case cmdUnion:
												{
													letter = _T("U");
													operation =  CsgNode::OPERATION::UNION;
												}
												break;
										}

										g_project->csgWorld.setGenerate(treeName);

										g_project->csgWorld.setNodeOperation(treeName, position, operation);

										RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::objectCsgControls, wParam);

										std::basic_string<TCHAR> string;
										if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, cmdCsgTreeNodes, string)))
										{
											try
											{
												string.replace(3, 1, letter);

												RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, cmdCsgTreeNodes, string, 0, position);

												RibbonControlManager::ComboBoxManager::SelectEntry(framework, cmdCsgTreeNodes, position);
											}
											catch(const std::out_of_range &)
											{
											}
										}

										if(g_project->graphicWorld.getDrawMode() == SceneObject::DRAWMODE::CSG)
										{
											InvalidateRect(childWindow, NULL, TRUE);
										}
									}
								}
							}
						}
						break;

					case cmdCsgCalculate:
						{
							std::basic_string<TCHAR> treeName;
							if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, cmdCsgTreeNames, treeName)))
							{
								const std::vector<CsgNode>* nodes = g_project->csgWorld.getCsgNodes(treeName);

								GraphicObjectC* object = Carve::calculate(nodes);

								if(object != NULL)
								{
									g_project->graphicWorld.objectManager.setObjectId(object);

									std::basic_ostringstream<TCHAR> objectName;
									objectName << _T("csgObject#") << object->getObjectId();
									object->setObjectName(objectName.str());

									g_project->graphicWorld.addGraphicObject(object);

									InvalidateRect(childWindow, NULL, TRUE);
								}
							}
						}
						break;

					case cmdAutomatic:
						{
							OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::Algorithm::Automatic);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgAlgorithmControls, wParam);
						}
						break;

					case cmdSCS:
						{
							OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::Algorithm::SCS);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgAlgorithmControls, wParam);
						}
						break;

					case cmdGoldfeather:
						{
							OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::Algorithm::Goldfeather);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgAlgorithmControls, wParam);
						}
						break;

					case cmdNoDepthComplexitySampling:
						{
							OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::DepthComplexityAlgorithm::NoDepthComplexitySampling);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgDepthAlgorithmControls, wParam);
						}
						break;

					case cmdOcclusionQuery:
						{
							OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::DepthComplexityAlgorithm::OcclusionQuery);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgDepthAlgorithmControls, wParam);
						}
						break;

					case cmdDepthComplexitySampling:
						{
							OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::DepthComplexityAlgorithm::DepthComplexitySampling);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgDepthAlgorithmControls, wParam);
						}
						break;

					case cmdAutomaticOffscreenType:
						{
							OpenCSG::setOption(OpenCSG::OffscreenSetting, OpenCSG::OffscreenType::AutomaticOffscreenType);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOfscreenTypeControls, wParam);
						}
						break;

					case cmdFrameBufferObject:
						{
							OpenCSG::setOption(OpenCSG::OffscreenSetting, OpenCSG::OffscreenType::FrameBufferObject);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOfscreenTypeControls, wParam);
						}
						break;

					case cmdPBuffer:
						{
							OpenCSG::setOption(OpenCSG::OffscreenSetting, OpenCSG::OffscreenType::PBuffer);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOfscreenTypeControls, wParam);
						}
						break;

					case cmdFrameBufferObjectARB:
						{
							OpenCSG::setOption(OpenCSG::OffscreenSetting, OpenCSG::OffscreenType::FrameBufferObjectARB);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOfscreenTypeControls, wParam);
						}
						break;

					case cmdFrameBufferObjectEXT:
						{
							OpenCSG::setOption(OpenCSG::OffscreenSetting, OpenCSG::OffscreenType::FrameBufferObjectEXT);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOfscreenTypeControls, wParam);
						}
						break;

					case cmdOptimizationDefault:
						{
							OpenCSG::setOption(OpenCSG::DepthBoundsOptimization, OpenCSG::Optimization::OptimizationDefault);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOptimizationControls, wParam);
						}
						break;

					case cmdOptimizationOff:
						{
							OpenCSG::setOption(OpenCSG::DepthBoundsOptimization, OpenCSG::Optimization::OptimizationOff);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOptimizationControls, wParam);
						}
						break;

					case cmdOptimizationOn:
						{
							OpenCSG::setOption(OpenCSG::DepthBoundsOptimization, OpenCSG::Optimization::OptimizationOn);

							RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOptimizationControls, wParam);
						}
						break;

					case cmdHelp:
						{
							DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_HELP), g_windowsUI->getHwnd(mWINDOW::MAIN), helpDialogProcedure);
						}
						break;
				}
			}
			return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam) 
{	
	if(g_windowsUI != NULL && g_ribbon != NULL)
	{
		RECT rect;
		RECT statusBar;
		UINT ribbonHeight;

		if(hWndChild == g_windowsUI->getHwnd(mWINDOW::STATUSBAR))
		{
			SendMessage(hWndChild, WM_SIZE, 0, 0);
		}

		if(hWndChild == g_windowsUI->getHwnd(mWINDOW::CHILD))
		{
			if(GetClientRect((HWND)lParam, &rect))
			{
				if(!GetClientRect(g_windowsUI->getHwnd(mWINDOW::STATUSBAR), &statusBar))
				{
					statusBar.bottom = 0;
				}

				HRESULT hResult = g_ribbon->GetRibbonHeight(&ribbonHeight);

				if(SUCCEEDED(hResult))
				{
					USHORT width = static_cast<USHORT>(rect.right);
					USHORT height = static_cast<USHORT>(rect.bottom - ribbonHeight - statusBar.bottom);

					if(SetWindowPos(hWndChild, HWND_BOTTOM, 0, ribbonHeight, width, height, 0))
					{
						g_infoMessage.position = std::array<USHORT, 2>{50, static_cast<USHORT>(height - 50)};

						if(g_project != NULL)
						{
							g_project->graphicWorld.setProjection(45, 1, 50, width, height);

							g_project->graphicWorld.setTransformationAxisPosition(width - 200, 150);

							g_project->fontManager.setOrthoProjection(g_project->graphicWorld.getOrthoProjection());
						}

						if(g_printScreen != NULL)
						{
							g_printScreen->createRenderBuffer(std::array<USHORT, 2>{width, height});
						}
					}
				}
			}
		}
	}

	return TRUE;
}

VOID writeInfoMessage(HWND hWnd, IUIFramework* framework, _IN_(UINT & command), std::basic_string<TCHAR> reset, std::basic_string<TCHAR> message)
{
	if(command > 0)
	{
		RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, command, reset, 0, 0, TRUE);
	}

	if(g_printScreen != NULL)
	{
		if(g_infoMessage.message.empty())
		{
			g_printScreen->saveScreen();
		}
		else
		{
			g_printScreen->loadScreen();
		}

		PrintScreen::post(FALSE);
	}

	GraphicFunctions::beginDrawFront();

	g_infoMessage.message = message;
	g_project->fontManager.drawText(std::basic_string<TCHAR>{_T("verdana")}, g_infoMessage.message, g_infoMessage.position[mCOORDINATE::X], g_infoMessage.position[mCOORDINATE::Y], 1.0f, std::array<FLOAT, 3>{1.0f, 0.0f, 0.0f});

	GraphicFunctions::endDrawFront();

	g_timer.setTimer(hWnd);
}

BOOL handleFloatListControl(HWND hWnd, IUIFramework* framework, _IN_(UINT & command), _IN_(std::vector<UINT> & commands), _OUT_(std::basic_string<TCHAR> & string), _OUT_(UINT & type), std::basic_string<TCHAR> reset, std::basic_string<TCHAR> message)
{
	BOOL returnValue = FALSE;

	if(SUCCEEDED(RibbonControlManager::GetStringValue(framework, command, string)))
	{
		if(!string.empty())
		{
			type = 0;
			RibbonControlManager::GetSelectedControl(framework, commands, type);

			if(type == 0)
			{
				writeInfoMessage(hWnd, framework, command, reset, message);
			}
			else
			{
				if(!RegularExpression::match(string, std::basic_string<TCHAR>{_T("^\\d{1,3}(\\.\\d{1,2})?(;\\d{1,3}(\\.\\d{1,2})?){0,3}$")}))
				{
					writeInfoMessage(hWnd, framework, command, reset, std::basic_string<TCHAR>{_T("Invalid input!")});
				}
				else
				{
					returnValue = TRUE;
				}
			}
		}
	}

	return returnValue;
}

VOID formatLightMaterialString(_IN_(ARRAY4REF(FLOAT, values)), _OUT_(std::basic_string<TCHAR> & string), BYTE lightMaterialType)
{
	std::basic_ostringstream<TCHAR> stringFormated;
	stringFormated.setf(std::ios::showpoint | std::ios::left);
	stringFormated.precision(2);

	stringFormated << _T("r=") << FORMATNUM(values[mCOLOR::R]);
	stringFormated << _T(" g=") << FORMATNUM(values[mCOLOR::G]);
	stringFormated << _T(" b=") << FORMATNUM(values[mCOLOR::B]);
	stringFormated << _T(" a=") << FORMATNUM(values[mCOLOR::A]);

	stringFormated << _T(" (");

	switch(lightMaterialType)
	{
		case 1:
			{
				stringFormated << _T("ambient");
			}
			break;

		case 2:
			{
				stringFormated << _T("diffuse");
			}
			break;

		case 3:
			{
				stringFormated << _T("specular");
			}
			break;

		case 4:
			{
				stringFormated << _T("emissive");
			}
			break;
	}

	stringFormated << _T(")");

	string = stringFormated.str();
}

VOID formatLightPosition(_IN_(ARRAY3REF(FLOAT, values)), _OUT_(std::basic_string<TCHAR> & string))
{
	std::basic_ostringstream<TCHAR> stringFormated;
	stringFormated.setf(std::ios::showpoint);
	stringFormated.precision(4);

	stringFormated << _T("x=") << values[mCOORDINATE::X];
	stringFormated << _T(" y=") << values[mCOORDINATE::Y];
	stringFormated << _T(" z=") << values[mCOORDINATE::Z];

	string = stringFormated.str();
}

VOID removeSelection(IUIFramework* framework)
{
	g_project->graphicWorld.drawTransformationAxis = FALSE;

	g_ribbon->EnableControlsOnCreate(FALSE, mCONTROLGROUP::OBJECT);

	for(BYTE i = static_cast<BYTE>(Material::MATERIALTYPE::AMBIENT), j = static_cast<BYTE>(Material::MATERIALTYPE::EMISSIVE); i <= j; i++)
	{
		RibbonControlManager::ComboBoxManager::RemoveEntry(framework, cmdLightMaterialValues, 4);
	}

	RibbonControlManager::EnableDisableCommands(framework, RibbonControlGroups::objectControls, FALSE);
}

VOID setMaterial(IUIFramework* framework, size_t position, BOOL selected, BOOL single)
{
	for(BYTE i = static_cast<BYTE>(Material::MATERIALTYPE::AMBIENT), j = static_cast<BYTE>(Material::MATERIALTYPE::EMISSIVE); i <= j; i++)
	{
		const std::array<FLOAT, 4>* material = g_project->graphicWorld.getMaterialValues(position, Material::convertMaterialType(i));

		if(material != NULL)
		{
			std::basic_string<TCHAR> string;
			formatLightMaterialString(*material, string, i);

			if(selected)
			{
				if(!single)
				{
					string.insert(0, _T("*"));
				}

				RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, cmdLightMaterialValues, string, 2, 3 + i);
			}
			else
			{
				RibbonControlManager::ComboBoxManager::AddEntry(framework, cmdLightMaterialValues, string, 2);
			}
		}
	}
}

BOOL getExePath(_OUT_(std::basic_string<TCHAR> & path))
{
	path.resize(255);

	if(GetModuleFileName(NULL, &path.front(), path.size()) > 0)
	{
		size_t position = path.rfind(_T("\\"));

		if(position > 0)
		{
			path = path.substr(0, position);

			return TRUE;
		}
	}

	return FALSE;
}

VOID resetRibbon(IUIFramework* framework)
{
	std::basic_string<TCHAR> string;

	RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::objectTypeControls, 0);

	RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, cmdObjectName, std::basic_string<TCHAR>{_T("-")}, 0, 0, TRUE);
	RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, cmdObjectValues, std::basic_string<TCHAR>{_T("-")}, 0, 0, TRUE);

	RibbonControlManager::ComboBoxManager::SelectEntry(framework, cmdSceneModeType, 1);

	RibbonControlManager::ComboBoxManager::SelectEntry(framework, cmdTransformationType, 0);
	RibbonControlManager::SetDecimalValue(framework, cmdTransformationStep, 1.0f);

	RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::axisControls, cmdTransformationXAxis);

	std::vector<UINT> lightMaterialControls = RibbonControlGroups::lightControls;
	lightMaterialControls.insert(lightMaterialControls.end(), RibbonControlGroups::materialControls.begin(), RibbonControlGroups::materialControls.end());
	RibbonControlManager::SetToggleGroup(framework, lightMaterialControls, 0);

	std::array<FLOAT, 4> values4{1.0f, 1.0f, 1.0f, 1.0f};
	for(BYTE i = static_cast<BYTE>(Light::LIGHTTYPE::AMBIENT), j = static_cast<BYTE>(Light::LIGHTTYPE::SPECULAR); i <= j; i++)
	{
		formatLightMaterialString(values4, string, i);
		RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, cmdLightMaterialValues, string, 1, i);
	}

	std::array<FLOAT, 3> values3{0.0f, 0.0f, 0.0f};
	formatLightPosition(values3, string);
	RibbonControlManager::ComboBoxManager::ReplaceEntry(framework, cmdLightPosition, string, 1, 1);

	RibbonControlManager::SetToggleControl(framework, cmdShowAxis);
	RibbonControlManager::SetToggleControl(framework, cmdShowGrid);

	if(SUCCEEDED(RibbonControlManager::ComboBoxManager::ClearContent(framework, cmdCsgTreeNames)))
	{
		RibbonControlManager::ComboBoxManager::AddEntry(framework, cmdCsgTreeNames, std::basic_string<TCHAR>{_T("default")});
	}

	RibbonControlManager::ComboBoxManager::ClearContent(framework, cmdCsgTreeNodes);

	RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::objectCsgControls, 0);

	RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgAlgorithmControls, cmdSCS);
	RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgDepthAlgorithmControls, cmdNoDepthComplexitySampling);
	RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOfscreenTypeControls, cmdAutomaticOffscreenType);
	RibbonControlManager::SetToggleGroup(framework, RibbonControlGroups::csgOptimizationControls, cmdOptimizationOff);
}

VOID free()
{
	if(g_printScreen != NULL)
	{
		delete g_printScreen;
		g_printScreen = NULL;
	}

	if(g_project != NULL)
	{
		delete g_project;
		g_project = NULL;
	}

	if(g_ribbon != NULL)
	{
		delete g_ribbon;
		g_ribbon = NULL;
	}

	if(g_windowsUI != NULL)
	{
		delete g_windowsUI;
		g_windowsUI = NULL;
	}
}

INT_PTR CALLBACK helpDialogProcedure(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				HFONT hFont = NULL;
				HICON hImage = NULL;

				Dialog::setControlFont(hWndDlg, IDC_STATIC_1, 21, FW_BOLD);
				hFont = Dialog::setControlFont(hWndDlg, IDC_STATIC_2P, 17, FW_MEDIUM);

				if(hFont != NULL)
				{
					SendDlgItemMessage(hWndDlg, IDC_STATIC_2E, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(TRUE, 0));
					SendDlgItemMessage(hWndDlg, IDC_STATIC_2C, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(TRUE, 0));
				}

				hImage = reinterpret_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));

				if(hImage != NULL)
				{
					SendDlgItemMessage(hWndDlg, IDC_LOGO, STM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(hImage));
				}

				Dialog::setControlBitmap(hWndDlg, IDC_STATIC_2PB, IDB_BITMAPP);
				Dialog::setControlBitmap(hWndDlg, IDC_STATIC_2EB, IDB_BITMAPE);
				Dialog::setControlBitmap(hWndDlg, IDC_STATIC_2CB, IDB_BITMAPC);
			}
			break;

		case WM_CTLCOLORDLG:
			{
				return reinterpret_cast<INT_PTR>(GetStockObject(WHITE_BRUSH));
			}
			break;

		case WM_CTLCOLORSTATIC: 
			{
				HDC hDC = reinterpret_cast<HDC>(wParam);
				HWND hControl = reinterpret_cast<HWND>(lParam);

				if(hControl == GetDlgItem(hWndDlg, IDC_STATIC_1))
				{
					SetTextColor(hDC, RGB(24, 122, 201));
				}
				else if(hControl == GetDlgItem(hWndDlg, IDC_STATIC_2P) || hControl == GetDlgItem(hWndDlg, IDC_STATIC_2E) || hControl == GetDlgItem(hWndDlg, IDC_STATIC_2C))
				{
					SetTextColor(hDC, RGB(102, 77, 77));
				}
				else
				{
					SetTextColor(hDC, RGB(77, 77, 102));
				}

				return reinterpret_cast<INT_PTR>(GetStockObject(WHITE_BRUSH));
			}
			break;

		case WM_COMMAND:
			{
				switch(LOWORD(wParam))
				{
					case IDC_OKBUTTON:
						{
							EndDialog(hWndDlg, IDC_OKBUTTON);
						}
						break;
				}
			}
			break;

		case WM_NOTIFY:
			{
				switch((reinterpret_cast<LPNMHDR>(lParam))->code)
				{
					case NM_CLICK:
						{
							if(wParam == IDC_SYSLINK_1)
							{
								openUrl(std::basic_string<TCHAR>{_T("http://glew.sourceforge.net/")});
							}
							else if(wParam == IDC_SYSLINK_2)
							{
								openUrl(std::basic_string<TCHAR>{_T("http://opencsg.org/")});
							}
							else if(wParam == IDC_SYSLINK_3)
							{
								openUrl(std::basic_string<TCHAR>{_T("https://github.com/folded/carve/")});
							}
							else if(wParam == IDC_SYSLINK_4)
							{
								openUrl(std::basic_string<TCHAR>{_T("https://glm.g-truc.net/")});
							}
							else if(wParam == IDC_SYSLINK_5)
							{
								openUrl(std::basic_string<TCHAR>{_T("https://github.com/nothings/stb/")});
							}
							else if(wParam == IDC_SYSLINK_6)
							{
								openUrl(std::basic_string<TCHAR>{_T("https://freetype.org/")});
							}
							else if(wParam == IDC_SYSLINK_7)
							{
								openUrl(std::basic_string<TCHAR>{_T("https://kinddragon.github.io/vld/")});
							}
							else if(wParam == IDC_SYSLINK_8)
							{
								openUrl(std::basic_string<TCHAR>{_T("https://icons8.com/")});
							}
							else if(wParam == IDC_SYSLINK_9)
							{
								openUrl(std::basic_string<TCHAR>{_T("http://miniweb.sourceforge.net/")});
							}
						}
						break;
				}
			}
			break;

		case WM_DESTROY:
			{
				Dialog::deleteControlFont(hWndDlg, IDC_STATIC_1);
				Dialog::deleteControlFont(hWndDlg, IDC_STATIC_2P);

				HICON hIcon = reinterpret_cast<HICON>(SendDlgItemMessage(hWndDlg, IDC_LOGO, STM_GETIMAGE, IMAGE_ICON, 0));

				if(hIcon != NULL)
				{
					DestroyIcon(hIcon);
				}

				Dialog::deleteControlBitmap(hWndDlg, IDC_STATIC_2PB);
				Dialog::deleteControlBitmap(hWndDlg, IDC_STATIC_2EB);
				Dialog::deleteControlBitmap(hWndDlg, IDC_STATIC_2CB);
			}
			break;
	}

	return FALSE;
}