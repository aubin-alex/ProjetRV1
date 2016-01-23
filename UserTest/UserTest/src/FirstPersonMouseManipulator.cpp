/*
 * FirstPersonMouseManipulator.cpp
 * Vincent Tourre
 */

#include <FirstPersonMouseManipulator.h>
#include <osg/BoundsChecking>
#include <cassert>

using namespace osg;
using namespace osgGA;

int FirstPersonMouseManipulator::_minimumDistanceFlagIndex =
		allocateRelativeFlag();

/// Constructor.
FirstPersonMouseManipulator::FirstPersonMouseManipulator(int flags) :
		inherited(flags), _distance(1.), _trackballSize(0.8) {
	setMinimumDistance(0.05, true);
	setWheelZoomFactor(0.1);
	if (_flags & SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT)
		setAnimationTime(0.2);
}

/// Constructor.
FirstPersonMouseManipulator::FirstPersonMouseManipulator(
		const FirstPersonMouseManipulator& fpmm, const CopyOp& copyOp) :
		osg::Object(fpmm, copyOp), inherited(fpmm, copyOp),
		_center(fpmm._center),
		_distance(fpmm._distance),
		_trackballSize(fpmm._trackballSize),
		_wheelZoomFactor(fpmm._wheelZoomFactor),
		_minimumDistance(fpmm._minimumDistance) {
}


/** horizontal movement with fixed height */
bool FirstPersonMouseManipulator::performMovementRightMouseButton(
		const double eventTimeDelta, const double dx, const double dy) {

	const double height = _eye.z();

	float scale = -0.3f * _distance * getThrowScale(eventTimeDelta) * 2000;
	moveForward(_rotation, -dy*scale*2);
	_eye.set(_eye.x(), _eye.y(), height);

	moveRight(-dx*scale);

	return true;
}


/** Set the minimum distance of the eye point from the center
 before the center is pushed forward.*/
void FirstPersonMouseManipulator::setMinimumDistance(
		const double& minimumDistance, bool relativeToModelSize) {
	_minimumDistance = minimumDistance;
	setRelativeFlag(_minimumDistanceFlagIndex, relativeToModelSize);
}

/** Set the mouse wheel zoom factor.
 The amount of camera movement on each mouse wheel event
 is computed as the current distance to the center multiplied by this factor.
 For example, value of 0.1 will short distance to center by 10% on each wheel up event.
 Use negative value for reverse mouse wheel direction.*/
void FirstPersonMouseManipulator::setWheelZoomFactor(double wheelZoomFactor) {
	_wheelZoomFactor = wheelZoomFactor;
}

