/*
 * AnimationPath.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: fanzhang
 */

#include <cmath>
#include <osg/AnimationPath>
#include <osg/ref_ptr>
#include <osg/Vec3>
#include <osg/Vec3d>
#include <fstream>

#include "TestWithNantesModel.h"
#include "AnimationPath.h"

osg::ref_ptr<osg::AnimationPath> createPath() {

	osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
	path->setLoopMode(osg::AnimationPath::SWING);

	/* to read path from file */
	/*
	std::ifstream infile;
	infile.open("Outputs/animationPath.txt");
	path->read(infile);
	*/

	/* to define path by hand
	 * in Quat, we can have 3 rotations, one for each axis
	 * */
	osg::AnimationPath::ControlPoint cp1 = osg::AnimationPath::ControlPoint(
			osg::Vec3f(ModelCenter.x() - 000, ModelCenter.y() - 4500, 4000),
			osg::Quat(M_PI_4, osg::Vec3f(1, 0, 0)));

	osg::AnimationPath::ControlPoint cp2 = osg::AnimationPath::ControlPoint(
			osg::Vec3f(ModelCenter.x() - 000, ModelCenter.y() - 2500, 2500),
			osg::Quat(M_PI_4, osg::Vec3f(1, 0, 0), 0, osg::Vec3f(0, 1, 0), 0,
					osg::Vec3f(0, 0, 1)));

	osg::AnimationPath::ControlPoint cp3 = osg::AnimationPath::ControlPoint(
			osg::Vec3f(ModelCenter.x() + 2000, ModelCenter.y() - 2500, 2500),
			osg::Quat(M_PI_4, osg::Vec3f(1, 0, 0), 0, osg::Vec3f(0, 1, 0), 0,
					osg::Vec3f(0, 0, 1)));

	osg::AnimationPath::ControlPoint cp4 = osg::AnimationPath::ControlPoint(
			osg::Vec3f(ModelCenter.x() + 2000, ModelCenter.y() - 2500, 2500),
			osg::Quat(M_PI_4, osg::Vec3f(1, 0, 0), 0, osg::Vec3f(0, 1, 0), M_PI_4,
					osg::Vec3f(0, 0, 1)));

	path->insert(0.0, cp1);
	path->insert(1.0, cp1);
	path->insert(5.0, cp2);
	path->insert(10.0, cp3);
	path->insert(15.0, cp4);
	path->insert(16.0, cp4);


	/* to store path file */

	std::ofstream outfile;
	outfile.open("Outputs/animationPath.txt");
	path->write(outfile);


	return path;
}

