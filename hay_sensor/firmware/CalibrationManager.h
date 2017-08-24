#include "Particle.h"

struct CalibrationData {
    float moist_calibration_constants[4];
    float temp_calibration_constants[2];
};

class CalibrationManager{
  public:
    CalibrationManager();

  private:
    int address;
    struct CalibrationData cal_data;
    CalibrationData convert_args(String *command);
    int update_values(String command);
};
