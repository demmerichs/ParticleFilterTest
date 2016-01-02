/*
 * Wall.cpp
 *
 *  Created on: 27.12.2015
 *      Author: Marvin
 */

#include "wall.h"

#include "laser.h"
#include "lasersensor.h"

Wall::Wall(World* const world,const CoordinateSystem* const refBase, const Coordinate& origin, const Vector& normale):
	Object(world,
		   CoordinateSystem(refBase,
							origin,
							normale))
{
}

Wall::~Wall()
{
}

double Wall::evalLaser(const Laser &laser) const
{
	if(this->base.axes[0]*laser.axes[0]==0){	//wall parallel to laser
		if((this->base-laser)*this->base.axes[0]==0){	//laser completly inside the wall
			return 0;
		} else {	//laser completly outside the wall
			return laser.getRange();
		}
	}

	double intersectionLength=((this->base-Coordinate(this->base.getBase()))*this->base.axes[0])/(laser.axes[0]*this->base.axes[0]);
	if(intersectionLength<0 || intersectionLength>laser.getRange()) {
		return laser.getRange();
	}

	return intersectionLength;
}
