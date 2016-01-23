/*
 * SetUp.h
 *
 *  Created on: Mar 7, 2014
 *      Author: fanzhang
 */

#ifndef SETUP_H_
#define SETUP_H_

#include <xercesc/dom/DOM.hpp>

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <osgViewer/Viewer>

osg::ref_ptr<osg::LOD> LoadUrbanModel(float MaxDistance, osg::Vec3f Center);

void GetScreenCenter(unsigned int & x, unsigned int & y);

void SetUpViewer(osg::ref_ptr<osgViewer::Viewer> myviewer);

void SetUpCamera(osg::ref_ptr<osg::Camera> myCam);

xercesc::DOMNodeList* SetUpPosition(std::string positionpath);

void MatchDataPosition(std::string & data, std::string  & posi);

xercesc::DOMNodeList* SetUpDataset(std::string datapath);

#endif /* SETUP_H_ */
