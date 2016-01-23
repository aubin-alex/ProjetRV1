/*
 * Filters.h
 *
 *  Created on: Dec 11, 2013
 *      Author: fzhang
 */

#ifndef FILTERS_H_
#define FILTERS_H_

//This filter is to evaluate the visibility of HUD texts
void HUDFilter(osg::ref_ptr<osg::Camera> Cam);

//Object space sinusoid function
float objSinusoidFilter(float distance);

//Object space distance piecewise filter
float objPiecewiseFilter(float camtextdistance);

float objConstantPiecewise(float camtextdistance);

//Object space fading function
float objFadingFilter(float camtextdistance);

//Object space linear function
float objLinearFilter(float camtextdistance);

float objOrderingFilter(float NearestDistance, float NearTextDistance);

//Screen space distance linear filter
float scrLinearFilter(float ScrDistance);

//Screen space simple circle fisheye filter
float scrFisheyeFilter(float ScrDistance);

//Simple levels-of-importance filter. For LOI, there are only three values, 3,2,1, hence
//the filter value is 1.0, 0.67, 0.33 respectively
float LOIFilter(int LOI);

float ParaStudy();

//The final output can be decided by just one filter or co-decided by more than one filters
//Obj for object space distance, Scr for screen distance
float CombinedFiltersEffect(float Obj, float Scr, float Nearest, float NearText,
		int LOI, osg::Vec3 ScrPosi);
#endif /* FILTERS_H_ */
