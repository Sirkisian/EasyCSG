#pragma once
//-----
#include <vector>
#include <minwindef.h>
#include "ribbonxml.h"
//----
namespace RibbonControlGroups
{
	const std::vector<UINT> projectControls{cmdMenuSaveAsProject, cmdMenuSaveProject, cmdSaveAsProject, cmdSaveProject,
											cmdObjectType, cmdObjectName, cmdObjectValues, cmdObjectAdd,
											cmdSceneModeType, cmdTransformationType, cmdTransformationStep, cmdTransformationXAxis, cmdTransformationYAxis, cmdTransformationZAxis,
											cmdLightMaterial, cmdLightMaterialValues, cmdLightPosition, cmdLightSource,
											cmdShowAxis, cmdShowGrid,
											cmdCsgTreeNames, cmdTreeDelete,
											cmdCsgCalculate,
											cmdCsgAlgorithm, cmdDepthAlgorithm, cmdOffscreenType, cmdOptimization};
	
	const std::vector<UINT> objectControls{cmdObjectClone, cmdObjectDelete,
										   cmdAmbientMaterial, cmdDiffuseMaterial, cmdSpecularMaterial, cmdEmissiveMaterial, cmdTextureAdd,
										   cmdNodeAdd};

	const std::vector<UINT> objectTypeControls{cmd3Dcube, cmd3Dsphere, cmd3Dcylinder, cmd3Dcone, cmd3Dpyramid, cmd3Dtorus};
	const std::vector<UINT> axisControls{cmdTransformationXAxis, cmdTransformationYAxis, cmdTransformationZAxis};
	const std::vector<UINT> lightControls{cmdAmbientLight, cmdDiffuseLight, cmdSpecularLight};
	const std::vector<UINT> materialControls{cmdAmbientMaterial, cmdDiffuseMaterial, cmdSpecularMaterial, cmdEmissiveMaterial};
	const std::vector<UINT> objectCsgControls{cmdIntersection, cmdSubtraction, cmdUnion};
	const std::vector<UINT> csgAlgorithmControls{cmdAutomatic, cmdGoldfeather, cmdSCS};
	const std::vector<UINT> csgDepthAlgorithmControls{cmdOcclusionQuery, cmdDepthComplexitySampling, cmdNoDepthComplexitySampling};
	const std::vector<UINT> csgOfscreenTypeControls{cmdFrameBufferObject, cmdPBuffer, cmdFrameBufferObjectARB, cmdFrameBufferObjectEXT, cmdAutomaticOffscreenType};
	const std::vector<UINT> csgOptimizationControls{cmdOptimizationDefault, cmdOptimizationOn, cmdOptimizationOff};
	const std::vector<UINT> databaseControls{cmdDatabaseDisconnect, cmdDatabaseUpload, cmdDatabaseDownload, cmdProjectsList};
}
//---
//--