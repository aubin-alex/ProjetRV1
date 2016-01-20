/*
 * Display.cpp

 *
 *  Created on: Mar 7, 2014
 *      Author: fanzhang
 */

#include <osgText/Text>
#include <osgText/FadeText>

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/BoundingBox>

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>

#include "TestWithNantesModel.h"
#include "myXparser.h"
#include "Display.h"
#include "Filters.h"

const XMLCh* xmlch_AttrName;
const XMLCh* xmlch_AttrContent;
const XMLCh* xmlch_AttrLOI;
const XMLCh* xmlch_x;
const XMLCh* xmlch_y;
const XMLCh* xmlch_z;

float colorbase = 0.1f;
float sizebase = 20;
float visibilitybase = 1.0f;
float resolutionbase = 64;

extern bool focusPara;
//extern bool usertestPara;

osg::Vec3 ScreenCenter(ScrCenterX, ScrCenterY, 0);

osg::ref_ptr<osg::Geode> AddDetailedInfo(const XMLCh* x, const XMLCh* y,
		const XMLCh* z, const XMLCh* AttrContent) {
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osgText::Text> text = new osgText::Text;

	osg::Vec3 textpos;
	textpos.set(atoi(XMLString::transcode(x)), atoi(XMLString::transcode(y)),
			atoi(XMLString::transcode(z)));
	text->setPosition(textpos);
	text->setAlignment(osgText::Text::CENTER_BOTTOM);
	text->setAxisAlignment(osgText::Text::SCREEN);
	text->setFont("Data/Vera.ttf");
	text->setFontResolution(32, 32);
	text->setText(XMLString::transcode(AttrContent));
	text->setCharacterSize(25, 1.0);
	text->setBackdropType(osgText::Text::OUTLINE);
	text->setBackdropColor(osg::Vec4(0.8f,0.9f,0.2f,1.0f));
	float h = text->getCharacterHeight();
	osg::Vec3 newtextpos;
	newtextpos.set(textpos.x(), textpos.y(), textpos.z() + 1.5 * h);
	text->setPosition(newtextpos);
	geode->addDrawable(text);
	return geode;
}

osg::ref_ptr<osgText::Text> ShowParaStudyInfo(osg::ref_ptr<osgText::Text> text,
		float ObjDis, float & coefficient) {

	extern float sizebase;

	const osg::Vec3 position = text->getPosition();

	osg::ref_ptr<osgText::Text> ParaStudyInfo = new osgText::Text;
	osg::Vec3 dispos;
	ParaStudyInfo->setAlignment(osgText::Text::CENTER_BOTTOM_BASE_LINE);
	ParaStudyInfo->setFont("${workspace_loc:/${ProjName}/Data/Vera.ttf");
	ParaStudyInfo->setAxisAlignment(osgText::Text::SCREEN);

	dispos.set(position.x(), position.y(),
			position.z() + text->getCharacterHeight());
	std::string sizeis;

	if (SizePara) {
		float size = coefficient * sizebase;
		size = size * 0.6;
		ParaStudyInfo->setCharacterSize(size, 1.0);
		sizeis = ", "; //+ to_string(coefficient);
	} else {
		float size = sizebase * 0.6;
		ParaStudyInfo->setCharacterSize(size, 1.0);
		sizeis = ", 1.0";
	}
	ParaStudyInfo->setPosition(dispos);
	std::string dis = " "; // + to_string(ObjDis);
	std::string finaltext = dis + sizeis;
	ParaStudyInfo->setText(finaltext);

	return ParaStudyInfo;
}

void ReadXMLInfo(DOMElement* datasetElement, DOMElement* positionElement) {
	xmlch_AttrName = datasetElement->getAttribute(XMLString::transcode("Name"));
	xmlch_AttrContent = datasetElement->getAttribute(
			XMLString::transcode("Content"));
	xmlch_AttrLOI = datasetElement->getAttribute(XMLString::transcode("LOI"));
	xmlch_x = positionElement->getAttribute(XMLString::transcode("X"));
	xmlch_y = positionElement->getAttribute(XMLString::transcode("Y"));
	xmlch_z = positionElement->getAttribute(XMLString::transcode("Z"));
}

void SetTextSize(osgText::Text*  text, float & coefficient) {
	if (SizePara) {
		text->setCharacterSize(coefficient * sizebase, 1.0);
	} else {
		text->setCharacterSize(sizebase, 1.0);
	}
}

void SetTextResolution(osgText::Text*  text,
		float & coefficient) {
	if (ResolutionPara) {
		text->setFontResolution(resolutionbase * coefficient,
				resolutionbase * coefficient);
	} else {
		text->setFontResolution(resolutionbase, resolutionbase);
	}
}

void SetTextColorVisibility(osgText::Text* text,
		float & coefficient) {
	if (ColorPara) {
		//0.988f, 0.812f, 0.333f
		if (VisibilityPara) {
			text->setColor(
					osg::Vec4(1.0f, colorbase / coefficient, 0.25f,
							visibilitybase * coefficient));
		} else {
			text->setColor(
					osg::Vec4(1.0f, colorbase / coefficient, 0.25f,
							visibilitybase));
		}
	} else {
		if (VisibilityPara) {
			text->setColor(
					osg::Vec4(1.0f, colorbase, 0.25f,
							visibilitybase * coefficient));
		} else {
			//			text->setColor(osg::Vec4(1.0f, colorbase, 0.25f, visibilitybase));
			text->setColor(osg::Vec4(0.0f, 0.f, 0.f, visibilitybase));
		}
	}
}

//Using my method to get the bound
osg::Vec4d GetTextScreenBound(osg::ref_ptr<osgText::Text> tx) {
	osgText::String str = tx->getText();
	unsigned int strsize = str.size();
	unsigned int alfa; //horizontal
	unsigned int beta = tx->getCharacterHeight(); //vertical
	alfa = strsize * beta;
	alfa = alfa / 4;
	osgText::String temp = tx->getText();

	osg::Vec3 ObjTextPos = tx->getPosition();
	osg::Vec3 Low3D(ObjTextPos.x() - alfa, ObjTextPos.y(), ObjTextPos.z());
	osg::Vec3 ScrLow = Low3D * M * V * P * W;
	osg::Vec3 High3D(ObjTextPos.x() + alfa, ObjTextPos.y(),
			ObjTextPos.z() + beta);
	osg::Vec3 ScrHigh = High3D * M * V * P * W;

	osg::Vec4d bound(ScrLow.x(), ScrLow.y(), ScrHigh.x(), ScrHigh.y());

	return bound;

}

float GetTextScreenWidth(osg::ref_ptr<osgText::Text> tx) {
	osg::Vec4 bb = GetTextScreenBound(tx);
	osg::Vec2 low(bb.x(), bb.y());
	osg::Vec2 high(bb.z(), bb.w());
	float width = fabs(high.x() - low.x());
	return width;
}

float GetTextScreenHeight(osg::ref_ptr<osgText::Text> tx) {
	osg::Vec4 bb = GetTextScreenBound(tx);
	osg::Vec2 low(bb.x(), bb.y());
	osg::Vec2 high(bb.z(), bb.w());
	float height = fabs(high.y() - low.y());
	return height;
}

float GetTextScreenSize(osg::ref_ptr<osgText::Text> tx) {
	float a = GetTextScreenWidth(tx);
	float b = GetTextScreenHeight(tx);
	float screensize = a * b;
	return screensize;
}

float Compute2TextsScrXDistance(osg::ref_ptr<osgText::Text> text1,
		osg::ref_ptr<osgText::Text> text2) {

	osg::Vec4 bb1 = GetTextScreenBound(text1);
	osg::Vec4 bb2 = GetTextScreenBound(text2);
	osg::Vec2 high1(bb1.z(), bb1.w());
	osg::Vec2 low2(bb2.x(), bb2.y());

	float distance = fabs(low2.x() - high1.x());
	return distance;
}

float Compute2TextsScrYDistance(osg::ref_ptr<osgText::Text> text1,
		osg::ref_ptr<osgText::Text> text2) {

	float distance = 0;

	osg::Vec4 bb1 = GetTextScreenBound(text1);
	osg::Vec4 bb2 = GetTextScreenBound(text2);

	osg::Vec2 low1(bb1.x(), bb1.y());
	osg::Vec2 high1(bb1.z(), bb1.w());

	osg::Vec2 low2(bb2.x(), bb2.y());
	osg::Vec2 high2(bb2.z(), bb2.w());

	if (high2.y() - high1.y() > 0) {
		distance = fabs(low2.y() - high1.y());
	} else {
		distance = fabs(low1.y() - high2.y());
	}

	return distance;
}

std::vector<osg::ref_ptr<osgText::Text> > GetSortListHorizontal(
		std::vector<osg::ref_ptr<osgText::Text> > list) {

	for (unsigned int i = 0; i <= list.size() - 2; i++) {
		for (unsigned int j = list.size() - 1; j >= 1; j--) {

			osg::ref_ptr<osgText::Text> text1 = list[j];
			osg::ref_ptr<osgText::Text> text2 = list[j - 1];
			osg::Vec4 bb1 = GetTextScreenBound(text1);
			osg::Vec4 bb2 = GetTextScreenBound(text2);
			osg::Vec2 low1(bb1.x(), bb1.y());
			osg::Vec2 low2(bb2.x(), bb2.y());

			if (low1.x() <= low2.x()) {
				std::swap(list[j], list[j - 1]);
			}
		}
	}
	return list;
}

int ComputeNumOccludedLabels(std::vector<osg::ref_ptr<osgText::Text> > & list) {
	int num = 0;

	for (unsigned int i = 0; i < list.size(); i++) {
		for (unsigned int j = 0; j < list.size(); j++) {
			if (j != i) {
				float XTolerance =
						GetTextScreenWidth(list[i])
						< GetTextScreenWidth(list[j]) ?
								GetTextScreenWidth(list[i]) :
								GetTextScreenWidth(list[j]);
				XTolerance = XTolerance * 0.3;
				float YTolerance =
						GetTextScreenHeight(list[i])
						< GetTextScreenHeight(list[j]) ?
								GetTextScreenHeight(list[i]) :
								GetTextScreenHeight(list[j]);
				YTolerance = YTolerance * 0.2;
				if (Compute2TextsScrXDistance(list[i], list[j]) < XTolerance) {
					if (Compute2TextsScrYDistance(list[i], list[j])
							< YTolerance) {
						num = num + 1;
						//list[i]->setColor(osg::Vec4(0.8f,0.8f,0.2f,1.0));
						break;//to break j loop since this 'i' text already occlude one text
					} // j!=i if
				} //y if
			} //x if
		} //j loop
	} //i loop

	return num;
}

//Use the system bounding box way. Always something wrong in screen Y-axis
osg::Vec4 ComputeScreenBound(osg::ref_ptr<osgText::Text> tx) {
	osg::BoundingBox bbox = tx->computeBound();
	osg::Matrix MVPW = M * V * P * W;
	osg::Vec3 low = osg::Vec3(bbox.xMin(), bbox.yMin(), bbox.zMin());
	osg::Vec3 high = osg::Vec3(bbox.xMax(), bbox.yMax(), bbox.zMax());

	osg::Vec3 low2d = low * MVPW;
	osg::Vec3 high2d = high * MVPW;

	float x, y, z, w;
	x = low2d.x();
	y = low2d.y();
	z = high2d.x();
	w = high2d.y();

	osg::Vec4 bounds = osg::Vec4(x, y, z, w);

	return bounds;
}

void OpenOutputFiles(std::ofstream & file) {

	if (objConstPiecePara) {
		file.open("Outputs/OCPW.txt");
		file << "Object constant piecewise function applied" << std::endl;
	} else if (objOrderingPara) {
		file.open("Outputs/OO.txt");
		file << "Object ordering function applied" << std::endl;
	} else if (objSinusoidPara) {
		file.open("Outputs/OS.txt");
		file << "Object sinusoid function applied" << std::endl;
	} else if (objLinearPara) {
		file.open("Outputs/OL.txt");
		file << "Object linear function applied" << std::endl;
	} else if (objPiecewisePara) {
		file.open("Outputs/OPW.txt");
		file << "Object piecewise function applied" << std::endl;
	} else if (scrLinearPara) {
		file.open("Outputs/SL.txt");
		file << "ScreenLinear function applied" << std::endl;
	} else if (scrFisheyePara) {
		file.open("Outputs/SFE.txt");
		file << "Screen fisheye function applied" << std::endl;
	} else if (scrStdCosinPara) {
		file.open("Outputs/SC.txt");
		file << "Screen standard cosin function applied" << std::endl;
	} else if (scrCosinusPara) {
		file.open("Outputs/SCOMC.txt");
		file << "Screen cosinusoid function applied" << std::endl;
	} else if (scrEllipsePara) {
		file.open("Outputs/SE.txt");
		file << "Screen ellipse function applied" << std::endl;
	} else if (scrCplLinearPara) {
		file.open("Outputs/SCOML.txt");
		file << "Screen complex linear function applied" << std::endl;
	} else if (LOIPara) {
		file.open("Outputs/LOI.txt");
		file << "LOI function applied" << std::endl;
	} else {
		file.open("Outputs/Original.txt");
		file << "No function applied" << std::endl;
	}

}

//This function is to simply add texts into the scene,needs to improve parameters of distances
osg::ref_ptr<osg::Group> loadInfo(DOMNodeList* dataset,
		DOMNodeList* position) {

	osg::ref_ptr<osg::Group> group = new osg::Group; //To be returned into the frame() loop
	osg::ref_ptr<osg::Geode> geode = new osg::Geode; //To contain the general text information

	std::vector<osg::ref_ptr<osgText::Text> > unsortedLabelList;

	if (dataset->getLength() == position->getLength()) {
		const XMLSize_t nodeCount = dataset->getLength();
		for (XMLSize_t i = 0; i < nodeCount; i++) {
			DOMNode* datasetNode = dataset->item(i);
			DOMNode* positionNode = position->item(i);
			if (datasetNode->getNodeType()
					&& datasetNode->getNodeType() == DOMNode::ELEMENT_NODE) // is element
			{
				DOMElement* datasetElement =
						dynamic_cast<xercesc::DOMElement*>(datasetNode);

				DOMElement* positionElement =
						dynamic_cast<xercesc::DOMElement*>(positionNode);

				ReadXMLInfo(datasetElement, positionElement);

				//Initialize the text to be displayed as general information
				//				osg::ref_ptr<osgText::FadeText> text = new osgText::FadeText;
				osg::ref_ptr<osgText::Text> text = new osgText::Text;
				osg::Vec3 ObjTextPos;
				ObjTextPos.set(atoi(XMLString::transcode(xmlch_x)),
						atoi(XMLString::transcode(xmlch_y)),
						atoi(XMLString::transcode(xmlch_z)));
				text->setPosition(ObjTextPos);
				text->setAlignment(osgText::Text::CENTER_BOTTOM_BASE_LINE);
				text->setFont("Data/Vera.ttf");
				text->setAxisAlignment(osgText::Text::SCREEN);
				text->setText(XMLString::transcode(xmlch_AttrName));
				//				text->setFadeSpeed(0.1);

				//If some part ( a tolerance ratio to its size) of the label is outside screen, it disappears
				unsortedLabelList.push_back(text);
			} //if
		} //for
	} //if lengths are the same

	NumOfTexts = unsortedLabelList.size();
	for (int i = 0; i < NumOfTexts; i++) {
		geode->addDrawable(unsortedLabelList[i].get());
	}

	group->addChild(geode);
	return group;
}

//This function is to simply add texts into the scene, needs to improve parameters of distances
void DisplayInfo(osg::ref_ptr<osg::Group> group) {

	osg::Geode* geode = group->getChild(0)->asGeode(); //To contain the general text information

	std::vector<osg::ref_ptr<osgText::FadeText> > unsortedLabelList;
	osgText::Text* text;
	osg::Vec3 ObjTextPos;
	osg::Vec3 ScrTextPos;

	float scrdistance;
	float objdistance;
	float nearestdistance;
	float neartextdistance;

	int LOI = atoi(XMLString::transcode(xmlch_AttrLOI));

	float coefficient;

	unsigned int linLen;
	unsigned int alfa;
	unsigned int beta;

	osg::Vec3 Low3D;
	osg::Vec3 High3D;
	osg::Vec3 ScrLow;
	osg::Vec3 ScrHigh;

	osg::Vec2 Low2D;
	osg::Vec2 High2D;

//	for (int i = 0; i < geode->getNumDrawables(); i++) {
	for (const osg::ref_ptr<osg::Drawable> &d : geode->getDrawableList()) {

		text = dynamic_cast<osgText::Text*>(d.get());
//		text = (osgText::Text*)geode->getDrawable(i);
		ObjTextPos = text->getPosition();

		//Distance
		ScrTextPos = ObjTextPos * M * V * P * W;
		scrdistance = (ScrTextPos - ScreenCenter).length();
		objdistance = (ObjTextPos - eye).length();
		nearestdistance = (NearestPoint - eye).length();
		neartextdistance = (ObjTextPos - NearestPoint).length();

		//This line is to reset the screen center
		if (focusPara) {
			ScreenCenter.set(clkx, clky, ScrTextPos.z());
		} else {
			ScreenCenter.set(ScrCenterX, ScrCenterY, ScrTextPos.z());
		}

		//This block is to enable the user test left-click to trigger displaying detailed information
		//				if (usertestPara) {
		//					osg::Vec2 textscrposi(ScrTextPos.x(), ScrTextPos.y()+20);
		//					osg::Vec2 clickscrposi(clkx, clky);
		//					float clickdistance;
		//					clickdistance = (textscrposi - clickscrposi).length();
		//					if (clickdistance < 40) {
		//						group->addChild(
		//								AddDetailedInfo(xmlch_x, xmlch_y, xmlch_z,
		//										xmlch_AttrContent));
		//					}
		//				}

		//This block is to apply perceptive effects
		coefficient = CombinedFiltersEffect(objdistance,
				scrdistance, nearestdistance, neartextdistance, LOI,
				ScrTextPos);

		SetTextColorVisibility(text, coefficient);
		SetTextSize(text, coefficient);
		SetTextResolution(text, coefficient);

		//This block is to acquire the bounding box of text on screen.
		linLen = XMLString::stringLen(xmlch_AttrName);
		alfa = linLen;
		beta = text->getCharacterHeight();
		alfa = (alfa * beta)/4;

		Low3D = osg::Vec3(ObjTextPos.x() - alfa, ObjTextPos.y(),
				ObjTextPos.z());
		High3D = osg::Vec3(ObjTextPos.x() + alfa, ObjTextPos.y(),
				ObjTextPos.z() + beta);
		ScrLow = Low3D * M * V * P * W;
		ScrHigh = High3D * M * V * P * W;

		Low2D = osg::Vec2(ScrLow.x(), ScrLow.y());
		High2D = osg::Vec2(ScrHigh.x(), ScrHigh.y());

	} //for

}

