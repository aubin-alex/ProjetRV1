/*
 * TestWithNantesModel.h
 *
 *  Created on: Mar 7, 2014
 *      Author: fanzhang
 */

#ifndef TESTWITHNANTESMODEL_H_
#define TESTWITHNANTESMODEL_H_

#include <osg/Matrix>
#include <osg/Timer>

/*
 * declaration of extern variables
 */

extern osg::Vec4d drawbd;

extern const float MaxDistance;

extern const osg::Vec3f ModelCenter;

extern const osg::Vec3d NearestPoint;

extern osg::Matrix M, V, P, W;

extern osg::Vec3d eye, center, up;

extern unsigned int ScrCenterX, ScrCenterY;

extern float clkx, clky;

extern int NumOfTexts, NumOfOccludedTexts;

//for perceptive factors
extern bool SizePara;
extern bool ColorPara;
extern bool VisibilityPara;
extern bool ResolutionPara;
extern bool HeightPara;

//for processing functions
extern bool objPiecewisePara;
extern bool objOrderingPara;
extern bool objSinusoidPara;
extern bool objLinearPara;
extern bool objConstPiecePara;
extern bool originalPara;
extern bool scrLinearPara;
extern bool scrFisheyePara;
extern bool scrCosinusPara;
extern bool scrStdCosinPara;
extern bool scrEllipsePara;
extern bool scrCplLinearPara;
extern bool LOIPara;
extern bool defaultPara;
extern bool starttimePara;
extern bool endtimePara;
extern bool scrMyFilterPara;

extern bool AuxiliaryLinePara;

extern bool focusPara;
extern bool usertestPara;

extern osg::Timer_t starttime, endtime;

#endif /* TESTWITHNANTESMODEL_H_ */
