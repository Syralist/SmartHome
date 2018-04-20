EESchema Schematic File Version 4
LIBS:wemos-bme-cache
EELAYER 26 0
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
L wemos_mini:WeMos_mini U1
U 1 1 5AD9F0AE
P 4000 3550
F 0 "U1" H 4000 4050 60  0000 C CNN
F 1 "WeMos_mini" H 4000 3050 60  0000 C CNN
F 2 "wemos_d1_mini:D1_mini_board" H 4550 2850 60  0001 C CNN
F 3 "" H 4550 2850 60  0000 C CNN
	1    4000 3550
	1    0    0    -1  
$EndComp
$Comp
L wemos-bme-rescue:Conn_01x04_Female J1
U 1 1 5AD9F2C0
P 3900 2300
F 0 "J1" H 3900 2500 50  0000 C CNN
F 1 "BME280" H 3900 2000 50  0000 C CNN
F 2 "Legacy_Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 3900 2300 50  0001 C CNN
F 3 "" H 3900 2300 50  0001 C CNN
	1    3900 2300
	0    -1   -1   0   
$EndComp
Text Notes 4700 2450 0    60   ~ 0
1 VIN\n2 GND\n3 SCL\n4 SDA
Wire Wire Line
	3800 2500 3800 2700
Wire Wire Line
	3800 2700 3500 2700
Wire Wire Line
	3500 2700 3500 3200
Wire Wire Line
	3900 2500 3900 2950
Wire Wire Line
	3900 2950 3350 2950
Wire Wire Line
	3350 2950 3350 3300
Wire Wire Line
	3350 3300 3500 3300
Wire Wire Line
	4000 2500 4000 3700
Wire Wire Line
	4000 3700 3500 3700
Wire Wire Line
	4100 2500 4100 3600
Wire Wire Line
	4100 3600 3500 3600
Wire Wire Line
	4500 3700 4800 3700
Wire Wire Line
	4800 3700 4800 3900
Wire Wire Line
	4800 3900 4500 3900
$Comp
L wemos-bme-rescue:Conn_01x02_Male J2
U 1 1 5AD9F704
P 5300 3000
F 0 "J2" H 5300 3100 50  0000 C CNN
F 1 "DeepSleep" H 5300 2800 50  0000 C CNN
F 2 "Legacy_Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5300 3000 50  0001 C CNN
F 3 "" H 5300 3000 50  0001 C CNN
	1    5300 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	5200 3200 5200 3300
Wire Wire Line
	5200 3300 4500 3300
Wire Wire Line
	5300 3200 5300 3450
Wire Wire Line
	5300 3450 3900 3450
Wire Wire Line
	3900 3450 3900 3300
Connection ~ 3500 3300
Wire Wire Line
	3500 3300 3900 3300
$EndSCHEMATC
