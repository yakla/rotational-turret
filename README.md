# infinit Rotational Smart Tracking Camera

first i started the project as a idea for a consept for a rotating turret that can go forever without needing to retract. i wanted to make it a concept so i made the build realy small but after i designed the base and then decited to go all out and make a follower cammera. in the end it ended up super small in just 10cm×10cm×9cm. for me its just a fun project to do in the meantime of the war currently. 

## Mechanics & Power
The project uses a high speed brushless motor to rotate the base gear and a very small servo to rotate the head.
The power uses usb c 12v and buck convertor to power everything and passes the data and the power throught a slip ring  inside the shaft .

## Communication & Vision Pipeline
To comunicate with other devices and transfer the stream it uses a udp server to get the ip of the device and send back the camera data to it for high speed transfer and low cpu usage on the small esp . 
It also uses bluetooth for manual movment mode. 
For the color pipeline i use a optimised version of rgb to hsv and color range checking for each pixel summing the position and getting the aproximet position to rotate to.

## Modes
the turret has 3 main modes.
* Manual that streams with jpeg optimised stream and let the user control the turret
* Follower that sets the turret to follow the target (for now set in the code)
* Lock that send the data of the target position without following it

## Design & Hardware
For the head i designed a costume pcb to fit the tof sensor the esp camera and the leds.
For a encloseure i use a thin sheet of invisable plastic that rotates with the turret. that make it realy cool in my opinion.

CAD Link : https://cad.onshape.com/documents/5493d3bfb7c584824d124c44/w/b9fe300d7cd71857bff2772f/e/d3c804d532ca15a2a7af37b9?renderMode=0&uiState=69b6b6c78c596208d828381b

(Please copy the link. it can't be opened from github)

For the Hardware i used:
* XIAO ESP32-S3 - Main Controller & Vision Processing
* PTK 7308H Servo - Pan/Tilt Movement
* Emax ECO 1407 Motor - Turret Movement
* Custom PCB - Power distribution, Leds & sensor integration
*  - Distance detection (ToF)
   - Camera OV2640
   - SK6805 NeoPixel
<img width="2000" height="1432" alt="image" src="https://github.com/user-attachments/assets/4082ebb7-f81b-4d76-985a-376f5406363f" />
<img width="1341" height="1145" alt="Screenshot 2026-03-17 at 10-38-05 inf turret turret assembly" src="https://github.com/user-attachments/assets/99a351d3-e626-499a-a301-cde01e98c94b" />
<img width="1421" height="1230" alt="image" src="https://github.com/user-attachments/assets/c5527134-ff11-4244-b97f-970cea70908f" />
<img width="1355" height="1231" alt="image" src="https://github.com/user-attachments/assets/9f0abd68-75c3-4229-89a5-14f2000f871f" />


## Project Status & Roadmap
- [ ] Mobile App for Remote control and setting Wifi detection and simple viewing window (Planned)
- [x] Design the project with a cheaper servo (Planned) 

## How to use
* **Build:** Use PlatformIO to flash the Firmware/ folder to the XIAO.
* **Setup_bluetooth:** Connect to the device via Bluetooth.
* **Setup_wifi:** Currently have to set it in the main code. will probebly add a way in the app later
* **UDP_client** you can use the python code in tools folder of the firmware currently. Will add a viewing window in the app later
* **Run:** Point the camera at a target object; the turret will lock on using the HSV threshold defined in config.h.

## Credits
* Designed and built as part of Hack Club.

## BOM
| Item                               | Description                                                   | Quantity             | Unit Price ($) | Total Price ($) | URL |
|:-----------------------------------|:--------------------------------------------------------------|:---------------------|:---------------|:----------------|:----|
| Hobbywing Skywalker 12A            | esc for the brushless motor                                   | 1                    | $5.23          | $5.23           | [Link](https://he.aliexpress.com/item/1005006848267224.html) |
| Emax ECO 1407 Motor 2800KV         | the brushless motor for the base turret rotation              | 1                    | $12.39         | $16.11          | [Link](https://he.aliexpress.com/item/1005006857011061.html) |
| Ptk 7308h                          | the servo that moves the head of the camera                   | 1                    | $27.00         | $27.16          | [Link](https://he.aliexpress.com/item/1005005891940798.html) |
| slip ring 6.5mm 8ch a1             | the most important part to move electucity even when rotating | 1                    | $4.04          | $7.04           | [Link](https://he.aliexpress.com/item/1005009463382321.html) |
| USB-C PD PD/QC Decoy Board         | the usb port                                                  | 1                    | $1.30          | $1.30           | [Link](https://he.aliexpress.com/item/1005005622939899.html) |
| Mini360 buck convertor             | get the usb voltage down                                      | 1                    | $0.28          | $1.74           | [Link](https://he.aliexpress.com/item/1005008724925620.html) |
| XIAO ESP32-S3 Sense                | the brain                                                     | 1                    | $13.85         | $17.21          | [Link](https://he.aliexpress.com/item/1005005544155799.html) |
| Adafruit VL53L4CX                  | the tof sensor                                                | 1                    | $19.57         | $19.57          | [Link](https://he.aliexpress.com/item/1005010327402661.html) |
| SK6805 2427                        | neopixel for the pcb                                          | 1 (50 in one order)  | $3.73          | $6.30           | [Link](https://he.aliexpress.com/item/1005008979678361.html) |
| capcacitor 100nf 0805              | just there for the neopixel                                   | 1 (100 in one order) | $1.24          | $1.24           | [Link](https://he.aliexpress.com/item/1005005293453925.html) |
| capcacitor 10uf 1206               | just there for the neopixel                                   | 1 (20 in one order)  | $1.35          | $1.35           | [Link](https://he.aliexpress.com/item/1005007363840397.html) |
| resestor 47R 0805                  | for the neopixel din                                          | 1 (100 in one order) | $1.11          | $1.11           | [Link](https://he.aliexpress.com/item/32382494431.html) |
| Solder Paste Sn63 XG-Z40           | for sodering to the pcb                                       | 1                    | $4.88          | $4.88           | [Link](https://he.aliexpress.com/item/32958428538.html) |
|o-ring thickring 2mm 26mm od        |for the conveyor of the head spin                              |1(100 in one order)   |1.99$           |$1.99            |[Link](https://he.aliexpress.com/item/1005006851111300.html) |
|PCB                              	 |the PCB	             |1 (5 for one order)	                 |$2.00	                |$7.29|
| **Sum** |                                                               |                      | **$97.96** | **$112.23** | |
