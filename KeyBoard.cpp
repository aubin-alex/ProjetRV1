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
#include "TestWithNantesModel.h"
#include <iostream>
/*extern bool SizePara, ColorPara, VisibilityPara, ResolutionPara;
extern bool objPiecewisePara, objOrderingPara, objSinusoidPara, objLinearPara,
		objConstPiecePara;
extern bool scrEllipsePara, scrLinearPara, scrFisheyePara, scrCosinusPara,
		scrStdCosinPara, scrCplLinearPara, LOIPara, AuxiliaryLinePara;
extern bool defaultPara, originalPara;
extern bool starttimePara, endtimePara;

extern bool usertestPara, focusPara;

extern float clkx, clky;

extern osg::Vec3f ModelCenter;*/
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
			scrEllipsePara=!scrEllipsePara;
			return false;
			break;
		case 'b':
		case 'B':
			scrCosinusPara=!scrCosinusPara;
			return false;
			break;
		case 'c':
		case 'C':
			ColorPara=!ColorPara;
			return false;
			break;
		case 'd':
		case 'D':
		    scrStdCosinPara=!scrStdCosinPara;
		    if(scrStdCosinPara){
                eyeHome.set(south);
		    }
			return false;
			break;
		case 'e':
		case 'E':
			scrFisheyePara=!scrFisheyePara;
			if(scrFisheyePara){
                eyeHome.set(north);
			}
			return false;
			break;
		case 'g':
		case 'G':
			focusPara=!focusPara;
			return false;
			break;
		case 'h':
		case 'H':
			originalPara=!originalPara;
			return false;
			break;
		case 'i':
		case 'I':
			LOIPara=!LOIPara;
			return false;
			break;
		case 'j':
		case 'J':
			objSinusoidPara=!objSinusoidPara;
			return false;
			break;
		case 'k':
		case 'K':
			objLinearPara=!objLinearPara;
			return false;
			break;
		case 'l':
		case 'L':
			scrLinearPara=!scrLinearPara;
			return false;
			break;
		case 'm':
		case 'M':
			objConstPiecePara=!objConstPiecePara;
			return false;
			break;
		case 'n':
		case 'N':
			defaultPara=!defaultPara;
			return false;
			break;
		case 'o':
		case 'O':
			objOrderingPara=!objOrderingPara;
			return false;
			break;
		case 'p':
		case 'P':
			objPiecewisePara=!objPiecewisePara;
			return false;
			break;
		case 'r':
		case 'R':
			ResolutionPara=!ResolutionPara;
			return false;
			break;
		case 't':
		case 'T':
			SizePara=!SizePara;
			return false;
			break;
		case 'v':
		case 'V':
			VisibilityPara=!VisibilityPara;
			return false;
			break;
		case 'w':
		case 'W':
			scrCplLinearPara=!scrCplLinearPara;
			return false;
			break;
		case 'x':
		case 'X':
			AuxiliaryLinePara=!AuxiliaryLinePara;
			return false;
			break;
		case 'y':
		case 'Y':
			usertestPara=!usertestPara;
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

