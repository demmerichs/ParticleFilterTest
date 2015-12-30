/*
 * robotintelligence.h
 *
 *  Created on: 28.12.2015
 *      Author: Marvin
 */

#ifndef ROBOTINTELLIGENCE_H_
#define ROBOTINTELLIGENCE_H_

#include <vector>
#include <QtCore>

#include "lasersensorinterface.h"
#include "motoractuatorinterface.h"
#include "timeinterface.h"
#include "simulatedtestrobot.h"
#include "map.h"

struct Particle{
	double x, y;
	double ori;
	double weight;
};

class RobotIntelligence : public QThread {
private:
	LaserSensorInterface &laserData;
	MotorActuatorInterface &motorData;
	TimeInterface &timeData;

	Map map;
	std::vector<Particle> particles;
	SimulatedTestRobot myFriend;

	void run();
	void initParticles();
	std::vector<double> readSensors();
	void evalSensors();
	void resampling();
	void estimatePosition();
	void move();
	void moveParticles(double);
	double random();
	double random(double, double);
	double gaussian(double, double, double);

public:
	RobotIntelligence(LaserSensorInterface&, MotorActuatorInterface&, TimeInterface&);
	virtual ~RobotIntelligence();
	void startRobot();
	void stopRobot();
};

#endif /* ROBOTINTELLIGENCE_H_ */
