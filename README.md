# Weather-Station-Dashboard
A fully integrated IoT solution that collects real-time environmental data (temperature, humidity, light intensity) using embedded sensors, transmits it to a cloud-based backend via WiFi, and visualizes metrics through an interactive web dashboard with alerting capabilities.

# Project Flow Chart
![FS1](https://github.com/user-attachments/assets/ebd628db-bea2-4b7d-9629-704494cd6f30)

# Project Sequence Diagram
![SD1](https://github.com/user-attachments/assets/1438699d-96a4-4d62-b66c-fe44821b53fb)

__________________________________________________________________________________________________________


### Embedded System Documentation

__Title: Weather Station DashBoard__

- Version: 1.0
- Last Updated: 05/22/2025
  
__1. Overview:__
   
•	Purpose: Collect environmental data (temperature, humidity, light) and transmit to web backend.

•	Target Hardware: TM4C123GH6PM + ESP8266 WiFi module.

__2. Hardware Setup__

Component          	Connection          	Notes

__________________________________________________________________________________________________________

### Web Application Documentation

__Title: Weather Dashboard - Web Application Documentation__

- Version: 1.0
- Last Updated: 05/22/2025

__1. System Diagram__

![SYSd1](https://github.com/user-attachments/assets/235209ee-6b3e-4f3d-9645-a459843f5f76)

__2. API Endpoints__

### Endpoint	Method	Description

/api/weather	POST	Store sensor data
/api/weather	GET	Retrieve all data
/api/weather/latest	GET	Latest sensor reading

__________________________________________________________________________________________________________
