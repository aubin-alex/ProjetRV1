/*
 * Filters.cpp

 *
 *  Created on: Mar 7, 2014
 *      Author: fanzhang
 */

#include <osg/Geode>
#include <osg/Timer>
#include <osgText/Text>

#include <string>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "myXparser.h"
#include "TestWithNantesModel.h"

template <typename T>
std::string NumberToString ( T Number )
{
	std::stringstream ss;
	ss << Number;
	return ss.str();
}

osg::ref_ptr<osg::Geode> DrawHUDLine() {
	osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array;
	points->push_back(osg::Vec3(0, ScrCenterY, 0));
	points->push_back(osg::Vec3(2 * ScrCenterX, ScrCenterY, 0));
	points->push_back(osg::Vec3(ScrCenterX, 2 * ScrCenterY, 0));
	points->push_back(osg::Vec3(ScrCenterX, 0, 0));

	osg::ref_ptr<osg::Geometry> line1 = new osg::Geometry;
	line1->setVertexArray(points.get());
	line1->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINES, 0, 2));

	osg::ref_ptr<osg::Geometry> line2 = new osg::Geometry;
	line2->setVertexArray(points.get());
	line2->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINES, 2, 2));

	osg::ref_ptr<osg::Geode> gd = new osg::Geode;
	gd->addDrawable(line1);
	gd->addDrawable(line2);
	return gd;
}

osg::ref_ptr<osg::Geode> DrawBoundingLine() {

	osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array;

	points->push_back(osg::Vec3(drawbd.x(), drawbd.y(), 0));
	points->push_back(osg::Vec3(drawbd.z(), drawbd.y(), 0));
	points->push_back(osg::Vec3(drawbd.z(), drawbd.w(), 0));
	points->push_back(osg::Vec3(drawbd.x(), drawbd.w(), 0));

	osg::ref_ptr<osg::Geometry> line1 = new osg::Geometry;
	line1->setVertexArray(points.get());
	line1->addPrimitiveSet(
			new osg::DrawArrays(osg::DrawArrays::LINE_LOOP, 0, 4));

	osg::ref_ptr<osg::Geode> gd = new osg::Geode;
	gd->addDrawable(line1);

	return gd;
}

osg::ref_ptr<osg::Geode> DrawHUDCircle(osg::Vec2d focus, float radius) {
	osg::ref_ptr<osg::Vec2Array> vertices = new osg::Vec2Array(67);
	(*vertices)[0].set(focus.x(), focus.y());
	for (unsigned int i = 1; i <= 65; ++i) {
		float angle = (float) (i - 1) * osg::PI / 32.0f;

		(*vertices)[i].set(focus.x() + radius * cosf(angle),
				focus.y() + radius * sinf(angle));
	}
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
	(*normals)[0].set(0.0f, -1.0f, 0.0f);
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
	(*colors)[0].set(1.0f, 1.0f, 1.0f, 1.0f);
	// Avoid color state inheriting
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->setVertexArray(vertices.get());
	geom->setNormalArray(normals.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	geom->addPrimitiveSet(
			new osg::DrawArrays(osg::DrawArrays::LINE_LOOP, 1, 65));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom.get());

	return geode;
}

osg::ref_ptr<osg::Geode> DrawHUDEllipse(osg::Vec2d focus, float a, float b) {
	osg::ref_ptr<osg::Vec2Array> vertices = new osg::Vec2Array(67);
	(*vertices)[0].set(focus.x(), focus.y());
	for (unsigned int i = 1; i <= 65; ++i) {
		float angle = (float) (i - 1) * osg::PI / 32.0f;

		(*vertices)[i].set(focus.x() + a * cosf(angle),
				focus.y() + b * sinf(angle));
	}
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
	(*normals)[0].set(0.0f, -1.0f, 0.0f);
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
	(*colors)[0].set(1.0f, 1.0f, 1.0f, 1.0f);
	// Avoid color state inheriting
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->setVertexArray(vertices.get());
	geom->setNormalArray(normals.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	geom->addPrimitiveSet(
			new osg::DrawArrays(osg::DrawArrays::LINE_LOOP, 1, 65));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom.get());

	return geode;
}

//This filter is to evaluate the visibility of HUD texts
void HUDFilter(osg::ref_ptr<osg::Camera> Cam) {

	//The (0) geode is the bottom HUD IN SetUp.cpp, so always keeps it
	while (Cam->getNumChildren() > 1) {
		Cam->removeChild(Cam->getNumChildren() - 1);
	}

	/**********Top time HUD information geode**********/
	std::string timestr;
	if (starttimePara) {
		starttime = osg::Timer::instance()->tick();
		starttimePara = false;
		endtime = 0;
	}
	if (endtimePara) {
		endtime = osg::Timer::instance()->tick();
		endtimePara = false;
	}

	if(starttime!=0){
		timestr = timestr + " Test starts at (system tick unit) " + NumberToString(starttime) + "; Test ends at ";
	}

	if ((starttime != 0) && (endtime != 0)) {
		timestr = timestr + NumberToString(endtime);
		timestr = timestr+ "; Total time elapsed (seconds): " + NumberToString(osg::Timer::instance()->delta_s(starttime, endtime));
	}

	osg::ref_ptr<osg::Geode> timegeode = new osg::Geode;
	osg::ref_ptr<osgText::Text> timehud = new osgText::Text;
	timehud->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	timehud->setCharacterSize(ScrCenterY / 22, 1.0);
	timehud->setBackdropColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	timehud->setFont("Data/fudd.ttf");
	timehud->setPosition(osg::Vec3(ScrCenterX/12, ScrCenterY * 1.8, 0.0f));
	timehud->setFontResolution(32, 32);
	timehud->setText(timestr);
	timegeode->addDrawable(timehud);
	Cam->addChild(timegeode);

	/**********Top hud information geode**********/
	osg::ref_ptr<osg::Geode> topgeode = new osg::Geode;
	std::string keystr = " Current active keys:";
	std::string modestr = " Current mode:";
	if (defaultPara) {
		modestr = modestr + "  training mode";
	} else {
		modestr = modestr + "  test mode";
		if (ColorPara) {
			keystr = keystr + "  C,";
		}
		if (SizePara) {
			keystr = keystr + "  T,";
		}
		if (VisibilityPara) {
			keystr = keystr + "  V,";
		}
		if (ResolutionPara) {
			keystr = keystr + "  R,";
		}
		if (objPiecewisePara) {
			keystr = keystr + "  P,";
		}
		if (objOrderingPara) {
			keystr = keystr + "  O,";
		}
		if (objSinusoidPara) {
			keystr = keystr + "  J,";
		}
		if (objLinearPara) {
			keystr = keystr + "  K,";
		}
		if (objConstPiecePara) {
			keystr = keystr + "  M,";
		}
		if (scrLinearPara) {
			keystr = keystr + "  L,";
		}
		if (scrFisheyePara) {
			keystr = keystr + "  E,";
		}
		if (scrStdCosinPara) {
			keystr = keystr + "  D,";
		}
		if (scrCosinusPara) {
			keystr = keystr + "  B,";
		}
		if (scrEllipsePara) {
			keystr = keystr + "  A,";
		}
		if (scrCplLinearPara) {
			keystr = keystr + "  W,";
		}
		if (LOIPara) {
			keystr = keystr + "  I,";
		}
		if (usertestPara) {
			keystr = keystr + "  Y,";
		}
		if (originalPara) {
			keystr = keystr + "  H,";
		}
	}

	osg::ref_ptr<osgText::Text> modehud = new osgText::Text;
	modehud->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	modehud->setCharacterSize(ScrCenterY / 22, 1.0);
	modehud->setBackdropColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	modehud->setFont("Data/fudd.ttf");
	modehud->setPosition(osg::Vec3(ScrCenterX / 12, ScrCenterY * 1.9, 0.0f));
	modehud->setFontResolution(32, 32);
	modehud->setText(modestr);

	osg::ref_ptr<osgText::Text> keyhud = new osgText::Text;
	keyhud->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	keyhud->setCharacterSize(ScrCenterY / 22, 1.0);
	keyhud->setBackdropColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	keyhud->setFont("Data/fudd.ttf");
	keyhud->setPosition(osg::Vec3(ScrCenterX / 12, ScrCenterY * 1.85, 0.0f));
	keyhud->setFontResolution(32, 32);
	keyhud->setText(keystr);

	topgeode->addDrawable(modehud);
	topgeode->addDrawable(keyhud);
	Cam->addChild(topgeode);

	/******************Bottom HUD information geode**********************/
	osg::ref_ptr<osg::Geode> bottomgeode = new osg::Geode;

	//This block is to add the perceptive factor HUD
	std::string factorstr = " Current perceptive factor(s):";
	if (ColorPara) {
		factorstr = factorstr + "  color";
	}
	if (SizePara) {
		factorstr = factorstr + "  size";
	}
	if (VisibilityPara) {
		factorstr = factorstr + "  visibility";
	}
	if (ResolutionPara) {
		factorstr = factorstr + "  resolution";
	}

	osg::ref_ptr<osgText::Text> factorhud = new osgText::Text;
	factorhud->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	factorhud->setCharacterSize(ScrCenterY / 22, 1.0);
	factorhud->setBackdropColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	factorhud->setFont("Data/fudd.ttf");
	factorhud->setPosition(osg::Vec3(ScrCenterX / 12, ScrCenterY * 0.19, 0.0f));
	factorhud->setFontResolution(32, 32);
	factorhud->setText(factorstr);
	bottomgeode->addDrawable(factorhud);

	//This block is to add the function HUD
	std::string funcstr = " Current applied function(s):";
	if (objPiecewisePara) {
		funcstr = funcstr + "  OPW";
	}
	if (objOrderingPara) {
		funcstr = funcstr + "  OO";
	}
	if (objSinusoidPara) {
		funcstr = funcstr + "  OS";
	}
	if (objLinearPara) {
		funcstr = funcstr + "  OL";
	}
	if (objConstPiecePara) {
		funcstr = funcstr + "  OCPW";
	}
	if (scrLinearPara) {
		funcstr = funcstr + "  SL";
	}
	if (scrFisheyePara) {
		funcstr = funcstr + "  SFE";
	}
	if (scrStdCosinPara) {
		funcstr = funcstr + "  SC";
	}
	if (scrCosinusPara) {
		funcstr = funcstr + "  SCOMC";
	}
	if (scrEllipsePara) {
		funcstr = funcstr + "  SE";
	}
	if (scrCplLinearPara) {
		funcstr = funcstr + "  SCOML";
	}
	if (LOIPara) {
		funcstr = funcstr + "  LOI";
	}

	osg::ref_ptr<osgText::Text> funchud = new osgText::Text;
	funchud->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	funchud->setCharacterSize(ScrCenterY / 22, 1.0);
	funchud->setBackdropColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	funchud->setFont("Data/fudd.ttf");
	funchud->setPosition(osg::Vec3(ScrCenterX / 12, ScrCenterY * 0.15, 0.0f));
	funchud->setFontResolution(32, 32);
	funchud->setText(funcstr);
	bottomgeode->addDrawable(funchud);

	Cam->addChild(bottomgeode);

	//This block is to add the texts number HUD
	osg::ref_ptr<osg::Geode> evaluationgeode = new osg::Geode;
	std::string numstr;
	osg::ref_ptr<osgText::Text> numhud = new osgText::Text;
	numhud->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	numhud->setCharacterSize(ScrCenterY / 22, 1.0);
	numhud->setBackdropColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	numhud->setFont("Data/fudd.ttf");
	numhud->setPosition(osg::Vec3(ScrCenterX / 12, ScrCenterY * 0.23, 0.0f));
	numhud->setFontResolution(32, 32);
	numstr = " Current text number:" + NumberToString(NumOfTexts);
	numhud->setText(numstr);
	evaluationgeode->addDrawable(numhud);

	//This block is to add the occluded texts number HUD
	std::string occludedstr;
	occludedstr = " Occluded text number:" + NumberToString(NumOfOccludedTexts);
	osg::ref_ptr<osgText::Text> occnumhud = new osgText::Text;
	occnumhud->setColor(osg::Vec4(0.988f, 0.812f, 0.333f, 1.0f));
	occnumhud->setCharacterSize(ScrCenterY / 22, 1.0);
	occnumhud->setBackdropColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	occnumhud->setFont("Data/fudd.ttf");
	occnumhud->setPosition(osg::Vec3(ScrCenterX, ScrCenterY * 0.19, 0.0f));
	occnumhud->setFontResolution(32, 32);
	occnumhud->setText(occludedstr);

	Cam->addChild(evaluationgeode);

	if (AuxiliaryLinePara) {
		Cam->addChild(DrawHUDLine());
	}
	if (scrFisheyePara) {
		osg::Vec2 foc(ScrCenterX, ScrCenterY);
	}
	if (scrEllipsePara) {
		osg::Vec2 foc(ScrCenterX, ScrCenterY);
	}

}

//Object space sinusoid function
float objSinusoidFilter(float distance) {
	float objSinusoid;
	float u, v, x, ratio;

	ratio = distance / MaxDistance;
	x = ratio;
	u = 1.0;
	v = 0.69;

	if ((0 < x) && (x < v)) {
		objSinusoid = M_PI * x;
		objSinusoid = objSinusoid / v;
		objSinusoid = objSinusoid - M_PI / 2;
		objSinusoid = sin(objSinusoid);
		objSinusoid = objSinusoid * u;
		objSinusoid = objSinusoid / 2;
		objSinusoid = objSinusoid + u / 2;
	} else {
		objSinusoid = u;
	}
	objSinusoid = 1 - objSinusoid;
	return objSinusoid;
}

//Object space distance piecewise filter
float objPiecewiseFilter(float camtextdistance) {

	float objPiecewise;
	float ratio = camtextdistance / MaxDistance;
	float x = 1 - ratio;

	if (x <= 0.69) {
		objPiecewise = 1.0;
	} else if (x <= 0.75) {
		objPiecewise = 5.45 - 5 * x;
		objPiecewise = objPiecewise / 2;
	} else if (x <= 0.9) {
		objPiecewise = 8.55 - 8 * x;
		objPiecewise = objPiecewise / 3;
	} else if (x <= 0.979) {
		objPiecewise = 368.55 - 370 * x;
		objPiecewise = objPiecewise / 79;
	} else if (x <= 0.986) {
		objPiecewise = 4.224 - 4 * x;
		objPiecewise = objPiecewise / 7;
	} else {
		objPiecewise = 0;
	}

	return objPiecewise;
}

float objConstantPiecewise(float camtextdistance) {

	float ratio, objConstPiece;
	ratio = camtextdistance / MaxDistance;
	if (ratio < 0.014) {
		objConstPiece = 0.01;
	} else if (ratio < 0.027) {
		objConstPiece = 0.04;
	} else if (ratio < 0.061) {
		objConstPiece = 0.09;
	} else if (ratio < 0.127) {
		objConstPiece = 0.18;
	} else if (ratio < 0.233) {
		objConstPiece = 0.33;
	} else if (ratio < 0.342) {
		objConstPiece = 0.465;
	} else if (ratio < 0.452) {
		objConstPiece = 0.55;
	} else if (ratio < 0.55) {
		objConstPiece = 0.65;
	} else if (ratio < 0.63) {
		objConstPiece = 0.71;
	} else if (ratio < 0.68) {
		objConstPiece = 0.76;
	} else if (ratio < 0.81) {
		objConstPiece = 0.8;
	} else if (ratio < 0.8) {
		objConstPiece = 0.83;
	} else if (ratio < 0.85) {
		objConstPiece = 0.86;
	} else if (ratio < 0.9) {
		objConstPiece = 0.91;
	} else if (ratio < 0.98) {
		objConstPiece = 0.95;
	} else {
		objConstPiece = 1.0;
	}
	return objConstPiece;
}

//Object space linear function
float objLinearFilter(float camtextdistance) {
	float objLinear;
	float u, v, ratio, x;

	ratio = camtextdistance / MaxDistance;
	x = ratio;
	u = 1.0;
	v = 0.73;

	if ((0 <= x) & (x < v)) {
		objLinear = u / v;
		objLinear = objLinear * x;
	} else {
		objLinear = u;
	}

	objLinear = 1 - objLinear;
	return objLinear;

}

float objOrderingFilter(float NearestDistance, float NearTextDistance) {
	float objOrdering;

	//This block is to calculate the coefficient base at this distance
	float tempSize = 0.0;
	float a = 42 / 13;
	a = a * NearestDistance;
	a = a / MaxDistance;
	float b = 0.36 / 13;
	tempSize = a + b;
	tempSize = tempSize * 2;

	//tempSize is used to calculate the ordering coefficient
	objOrdering = 0.5 * NearTextDistance;
	objOrdering = objOrdering / 2000;
	objOrdering = objOrdering * tempSize;

	return objOrdering;
}

//Screen space distance linear filter
float scrLinearFilter(float ScrDistance) {
	float a, b, c;

	a = ScrCenterX * ScrCenterX;
	b = ScrCenterY * ScrCenterY;
	c = sqrt(a + b);
	float scrlinear = ScrDistance / c;
	scrlinear = 1 - scrlinear;
	return scrlinear;
}

float scrCplLinearFilter(float ScrDistance) {
	float scrCplLinear = 1.0;
	float a, b, c;
	a = ScrCenterX * ScrCenterX;
	b = ScrCenterY * ScrCenterY;
	c = sqrt(a + b);
	float u, v, i;
	u = 0.6;
	v = 0.4;
	i = 0.1;
	float x = ScrDistance / c;

	if (x <= i) {
		scrCplLinear = 1.0;
	} else if (x <= v) {
		scrCplLinear = 1 - u * (x - i) / (v - i);
	} else {
		scrCplLinear = 1 - u;
	}
	return scrCplLinear;
}
//Screen space fisheye filter
float scrFisheyeFilter(float ScrDistance) {
	float scrfisheye = 1;

	float ratio = ScrDistance / ScrCenterX;

	if (ratio <= 0.4) {
		scrfisheye = scrfisheye * 1.0;
	} else if (ratio <= 0.6) {
		scrfisheye = scrfisheye * 0.75;
	} else {
		scrfisheye = scrfisheye * 0.4;
	}

	return scrfisheye;
}

//Screen space simple ellipse filter
//Need to add 'Focus' point into the function, currently is the screen center
float scrEllipseFilter(osg::Vec3 Posi) {

	float screllipse = 1;
	float x = Posi.x();
	float y = Posi.y();
	float a = 0.7 * ScrCenterX;
	float b = 0.5 * ScrCenterX;

	float value1 = (x - ScrCenterX) * (x - ScrCenterX);
	value1 = value1 / a;
	value1 = value1 / a;
	float value2 = (y - ScrCenterY) * (y - ScrCenterY);
	value2 = value2 / b;
	value2 = value2 / b;

	float value = value1 + value2;

	if (value <= 1) {
		screllipse = 1.2;
	} else {
		screllipse = 0.6;
	}
	return screllipse;
}

//Screen space parametrised cosinusoid filter
float scrCosinusFilter(float ScrDistance) {
	float scrcosin = 1;
	float reduction = 0.6;
	float width = 0.4;

	float a, b, c;
	a = ScrCenterX * ScrCenterX;
	b = ScrCenterY * ScrCenterY;
	c = sqrt(a + b);
	float ratio = ScrDistance / c;

	if (ratio <= width) {
		scrcosin = scrcosin - reduction
				+ (reduction / 2) * (cos(M_PI * ratio / width) + 1);
	} else {
		scrcosin = scrcosin - reduction;
	}
	return scrcosin;
}

float scrStdCosinFilter(float ScrDistance) {
	float scrstdcosin = 1;
	float a, b, c;
	a = ScrCenterX * ScrCenterX;
	b = ScrCenterY * ScrCenterY;
	c = sqrt(a + b);

	float x = ScrDistance / c;

	scrstdcosin = cos(M_PI * x / 2);

	return scrstdcosin;
}

//Simple level-of-importance filter. For LOI, there are only three values, 3,2,1, hence
//the filter value is 1.0, 0.67, 0.33 respectively
float LOIFilter(int LOI) {
	float LOIfilter;
	LOIfilter = LOI / 3.0;
	return LOIfilter;
}

//The final output can be decided by just one filter or co-decided by more than one filters
//Obj for object space distance, Scr for screen distance
float CombinedFiltersEffect(float Obj, float Scr, float Nearest, float NearText,
		int LOI, osg::Vec3 ScrPosi) {

	float finalfilter = 1;

	if (objPiecewisePara) {
		finalfilter = finalfilter * objPiecewiseFilter(Obj);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (scrLinearPara) {
		finalfilter = finalfilter * scrLinearFilter(Scr);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (scrEllipsePara) {
		finalfilter = finalfilter * scrEllipseFilter(ScrPosi);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (LOIPara) {
		finalfilter = finalfilter * LOIFilter(LOI);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (scrCosinusPara) {
		finalfilter = finalfilter * scrCosinusFilter(Scr);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (scrFisheyePara) {
		finalfilter = finalfilter * scrFisheyeFilter(Scr);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (scrStdCosinPara) {
		finalfilter = finalfilter * scrStdCosinFilter(Scr);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (objOrderingPara) {
		finalfilter = finalfilter * objOrderingFilter(Nearest, NearText);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (objConstPiecePara) {
		finalfilter = finalfilter * objConstantPiecewise(Obj);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (scrCplLinearPara) {
		finalfilter = finalfilter * scrCplLinearFilter(Scr);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (objSinusoidPara) {
		finalfilter = finalfilter * objSinusoidFilter(Obj);
	} else {
		finalfilter = finalfilter * 1;
	}

	if (objLinearPara) {
		finalfilter = finalfilter * objLinearFilter(Obj);
	} else {
		finalfilter = finalfilter * 1;
	}

	return finalfilter;
}

