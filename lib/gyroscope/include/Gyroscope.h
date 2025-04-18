#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

class Gyroscope
{
public:
    Gyroscope(const uint8_t busId);

    void Init();
    void Process();

    float GetRollRate() { return rateRoll; }
    float GetPitchRate() { return ratePitch; }
    float GetYawRate() { return rateYaw; }

private:
    static constexpr float SCALE = 65.5F;
    static constexpr uint16_t CALIBRATION_MEASURMENTS = 2000U;

    uint8_t busId = 0U;

    float rateRoll = 0.F;
    float ratePitch = 0.F;
    float rateYaw = 0.F;

    float calibrationOffsetRoll = 0.F;
    float calibrationOffsetPitch = 0.F;
    float calibrationOffsetYaw = 0.F;
};

#endif /* GYROSCOPE_H_ */