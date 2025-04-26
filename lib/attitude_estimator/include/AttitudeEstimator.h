#ifndef ATTITUDE_ESTIMATOR_H_
#define ATTITUDE_ESTIMATOR_H_

#include <Arduino.h>

class AttitudeEstimator {
public:
	AttitudeEstimator(const float stdARate, const float stdAAngle,
					  const float dt);
	void Init();
	void Process(const float rollRate, const float rollAngle,
				 const float pitchRate, const float pitchAngle);

	float GetRoll()
	{
		return currentRoll;
	}

	float GetPitch()
	{
		return currentPitch;
	}

private:
	float varianceRateMeasurement;
	float varianceAngleMeasurement;

	float currentRoll = 0.F;
	float uncertaintyRoll;

	float currentPitch = 0.F;
	float uncertaintyPitch;

	float dt;
};

#endif /* ATTITUDE_ESTIMATOR_H_ */
