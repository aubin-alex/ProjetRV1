/*
 * Display.h
 *
 *  Created on: Jan 13, 2014
 *      Author: fanzhang
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <osgText/Text>

osg::ref_ptr<osg::Geode> AddDetailedInfo(const XMLCh* x, const XMLCh* y,
		const XMLCh* z, const XMLCh* AttrContent);

osg::ref_ptr<osgText::Text> ShowParaStudyInfo(osg::ref_ptr<osgText::Text> text, float ObjDis, float & coefficient);

void ReadXMLInfo(DOMElement* datasetElement, DOMElement* positionElement);

//This function is to simply add texts into the scene,needs to improve parameters of distances
//osg::ref_ptr<osg::Group> DisplayInfo(DOMNodeList* dataset, DOMNodeList* position);
osg::ref_ptr<osg::Group> loadInfo(DOMNodeList* dataset,		DOMNodeList* position);
void DisplayInfo(osg::ref_ptr<osg::Group> group);


#endif /* DISPLAY_H_ */
