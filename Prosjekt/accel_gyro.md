#Notes, ideas etc

Et fly har 6 verdier i rommet. Posisjon x, posisjon y, posisjon z, vinkel langs x, vinkel langs y, vinkel langs z.
Vi har ikke gps, så posisjon blir irrelevant. Det vi derimot har er en accelerometer. Den skal gi oss akselerasjonen langs x,y,z. 
Vi kan enten integrere denne opp (euler chrome, runge-kutta, euler) til å finne posisjon

### Test acceleormeteret
	- Ved ro, med siden opp, på et bord
	- De verdiene du får nå skal gi deg retningen til g vektoren
	- Dersom g vektoren ikke peker langs Z, ikke noe problem, bruk dot produkt til å finne
	  vinkelen i mellom de to. For referanse. 
	- Eventuelt kan du trekke ifra g vektoren slik at den peker langs Z. Da har du fåttt 
	  en kalibrering. Dette gjør feilsøking osv mye enklere vil jeg tro. 
	- Testen skal gi oss lesbare ax, ay, az


### Test gyro

	- Her får vi vinkler, eller rettere sagt vinkelen mellom inertial force vector og x,y,z
	- En pitch, en yaw og en roll


### Filter
	- Akselerasjon utsatt for lineær støy. Dette i form av random krefer i xyz, temp flux, små windgusts etc
	- Gyroscope utsatt for støy i form av drift, tenk deg kule på gummistrikk, vil dale ned mot laveste punkt, og hvis heldig svinge tilbake til
	- der kraften tilsier at den egentlig skal være, eller i verste fall svinge før den setter seg i ro.
	- Må da ta hensyn til dette.
	- I profesjonelt sammenheng brukes no som heter kalman-filtering (ICBM, NASA, mer avanserte droner etc bruker denna)
	- Men den er for kompleks for oss.
	- Vi kan si oss fornøyd med en timeaveraged weighted filter.
	- Dette for å jevne ut støy (random flux)
	- Erfaring tilsier at akselerometeret (pga lineær sensor) er mer utsatt for støy
	- Vekting slik at gyro stoles på mer.