#include "main.h"
#include "okapi/api.hpp"
#include "driver/flywheel.hpp"

using namespace okapi;

void indexer_task(void*) {
	ControllerButton L1(ControllerDigital::L1);
	ControllerButton L2(ControllerDigital::L2);
	ControllerButton A(ControllerDigital::A);

	Controller controller;

	pros::ADIPort indexer('A', pros::E_ADI_DIGITAL_OUT);
	Motor flywheel(13);
	flywheel.setGearing(AbstractMotor::gearset::red);

	double rate = 5.1;

	bool run_flywheel = true;
	double active = 63;

	indexer.set_value(false);

	while (true) {
		if (A.changedToPressed()) {
			run_flywheel = !run_flywheel;
		}
		if (run_flywheel) {
			if (angled_up) {
				active = 70;
				rate = 4.3;
			} else {
				active = 70;
				rate = 4.8;
			}
			
			flywheel.moveVoltage(ptv(active));

			if (L1.changedToPressed()) {
				flywheel.moveVoltage(ptv(100));
				for (int i = 0; i < 3; i++) {
					if (i == 2) {
						pros::delay(50);
					}
					indexer.set_value(true);
					pros::delay(1000 / rate * 0.3);

					indexer.set_value(false);
					pros::delay(1000 / rate * 0.7);
				}
				pros::delay(250);

				flywheel_idle = true;
			}

			if (L2.changedToPressed()) {
				indexer.set_value(true);
				pros::delay(1000 / rate * 0.3);

				indexer.set_value(false);
				pros::delay(1000 / rate * 0.7);

				flywheel.moveVoltage(ptv(active));
			}
		} else {
			flywheel.moveVoltage(0);
		}
		pros::delay(10);
	}
}
