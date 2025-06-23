ESP-NOW

Initiator --> Responder

Initiator can send data to any of the Responder devices.
Initiator 	--> Responder 1
		--> Responder 2
		--> Responder 3

Conversely can also have a configuration with one responder and multiple initiators.
Any Initiator can send data to the responder.
Initiator 1 --> Responder
Initiator 2 -->
Initiator 3 -->


Two-way communication the device act as both initiator and responder, can arrange two-way devices within a network (each device acting as both initiator and responder) being capable of having two-way communication with other devices (mix and match ESP32 and ESP8266). 

ESP-NOW protocol dose not support Broadcast MAC Address, ESP-NOW supports pseudo-broadcast mode,
which allows to broadcast information to every device in the network, simultaneously.

This is a common point of confusion. ESP-NOW does use the official IEEE 802.11 broadcast MAC address (FF:FF:FF:FF:FF:FF). 
The reason it might be called "pseudo" is because of an ESP-NOW rule: you must explicitly add the broadcast MAC address as a peer using esp_now_add_peer() before you can send data to it. 
So, while it's a true broadcast at the radio level, it requires a setup step that a standard network broadcast might not. 
Your later note correctly captures this: "A device with a broadcast MAC address must be added before sending broadcast data."

Important notes
*Data-link layer defined by espressif
*Reduce the five layers of the OSI-model to one (data dose not need to transmit through Application, Presentation, Session, Transport and Network Layer)
*No need for packet headers, Unpackers, 

It's more precise to say: "No need for upper-layer (TCP/IP) packet headers." ESP-NOW packets absolutely have headers. 
They have the standard 802.11 MAC header (which includes the source/destination MACs) and the Vendor-Specific Element structure. 
Your application does need to unpack this custom payload, which is exactly what the example_espnow_data_parse() function does. You have bypassed the complexity of TCP/IP, but not all headers entirely.

*Paired devices to communicate directly with one-another through the data-link layer.
*While device pairing can use the wirelessly received signal strength indication, to determine the distance between the devices, and authorize that distance.
*Only devices within the authorized distance can be paired successfully.
*Using RSSI as a filter can also ensure that responders in different positions do-not interfere with each other during device pairing.

This is a great application concept, but it's important to know that this is application-level logic, not a built-in feature of the ESP-NOW protocol itself. The esp_now_add_peer() function will pair any device it hears from. Your code would need to:
Receive a packet.
Check the RSSI value from the packet's metadata.
Then, decide whether to call esp_now_add_peer() based on that RSSI value.


*After power on the devices can transmit data and control other paired devices directly, without any wireless connection
(.without any Wi-Fi association to an access point." It is, of course, still a wireless communication.), 
with a response speed of milliseconds.
*When the device is connected to a router, or works as a hotspot realize fast and stable communication through esp-now.

*Device provisioning debugging and firmware update.
*ESP-NOW can be used to device provisioning along with wifi and Bluetooth.
*First device provisioned via Bluetooth, once this is done other devices dose not require ssid or password because first device will send these information to the other devices directly.
*User can decide weather to allow the other devices to connect to the network. 
*Can obtain data-logs 
- Aging test, Interference test, wireless connectivity test, module read and write test, version verification test.


*Even without a router can keep stable communication through esp-now.

Note****
Why do I need to enable WIFI befor initializing esp-now ?

The Highway is the 802.11 Physical Layer (PHY): This is the actual radio hardware and the low-level driver that makes it send and receive signals on a specific 2.4 GHz channel.
The function esp_wifi_start() is what builds and opens this highway. 
It powers up the radio, allocates memory for data packets, and gets the hardware ready for action.

Protocols - Standard Wi-Fi (TCP/IP) , ESP-NOW

Therefore, you must call esp_wifi_start() first because ESP-NOW, despite being "connectionless," is not a separate technology from Wi-Fi. 
It is a lightweight protocol that sits directly on top of the Wi-Fi physical layer. 
Without starting Wi-Fi, the radio hardware is off, the channel isn't set, and the low-level drivers are inactive.

2. Why are ESP-NOW Send/Receive callbacks executed in high-priority Wi-Fi tasks?

==============

Rules of ESP-NOW

wifi/espnow demonstrates how to use the ESPNOW feature of ESP32's Wi-Fi, including 
starting Wi-Fi, initialization ESP-NOW,
registering ESP-NOW,
sending or reveiving callback function,
adding ESP-NOW peer information,
sending and receiving ESP-NOW daya between two devices.

CTR with CBC-MAC Protocol (CCMP) is used to protect the action frame for security.
The default ESP-NOW bit rate is 1 Mbps.

As ESP-NOW is connectionless, the MAC header is a little different from that of standard frames.
The FromDS and ToDS bits of FrameControl field are both 0. The first address field is set to the destination address. The second address field is set to the source address. 
The third address field is set to broadcast address.
(0xff:0xff:0xff:0xff:0xff:0xff).

What is the vendor-specific action frame ?
 - ESP-NOW uses one vendor-specific element field of action frame to transmit ESP-NOW data, whose length field is only 1 byte (0xff = 255) as defined by IEEE 802.11. Thus, the maximum length of ESP-NOW data is limited to 250 bytes.
 - In ESP-NOW, application data is encapsulated in action frames from different vendors and then transmitted from one Wi-Fi device to another without a connection.

What is vendor-specific element frame ?
- Vendor Specific Content: The Vendor Specific Content contains several (at least one) vendor-specific element fields. 

IEEE Std. 802.11 Wi-Fi Frame Format

ESP-NOW uses the CCMP method, which is described in IEEE Std. 802.11-2012, to protect the vendor-specific action frame.

======================

"esp-now-wifi-com" impliments a "discovery" and "communication" logic.
A device starts by broadcasting a message and any other device that hears it can respond via unicast.

===========		esp-now-wifi-com.h		=============

Defines the data structures used to manage ESP-NOW events and application-specific data.

example_espnow_event_t: This is a crucial structure for handling events asynchronously. 
The ESP-NOW send/receive callbacks are executed in a high-priority Wi-Fi task.

(This structure allows the callbacks to quickly bundle up the necessary information
(like who sent data, or if a send succeeded) and post it to a FreeRTOS queue (s_example_espnow_queue),)

example_espnow_data_t: This is the application-level packet structure. 
This is the data that gets put inside the "vendor-specific element."

'type', 'state', 'seq_num', 'crc', 'magic', 'payload[0]'

============	esp-now-wifi-com.c function contains the main logic.	==============

Initialization and Deinitialization

1.0	Call 'esp-now_init()'  to initialize ESP-NOW and 'esp_now_deinit()' to de-initialize ESP-NOW.
	ESP-NOW data must be transmitted after Wi-Fi is started, so it is recommended to start Wi-Fi before initializing ESP-NOW and stop Wi-Fi after de-initializing ESP-NOW.
	When 'esp_now_deinit()' is called, all of the information of paired devices are deleted.

'app_main()', 'example_wifi_init()', 'example_espnow_init()'

1. app_main() is the entry point. 
It first initializes the Non-Volatile Storage (nvs_flash_init), 
which is required by the Wi-Fi stack.

2. It then calls example_wifi_init(). As the documentation states, 

3. Next, app_main() calls example_espnow_init(). This function performs the core ESP-NOW setup:
	- It calls esp_now_init() to initialize the ESP-NOW service.
	- It registers the send and receive callback functions (example_espnow_send_cb and example_espnow_recv_cb).
	- It creates the queue s_example_espnow_queue for communication between the callbacks and the main task.
	- It creates and starts the main application logic task, example_espnow_task.

4. The example_espnow_deinit() function does the cleanup: it frees allocated memory, deletes the queue, and calls esp_now_deinit().

===========		Add Paired devices	=============

Add Paired Device

2.0	Call 'esp_now_add_peer()' to add the device to the paired device list before you send data to this device.
	If security is enabled, the LMK must be set.
	You can send ESP-NOW data via both the Station and the SoftAP interface. Make sure that the interface is enabled before sending ESP-NOW data.

2.1	The maximum number of paired devices is 20, and the paired encryption devices are no more than 17, the default is 7.
	To change the number of paired encryption devices, set CONFIG_ESP_WIFI_ESPNOW_MAX_ENCRYPT_NUM in the Wi-Fi component configuration menu.

2.2	A device with a broadcast MAC address must be added before sending broadcast data.
	The range of the channel of paired devices is from 0 to 14.
	If the channel is set to 0, data will be sent on the current channel. 
	 Otherwise, the channel must be set as the channel that the local device is on.

Code: example_espnow_init() and the case EXAMPLE_ESPNOW_RECV_CB block in example_espnow_task.

1. Initial Broadcast Peer: In example_espnow_init(), a peer is added using esp_now_add_peer(). 
The MAC address used is s_example_broadcast_mac (FF:FF:FF:FF:FF:FF). 
This is mandatory. You must add the broadcast address as a peer before you can send broadcast data. 
Notice peer->encrypt is set to false for this peer.

2. Dynamic Unicast Peer: The more interesting part is in example_espnow_task.
When a broadcast message is received (if (ret == EXAMPLE_ESPNOW_DATA_BROADCAST)), the code checks if the sender is already a known peer using esp_now_is_peer_exist(). 
If not, it creates a new esp_now_peer_info_t struct, fills it with the sender's MAC address, channel, and the encryption key (lmk), and adds it using esp_now_add_peer(). 
This is how a device dynamically learns about other devices and adds them to its list of peers for secure, direct communication.

============	Send ESP-NOW Data	=============

Send ESP-NOW Data

3.0	Call 'esp_now_send()' to send ESP-NOW data and 'esp_now_register_send_cb()'  to register sending callback function.
	It will return ESP_NOW_SEND_SUCCESS in sending callback function if the data is received successfully on the MAC layer.
	Otherwise, it will return ESP_NOW_SEND_FAIL.

3.1	Several reasons can lead to ESP-NOW fails to send data.
		- destination device does not exist;
		- the channels of the devices are not the same;
		- the action frame is lost when transmitting on the air,

3.2 It is not guaranteed that application layer can receive the data.
	If necessary, send back ack data when receiving ESP-NOW data. 
	If receiving ack data timeouts, retransmit the ESP-NOW data.
	A sequence number can also be assigned to ESP-NOW data to drop the duplicate data.

3.3	If there is a lot of ESP-NOW data to send, call esp_now_send() to send less than or equal to 250 bytes of data once a time.
	Note that too short interval between sending two ESP-NOW data may lead to disorder of sending callback function.
	So, it is recommended that sending the next ESP-NOW data after the sending callback function of the previous sending has returned.
	The sending callback function runs from a high-priority Wi-Fi task.
	So, do not do lengthy operations in the callback function.
	Instead, post the necessary data to a queue and handle it from a lower priority task.


Code: example_espnow_data_prepare(), esp_now_send() (called from example_espnow_task), 
and the callback example_espnow_send_cb().

1. example_espnow_data_prepare() is called. 
This function populates the example_espnow_data_t structure with the current state, sequence number, magic number, and calculates a CRC checksum.

2. The sending process is managed within example_espnow_task. 
It starts by sending a broadcast message.

3. The data is sent with esp_now_send(destination_mac, data, data_len). 
This function is non-blocking; it returns immediately after queuing the packet for transmission.

4. Sometime later, the Wi-Fi driver will call the registered send callback, example_espnow_send_cb(), 
with the status of the transmission (success or fail).

5. The callback does not do much processing. 
It simply packages the result into an example_espnow_event_t and posts it to the queue.

6. The while loop in example_espnow_task waits for this event.
Receiving the send callback event acts as a signal that it's okay to prepare and send the next packet. 
This prevents overwhelming the send buffers.

========	Receiving ESP-NOW Data	============

Receiving ESP-NOW Data

4.0	Call esp_now_register_recv_cb() to register receiving callback function. 
	Call the receiving callback function when receiving ESP-NOW.
	The receiving callback function also runs from the Wi-Fi task.
	So, do not do lengthy operations in the callback function.
	Instead, post the necessary data to a queue and handle it from a lower priority task.

Code: example_espnow_recv_cb() and the case EXAMPLE_ESPNOW_RECV_CB block in example_espnow_task.

1. When a valid ESP-NOW packet arrives, the Wi-Fi driver calls the registered receive callback, example_espnow_recv_cb().

2. Just like the send callback, this function must be fast. 
It allocates memory for the incoming data, copies the data and the sender's MAC address into an example_espnow_event_t struct, and posts it to the queue.

3. The main example_espnow_task receives the event from the queue. 
It calls example_espnow_data_parse() to validate the data's CRC and extract the information from the example_espnow_data_t payload.

4. Based on the parsed data (e.g., if it's a broadcast from a new device), it then takes action, such as adding the new peer and switching to unicast communication.

=========	Config ESP-NOW rate		========

Config ESP-NOW Power Saving Parameter

6.0 Sleep is supported only when ESP32 is configured as station.

Config ESP-NOW rate

5.0 Call 'esp_wifi_config_espnow_rate()' to config ESP-NOW rate of specified interface.
	Make sure that the interface is enabled before config rate.
	This API should be called after esp_wifi_start().

esp_wifi_set_protocol() in example_wifi_init().

The example code doesn't directly call esp_wifi_config_espnow_rate().
However, it influences the rate indirectly. The line esp_wifi_set_protocol(..., WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N|WIFI_PROTOCOL_LR) inside an #if CONFIG_ESPNOW_ENABLE_LONG_RANGE block tells the Wi-Fi driver which protocols are allowed.
If Long Range (LR) mode is enabled in the project configuration, this allows ESP-NOW to use lower data rates that can travel further.

=========	Config ESP-NOW Power Saving Parameter	=============

Code: esp_now_set_wake_window() and esp_wifi_connectionless_module_set_wake_interval() within example_espnow_init().

These lines are wrapped in an #if CONFIG_ESPNOW_ENABLE_POWER_SAVE directive. 
If power saving is enabled in the project configuration, these functions are called during initialization. 
They configure the ESP32 (in station mode) to sleep for a set interval and then wake up for a short "wake window" to listen for incoming ESP-NOW packets, significantly reducing power consumption. 
The provided code shows how to call these functions but their activation depends on the project's sdkconfig.