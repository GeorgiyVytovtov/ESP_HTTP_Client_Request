# ESP_HTTP_Cient_Request

## 1. Tasks list. 
1. UART initialization
2. Create a task for receiving data via UART
3. Implement the parser and the parsing task
4. Wi-Fi initialization
5. Implement HTTP requests (GET and POST)
6. Create a task to output data to UART
7. Unit testing of the main components
8. Error handling and logging
9. Documentation
## 2. Estimated time for all tasks. 
| №  | Task                                     | Time (hours) |
|-----|--------------------------------------------|--------------|
| 1   | UART initialization                         | 0.5          |
| 2   | Create a task for receiving data via UART  | 0.5          |
| 3   | Implement the parser and the parsing task  | 3            |
| 4   | Wi-Fi initialization                        | 1            |
| 5   | Implement HTTP requests (GET and POST)      | 1            |
| 6   | Create a task to output data to UART        | 0.5          |
| 7   | Unit testing of the main components          | 4            |
| 8   | Error handling and logging                    | 3            |
| 9   | Documentation                                | 1            |

**Total estimated time: 14.5 hours**

## 3. Block diagram of the program. 
![Photo1](https://github.com/GeorgiyVytovtov/ESP_HTTP_Client_Request/blob/master/Block_diagram.bmp)

## 4. Project Build Instructions

### Requirements
To build this project, make sure you have the following tools installed:

- Visual Studio Code (VS Code)
- ESP-IDF v5.3.3
- CMake

In my case, I use Windows OS.

### Build Steps

1. **Clone the repository**  
   Download or clone the project to your local machine.

2. **Configure VS Code**  
   When you open the project in VS Code for the first time, it will prompt you to recompile all files in the `.vscode` folder. Accept the prompt to configure the project properly.

3. **Add your Wi-Fi credentials**  
   Create a file named `password.h` in the `components/http/` directory with the following content:

   ```c
   #ifndef __PASSWORD_H
   #define __PASSWORD_H

   #define WIFI_SSID "your_ssid"
   #define WIFI_PASS "your_password"

   #endif /* __PASSWORD_H */
Note: This file is included in .gitignore for security reasons and will not be committed to the repository.

4. **Build the project**
  Use the ESP-IDF build system to compile the project.

5. **Upload to the device**
  After a successful build, flash the firmware to your ESP32 device (e.g., ESP32-WROOM-32).

## 5. Testing the Program

### 1. Connect via USB-UART  
Connect your ESP32 device to your computer using a USB-to-UART converter.

### 2. Open a Serial Terminal  
Use a terminal program to interact with the ESP32, such as:  
- PuTTY 
- CoolTerm (used in this example)

### 3. Set Up the Terminal  
Make sure to configure the terminal with the following settings:
- **Port**: Select the correct COM port assigned to your ESP32
- **Baud rate**: `115200`

### 4. Send HTTP Requests  
After connecting to the serial terminal, you can send HTTP requests like:

```c
http GET http://httpbin.org
http GET http://httpbin.org/ip
http GET http://httpbin.org/headers
http GET http://httpbin.org/user-agent

http POST http://httpbin.org/post post_body
http POST http://httpbin.org/post {"name":"ESP32","status":"test1"}
http POST http://httpbin.org/post {"temperature":25,"unit":"C"}
http POST http://httpbin.org/post {"message":"Hello from ESP32!"}
```

### 5. Unit Testing

To run unit tests instead of the main application logic:

1. Open the `main.c` file.
2. **Comment out** the default `app_main()` function:

```c
void app_main(void)
{
    nvs_flash_init();
    uart_init(115200);
    wifi_init();
    xTaskCreate(vUartRxEventTask, "vUartRxEventTask", 1024*4, NULL, 5, NULL);
    xTaskCreate(vParseTask, "vParseTask", 1024*4, NULL, 5, NULL);
    xTaskCreate(vHttpRequestTask, "vHttpRequestTask", 1024*4, NULL, 5, NULL);
    xTaskCreate(vUartTxTask, "vUartTxTask", 1024*3, NULL, 5, NULL);
}
```

3. **Uncomment the unit test version of app_main():**

```c
// For unit tests
void app_main(void)
{
    nvs_flash_init();
    run_uart_tests();
    run_http_tests();
    run_parse_tests();
}
```

4. **Rebuild the project and upload the firmware to your ESP32 device.**
5. **Open the serial monitor using idf.py monitor or ESP-IDF's terminal integration.**
6. **Observe the test results in the monitor output.**

Tip: Don't forget to switch back to the main app_main() function after testing to resume normal operation.


