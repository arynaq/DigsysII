##Pensum: Kap 1-14

###Øving1
	- Float/Int conversion
		- Passe på avrundingsfeil og heltallsaritmetikk når man egentlig vil ha
		  flyttal. F.eks så er 9/5 = 1, men 9.0/5 = 1.8;
		- Kjenne til noen funksjoner fra math.h biblioteket. F.eks sqrt, abs
		- Innlesingen fra kommandolinje. 
		- Utskrift til kommandolinje. 
		- Bruk av randomfunksjonen
		- if/else statements

###Øving2
	- While loops
	- Seede random (srand())
	- 

###Øving3
	- Funksjoner
		- Deklarering, bruk
		- Bruk av funksjon i andre funksjoner

###Øving4
	- For-loops
	
###Øving5
	- Arrays
		- Deklarering, type (float, int, osv), bruk i funksjoner
	- Preprocessor-macros (#define), til å lage konstanter

###Øving6
	- Pointers
		- Deklarering, type (float, int, osv), bruk i funksjoner
		- Dereference (* på en pointer gir verdi)
		- Reference (& på en variabel gir adresse)
	- Sortering
		- Pensum?
	- Todimensjonale arrays
		- Deklarering, type (array av array av int, osv)
	- Tredimensjonale arrays
		- Samme som over

###Øving7
	- Innlesing av tekst til buffer. F.eks lese navn i en char navn[100];
	- Funksjoner i string.h, som strlen, strtok, strcmp
	- Oppdeling av tekst, strtok
	- 

###Øving8
	- Morsekode? Ihvertfall matching av A <-> B
	- Her har vi matchet en bokstav mot en morsekode
	- Bitshifting. Manipulere tall på binærform.

###Øving9
	- Struct, deklarering, bruk, bruk i funksjoner
	- Typedef til å gi nytt navn til ting. F.eks typedef unsigned char BYTE.
	- List initialisering. F.eks struct NAVN a = {"A"} dersom NAVN har member char* navn;
	- Dersom NAVN har char* fornavn, char* etternavn
		struct NAVN me = {.fornavn="Aryan", .etternavn="Naqid"};

###Øving10
	- Filbehandling
		- Skrive til en ny fil
		- Lese fra en fil
		- Skrive på slutten til en fil
		- Formattere til/fra fil. (Som i legge 3 mellomrom mellom fornavn og etternavn dersom man skal skrive navn til en fil)
		- Binary files
		- Random access (fseek,fgetpos, rewind)
		- Søk etter tekst i fil

###Øving11
	- Rekursive structer, slektstre
	- Lenkede structer, lists
	