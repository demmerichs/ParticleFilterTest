#include "vector.h"

#include <cmath>
#ifdef DEBUG
#include <cassert>	//NOTE assert
#endif
#include "coordinatesystem.h"

Vector::Vector(const CoordinateSystem* const refBase, const double &x, const double &y, const double &z):
	Coordinate(refBase,
			   x,
			   y,
			   z)
{
}

Vector Vector::transform(const CoordinateSystem* const toSystem) const
{
	if(toSystem==this->refBase)
		return *this;
	bool goDownTransformTree=false;
	const CoordinateSystem* preSystem=toSystem;
	const CoordinateSystem* curSystem=preSystem->getBase();
	while(curSystem){
		if(curSystem==this->refBase){
			goDownTransformTree=true;
			break;
		}
		preSystem=curSystem;
		curSystem=preSystem->getBase();
	}
	if(goDownTransformTree || this->refBase==0){	//go down the transform tree
		const CoordinateSystem* const localToSystem=preSystem;
		Vector transformedAxes[3]={Vector(localToSystem,
								   localToSystem->axes[0].x,
								   localToSystem->axes[1].x,
								   localToSystem->axes[2].x),
								   Vector(localToSystem,
								   localToSystem->axes[0].y,
								   localToSystem->axes[1].y,
								   localToSystem->axes[2].y),
								   Vector(localToSystem,
								   localToSystem->axes[0].z,
								   localToSystem->axes[1].z,
								   localToSystem->axes[2].z)};

		//two lines for transformation
		Vector result(localToSystem,this->x,this->y,this->z);
		result=result.x*transformedAxes[0]+result.y*transformedAxes[1]+result.z*transformedAxes[2];

		return result.transform(toSystem);
	} else {	//go up the transformation tree
		const CoordinateSystem* const localToSystem=this->refBase->getBase();
#ifdef DEBUG
		if(localToSystem==0){	//this actually shouldn't happen, so assert
			assert(localToSystem==0);	//NOTE assert
			Vector result(*this);
			result.refBase=preSystem;	//if above happens, this could fix correctly
			return result.transform(toSystem);
		}
#endif
		//one line for transformation
		Vector result=this->x*this->refBase->axes[0]+this->y*this->refBase->axes[1]+this->z*this->refBase->axes[2];

		return result.transform(toSystem);
	}
#ifdef DEBUG
	assert(false);	//NOTE assert
	return Vector(toSystem);
#endif
}

double Vector::length() const
{
	return sqrt((*this)*(*this));
}

Vector operator+(const Vector& a, const Vector& b)
{
	Vector b_=b.transform(a.refBase);
	return Vector(a.refBase,a.x+b_.x,a.y+b_.y,a.z+b_.z);
}

Coordinate operator+(const Coordinate& coo, const Vector& vec)
{
	Vector vec_=vec.transform(coo.refBase);
	return Coordinate(coo.getBase(),coo.x+vec_.x,coo.y+vec_.y,coo.z+vec_.z);
}

Coordinate operator+(const Vector& vec, const Coordinate& coo)
{
	Vector vec_=vec.transform(coo.refBase);
	return Coordinate(coo.getBase(),coo.x+vec_.x,coo.y+vec_.y,coo.z+vec_.z);
}

Vector operator*(const double& s, const Vector& vec)
{
	return Vector(vec.getBase(),s*vec.x,s*vec.y,s*vec.z);
}

Vector operator*(const Vector& vec, const double& s)
{
	return Vector(vec.getBase(),s*vec.x,s*vec.y,s*vec.z);
}

Vector operator/(const Vector& vec, const double& s)
{
	return Vector(vec.getBase(),vec.x/s,vec.y/s,vec.z/s);
}

double operator*(const Vector& u, const Vector& v)
{
	Vector v_=v.transform(u.getBase());
	return u.x*v_.x+u.y*v_.y+u.z*v_.z;
}

Vector cross(const Vector& a, const Vector& b)
{
	Vector b_=b.transform(a.getBase());
	return Vector(a.getBase(),a.y*b_.z-a.z*b_.y,a.z*b_.x-a.x*b_.z,a.x*b_.y-a.y*b_.x);
}
