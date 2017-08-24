float convert_moisture(float temp_hz, float moist_hz, float calibration_values[4]){
	float y_0 = calibration_values[0]*temp_hz+calibration_values[1];
	float y_100 = calibration_values[2]*temp_hz + calibration_values[3];
	float slope_prime = (100/(y_0-y_100));
	float intercept_prime=-slope_prime*(y_0);
	return moist_hz*slope_prime + intercept_prime;
}

float convert_temperature(float temp_hz, float calibration_values[2]){
	return calibration_values[0] * temp_hz + calibration_values[1];
}
