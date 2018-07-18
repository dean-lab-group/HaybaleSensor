//#include "Particle.h"
// HB_01
// String api_key = "8CSZJGWGJQFTRTN9";

// HB_02
// String api_key = "QM0ZYJCSC2LVJRLM";

// String name = "HB_03";
// String api_key = "3WK9XQIQZ0SZ7ESB";
// float temp_constants[] = {0.0044, 7.6176};
// float moist_constants[] = {-0.2744, 17645, 0.979, 109.22};

// HB_04
//String api_key = "8D287XO2KQV936JP";

String name = "HB_05";
String api_key = "D8T4LTQIQ31BZBV9";
float temp_constants[] = {0.0041, 10.407};
float moist_constants[] = {-0.22, 17838, 0.9861, 46.437};

// String name = "HB_06";
// String api_key = "NDO4K15P1C3TUBLV";
// float temp_constants[] = {0.0040, 11.112};
// float moist_constants[] = {-0.603, 19203, 0.9816, 78.208};

// String name = "HB_07";
// String api_key = "J4DZYJ56BSCF2MWK";
// float temp_constants[] = {0.0048, 6.1549};
// float moist_constants[] = {-1.3092, 22099, 0.9858, 50.6};

// HB_08
//String api_key = "N7DNS9700FZYVAH8";

// HB_09
//String api_key = "MHW6OE9W87LNPHB7";

// HB_10
//String api_key = "AXXGH3U7JZTZ9X1A";

// HB_11
//String api_key = "2E8BWMOAWO0TC1YN";

// HB_12
//String api_key = "YX42Q0QEQ0TA1PYK";




/* From Jeff Craven
% Solution
y_0 (varies, see below)
y_100 (varies, see below)
temp (varies, see below)
slope_prime=(100/(y_0-y_100)) % will be negative
intercept_prime=-slope*(y_0)  % will be positive
y_prime= x_prime*slope_prime + intercept_prime
y_0 = k1*xx + k2
y_100 = k3*xx + k4

HB_01  3d0026001751353335323536
HB_02  3b0050000c51353431383737 0.0045 6.5077
HB_03  370031000f47353235303035 0.0044 7.6176 -0.2744 17645 0.979 109.22
HB_04  300026000f47333530373233 0.0047 7.4616
HB_05  34004a000e51353338363333 0.0041 10.407 -0.22 17838 0.9861 46.437
HB_06  17002f001947333530373233 0.0040 11.112 -0.603 19203 0.9816 78.208
HB_07  210030001347353236343033   0.0048 6.1549 -1.3092 22099 0.9858 50.6
HB_08  210021000b47343432313031
HB_09  27002c001247353236343033
HB_10  2d002c000b47353235303037
HB_11  1c002a001247353236343033
HB_12  1f0041001347353236343033

*/
