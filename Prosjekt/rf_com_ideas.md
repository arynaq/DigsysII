#Forskjellige måter å få til sensor<>MCU kommunikasjon på

##Xbee
	Tillater 2-veis kommunikasjon mellom mange enheter
	Lett å utvide, fungerer som noder i nettverk
	1 hub (arduino), flerer sensorer (atmega16? trenger en eller form for mikrokontroller med de)

	Dyrt! 150kr+ for hver node, trenger i tillegg et skjold for arduino huben

##RF-link moduler
	Veldig billig, koster type 30kr for TX (transmitter), RX(receiver)
	Fungerer fint med 2 enheter, dvs fra TX (sensor) til RX (arduino hub), men må tenke på interferens, timing, osv når det er flere enheter. De billige sakene har EN eneste kanal. To sensorer kan ikke sende ut på samme frekvens.

	Med en potte:
		Plug & Play: Trenger en MCU for sensoren fra fuktighetsmåler. Kan også koble til sensoren fra pumpa. MCUen programmeres til å sende ut sensordata hvert 10.sekund eller noe (spare batteri) som en int (32 bits)

		0000 0000 | 0000 0000 | 0000 0000 | 0000 0000

		   A           B          C           D

		A = for feilhåndtering, mottageren vår vil ta imot mye støy også, det er jo stappfullt
		med radiosignaler i lufta. 
		Hvis vi på mottageren tar en signal ABCD og ser at A er f.eks 3 (vi bestemmer på forhånd)
		så vet vi at vi ikke leser støy men heller noe fra sensoren.

		B = pumpe sensor data
		C = fuktighetsmåler sensordata
		D = identifikator for sensoren

		D = 1, betyr at det er potte1
		D = 2, betyr at det er potte2 osv 

		En signal ut fra en potte kan da se slik ut:
			ABCD = 3|14|25|1
			"Dette er en signal fra vår potte", "Sensoravlesning på pumpa gir 14", 
			"Sensoravlesning på fuktighetsmåler gir 25" "Dette er fra potte1"

	Med flere potter:
		Litt verre, ikke lenger plug & play, men så lenge vi får det til å fungere med EN potte
		så skal vi nok finne på en lur måte med flere pumper. 


	Problemer:
		Pumpa, den trenger en mottager, hvordan skal vi få til det? Skru på pumpa hvis vannivået
		er slik og slik. Kjøpe enda en mottager? Begynner å bli dyrt detta :D hver potte kommer
		til å ende opp med sensorpris på >100kr


