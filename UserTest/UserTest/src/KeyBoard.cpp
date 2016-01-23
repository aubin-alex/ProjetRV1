/*
 * KeyBoard.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: fanzhang
 */

#include <osgGA/GUIEventHandler>
#include <osgGA/OrbitManipulator>
#include <osgViewer/Viewer>
#include "ParameterStudy.h"
#include "KeyBoard.h"
#include <iostream>
extern bool SizePara, ColorPara, VisibilityPara, ResolutionPara;
extern bool objPiecewisePara, objOrderingPara, objSinusoidPara, objLinearPara,
		objConstPiecePara;
extern bool scrEllipsePara, scrLinearPara, scrFisheyePara, scrCosinusPara,
		scrStdCosinPara, scrCplLinearPara, LOIPara, AuxiliaryLinePara;
extern bool defaultPara, originalPara;
extern bool starttimePara, endtimePara;

extern bool usertestPara, focusPara;

extern float clkx, clky;

extern osg::Vec3f ModelCenter;
extern osg::Vec3d eyeHome;




bool myKeyboardEventHandler::handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa) {

	double height = 400.0;
	const osg::Vec3d north(ModelCenter.x() - 000.0, ModelCenter.y() + 5000.0, ModelCenter.z() + height);
	const osg::Vec3d south(ModelCenter.x() - 000.0, ModelCenter.y() - 5000.0, ModelCenter.z() + height);

	switch (ea.getEventType()) {
	case (osgGA::GUIEventAdapter::KEYDOWN): {
		switch (ea.getKey()) {
		case '1':
			starttimePara = true;
			return false;
			break;
		case '2':
			endtimePara = true;
			return false;
			break;
		case 'a':
		case 'A':
			if (scrEllipsePara) {
				scrEllipsePara = false;
			} else {
				scrEllipsePara = true;
			}
			return false;
			break;
		case 'b':
		case 'B':
			if (scrCosinusPara) {
				scrCosinusPara = false;
			} else {
				scrCosinusPara = true;
			}
			return false;
			break;
		case 'c':
		case 'C':
			if (ColorPara) {
				ColorPara = false;
			} else {
				ColorPara = true;
			}
			return false;
			break;
		case 'd':
		case 'D':
			if (scrStdCosinPara) {
				scrStdCosinPara = false;
			} else {
				scrStdCosinPara = true;
				eyeHome.set(south);
			}
			return false;
			break;
		case 'e':
		case 'E':
			if (scrFisheyePara) {
				scrFisheyePara = false;
			} else {
				scrFisheyePara = true;
				eyeHome.set(north);
			}
			return false;
			break;
		case 'g':
		case 'G':
			if (focusPara) {
				focusPara = false;
			} else {
				focusPara = true;
			}
			return false;
			break;
		case 'h':
		case 'H':
			if (originalPara) {
				originalPara = false;
			} else {
				originalPara = true;
			}
			return false;
			break;
		case 'i':
		case 'I':
			if (LOIPara) {
				LOIPara = false;
			} else {
				LOIPara = true;
			}
			return false;
			break;
		case 'j':
		case 'J':
			if (objSinusoidPara) {
				objSinusoidPara = false;
			} else {
				objSinusoidPara = true;
			}
			return false;
			break;
		case 'k':
		case 'K':
			if (objLinearPara) {
				objLinearPara = false;
			} else {
				objLinearPara = true;
			}
			return false;
			break;
		case 'l':
		case 'L':
			if (scrLinearPara) {
				scrLinearPara = false;
			} else {
				scrLinearPara = true;
			}
			return false;
			break;
		case 'm':
		case 'M':
			if (objConstPiecePara) {
				objConstPiecePara = false;
			} else {
				objConstPiecePara = true;
			}
			return false;
			break;
		case 'n':
		case 'N':
			if (defaultPara) {
				defaultPara = false;
			} else {
				defaultPara = true;
			}
			return false;
			break;
		case 'o':
		case 'O':
			if (objOrderingPara) {
				objOrderingPara = false;
			} else {
				objOrderingPara = true;
			}
			return false;
			break;
		case 'p':
		case 'P':
			if (objPiecewisePara) {
				objPiecewisePara = false;
			} else {
				objPiecewisePara = true;
			}
			return false;
			break;
		case 'r':
		case 'R':
			if (ResolutionPara) {
				ResolutionPara = false;
			} else {
				ResolutionPara = true;
			}
			return false;
			break;
		case 't':
		case 'T':
			if (SizePara) {
				SizePara = false;
			} else {
				SizePara = true;
			}
			return false;
			break;
		case 'v':
		case 'V':
			if (VisibilityPara) {
				VisibilityPara = false;
			} else {
				VisibilityPara = true;
			}
			return false;
			break;
		case 'w':
		case 'W':
			if (scrCplLinearPara) {
				scrCplLinearPara = false;
			} else {
				scrCplLinearPara = true;
			}
			return false;
			break;
		case 'x':
		case 'X':
			if (AuxiliaryLinePara) {
				AuxiliaryLinePara = false;
			} else {
				AuxiliaryLinePara = true;
			}
			return false;
			break;
		case 'y':
		case 'Y':
			if (usertestPara) {
				usertestPara = false;
			} else {
				usertestPara = true;
			}
			return false;
			break;
		default:
			return false;
		}
	}
		return false;
		break;
	case (osgGA::GUIEventAdapter::PUSH): {
		_mx = ea.getX();
		_my = ea.getY();
		return false;
		break;
	}
	case (osgGA::GUIEventAdapter::RELEASE): {
		if (_mx == ea.getX() && _my == ea.getY()) {
			clkx = ea.getX();
			clky = ea.getY();
		}
		return false;
		break;
	}

	case (osgGA::GUIEventAdapter::MOVE): {
		clkx = ea.getX();
		clky = ea.getY();
		return false;
		break;
	}
	default:
		return false;
	}
}

