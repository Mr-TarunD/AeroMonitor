# AeroMonitor: IoT Air Quality & Monitoring System

AeroMonitor is a comprehensive IoT project that uses an Arduino Mega to monitor a wide range of environmental data. It collects data from 8 different sensors, displays the status locally on an LCD, triggers a local buzzer for alarms, and uploads all data to the ThingSpeak cloud for remote monitoring.

This project also includes a three-page web dashboard (HTML/CSS/JS) that connects to the ThingSpeak API to display both the latest live sensor values and historical data in live-updating graphs.

## 🚀 Features

  * **Multi-Sensor Monitoring:** Actively monitors Temperature, Humidity, Air Quality (MQ-135), Smoke (MQ-2), LPG (MQ-6), Alcohol (MQ-7), CO (MQ-3), and Flame presence.
  * **Cloud Data Logging:** Connects to WiFi using an ESP8266 module to send all 8 sensor readings to a ThingSpeak channel every 10 seconds.
  * **Local Display:** Sensor values are displayed sequentially on a 20x4 I2C LCD screen.
  * **Local Alarms:** An active buzzer provides an audible alert if dangerous levels of smoke or flame are detected.
  * **Web Dashboard:** A responsive web interface for remote monitoring:
      * `index.html`: A homepage linking to the data pages.
      * `livevaluedisplay.html`: Shows the latest value for all 8 sensors, updating every 5 seconds.
      * `livegraph.html`: Renders 8 separate live-updating line charts using Chart.js to visualize historical sensor data.

## 🛠️ Hardware Components

  * **Microcontroller:** Arduino Mega
  * **Connectivity:** ESP8266 WiFi Module (connected to `Serial1`)
  * **Display:** 20x4 I2C LCD Module
  * **Actuator:** 5V Active Buzzer
  * **Sensors:**
      * DHT11 (Temperature & Humidity) 
      * MQ-135 (Air Quality)
      * MQ-2 (Smoke)
      * MQ-6 (LPG)
      * MQ-7 (Alcohol)
      * MQ-3 (CO) 
      *Flame Sensor (Digital)
  * **From Diagram:** The circuit diagram also includes:
      * 2-Channel 5V Relay Module
      * 12V DC Fan
      * 12V DC Water Pump

## 💻 Software, Libraries & Services

  * **Arduino IDE**
  * **Arduino Libraries:**
      * `WiFiEsp.h` 
      * `ThingSpeak.h` 
      * `DHT.h`
      * `LiquidCrystal_I2C.h`
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

```

## ⚙️ How It Works

1.  **Hardware:** The Arduino Mega is the system's core. It reads analog data from the MQ gas sensors and digital data from the DHT11 and Flame sensors.
2.  **Local Processing:** The Arduino displays all sensor values sequentially on the 20x4 LCD. It simultaneously checks if any sensor values (Smoke, Flame, LPG, Alcohol, CO) have crossed their predefined safety thresholds. If a threshold is breached, it activates the `BUZZER_PIN`.
3.  **Cloud Upload:** The Arduino communicates with the ESP8266 module via `Serial1` to connect to the local WiFi network. Every 10 seconds, it formats the 8 sensor values and uploads them to 8 separate fields on a ThingSpeak channel.
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
      * `DHT sensor library` 
      * `LiquidCrystal_I2C`

3.  Update the secret definitions at the top of the file with your credentials:

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
| **Field 1** | Air Quality (MQ-135) | `ThingSpeak.setField(1, airQuality);`  |
| **Field 2** | Temperature (DHT11) | `ThingSpeak.setField(2, temperature);`  |
| **Field 3** | Humidity (DHT11) | `ThingSpeak.setField(3, humidity);`  |
| **Field 4** | Flame | `ThingSpeak.setField(4, flameValue);`  |
| **Field 5** | CO (MQ-3) | `ThingSpeak.setField(5, coValue);`  |
| **Field 6** | Smoke (MQ-2) | `ThingSpeak.setField(6, smokeValue);`  |
| **Field 7** | Alcohol (MQ-7) | `ThingSpeak.setField(7, alcoholValue);`  |
| **Field 8** | LPG (MQ-6) | `ThingSpeak.setField(8, lpgValue);`  |
