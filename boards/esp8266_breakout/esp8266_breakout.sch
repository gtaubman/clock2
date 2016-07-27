EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ESP8266
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ESP-12E U1
U 1 1 5797F74C
P 5325 3250
F 0 "U1" H 5325 3150 50  0000 C CNN
F 1 "ESP-12E" H 5325 3350 50  0000 C CNN
F 2 "ESP8266:ESP-12E" H 5325 3250 50  0001 C CNN
F 3 "" H 5325 3250 50  0001 C CNN
	1    5325 3250
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X08 P2
U 1 1 5797F76E
P 6700 3300
F 0 "P2" H 6700 3750 50  0000 C CNN
F 1 "CONN_01X08" V 6800 3300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08" H 6700 3300 50  0001 C CNN
F 3 "" H 6700 3300 50  0000 C CNN
	1    6700 3300
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X08 P1
U 1 1 5797F789
P 3875 3300
F 0 "P1" H 3875 3750 50  0000 C CNN
F 1 "CONN_01X08" V 3975 3300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08" H 3875 3300 50  0001 C CNN
F 3 "" H 3875 3300 50  0000 C CNN
	1    3875 3300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4075 2950 4425 2950
Wire Wire Line
	4075 3050 4425 3050
Wire Wire Line
	4075 3150 4425 3150
Wire Wire Line
	4075 3250 4425 3250
Wire Wire Line
	4075 3350 4425 3350
Wire Wire Line
	4075 3450 4425 3450
Wire Wire Line
	4075 3550 4425 3550
Wire Wire Line
	4075 3650 4425 3650
Wire Wire Line
	6225 2950 6500 2950
Wire Wire Line
	6500 3050 6225 3050
Wire Wire Line
	6225 3150 6500 3150
Wire Wire Line
	6500 3250 6225 3250
Wire Wire Line
	6225 3350 6500 3350
Wire Wire Line
	6500 3450 6225 3450
Wire Wire Line
	6225 3550 6500 3550
Wire Wire Line
	6500 3650 6225 3650
NoConn ~ 5075 4150
NoConn ~ 5175 4150
NoConn ~ 5275 4150
NoConn ~ 5375 4150
NoConn ~ 5475 4150
NoConn ~ 5575 4150
$EndSCHEMATC
