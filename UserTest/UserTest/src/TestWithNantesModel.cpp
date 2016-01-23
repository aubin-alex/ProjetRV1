//============================================================================
// Name        : TestWithNantesModel.cpp
// Author      : fzhang
// Version     :
// Copyright   : Your copyright notice
// Description : OSG smart text demo
//============================================================================

#include <osg/Camera>
#include <osg/Group>
#include <osg/Geode>
#include <osg/AnimationPath>
#include <osg/AutoTransform>
#include <osgGA/OrbitManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/CameraViewSwitchManipulator>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include <string>
#include <iostream>
#include <sstream>
#include <math.h>

#include "TestWithNantesModel.h"
#include "myXparser.h"
#include "Filters.h"
#include "KeyBoard.h"
#include "Display.h"
#include "SetUp.h"
#include "AnimationPath.h"

using namespace xercesc;

/*
 *  define external variables
 */

osg::Vec4d drawbd;

const float MaxDistance = 23000;

const osg::Vec3f ModelCenter(355613, 6689600, 0);

const osg::Vec3d NearestPoint(355613, 6687800, 0);

osg::Matrix M, V, P, W;

osg::Vec3d eye, center, up;

unsigned int ScrCenterX, ScrCenterY;

float clkx, clky;

int NumOfTexts, NumOfOccludedTexts;

bool SizePara = false;
bool ColorPara = false;
bool VisibilityPara = false;
bool ResolutionPara = false;

bool objPiecewisePara = false;
bool objOrderingPara = false;
bool objSinusoidPara = false;
bool objLinearPara = false;
bool objConstPiecePara = false;
bool originalPara = false;
bool scrLinearPara = false;
bool scrFisheyePara = false;
bool scrCosinusPara = false;
bool scrStdCosinPara = false;
bool scrEllipsePara = false;
bool scrCplLinearPara = false;
bool LOIPara = false;
bool defaultPara = true;
bool starttimePara = false;
bool endtimePara = false;

bool AuxiliaryLinePara = false;

bool focusPara = false;
bool usertestPara = false;

osg::Timer_t starttime = 0;
osg::Timer_t endtime = 0;

osg::Vec3d eyeHome(ModelCenter.x() - 3500.0, ModelCenter.y() + 000.0, ModelCenter.z() + 100.0);
osg::Vec3d centerHome(ModelCenter.x() + 0.0, ModelCenter.y() + 0.0, ModelCenter.z() + 0.0);
osg::Vec3d upHome(0.0, 0.0, 1.0);


/*
 *  main function
 */
int main(int argc, char** argv) {

	//Scene root, to load all things
	osg::ref_ptr<osg::Group> root = new osg::Group;

	//To add urban background into the scene
	osg::ref_ptr<osg::Node> city = osgDB::readNodeFile("Data/cityLOD1_smooth_color.osg");
	// osg::ref_ptr<osg::Node> city = osgDB::readNodeFile("Data/cityLOD1.osg");
	osg::ref_ptr<osg::Node> others = osgDB::readNodeFile("Data/others_smooth_color.osg");

    // optimize the scene graph, remove redundant nodes and state etc.
    osgUtil::Optimizer optimizer;
    optimizer.optimize(city.get());

	root->addChild(city);
	root->addChild(others);

	//A camera for HUD text
	osg::ref_ptr<osg::Camera> Cam = new osg::Camera;
	//Get screen center to be stored in two global variants to be used in later calculation
	GetScreenCenter(ScrCenterX, ScrCenterY);
	SetUpCamera(Cam);
	root->addChild(Cam);


	//Here needs this geode to be replaced in the loop, otherwise impossible to add texts into scene
	std::string datapath, positionpath;

	//A simple function to differentiate displayed info for each function
	MatchDataPosition(datapath, positionpath);
	datapath = "Data/CountriesEnglish.xml";
	positionpath = "Data/Position1.xml";
	myXparser datasetParser;
	myXparser positionParser;
	datasetParser.readXMLFile(datapath);
	positionParser.readXMLFile(positionpath);
	DOMDocument* datasetDoc = datasetParser.getParseredDoc();
	DOMDocument* positionDoc = positionParser.getParseredDoc();
	DOMNodeList* dataset = datasetDoc->getElementsByTagName(
			XMLString::transcode("Info"));
	DOMNodeList* position = positionDoc->getElementsByTagName(
			XMLString::transcode("Info"));

//	osg::ref_ptr<osg::Geode> textnode = loadInfo(dataset, position);
//	osg::ref_ptr<osg::Group> textGroup = new osg::Group;
//	textGroup->addChild(textnode);
	osg::ref_ptr<osg::Group> textGroup = loadInfo(dataset, position);
	root->addChild(textGroup);

	//Define the viewer
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	SetUpViewer(viewer);
	viewer->addEventHandler(new myKeyboardEventHandler);
	viewer->setSceneData(root.get());

	// to change to get the right position
	// press SPACE BAR to reset to this position
	viewer->getCameraManipulator()->setHomePosition(eyeHome,centerHome,upHome,false);


	//To get the MVPW matrices in this loop, to be used to calculate screen coordinates
	while (!viewer->done()) {
		viewer->frame();
		V = viewer->getCamera()->getViewMatrix();
		P = viewer->getCamera()->getProjectionMatrix();
		W = viewer->getCamera()->getViewport()->computeWindowMatrix();
		viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);

		/*std::cout<<"Eye values:"<<std::fixed<<eye.x()<<" "<<eye.y()<<" "<<eye.z()<<std::endl;
		std::cout<<"Center values:"<<std::fixed<<center.x()<<" "<<center.y()<<" "<<center.z()<<std::endl;
		std::cout<<"Up values:"<<std::fixed<<up.x()<<" "<<up.y()<<" "<<up.z()<<std::endl;*/
		/*I don't know why if I put this block into functions then it does not work
		 *those functions are okay without problems in SetUp.cpp
		 */

//		std::string datapath, positionpath;
//		//A simple function to differentiate displayed info for each function
//		MatchDataPosition(datapath, positionpath);
//		myXparser datasetParser;
//		myXparser positionParser;
//		datasetParser.readXMLFile(datapath);
//		positionParser.readXMLFile(positionpath);
//		DOMDocument* datasetDoc = datasetParser.getParseredDoc();
//		DOMDocument* positionDoc = positionParser.getParseredDoc();
//		DOMNodeList* dataset = datasetDoc->getElementsByTagName(
//				XMLString::transcode("Info"));
//		DOMNodeList* position = positionDoc->getElementsByTagName(
//				XMLString::transcode("Info"));

//		root->setChild(root->getNumChildren() - 1,
		//				DisplayInfo(dataset, position).get());
		DisplayInfo(textGroup);

		//To verify if HUD texts display or not
		HUDFilter(Cam);
	}

	return EXIT_SUCCESS;
}
