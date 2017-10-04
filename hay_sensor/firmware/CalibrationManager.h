#include "Particle.h"

struct CalibrationData {
    float moist_calibration_constants[4];
    float temp_calibration_constants[2];
    int version;
};

class CalibrationManager{
  public:
    CalibrationManager();

  private:
    int address;
    float calibration_values_moist[4] = { -0.2744, 17645.0, 0.979, 109.22 };
    float calibration_values_temp[2] = {0.0044, 7.6176};
    struct CalibrationData cal_data;
    int get_calibration_parameters();
    int convert_args(String *command);
    int update_values(String command);
};
