#include "Particle.h"
#include "CalibrationManager.h"

CalibrationManager::CalibrationManager(){
    Particle.function("calibrate", &CalibrationManager::update_values, this);
    this->address = 0;
}

CalibrationData CalibrationManager::convert_args(String *command){
    uint len = command->length();
    char *buf = (char*) malloc(len+1);
    command->toCharArray(buf,len);
    char *values = strtok(buf, ",");

    // Dummy values for now from HB3
    float calibration_values_moist[4] = { -0.2744, 17645.0, 0.979, 109.22 };
    float calibration_values_temp[2] = {0.0044, 7.6176};
    memcpy(this->cal_data.moist_calibration_constants, calibration_values_moist, sizeof(float)*4);
    memcpy(this->cal_data.temp_calibration_constants, calibration_values_temp, sizeof(float)*2);
    return this->cal_data;
}

int CalibrationManager::update_values(String command) {
    //this->convert_args(command);
    EEPROM.put(this->address, this->cal_data);
    return 0;
}
