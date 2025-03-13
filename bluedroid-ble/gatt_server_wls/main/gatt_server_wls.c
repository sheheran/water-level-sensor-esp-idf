/**********************************************************************************************************
 * 
 * This demo showcase creating a GATT database using a predefined attribute table.
 * It acts as a GATT server and can send adv data, be connect client.
 * Run the gatt_client demo, the client demo will automaticlly connect to the gatt_server_service.
 * Client demo will enable GATT server's notify after connection. The two devices will then exchange data.
 *  
***********************************************************************************************************/

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

#define GATTS_TABLE_TAG "GATTS_TABLE_WLS"


void app_main(void)
{

}
