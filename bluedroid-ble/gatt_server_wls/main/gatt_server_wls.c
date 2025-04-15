/**********************************************************************************************************
 * 
 * This demo showcase creating a GATT database using a predefined attribute table.
 * It acts as a GATT server and can send adv data, be connect client.
 * Run the gatt_client demo, the client demo will automaticlly connect to the gatt_server_service.
 * Client demo will enable GATT server's notify after connection. The two devices will then exchange data.
 *  
***********************************************************************************************************/


/**********************************************************************************************************
 * Includes are required for the "FreeRTOS" and underlying system components to run, including logging and library
 * to store data in non-volatile flash mempory.
 * `bt.h`: impliments BT controller and VHCL configuration procedures from the host side.
 * `eso_bt_main.h` : impliments initialization and enabling of the Bluedroid stack.
 * `esp_gap_ble_api.h` :  impliments GAP configurations such as advertising and connection parameters.
 * `esp_gatts_api.h` : impliments GATT server configurations such as creating services and characteristics.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "gatt_server_wls.h"
#include "esp_gatt_common_api.h"

#define GATTS_TABLE_TAG "GATTS_TABLE_WATER_LEVEL_PROFILE"


#define GATTS_SERVICE_UUID 0x00FF

/*
Finally, the Application Profiles are registered using the Application ID, which is an user-assigned number to identify each profile. 
In this way, multiple Application Profiles can run in one server. Applicatio ID is 0x55,
*/

#define PROFILE_NUM			1
#define PROFILE_APP_IDX		0
#define ESP_APP_ID			0x55
#define SAMPLE_DEVICE_NAME	"ESP_GATTS"
#define SCV_INST_ID			0

/* Application Profile */
/* 
Application profile was designed to have one service and one characteristic, and that the characteristic has one descriptor.
*/

struct gatts_profile_inst {
	esp_gatts_cd_t gatts_cd;
	uint16_t gatts_if;				//	GATT Interface
	uint16_t app_id;				//	Application ID
	uint16_t conn_id;				//	Connection ID
	uint16_t service_handle;		//	Service handle
	esp_gatt_srvc_id_t service_id;	//	Service ID
	uint16_t char_handle;			//	Character handle
	esp_bt_uuid_t char_uuid;		//	Characteristic UUID
	esp_gatt_perm_t perm;			//	Attribute permission
	esp_gatt_char_prop_t prioerty;	//	Characteristic properties
	uint16_t descr_handle;			//	Client Charecteristic Configuration descriptor handle
	esp_bt_uuid_t descr_uuid;		//	Client Charecteristic Configuration descriptor UUID
};

/* Application Profile Structure Implimentation */

static  

/* One gatt-based profile one 'app_id' and one `gatts_if`, this array will store the 'gatts_if' returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst heart_rate_profile_tab[PROFILE_NUM] = {
    [PROFILE_APP_IDX] = {
        .gatts_cb = gatts_profile_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
};

/* GATTS Event Handler */
static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
	/* If event is register event, store the gatts_if for each profile*/
}

/* GAP Event Handler */
static void gap_event_handler()
{

}

/* Setting GAP Parameters */
typedef struct {
	bool set_scan_rsp;				/* !< Set this advertising data as scan response or not */
	bool include_name;				/* !< Advertising data include device name or not */
	bool include_txpower;			/* !< Advertising data include TX power */
	int min_interval;				/* !< Advertising data show slave preferred connection min interval */
	int max_interval;				/* !< Advertising data show slave preferred connection max interval */
	int appearance;					/* !< External appearance of device */
	uint16_t manufacturer_len;		/* !< Manufacturer data length */
	uint8_t *p_manufacturer_data;	/* !< Manufacturer data length */
	uint16_t service_data_len;		/* !< Manufacturer data point */
	uint8_t *p_service_data;		/* !< Service data length */
	uint16_t service_uuid_len;		/* !< Service uuid array point */
	uint8_t flag;					/* Advertising flag of discovery mode, see BLE_ADV_DATA_FLAG detail */
} esp_ble_adv_data_t;

static esp_ble_adv_t water_level_adv_config = {
	.set_scan_rsp = false,
	.include_name = true,
	.include_txpower = true,
	.min_interval = 0x0006,
	.max_interval = 0x0001,
	.appearance = 0x00,
	.manufacturer_len = 0,
	.p_manufacturer_data = NULL,
	.service_uuid_len = sizeof(water_level_serviice_uuid),
	.p_service_uuid = water_level_service_uuid,
	.flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

/* 
	The minimum and maximum slave preferred connection intervals are set in units of 1.25ms. 
	In this example, the minimun slave preferred connection interval is defined as 0x0006 * 1.25ms = 7.5ms and
	the maximum slave preferred connection interval is initialized as 0x0010 * 1.25 ms 20ms.

	An advertising paylog can be up to 31 bytes of data. It is possible that some of the parameters surpass the 
	31-byte advertiesment packet limit which causes the stack to cut the message and leave some of the parameters out.

	To solve this, usually the longer parameters are stored in the scan response, which can be configured using the same
	`esp_ble_gap_config_adv_data()` function and an additional esp_ble_adv_data_t type structure with the .set_scan_rsp parameter
	is set to true.

	Finally, to set the device name the 'esp_ble_gap_set_device_name()` function is used. 
*/

/* Full Database Description - Used to add attributes*/


static void gatts_profile_event_handler(esp_gatts_cb_event_t event,
					esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

void app_main(void)
{
	esp_err_t ret;

	/* Initialize NVS */
	/* The main function starts by initializing the non-volatile storage library */
	ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGE || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init()
	}
	ESP_ERROR_CHECK( ret );

	ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

	/* esp bluetooth controller initialize */

	/* The main function initializes the BT controller by first creating a BT controller configuration structure,
	named esp_bt_controller_config_t with default settings generated by the BT_CONTROLLER_INIT_CONFIG_DEFAULT() macro 
	
	The BT controller implements the "Host Controller Interface (HCI)" on the controller side, 
	the Link Layer (LL) and the Physical Layer (PHY).  The BT Controller is invisible to the user applications,
	and deals with the lower layers of the BLE stack.

	The controller configuration includes setting the BT controller stack size, priority and HCI baud rate. 
	With the settings created, the BT controller is initialized and enabled with the esp_bt_controller_init() function:

	*/
	esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
	ret = esp_bt_controller_init(&bt_cfg);
	if (ret) {
		ESP_LOGE(GATTS_TABLE_TAG, "%s enable controller failed", __func__, esp_err_to_name(ret));
		return;
	}

	ESP_LOGI(GATTS_TABLE_TAG, "%s init bluetooth", __func__, esp_err_to_name(ret));

	/* esp bluetooth controller enable */
	/*
	Next, the controller is enabled in BLE Mode. (Other Modes:  Bluetooth not running, BLE mode, BT Classic mode, Dual mode) 
	*/
	ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
	if (ret) {
		ESP_LOGE(GATTS_TABLE_TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
		return;
	}

	/* esp bluedroid init */
	/* After the initialization of the BT controller, the Bluedroid stack, which includes the common definitions and APIs for both BT Classic and BLE, is initialized and enabled.
	
	The Bluetooth stack is up and running at this point in the program flow, however the functionality of the application has not been defined yet.
	*/
	ret = esp_bluedroid_init();
	if (ret) {
		ESP_LOGE(GATTS_TABLE_TAG, "%s int bluetooth failed: %s", __func__, esp_err_to_name(ret));
		return;
	}

	/* esp bluedriod enable */
	ret = esp_bluedriod_enable();
	if (ret) {
		ESP_LOGE(GATTS_TABLE_TAG, "%s enable bluetooth failed: %s", __func__, esp_err_to_name(ret));
		return;
	}

	/*
	The functionality is defined by reacting to events such as what happens when another device tries to read or write parameters and establish a connection. 
	GAP and GATT event handler managers.

	The application needs to register a callback function for each event handler in order to let the application know which functions are going to handle the GAP and GATT events.

	The functions gatts_event_handler() and gap_event_handler() handle all the events that are pushed to the application from the BLE stack.
	*/
	ret = esp_ble_gatts_register_callback(gatts_event_handler);
	if (ret){
		ESP_LOGE(GATTS_TABLE_TAG, "gatts register error, error code = %x", ret);
		return;
	}

	ret = esp_ble_gap_register_callback(gap_event_handler);
	if (ret){
		ESP_LOGE(GATTS_TABLE_TAG, "gap app register error, error code = %x", ret);
		return;
	}
	
	/* Finally, the Appllication Profiles are registered using tha Application ID, which is a user-assigned number to identify each profile1 */
	ret = esp_ble_gatts_app_register(ESP_APP_ID);
	if (ret){
		ESP_LOGE(GATTS_TABLE_TAG, "gatts app register error, error code = %x", ret);
	}

	/*  */
	esp_err_t local_mut_ret = esp_ble_gatt_set_local_mtu(500);
	if (local_mut_ret){
		ESP_LOGE(GATTS_TABLE_TAG, "set local MTU failed, error code = %x", local_mut_ret);
	}
}


static void gatts_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
	switch (event)
	{
	case ESP_GATTS_REG_EVT:{
		/*
		Run-time Information,
		'param->reg.status' -> tells you if registration succeeded (ESP_GATT_OK = 0)
		'param->reg.app_id' -> the application ID you used when registering.
		'gatts_if' -> the interface ID assigned to this GATT server profile.

		ESP_GATTS_REG_EVT

		esp_ble_gap_set_device_name
		esp_ble_gap_config_adv_data_raw
		esp_ble_gap_config_scan_rsp_data_raw
		esp_ble_gatts_create_attr_tab / esp_ble_gatts_create_service

		*/ 
		ESP_LOGI(GATTS_TABLE_TAG, "GATT server register, status %d, app_id %d, gatts_if %d", param->reg.status, param->reg.app_id, gatts_if);
		gl_profile_tab[ESP_APP_ID].service_id.is_primary = true;
		gl_profile_tab[ESP_APP_ID].service_id.id.inst_id = 0x00;
		gl_profile_tab[ESP_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
		gl_profile_tab[ESP_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID;
	
		// Device Name: Sets to SIMPLE_DEVICE_NAME
		esp_err_t set_dev_name_ret = esp_ble_gap_Set_device_name(SAMPLE_DEVICE_NAME);
		if (set_dev_name_ret){
			ESP_LOGE(GATTS_TABLE_TAG, "set device name failed, error code = %x", set_dev_name_ret);
		}
	#ifdef CONFIG_SET_RAW_ADV_DATA
		// Advertising Data: Configured, uses raw_adv_data
		esp_err_t raw_adv_ret = esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
		if (raw_adv_ret){
			ESP_LOGE(GATTS_TABLE_TAG, "config raw dav data failed, error code = %x", raw_adv_ret);
		}
		adv_config_done |= ADV_CONFIG_FLAG;
		esp_err_t raw_scan_ret = esp_ble_gap_config_scan_rsp_data_raw(raw_scan_rsp_data, sizeof(raw_scan_rsp_data));
		if (raw_scan_ret){
			ESP_LOGE(GATTS_TABLE_TAG, "config raw scan rsp data failed, error code = %x", raw_scan_ret);
		}
		adv_config_done |= SCAN_RSP_CONFIG_FLAG;
	#else
		// Advertising Data: Configured, uses adv_data
		esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
		if (ret){
			ESP_LOGE(GATTS_TABLE_TAG, "config adv data failed, error code = %x", ret);
		}
		adv_config_done |= SCAN_RSP_CONFIG_FLAG;
		// Advertising Data: Configured, uses scan_rsp_data (scan response data)
		ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
		if (ret){
			ESP_LOGE(GATTS_TABLE_TAG, "config scan response data failed, error code = %x", ret);
		}
		adv_config_done |= SCAN_RSP_CONFIG_FLAG;
	#endif
		// Attribute Table: Created using esp_ble_gatts_create_attr_tab
		esp_err_t create_attr_ret = esp_ble_gatts_create_attr_tab(gatt_db, gatts_if, HRS_IDX_NB, SCV_INST_ID);
		if (create_attr_ret){
			ESP_LOGE(GATTS_TABLE_TAG, "create attr table failed, error code = %x", create_attr_ret);
		}
		// Service UUID: Implicitly defined within the gatt_db array (not explicitly set here)
        // Service Creation: Implicitly done via esp_ble_gatts_create_attr_tab
	}
		break;
	
	default:
		break;
	}
	
}
