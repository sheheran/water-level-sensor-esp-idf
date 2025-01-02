https://github.com/espressif/esp-idf/tree/v5.3.2/examples/system/ota

OTA Example setup
Application on dev-board will upgrade at runtime by downloading a new image
and flashing it to an OTA partition. 

Method 01 - native API by app_update component, demonstrated in the 'native_ota_example'.
Method 02 - Simplified APIs by esp_https_ota, demonstrated under 'simple_ota_example' and 'advanced_https_ota.

Using pre-defined partition table in menuconfig by enabling,
'CONFIG_PARTITION_TABLE_TWO_OTA'. Support three app partitions:
'factory', 'OTA_0' and 'OTA1'.

1) First boot, will load the image contained on the 'factory' partition.

2) This firmware triggers an OTA upgrade. It will download a new image from 
an HTTPS server and save it into the 'OTA0' partition.

3) Next, it updates the 'ota_data' partition to indicate which app
image should boot after the next reset.
upon reset 
