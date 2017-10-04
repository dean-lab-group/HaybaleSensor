#include "Particle.h"
#include "CalibrationManager.h"

CalibrationManager::CalibrationManager(){
    Particle.function("calibrate", &CalibrationManager::update_values, this);
    this->address = 0;
}

int CalibrationManager::get_calibration_parameters(){
    EEPROM.get(this->address, this->cal_data);
    if(this->cal_data.version != 0){
        return false;
    }else{
        return true;
    }
}

int CalibrationManager::convert_args(String *command){
    uint len = command->length();
    char *buf = (char*) malloc(len+1);
    command->toCharArray(buf,len);
    scanf(buf, "%f,%f,%f,%f,%f,%f,%d", &this->cal_data);
    // We set the version to "1" so that we know the values were updated. In the future, I would like to actually put real numbers in there.

    // Dummy values for now from HB3. Will remove this code later.
    //memcpy(this->cal_data.moist_calibration_constants, this->calibration_values_moist, sizeof(float)*4);
    //memcpy(this->cal_data.temp_calibration_constants, this->calibration_values_temp, sizeof(float)*2);

    // Need to build some logic here to ensure that the function succeeded.
    return true;
}

int CalibrationManager::update_values(String command) {
    //this->convert_args(command);
    EEPROM.put(this->address, this->cal_data);
    return 0;
}
