###fscanf
Ganske genialt å bruke.  I de tilfeller der vi stoler på formateringen av teksten så kan vi bruke den direkte til å hente ut tekst.
Den ignorerer newline og all form for whitespace. F.eks gitt data på formen

Navn Etternavn Fødselsdato

Så vil fscanf(fil, "%s", person.fornavn), fscanf(fil, "%s", person.etternavn) og fscanf(fil, "%d", &person.fdato);
lage personen for oss.

###malloc
malloc(bytes) returnerer en void pointer som kan tildeles hva som helst.