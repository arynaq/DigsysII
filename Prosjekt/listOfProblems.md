1. Gyro/Accel data. Tar lang tid (sekunder..) før den måler riktig vinkel, drift
	-Må kalibreres
	-Kjør kalibreringskode på arduinoen med horisontal gyro
2. PID tuning gains
	- Vanskelig, vi har brukt flere dager og mange timer på dette. Største problemet er testriggen vår. Optimalt
	  burde vi ha stang igjennom ene aksen på dronen for å teste.
	- Prøve Ziegler - Nicholas på mandag?
3. PID to PWM
	- Ser jo fra dataROLL at selv på maks utslag (20-30 grader) så er PIDen ikke større enn 1%
	- 1% avvik i PWM for balansering av en tung drone? Det høres ut som tull
	- Da må KP KI KD være mye høyere enn vi har satt de idag.
	. Får vi større verdier fra PIDen med KP KD KI fra Ziegler-Nicholas?
