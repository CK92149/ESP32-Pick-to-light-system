# ESP32-Pick-to-light-system
Open source pick to light system using esp32 and google sheets. Includes the ability to connect pick by weight and remote monitoring of stock. primarily used for order picking and monitoring

Join us on the discord server https://discord.gg/6yepWSbB for help, discussion and development. 

Recomended hardware; ESP32 TTGO (program should also work on other esp32 devices)

dependencies:

First prepare the Google Sheet and connection as described here: https://github.com/mobizt/ESP-Google-Sheet-Client

Follow the quick start guide to install the ESP32 TTGO T-Display: https://github.com/Xinyuan-LilyGO/TTGO-T-Display



initial setup:

make sure each device has the correct product name ("<ProductName>") as described on the google sheet.

copy the following sheet to test the orders: https://docs.google.com/spreadsheets/d/1m71qk6SqQp7u8bPOYln_qpymAGtUYlZgMs0AhN84Ddc/
  

How it works:
  
The order with the lowest value will have the status set to "1" in column B. The esp32 check if the order status is 1 & product name in column C = "<ProductName>" as described on the esp32 device. If both are true the order will be displayed on the esp32 device. When the order is picked and the (boot) button is pressed, the order is completed and the order# in column E is deleted. Once all orders of the same order# is picked, it will move to the next order# and send the pick orders to all relevant devices.  

To do:
  
add button pin
  
add (RGB) LED pins
