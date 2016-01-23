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
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>

#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include <cstdlib>

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
bool HeightPara = false;

bool AuxiliaryLinePara = false;

bool focusPara = false;
bool usertestPara = false;
bool scrMyFilterPara = false;

osg::Timer_t starttime = 0;
osg::Timer_t endtime = 0;

//osg::Vec3d eyeHome(ModelCenter.x() - 3500.0, ModelCenter.y() + 000.0, ModelCenter.z() + 100.0);
osg::Vec3d eyeHome(355000.0, 6690400.0, 500.0);
//osg::Vec3d centerHome(ModelCenter.x() + 0.0, ModelCenter.y() + 0.0, ModelCenter.z() + 0.0);
osg::Vec3d centerHome(355969.0, 6690526.0, 53.5);
osg::Vec3d upHome(0.0, 0.0, 1.0);


/*
 *  main function
 */
int main(int argc, char** argv) {

	//Scene root, to load all things
	osg::ref_ptr<osg::Group> root = new osg::Group;

	//To add urban background into the scene
	osg::ref_ptr<osg::Node> city = osgDB::readNodeFile("Data/cityLOD0A.osg");
	// osg::ref_ptr<osg::Node> city = osgDB::readNodeFile("Data/cityLOD1.osg");
	osg::ref_ptr<osg::Node> others = osgDB::readNodeFile("Data/others.osg");


    // optimize the scene graph, remove redundant nodes and state etc.
    osgUtil::Optimizer optimizer;

    optimizer.optimize(city);

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
	datapath = "Data/RoadNamesFrench.xml";
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


	//Pour afficher des cercles sous les textes pour repérer la position sur la carte

	osg::ref_ptr<osg::Shape> myCircle (new osg::Cylinder(osg::Vec3d(0.0, 0.0, 0.0), 10.0f, 0.0f));
	osg::ref_ptr<osg::ShapeDrawable> circleDrawable (new osg::ShapeDrawable(myCircle.get()));

	circleDrawable->setColor(osg::Vec4d(0.0,0.0,0.0,1.0));

	osg::ref_ptr<osg::Group> circleGroup (new osg::Group);
	osg::ref_ptr<osg::Geode> circleGeode (new osg::Geode);

    const XMLCh* xmlch_x;
    const XMLCh* xmlch_y;
    const XMLCh* xmlch_z;

    root->addChild(circleGroup.get());

	for(int i=0; i < (position->getLength()); i++){
        osg::Vec3d positionTexte;

        DOMNode* positionNode = position->item(i);
        DOMElement* positionElement = dynamic_cast<xercesc::DOMElement*>(positionNode);

        xmlch_x = positionElement->getAttribute(XMLString::transcode("X"));
        xmlch_y = positionElement->getAttribute(XMLString::transcode("Y"));
        xmlch_z = positionElement->getAttribute(XMLString::transcode("Z"));

        osg::Vec3 ObjTextPos;
        ObjTextPos.set(atoi(XMLString::transcode(xmlch_x)),
						atoi(XMLString::transcode(xmlch_y)),
						atoi(XMLString::transcode(xmlch_z)));

        osg::ref_ptr<osg::PositionAttitudeTransform> positionCourant (new osg::PositionAttitudeTransform);

        positionCourant->setPosition(ObjTextPos);

        circleGeode->addChild(new osg::Geode);
        osg::Geode* noeudCourant = circleGeode->getChild(i)->asGeode();

        noeudCourant->addDrawable(circleDrawable.get());

        circleGroup->addChild(positionCourant);
        positionCourant->addChild(noeudCourant);

    }



	//Define the viewer
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	SetUpViewer(viewer);
	viewer->addEventHandler(new myKeyboardEventHandler);
	viewer->setSceneData(root);

	// to change to get the right position
	// press SPACE BAR to reset to this position
	//viewer->getCameraManipulator()->setHomePosition(eyeHome,centerHome,upHome,false);

    viewer->getCameraManipulator()->setHomePosition(eyeHome,centerHome,upHome,false);


	//To get the MVPW matrices in this loop, to be used to calculate screen coordinates
	while (!viewer->done()) {
		viewer->frame();
		V = viewer->getCamera()->getViewMatrix();
		P = viewer->getCamera()->getProjectionMatrix();
		W = viewer->getCamera()->getViewport()->computeWindowMatrix();

		viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);

		//std::cout<<"Eye values:"<<std::fixed<<eye.x()<<" "<<eye.y()<<" "<<eye.z()<<std::endl;
		//std::cout<<"Center values:"<<center.x()<<" "<<center.y()<<" "<<center.z()<<std::endl;
		//std::cout<<"Up values:"<<std::fixed<<up.x()<<" "<<up.y()<<" "<<up.z()<<std::endl;
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

    viewer->run();

	return EXIT_SUCCESS;
}
