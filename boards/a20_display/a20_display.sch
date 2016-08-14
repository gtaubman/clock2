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
LIBS:liteon
LIBS:a20_display-cache
EELAYER 25 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "A20 Display"
Date "2016-08-14"
Rev "1"
Comp "Taubman Electronics"
Comment1 "Drawn by G. Taubman"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LTD-2601WC AFF1
U 1 1 57B0D469
P 3600 5225
F 0 "AFF1" H 3600 5225 50  0000 C CNN
F 1 "LTD-2601WC" H 3600 4775 50  0000 C CNN
F 2 "liteon:LTD-2601WC" H 3600 5225 50  0001 C CNN
F 3 "" H 3600 5225 50  0000 C CNN
	1    3600 5225
	1    0    0    -1  
$EndComp
Text GLabel 4900 2025 0    60   Input ~ 0
Digit1Power
Text GLabel 4900 2150 0    60   Input ~ 0
Digit2Power
Text GLabel 4900 2275 0    60   Input ~ 0
Digit3Power
Text GLabel 4900 2400 0    60   Input ~ 0
Digit4Power
Text GLabel 4900 2525 0    60   Input ~ 0
Digit5Power
Text GLabel 4900 2650 0    60   Input ~ 0
Digit6Power
Text GLabel 6475 2850 2    60   Input ~ 0
SegA
Text GLabel 6475 2725 2    60   Input ~ 0
SegB
Text GLabel 6475 2600 2    60   Input ~ 0
SegC
Text GLabel 6475 2350 2    60   Input ~ 0
SegE
Text GLabel 6475 2225 2    60   Input ~ 0
SegF
Text GLabel 6475 2100 2    60   Input ~ 0
SegG
Text GLabel 6475 1975 2    60   Input ~ 0
SegP
Text GLabel 2500 4725 0    60   Input ~ 0
SegA
Text GLabel 2500 4850 0    60   Input ~ 0
SegB
Text GLabel 2500 4975 0    60   Input ~ 0
SegC
Text GLabel 2500 5225 0    60   Input ~ 0
SegE
Text GLabel 2500 5350 0    60   Input ~ 0
SegF
Text GLabel 2500 5475 0    60   Input ~ 0
SegG
Text GLabel 2500 5600 0    60   Input ~ 0
SegP
Text GLabel 6475 2475 2    60   Input ~ 0
SegD
Text GLabel 2500 5100 0    60   Input ~ 0
SegD
Wire Wire Line
	2500 4725 2750 4725
Wire Wire Line
	2750 4725 2750 4825
Wire Wire Line
	2500 4850 2750 4850
Wire Wire Line
	2750 4850 2750 4925
Wire Wire Line
	2500 4975 2750 4975
Wire Wire Line
	2750 4975 2750 5025
Wire Wire Line
	2500 5100 2750 5100
Wire Wire Line
	2750 5100 2750 5125
Wire Wire Line
	2500 5225 2750 5225
Wire Wire Line
	2500 5350 2750 5350
Wire Wire Line
	2750 5350 2750 5325
Wire Wire Line
	2500 5475 2750 5475
Wire Wire Line
	2750 5475 2750 5425
Wire Wire Line
	2500 5600 2750 5600
Wire Wire Line
	2750 5600 2750 5525
Text GLabel 3525 4375 1    60   Input ~ 0
Digit1Power
Text GLabel 3675 4375 1    60   Input ~ 0
Digit2Power
Wire Wire Line
	3525 4375 3550 4375
Wire Wire Line
	3550 4375 3550 4475
Wire Wire Line
	3675 4375 3650 4375
Wire Wire Line
	3650 4375 3650 4475
$Comp
L LTD-2601WC AFF2
U 1 1 57B0D94F
P 5925 5225
F 0 "AFF2" H 5925 5225 50  0000 C CNN
F 1 "LTD-2601WC" H 5925 4775 50  0000 C CNN
F 2 "liteon:LTD-2601WC" H 5925 5225 50  0001 C CNN
F 3 "" H 5925 5225 50  0000 C CNN
	1    5925 5225
	1    0    0    -1  
$EndComp
Text GLabel 4825 4725 0    60   Input ~ 0
SegA
Text GLabel 4825 4850 0    60   Input ~ 0
SegB
Text GLabel 4825 4975 0    60   Input ~ 0
SegC
Text GLabel 4825 5225 0    60   Input ~ 0
SegE
Text GLabel 4825 5350 0    60   Input ~ 0
SegF
Text GLabel 4825 5475 0    60   Input ~ 0
SegG
Text GLabel 4825 5600 0    60   Input ~ 0
SegP
Text GLabel 4825 5100 0    60   Input ~ 0
SegD
Wire Wire Line
	4825 4725 5075 4725
Wire Wire Line
	5075 4725 5075 4825
Wire Wire Line
	4825 4850 5075 4850
Wire Wire Line
	5075 4850 5075 4925
Wire Wire Line
	4825 4975 5075 4975
Wire Wire Line
	5075 4975 5075 5025
Wire Wire Line
	4825 5100 5075 5100
Wire Wire Line
	5075 5100 5075 5125
Wire Wire Line
	4825 5225 5075 5225
Wire Wire Line
	4825 5350 5075 5350
Wire Wire Line
	5075 5350 5075 5325
Wire Wire Line
	4825 5475 5075 5475
Wire Wire Line
	5075 5475 5075 5425
Wire Wire Line
	4825 5600 5075 5600
Wire Wire Line
	5075 5600 5075 5525
$Comp
L LTD-2601WC AFF3
U 1 1 57B0D997
P 8325 5225
F 0 "AFF3" H 8325 5225 50  0000 C CNN
F 1 "LTD-2601WC" H 8325 4775 50  0000 C CNN
F 2 "liteon:LTD-2601WC" H 8325 5225 50  0001 C CNN
F 3 "" H 8325 5225 50  0000 C CNN
	1    8325 5225
	1    0    0    -1  
$EndComp
Text GLabel 7225 4725 0    60   Input ~ 0
SegA
Text GLabel 7225 4850 0    60   Input ~ 0
SegB
Text GLabel 7225 4975 0    60   Input ~ 0
SegC
Text GLabel 7225 5225 0    60   Input ~ 0
SegE
Text GLabel 7225 5350 0    60   Input ~ 0
SegF
Text GLabel 7225 5475 0    60   Input ~ 0
SegG
Text GLabel 7225 5600 0    60   Input ~ 0
SegP
Text GLabel 7225 5100 0    60   Input ~ 0
SegD
Wire Wire Line
	7225 4725 7475 4725
Wire Wire Line
	7475 4725 7475 4825
Wire Wire Line
	7225 4850 7475 4850
Wire Wire Line
	7475 4850 7475 4925
Wire Wire Line
	7225 4975 7475 4975
Wire Wire Line
	7475 4975 7475 5025
Wire Wire Line
	7225 5100 7475 5100
Wire Wire Line
	7475 5100 7475 5125
Wire Wire Line
	7225 5225 7475 5225
Wire Wire Line
	7225 5350 7475 5350
Wire Wire Line
	7475 5350 7475 5325
Wire Wire Line
	7225 5475 7475 5475
Wire Wire Line
	7475 5475 7475 5425
Wire Wire Line
	7225 5600 7475 5600
Wire Wire Line
	7475 5600 7475 5525
Text GLabel 5825 4425 1    60   Input ~ 0
Digit3Power
Text GLabel 6025 4425 1    60   Input ~ 0
Digit4Power
Text GLabel 8225 4425 1    60   Input ~ 0
Digit5Power
Text GLabel 8400 4425 1    60   Input ~ 0
Digit6Power
Wire Wire Line
	5825 4425 5875 4425
Wire Wire Line
	5875 4425 5875 4475
Wire Wire Line
	6025 4425 5975 4425
Wire Wire Line
	5975 4425 5975 4475
Wire Wire Line
	8225 4425 8275 4425
Wire Wire Line
	8275 4425 8275 4475
Wire Wire Line
	8400 4425 8375 4425
Wire Wire Line
	8375 4425 8375 4475
Wire Wire Line
	4900 2025 5050 2025
Wire Wire Line
	5050 2025 5050 1850
Wire Wire Line
	4900 2150 5150 2150
Wire Wire Line
	5150 2150 5150 1850
Wire Wire Line
	4900 2275 5250 2275
Wire Wire Line
	5250 2275 5250 1850
Wire Wire Line
	4900 2400 5350 2400
Wire Wire Line
	5350 2400 5350 1850
Wire Wire Line
	4900 2525 5450 2525
Wire Wire Line
	5450 2525 5450 1850
Wire Wire Line
	4900 2650 5550 2650
Wire Wire Line
	5550 2650 5550 1850
Wire Wire Line
	6350 1850 6350 1975
Wire Wire Line
	6350 1975 6475 1975
Wire Wire Line
	6250 1850 6250 2100
Wire Wire Line
	6250 2100 6475 2100
Wire Wire Line
	6150 1850 6150 2225
Wire Wire Line
	6150 2225 6475 2225
Wire Wire Line
	6050 1850 6050 2350
Wire Wire Line
	6050 2350 6475 2350
Wire Wire Line
	5950 1850 5950 2475
Wire Wire Line
	5950 2475 6475 2475
Wire Wire Line
	5850 1850 5850 2600
Wire Wire Line
	5850 2600 6475 2600
Wire Wire Line
	6475 2725 5750 2725
Wire Wire Line
	5750 2725 5750 1850
Wire Wire Line
	5650 1850 5650 2850
Wire Wire Line
	5650 2850 6475 2850
$Comp
L CONN_01X14 P1
U 1 1 57B0E0BE
P 5700 1650
F 0 "P1" H 5700 2400 50  0000 C CNN
F 1 "CONN_01X14" V 5800 1650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x14" H 5700 1650 50  0001 C CNN
F 3 "" H 5700 1650 50  0000 C CNN
	1    5700 1650
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
