Question 01)

Why is that 'esp_ble_gap_config_adv_data()' and 'esp_ble_gap_set_device_name()' are called within 'gatts_profile_event_handler()' ?

bit mysterious at first, but it makes a lot of sense once you understand how BLE profiles, event handlers, and the BLE stack initialization process fit together.

'gatts_profile_event_handler()' is a pre-profile event handler, different from the global 'gatts_event_handler()'. It's usually called from the global one to *keep logic for different profiles (services) separate.*

Why call `esp_ble_gap_config_adv_data()' and 'esp_ble_gap_set_device_name()' here ?

These functions configure GAP (Generic Access Profile) settings like:

 - Device name (what the client sees in a scan)
 - Advertising data (How the device advertises it's presence)

And they're typically called in response to the `ESP_GATTS_REG_EVT' (GATT Server has registered successfully and ready to proceed). Since BLE has step-by-step initialization process,(setting advertisement data or device name).

Dose the role of GAP remain unchanged for BLE device while GATT event handler changes with the application.

GATT (Generic Attribute Profile) → Defines how BLE data is structured and exchanged.
Once the connection is established, GATT is used for exchanging data between client and server.

GAP (Generic Access Profile) → Defines how devices connect and advertise.
A BLE device first uses GAP to advertise itself or scan for other devices. GAP defines how BLE devices find each other, connect, and manage security. A device's GAP role remains unchanged in a connection.
GAP Roles - Broadcaster (Send advertisements, dose not connect), Observer (Scans for advertisements, dose not connect), Peripheral (Advertise & connects as a slave), Central (Scans & connects as a master).

GATT defines how devices exchange data once connected. Unlike GAP, GATT roles are not fixed—they can change depending on the application.
GATT Server (Stores & provides data when requested), GATT Client (Request & processes data from a server)

Peripheral and Central Connection vs Server and Client Connection

Example Case

"A smartphone (GATT Client) reads sensor data (GATT Server)."
Is this correct ? Because the sensor advertises it'self as a pheripheral which connect as a slave, to the Central (mobile phone) which scans connects as a master ?

GAP Roles (Connection)
The Sensor (like a heart rate monitor) acts as a GAP Peripheral, Advertises itself, Accepts connections.
The Smartphone acts as a GAP Central, Scans for advertisements, Initiates connection to the Peripheral.
Peripheral (Slave in connection setup), Central (Master in connection setup)

GATTS Roles (Data Transfer after connection)
The Sensor acts as the GATT Server (it provides data: heart rate, temperature, etc.). The Phone acts as the GATT Client (it requests/reads data from the server).
Sensor = GATT Server, Phone = GATT Client.

"GATTS Server callback events"

void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)

When gatts_event_handler gets called it accesses it's argument list (event, gatts_if, *param) from the global variables.

esp_gatts_cb_event_t event
This is an enum that represents the type of GATT server event (e.g., registration, read, write, connect, disconnect, etc.).
The function reacts differently depending on what event occurred.

esp_gatt_if_t gatts_if
This is a GATT interface ID assigned by the system.
It tells you which application interface the event is for.
You often use this value to send responses or perform actions through the correct interface.

c. esp_ble_gatts_cb_param_t *param
This is a pointer to a union that holds the parameters associated with the event.
Since different events have different kinds of data, this is a union type, and you select the correct field depending on the event type.

What 'gatts_event_handler' does ?

When an Application Profile is registered, an `ESP_GATTS_REG_EVT' event is triggered. The parameters of the 'ESP_GATTS_REG_EVT' are: 

esp_gatt_status_t status;  /*!< Operation status */`
uint16_t app_id;           /*!< Application id which input in register API */`

1. *Handling the ESP_GATTS_REG_EVT Event*:
   - If the event is ESP_GATTS_REG_EVT (a registration event), the function checks the registration status (param->reg.status).
   - If the registration is successful (status is ESP_GATT_OK), it stores the 'gatts_if' (GATT interface)  in the corresponding profile (gl_profile_tab[param->reg.app_id].gatts_if).
(The 'gatts_event_handler' function manages both profiles by iterating through the 'gl_profile_tab' array, which contains the registered GATT profiles, and invoking the appropriate callback function for each profile.)
  - If the registration fails, it logs the failure and exits the function.

Summery 
 During registration (ESP_GATTS_REG_EVT), each profile with an application id is registered. It saves gatts_if (GATT interface handle) for that profile into

2. *Iterating Through each profile*
  - For all other events (other than registration event ), the 'gatts_event_handler' function enters a loop to iterate through all profiles in 'gl_profile_tab' up to PROFILE_NUM.
  - For each profile :
	-  It checks wheather the 'gatts_if' matches the profiles's stored 'gatts_if' or if the 'gatts_if' is 'ESP_GATTS_IF_IS_NONE', is provided, (it indicates that the event is not specific to a particular GATT interface, so the callback functions for all profile should be called.
  - If the profile has a registered callback functon (gl_profile_tab[idx].gatts_cd), the callback function is invoked with the event, 'gatts_if', and the event parameters (param).

Through this design,m the 'gatts_event_handler' ensures that: 
  - Each profiles's specific callback is invoked when an event is relevent to that profile.
  - Events noit ited to a specific GATT interface are broadcast to all profiles.
	
'gatts_event_handler' is the global dispatcher for all GATT server events.

Where dose 'gatts_event_handler' get an event from ?
'ret = esp_ble_gatts_register_callback(gatts_event_handler);'

This registers, 'gatts_event_handler' function with the ESP-IDF Bluetooth stack.
After registration, whenever something happens in the Bluetooth GATT server (like client connects, reads data, writes a characteristic), the Bluetooth stack automatically calls 'gatts_Event_handler()' with the event information.
So what happens is 'esp_ble_gatts_register_callback()' tells the ESP BLE system: Hey BLE Stack! Whenever a GATT Server event happens, please call 'gatts_event_handler()' function and pass it the event details."
Thus you don't call the  'gatts_event_handler' BLE stack calls it for you with the three arguments (event - Event type, gatts_if - GATT server interface ID (assigned during registreation) and param)

❓ | ✅
Who calls gatts_event_handler? | The BLE stack inside ESP-IDF
When does it call? | Whenever a GATT server event occurs
How does it know which function to call? | Because you registered it using esp_ble_gatts_register_callback(gatts_event_handler)



Time Line 

1)	Boot -> app_main()
2)	Initialize Bluetooth

void app_main(void)
{
    // Initialize NVS
    nvs_flash_init();
    
    // Initialize the Bluetooth controller
    esp_bt_controller_init();
    esp_bt_controller_enable();

    // Initialize Bluedroid stack
    esp_bluedroid_init();
    esp_bluedroid_enable();
}

3)	Register GATTS Server Event Handler ("Whenever GATT Server event occurs, call this function.")
Register  GATT callback -> esp_ble_gatts_register_callback(gatts_event_handler) -> Now gatts_event_handler() is registered inside the BLE system.

4)	Register an Application Profile
App register -> esp_ble_gatts_app_register(APP_ID)
	Then ESP-IDF triggers the first event 'ESP_GATTS_REG_EVT'

	 ESP BLE stack automatically calls:
	gatts_event_handler(ESP_GATTS_REG_EVT, gatts_if, param); -> Your gatts_event_handler stores the gatts_if into the profile table (heart_rate_profile_tab/gl_profile_tab ).

5)	Start Advertising, You configure your advertisement parameters:
	esp_ble_gap_start_advertising(&adv_params);
	Now your device is visible to other BLE Central devices 

6)	Remote Device (Smartphone) Connects,  BLE stack automatically calls:
	gatts_event_handler(ESP_GATTS_CONNECT_EVT, gatts_if, param);

7) 	Remote Device Reads or Writes a Characteristic, BLE stack automatically calls:
	gatts_event_handler(ESP_GATTS_READ_EVT, gatts_if, param); or gatts_event_handler(ESP_GATTS_WRITE_EVT, gatts_if, param);


For Information

Function Callback,  is a function that is passed as an argument to another function, allowing the latter to execute (invoke) the former at a specific point. Instead of calling it immediately, you register the function, and the system will call it later when something happens. Responding to user actions like clicks or presses, here when an 'event is set.

Invoking a function refers to the act of executing a function. This is done by function's name followd by parantasus '()'

Listing all the possible GATT Server callback events from ESP-IDF BLE stack

Registration and Lifecycle Events

 ESP_GATTS_REG_EVT		-	GATTS server app has been registered successfully. First event to usually handle.
 ESP_GATTS_UNREG_EVT		-	GATT server app has been unregistered.
 ESP_GATTS_DELETE_EVT		-	A service has been deleted.

Attribute Access Events

ESP_GATTS_READ_EVT		-	A client has read a characteristic or descriptor.





Summery
 
In Bluetooth development with ESP-IDF, associated with GATT (Generic Attribute Profile) servers, the ESP-IDF framework uses a callback system tom handle Bluetooth events.

We have a function called 'gatts_event_handler', this function is registered to the Bluetooth stack using the line ' ret = esb_ble_gatts_register_callback(gatts_event_handler);

- Registering a callback means telling the system: "Whenever a Bluetooth event (connection, read request, write request)  happens, please call my function (gatts_event_handler).
- After registration, ESP-IDF will invoke (call and execute) 'gatts_event_handler' automatically whenever an event occurs.