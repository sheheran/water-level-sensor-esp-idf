/*
* GATT Server Service Table
* 
* 'WLS_IDX_SVS' - Water-Level Service Index
* 'WLS_IDX_WL_MEAS_CHAR' - Water-Level Measurement Characteristic 
* 'WLS_IDX_WL_MEAS_VAL' -  Water-Level Measurement Value
* 'WLS_IDX_WL_MEAS_NTF_CFG' - Water-Level Measurement Notification Configuration
* 'WLS_IDX_WL_SENSOR_LOC_CHAR' - Water-Level Sensor location Characteristics  
* 'WLS_IDX_WL_SENSOR_LOC_VAL' - Water-Level Sensor Location Value
* 'WLS_IDX_CTNL_PT_CHAR' - Warer-Level Sensor Control Point Characteristic
* 'WLS_IDX_CTNL_PT_VAL' - Water-Level Sensor Control Point Value
* 'WLS_IDX_NB' - 
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Attribute State Machine */
enum{
	WSL_IDX_SVS,

	WSL_IDX_WL_MEAS_CHAR,
	WSL_IDX_WL_MEAS_VAL,
	
	WSL_IDX_WL_MEAS_NTF_CFG,
	
	WSL_IDX_SENSOR_LOC_CHAR,
	WSL_IDX_SENSOR_LOC_VAL,

	WSL_IDX_CTNL_PT_CHAR,
	WSL_IDX_CTNL_PT_VAL,

	WSL_IDX_NB,
};
