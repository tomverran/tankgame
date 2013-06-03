#include "Computer.h"
#include "Tank.h"
#include <vector>
#include <iostream>

//Constructor - don't need to do anything, just initialise variables.
Computer::Computer(std::vector<Tank *> &tnks, int ply) : tanks(tnks), player(ply) {}

//called each frame
void Computer::tick() {

	//figure out where the player is
	glm::vec3 playerPos = tanks[player]->getPosition();
	for (std::vector<Tank *>::iterator it = tanks.begin(); it != tanks.end(); ++it) {

		Tank * tank = *it;
		if (tank != tanks[player]) {
			glm::vec3 myPos = tank->getPosition();
			float oldTurretAngle = tank->getTurretRotation();
			float oldTankAngle = tank->getAngleDegrees();
			float dx = myPos.x - playerPos.x;
			float dy = myPos.y - playerPos.y;

			//TOA, then get result in degrees.
			float angle = 90.0f - glm::degrees(atan(dx / dy));

			//sort out angle issues
			if (dx > 0 && dy > 0) {
				angle += 180.0f;
			} else if (dx < 0 && dy > 0) {
				angle -= 180.0f;
			}

			//fix overflowing
			if (angle < 0.0f) {
				angle = 360.0f - abs(angle);
			} else if (angle > 360.0f) {
				angle -= 360.0f;
			}

			//correct aim if out more than 5
			if (abs(angle - oldTurretAngle) > 2.0f) {
				if (((angle < oldTurretAngle && oldTurretAngle - angle < 180.0f) || angle - oldTurretAngle >= 180.0f)) {
					tank->setTurretRotation(oldTurretAngle - 2.0f);			
				} else {
					tank->setTurretRotation(oldTurretAngle + 2.0f);
				}			
			}

			//repeat for tank. Bad coding abounds.
			if (abs(angle - oldTankAngle) > 5.0f) {
				if (((angle < oldTankAngle && oldTankAngle - angle < 180.0f) || angle - oldTankAngle >= 180.0f)) {
					tank->rotate( - 1.0f);			
				} else {
					tank->rotate( + 1.0f);
				}			
			}

			//finally move towards the player
			if (glm::distance(playerPos, tank->getPosition()) > 150.0f) {
				tank->setVelocity(2.0f);
			} else {
				tank->setVelocity(0.0f);
			}

		}	
	}
}