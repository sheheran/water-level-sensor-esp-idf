"bluetooth/bluedroid/ble/gatt_security_client" demonstrates how to use ESP BLE security APIs on ESP32 to establish a 
secure connection and encrypt communication with peer devices while acting as a GATT client.

"bluetooth/bluedroid/ble/gatt_security_server" demonstrates how to use ESP BLE security APIs on ESP32 to establish a
secure connection and encription communication with peer devices while acting as a GATT server.

"bluetooth/bluedroid/ble/gatt_server" demonstrates how to create a GATT services by adding attributes individually 
and then starts advertising so that GATT client can connet and exchange data.

"bluetooth/bluedroid/ble/gatt_client" demonstrates how to create a GATT Client that connects to a GATT server, 
enabling the server's notification function to discover services. 



git sparse-checkout set examples/bluetooth/bluedroid/ble/gatt_server_service_table
git checkout release/v5.3
mv examples/bluetooth/bluedroid/ble/gatt_server_service_table/ .
rm -rf examples/bluetooth/bluedroid/


git sparse-checkout set examples/bluetooth/bluedroid/ble/gatt_client
git checkout release/v5.3
mv examples/bluetooth/bluedroid/ble/gatt_client/ .
rm -rf examples/bluetooth/bluedroid/


git sparse-checkout set  examples/bluetooth/bluedroid/ble/gatt_server
git checkout release/v5.3
mv examples/bluetooth/bluedroid/ble/gatt_server/ .
rm -rf examples/bluetooth/bluedroid/


git sparse-checkout set examples/bluetooth/bluedroid/ble/gatt_security_server
git checkout release/v5.3
mv examples/bluetooth/bluedroid/ble/gatt_security_server/ .
rm -rf examples/bluetooth/bluedroid/

git sparse-checkout set examples/bluetooth/bluedroid/ble/gatt_security_client
git checkout release/v5.3
mv examples/bluetooth/bluedroid/ble/gatt_security_client/ .
rm -rf examples/bluetooth/bluedroid/

The enumeration elements are set up in the same order as Heart Rate
Profile attrebutes, starting with the service followed by the characteriscs
of that service. 
In addition, the Heart Rate Measurement characteriscs has a Client characteriscs Configuration (CCC) descriptor.
The enumeration index can be used to identify each elements, when creating the actual attribute table.

- Heart Rate Service - 
HRS_IDX_SVC : Heart Rate Service index

- Heart Rate Measurement Characteristic - 
HRS_IDX_HR_MEAS_CHAR : Heart Rate Measurement characteristic index
- Heart Rate Measurement Value (Descriptor) -
HRS_IDX_HR_MEAS_VAL : Heart Rate Measurement characteriscs value index
- Heart Rate Measurement Notification Configuration (Descriptor)-
HRS_IDX_HR_MEAS_NTF_CFG : Heart Rate Measurement notifications configuration (CCC) index.

- Sensor Location Characteristic - 
HRS_IDX_BOBY_SENSOR_LOC_CHAR: Heart Rate Body Sensor Location characteristic index
- Sensor Location Value (Descriptor) - 
HRS_IDX_BOBY_SENSOR_LOC_VAL: Heart Rate Body Sensor Location characteristic value index
- Heart Rate Control Characteristic - 
HRS_IDX_HR_CTNL_PT_CHAR: Heart Rate Control Point characteristic index
- Control Point Value (Descriptor) - 
HRS_IDX_HR_CTNL_PT_VAL: Heart Rate Control Point characteristic value index

HRS_IDX_NB: Number of table elements.