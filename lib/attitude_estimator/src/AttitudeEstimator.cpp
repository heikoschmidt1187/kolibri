#include <AttitudeEstimator.h>
#include <DroneConfig.h>

AttitudeEstimator::AttitudeEstimator(const float stdARate,
									 const float stdAAngle, const float dt)
	: dt(dt)
{
	varianceRateMeasurement = pow(stdARate, 2.F);
	varianceAngleMeasurement = pow(stdAAngle, 2.F);

	uncertaintyRoll = varianceRateMeasurement;
	uncertaintyPitch = varianceRateMeasurement;
}

void AttitudeEstimator::Init()
{
}

void AttitudeEstimator::Process(const float rollRate, const float rollAngle,
								const float pitchRate, const float pitchAngle)
{
	// use Kalman 1D to estimate roll and pitch

	// calculate next Kalman state
	currentRoll += dt * rollRate;
	currentPitch += dt * pitchRate;

	// update the uncertainty
	uncertaintyRoll += pow(dt, 2.F) * varianceRateMeasurement;
	uncertaintyPitch += pow(dt, 2.F) * varianceRateMeasurement;

	// determine Kalman gain
	float gainRoll =
		uncertaintyRoll / (uncertaintyRoll + varianceAngleMeasurement);
	float gainPitch =
		uncertaintyPitch / (uncertaintyPitch + varianceAngleMeasurement);

	// weight Kalman state
	currentRoll += gainRoll * (rollAngle - currentRoll);
	currentPitch += gainPitch * (pitchAngle - currentPitch);

	// update uncertainties
	uncertaintyRoll = (1 - gainRoll) * uncertaintyRoll;
	uncertaintyPitch = (1 - gainPitch) * uncertaintyPitch;

#ifdef MEASUREMENT_MODE
	Serial.printf(
		"AttitudeEstimator: Roll %f ° | UncertRoll %f | Pitch %f ° | UncertPitch %f\n",
		currentRoll, uncertaintyRoll, currentPitch, uncertaintyPitch);
#endif
}