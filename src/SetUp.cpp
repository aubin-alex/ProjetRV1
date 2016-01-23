/*
 * SetUp.cpp
 *
 *  Created on: Jan 10, 2014
 *      Author: fanzhang
 */
#include <osg/PositionAttitudeTransform>
#include <osg/Camera>
#include <osg/GraphicsContext>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgText/Text>
#include <osg/LOD>
#include <osgDB/ReadFile>
#include <osg/Group>
#include <osgGA/OrbitManipulator>
#include <osgGA/FirstPersonManipulator>

#include "myXparser.h"
#include "TestWithNantesModel.h"
#include "SetUp.h"
#include "AnimationPath.h"
#include "FirstPersonMouseManipulator.h"

//To calculate screen center
void GetScreenCenter(unsigned int & x, unsigned int & y) {
	osg::GraphicsContext::WindowingSystemInterface* wsi =
			osg::GraphicsContext::getWindowingSystemInterface();
	/*if (!wsi) {
	 osg::notify(osg::NOTICE)
	 << "Error, no WindowSystemInterface available, cannot create windows."
	 << std::endl;
	 return 1;
	 }*/

	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), x, y);
	x = x / 2;
	y = y / 2;
}

void SetUpViewer(osg::ref_ptr<osgViewer::Viewer> myviewer) {

	//0.0f, 0.0f, 0.0f, 1.0f
	myviewer->getCamera()->setClearColor(osg::Vec4(0.75f, 0.75f, 0.75f, 1.0f));
	myviewer->setLightingMode(osgViewer::View::NO_LIGHT);
	myviewer->addEventHandler(new osgViewer::StatsHandler);
	myviewer->addEventHandler(new osgViewer::WindowSizeHandler);

//	myviewer->setCameraManipulator(new osgGA::OrbitManipulator);
	myviewer->setCameraManipulator(new FirstPersonMouseManipulator);

}

void SetUpCamera(osg::ref_ptr<osg::Camera> myCam) {
	myCam->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	myCam->setProjectionMatrixAsOrtho2D(0, 2 * ScrCenterX, 0, 2 * ScrCenterY);
	myCam->setViewMatrix(osg::Matrix::identity());
	myCam->setClearMask(GL_DEPTH_BUFFER_BIT);
	myCam->getOrCreateStateSet()->setMode(GL_LIGHTING,
			osg::StateAttribute::OFF);


	//To display the information on screen bottom
	osg::ref_ptr<osgText::Text> info = new osgText::Text;
	osg::ref_ptr<osg::Geode> bottominfo = new osg::Geode;
	info->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	info->setCharacterSize(ScrCenterY / 22, 1.0);
	info->setFont(osgText::readFontFile("Data/fudd.ttf"));
	info->setPosition(osg::Vec3(ScrCenterX / 12, ScrCenterY * 0.11, 0.0f));
	info->setFontResolution(32, 32);
	info->setText(
			" S:statistics information F:change window size T:text size C:text color V:text visibility R:text resolution \n H:Original P:OPW M:OCPW O:OO K:OL J:OS E:SFE L:SL B:SCOMC D:SC A:SE W:SCOML\n N:enter user test mode Y:user left-click on object to trigger detailed info G:reset focus point 1:start timer 2:end timer");
	bottominfo->addDrawable(info);
//	myCam->addChild(bottominfo);

}

DOMNodeList* SetUpPosition(std::string positionpath) {

	myXparser positionParser;

	positionParser.readXMLFile(positionpath);

	DOMDocument* positionDoc = positionParser.getParseredDoc();

	DOMNodeList* position = positionDoc->getElementsByTagName(
			XMLString::transcode("Info"));
	return position;

}

DOMNodeList* SetUpDataset(std::string datapath) {

	myXparser datasetParser;

	datasetParser.readXMLFile(datapath);

	DOMDocument* datasetDoc = datasetParser.getParseredDoc();

	DOMNodeList* dataset = datasetDoc->getElementsByTagName(
			XMLString::transcode("Info"));
	return dataset;
}

void MatchDataPosition(std::string & data, std::string & posi) {
	if (defaultPara) {
		data = "Data/Default.xml";
		posi = "Data/Default.xml";
	} else {
		if (originalPara) {
			data = "Data/CountriesEnglish.xml";
			posi = "Data/Position1.xml";
		} else if (scrFisheyePara) {
			data = "Data/RoadNamesFrench.xml";
			posi = "Data/Position1.xml";
		} else if (scrLinearPara) {
			data = "Data/FlowersEnglish.xml";
			posi = "Data/Position1.xml";
		} else if (objSinusoidPara) {
			data = "Data/BridgeNamesFrench.xml";
			posi = "Data/Position1.xml";
		} else if (objLinearPara) {
			data = "Data/RoadNamesFrench.xml";
			posi = "Data/Position2.xml";
		} else if (objOrderingPara) {
			data = "Data/CountriesEnglish.xml";
			posi = "Data/Position2.xml";
		} else if (objPiecewisePara) {
			data = "Data/FlowersEnglish.xml";
			posi = "Data/Position2.xml";
		} else if (objConstPiecePara) {
			data = "Data/BridgeNamesFrench.xml";
			posi = "Data/Position2.xml";
		} else if (scrEllipsePara) {
			data = "Data/RoadNamesFrench.xml";
			posi = "Data/Position3.xml";
		} else if (scrCosinusPara) {
			data = "Data/CountriesEnglish.xml";
			posi = "Data/Position3.xml";
		} else if (scrStdCosinPara) {
			data = "Data/FlowersEnglish.xml";
			posi = "Data/Position3.xml";
		} else if (scrCplLinearPara) {
			data = "Data/BridgeNamesFrench.xml";
			posi = "Data/Position3.xml";
		}
		else if (scrMyFilterPara) {
			data = "Data/RoadNamesFrench.xml";
			posi = "Data/Position1.xml";
        }
        else {
			data = "Data/Default.xml";
			posi = "Data/Default.xml";
		}

	}
}
