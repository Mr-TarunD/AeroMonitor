# AeroMonitor: IoT Air Quality & Environmental Monitoring System

AeroMonitor is a comprehensive IoT project that uses an Arduino Mega to monitor a wide range of environmental data. It collects data from 8 different sensors, displays the status locally on an LCD, triggers a local buzzer for alarms, and uploads all data to the ThingSpeak cloud for remote monitoring.

This project also includes a three-page web dashboard (HTML/CSS/JS) that connects to the ThingSpeak API to display both the latest live sensor values and historical data in live-updating graphs.

## 🚀 Features

  * **Multi-Sensor Monitoring:** Actively monitors Temperature, Humidity, Air Quality (MQ-135), Smoke (MQ-2), LPG (MQ-6), Alcohol (MQ-7), CO (MQ-3), and Flame presence.
  * [cite\_start]**Cloud Data Logging:** Connects to WiFi using an ESP8266 module to send all 8 sensor readings to a ThingSpeak channel every 10 seconds[cite: 31, 34].
  * [cite\_start]**Local Display:** Sensor values are displayed sequentially on a 20x4 I2C LCD screen[cite: 20, 21, 22, 23, 24, 25].
  * [cite\_start]**Local Alarms:** An active buzzer provides an audible alert if dangerous levels of smoke, flame, LPG, alcohol, or CO are detected[cite: 26, 27].
  * **Web Dashboard:** A responsive web interface for remote monitoring:
      * `index.html`: A homepage linking to the data pages.
      * `livevaluedisplay.html`: Shows the latest value for all 8 sensors, updating every 5 seconds.
      * `livegraph.html`: Renders 8 separate live-updating line charts using Chart.js to visualize historical sensor data.

## 🛠️ Hardware Components

  * **Microcontroller:** Arduino Mega
  * [cite\_start]**Connectivity:** ESP8266 WiFi Module (connected to `Serial1` [cite: 3, 8])
  * **Display:** 20x4 I2C LCD Module
  * [cite\_start]**Actuator:** 5V Active Buzzer [cite: 5]
  * **Sensors:**
      * [cite\_start]DHT11 (Temperature & Humidity) [cite: 4]
      * [cite\_start]MQ-135 (Air Quality) [cite: 5]
      * [cite\_start]MQ-2 (Smoke) [cite: 5]
      * [cite\_start]MQ-6 (LPG) [cite: 5]
      * [cite\_start]MQ-7 (Alcohol) [cite: 5]
      * [cite\_start]MQ-3 (CO) [cite: 5]
      * [cite\_start]Flame Sensor (Digital) [cite: 5]
  * **From Diagram:** The circuit diagram also includes:
      * 2-Channel 5V Relay Module
      * 12V DC Fan
      * 12V DC Water Pump

## 💻 Software, Libraries & Services

  * **Arduino IDE**
  * **Arduino Libraries:**
      * [cite\_start]`WiFiEsp.h` [cite: 1]
      * [cite\_start]`ThingSpeak.h` [cite: 1]
      * [cite\_start]`DHT.h` [cite: 1]
      * [cite\_start]`LiquidCrystal_I2C.h` [cite: 1]
  * **Cloud Platform:**
      * [ThingSpeak](https://thingspeak.com/) (for data logging and API)
  * **Web Technologies:**
      * HTML5 / CSS3
      * JavaScript (Fetch API)
      * [Chart.js](https://www.chartjs.org/) (for graphing)

## 📁 Project Structure

```
.
├── IOT_project_aqm.ino         # The main Arduino Mega sketch
├── index.html                  # Web dashboard homepage
├── livegraph.html              # Web page for historical sensor graphs
├── livevaluedisplay.html       # Web page for latest sensor values
└── Gaurav_Report.pdf-image-022.jpg # Hardware circuit diagram
```

## ⚙️ How It Works

1.  **Hardware:** The Arduino Mega is the system's core. [cite\_start]It reads analog data from the MQ gas sensors and digital data from the DHT11 and Flame sensors[cite: 16, 17].
2.  [cite\_start]**Local Processing:** The Arduino displays all sensor values sequentially on the 20x4 LCD [cite: 20-25]. [cite\_start]It simultaneously checks if any sensor values (Smoke, Flame, LPG, Alcohol, CO) have crossed their predefined safety thresholds[cite: 26]. [cite\_start]If a threshold is breached, it activates the `BUZZER_PIN`[cite: 27].
3.  [cite\_start]**Cloud Upload:** The Arduino communicates with the ESP8266 module via `Serial1` [cite: 3, 8] [cite\_start]to connect to the local WiFi network[cite: 13, 14]. [cite\_start]Every 10 seconds, it formats the 8 sensor values and uploads them to 8 separate fields on a ThingSpeak channel [cite: 28-31, 34].
4.  **Web Dashboard:** The HTML pages use JavaScript to make Fetch API calls to the ThingSpeak REST API.
      * `livevaluedisplay.html` requests the single most recent feed (`results=1`) and updates the text content of the page.
      * `livegraph.html` requests the 10 most recent feeds (`results=10`) and uses the Chart.js library to plot this data, refreshing every 5 seconds.

## 🔧 Setup & Installation

To get this project running, you need to configure three parts: ThingSpeak, the Arduino code, and the Web Dashboard.

### 1\. ThingSpeak Channel

1.  Create a free [ThingSpeak](https://thingspeak.com/) account.
2.  Go to **Channels** \> **My Channels** and click **New Channel**.
3.  Name your channel (e.g., "AeroMonitor").
4.  Enable **8 fields** and name them according to the "ThingSpeak Field Mapping" section below.
5.  Click **Save Channel**.
6.  Go to the **API Keys** tab and note your **Channel ID**, **Write API Key**, and **Read API Key**.

### 2\. Arduino Sketch (`IOT_project_aqm.ino`)

1.  Open `IOT_project_aqm.ino` in the Arduino IDE.

2.  Install the required libraries from the Library Manager:

      * `WiFiEsp`
      * `ThingSpeak`
      * `DHT sensor library` (by Adafruit)
      * `LiquidCrystal_I2C`

3.  [cite\_start]Update the secret definitions at the top of the file with your credentials[cite: 1]:

    ```c++
    #define SECRET_SSID "YOUR_WIFI_SSID"      // Your WiFi network name
    #define SECRET_PASS "YOUR_WIFI_PASSWORD"  // Your WiFi password
    #define SECRET_CH_ID 1234567               // Your ThingSpeak Channel ID
    #define SECRET_WRITE_APIKEY "YOUR_WRITE_API_KEY" // Your ThingSpeak Write API Key
    ```

4.  Select **Arduino Mega** as your board.

5.  Connect your hardware as shown in the circuit diagram and upload the sketch.

### 3\. Web Dashboard

1.  Open `livevaluedisplay.html` and `livegraph.html` in a text editor.

2.  In **both files**, find the `<script>` tag at the bottom and update the following constants with your ThingSpeak details:

    ```javascript
    const CHANNEL_ID = '1234567'; // Your ThingSpeak Channel ID
    const READ_API_KEY = 'YOUR_READ_API_KEY'; // Your ThingSpeak Read API Key
    ```

3.  You can now host these HTML files on a web server (like GitHub Pages) or simply open `index.html` in your local web browser.

## 📊 ThingSpeak Field Mapping

The project uses 8 fields on ThingSpeak. Ensure they are configured in this order:

| Field | Sensor Data | Arduino Code Reference |
| :--- | :--- | :--- |
| **Field 1** | Air Quality (MQ-135) | [cite\_start]`ThingSpeak.setField(1, airQuality);` [cite: 28] |
| **Field 2** | Temperature (DHT11) | [cite\_start]`ThingSpeak.setField(2, temperature);` [cite: 28] |
| **Field 3** | Humidity (DHT11) | [cite\_start]`ThingSpeak.setField(3, humidity);` [cite: 28, 29] |
| **Field 4** | Flame | [cite\_start]`ThingSpeak.setField(4, flameValue);` [cite: 29] |
| **Field 5** | CO (MQ-3) | [cite\_start]`ThingSpeak.setField(5, coValue);` [cite: 29] |
| **Field 6** | Smoke (MQ-2) | [cite\_start]`ThingSpeak.setField(6, smokeValue);` [cite: 29] |
| **Field 7** | Alcohol (MQ-7) | [cite\_start]`ThingSpeak.setField(7, alcoholValue);` [cite: 30] |
| **Field 8** | LPG (MQ-6) | [cite\_start]`ThingSpeak.setField(8, lpgValue);` [cite: 30, 31] |
