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

4) Upon reset, the bootloader reads the contents of the 'ota_data' partition to
determine which Application (partition) is selected to run.

Setting up the connection 

Open the project configuration menu 'idf.py menuconfig'

1) Inside the example, 'Connection Configuration' menu.
- If Wifi is used provide the Wifi-SSID and password of the AP you wish to connect to,

'Configuration' menu
Set the URL of the firmware to download in the 'Firmware Upgrade URL' option. 
The format should be 'https://<host-ip-address>:<host-port>/<firmware-image-filename>'
e.g. - 'https://192.168.2.106:8070/hello_world.bin'
** Note: The server part of the URL(192.168.102.6) must match the CN field used when 'generating certificate and the key'**

Build and flash

Run 'idf.py -p PORT flash monitor' to build and flash the project. This command check if the 
partition tab;le contains the 'ota_data' partition and restore it to an initial state. 
This allows the newly loaded app to run from the 'factory' partition.

Configure the HTTPS server

After building, need to create a self-signed certificate and run a simple HTTPS server as follows:

1)  Enter the directory containing build artifacts of the project, that will be hosted by the HTTPS server.

2) To create a new self-signed certificate and key run command ' openssl req -x509 -newkey rsa:2048 - keyout ca_key.pem -out ca_cert.pem -days 365 -nodes' .
 - When prompted for the 'common name (CN)', enter the name of the server that the "ESP-Dev-Board" will connect to
