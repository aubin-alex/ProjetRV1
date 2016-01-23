/*
 * FirstPersonMouseManipulator.h
 * Vincent Tourre
 */

#ifndef FIRST_PERSON_MOUSE_MANIPULATOR
#define FIRST_PERSON_MOUSE_MANIPULATOR 1

#include <osgGA/FirstPersonManipulator>


/** FirstPersonMouseManipulator is class for camera control based FirstPersonManipulator
    with mouse actions. */
class FirstPersonMouseManipulator : public osgGA::FirstPersonManipulator
{
        typedef FirstPersonManipulator inherited;

    public:

        FirstPersonMouseManipulator( int flags = DEFAULT_SETTINGS );
        FirstPersonMouseManipulator( const FirstPersonMouseManipulator& fpmm,
                                const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY );

        META_Object( osgGA, FirstPersonMouseManipulator );

        bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );

    protected:

        osg::Vec3d _center;
        double     _distance;

        double _trackballSize;
        double _wheelZoomFactor;

        double _minimumDistance;
        static int _minimumDistanceFlagIndex;

        void setMinimumDistance( const double& minimumDistance, bool relativeToModelSize );
        void setWheelZoomFactor( double wheelZoomFactor );

};


//
//  inline methods
//

/// Returns velocity.
//double FirstPersonMouseManipulator::getVelocity() const  { return _velocity; }


#endif /* FIRST_PERSON_MOUSE_MANIPULATOR */
