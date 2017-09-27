//
//  Pankkiautomaatti.v_10.c
//  Pankkiautomaatti nosto-, talletus- sekä saldonkysely -mahdollisuuksilla
//  Copyright (c) Mikko Rouru. All rights reserved.
//

// Käytettävät kirjastot
#include <stdio.h>
#include <string.h>

// Käytettävät aliohjelmafunktiot, rahan nosto, rahan talletus, tilin saldon tarkistus sekä syötteentarkistus.
void nosto ();
void talletus ();
void saldontarkistus ();
int lueLuku ();

// Main-funktio, jossa valitaan pankkiautomaatista haluttu toiminto. Ohjelma kysyy haluttua toimintoa niin kauan, kunnes valitaan Q (quit) eli lopetustoiminto. Q lähettää käyttäjälle
// viestin lopetuksesta ja lopettaa ohjelman.
int main(void) {
    
char valinta[50];
    
    printf("* Tervetuloa kayttamaan pankkiautomaattia! Valitse toiminto: *\n");
  
do {
	printf("N    Nosto\n");
    printf("T	 Talleta\n");
    printf("S	 Saldo\n");
    printf("Q    Lopeta\n");
    
    printf("Valintasi > ");
    scanf("%s", valinta);

    if (strcmp(valinta, "N") == 0) {
        nosto();}
    
    if (strcmp(valinta, "T") == 0) {
        talletus();}
    
    if (strcmp(valinta, "S") == 0) {
        saldontarkistus();}
    
} while (strcmp(valinta, "Q") != 0);
    
printf("Kiitos kaytosta, tervetuloa uudelleen!\n\n\n\n");
return(0);
}
    
//NOSTO-funktio
// N-komento avaa nosto-toiminnon, jossa ohjelma antaa käyttäjälleen hänen haluamansa summan.
//
// Toiminto antaa 20, 50 ja 100 euron seteleitä, jotka aiheuttavat rajoituksia (esim. 30e ei ole mahdollinen nostettava summa).
//
// Muuttujina yksi char-muuttuja ja kahdeksan int-muuttujaa, lisäksi tilitiedosto. Kakseur, viiseur ja sataeur ovat 20, 50 sekä 100 euron seteleiden muuttujia, jaol20 on luvun tarkistus
// kuinka monta kertaa 20 menee nostettavaan summaan.
//
// Funktiossa kysytään ensin tilinumero, jonka jälkeen ohjelma ottaa talteen tilin tiedostosta tunnusluvun ja saldon. Tämä siksi, koska myöhemmin luodaan uusi tilitiedosto jotta saadaan
// helposti muuttuneet tiedot talteen.
//
// Pankkiautomaatti ei anna rahaa jos: a) syötetty summa ei ole jaollinen kymmenellä, b) syötetty summa on vähemmän kuin 20 euroa, c) syötetty summa on suurempi kuin saldo, d) syötetty
// summa on 30 euroa. Jos tilillä on alle 20 euroa rahaa, käyttäjä saa viestin tästä ja hänet palautetaan alkuvalikkoon.
//
// Funktion lopussa tunnusluku alustetaan pitkäksi luvuksi, jotta pankkiautomaatti ei päästä tilille suoraan mahdollisella toisella kirjautumisella saman ohjelmaistunnon aikana.

void nosto() {
    
    char tilinumero[50];
    int tunnusluku, filetunnusluku, saldo, nostettavasumma, kakseur, viiseur, sataeur, jaol20;
    FILE *tili;

    do {
    printf("Syota tilinumero (ohjelma kysyy numeroa uudestaan jos tilinumeroa ei ole olemassa) > ");
    scanf("%s", tilinumero);
    strcat(tilinumero, ".tili.txt");
    tili = fopen(tilinumero, "r+");
    } while (tili == 0);
    
    fscanf(tili, "%d", &filetunnusluku);
    fscanf(tili, "%d", &saldo);
    fclose(tili);
    
    while(tunnusluku != filetunnusluku) {
    printf("Syota tunnusluku > ");
    tunnusluku = lueLuku();
    }
    
    if (saldo < 20) {
        printf("Tilillasi ei ole tarpeeksi rahaa nostoa varten!\n");
        tunnusluku = 9999999;
        return;
    }
    
    do {
    printf("Anna noston maara (min 20e, max tilin saldo, 30e ei mahdollinen) > ");
        nostettavasumma = lueLuku();
        if ((nostettavasumma % 10) != 0 || nostettavasumma < 20 || nostettavasumma > saldo || nostettavasumma == 30) {
            printf("Virheellinen luku, yrita uudelleen\n");
        }
    } while ((nostettavasumma % 10) != 0 || nostettavasumma < 20 || nostettavasumma > saldo || nostettavasumma == 30);
    
    saldo = saldo - nostettavasumma;
    
    tili = fopen(tilinumero, "w");
    
    fprintf(tili, "%d", filetunnusluku);
    fprintf(tili, "\n");
    
    printf("Saat:\n");
    viiseur = 0;
    if ((nostettavasumma % 20) != 0 && (nostettavasumma > 49)) {
        viiseur = 1;
        nostettavasumma = nostettavasumma - 50;
    }

    jaol20 = nostettavasumma / 20;
    sataeur = jaol20 / 5;
    kakseur = jaol20 - 5 * sataeur;
    fprintf(tili, "%d", saldo);
    printf("%d kpl 20e seteleita\n", kakseur);
    printf("%d kpl 50e seteleita\n", viiseur);
    printf("%d kpl 100e seteleita\n", sataeur);
    
    fclose(tili);
    tunnusluku = 9999999;
    printf("Kiitos kaytosta, tervetuloa uudelleen!\n\n\n\n");
}

//TALLETUS-funktio
// T-komento avaa talletusfunktion, jossa talletetaan tilille rahaa.
//
// Käyttäjä syöttää haluamansa talletettavan summan ja tämä summa lisätään tilin saldoon. Lopuksi käyttäjälle näytetään talletettu summa ja uusi saldo.
//
// Muuttujina yksi char-muuttuja ja neljä int-muuttujaa, lisäksi tilitiedosto. Tämä funktio ei tarvitse paljoa muuttujia; vain kirjautumismuuttujat sekä saldon ja talletettavan summan.
//
// Tilitiedosto avataan tietojen talletusta aluksi, jonka jälkeen luodaan uusi tilitiedosto uutta saldoa varten.
//
// Lopussa jälleen kerran nollataan tunnusluku mahdollista toista kirjautumista saman ohjelmaistunnon aikana.

void talletus() {
    
    char tilinumero[50];
    int tunnusluku, filetunnusluku, saldo, talletettavasumma;
    FILE *tili;

    do {
    printf("Syota tilinumero (ohjelma kysyy numeroa uudestaan jos tilinumeroa ei ole olemassa) > ");
    scanf("%s", tilinumero);
    strcat(tilinumero, ".tili.txt");
    tili = fopen(tilinumero, "r+");
    } while (tili == 0);
    
    fscanf(tili, "%d", &filetunnusluku);
    fscanf(tili, "%d", &saldo);
    fclose(tili);
    	
    while(tunnusluku != filetunnusluku) {
    printf("Syota tunnusluku > ");
    tunnusluku = lueLuku();
    }
    	
    do {
    printf("Syota talletuksen maara > ");
    talletettavasumma = lueLuku();
   
        if (talletettavasumma < 0) {
            printf("Virheellinen luku, yrita uudelleen\n");
        }
    } while (talletettavasumma < 0);
    
    tili = fopen(tilinumero, "w");
    	
    fprintf(tili, "%d", filetunnusluku);
    fprintf(tili, "\n");
    
    saldo = saldo + talletettavasumma;
    fprintf(tili, "%d", saldo);
    printf("Talletettu %d euroa, saldo on nyt %d euroa\n", talletettavasumma, saldo);
    
	fclose(tili);
    tunnusluku = 9999999;
    printf("Kiitos kaytosta, tervetuloa uudelleen!\n\n\n\n");
}
   
//SALDON TARKISTUS -funktio
// S-komento avaa saldon tarkistus -funktion, jossa käyttäjä voi tarkistaa saldonsa.
//
// Char-muuttujia on yksi, int-muuttujia kolme ja lisäksi tilitiedosto. Yksinkertaisin funktio toteuttaa pankkiautomaatissa.
//
// Funktio kysyy aluksi tilitiedot tuttuun tapaan, jonka jälkeen funktio lukee avatusta tiedostosta saldon ja tulostaa sen käyttäjälle. Tämän jälkeen nollataan tuttuun tapaan tunnusluku ja
// näytetään kiitosviesti.

void saldontarkistus() {
    
    char tilinumero[50];
    int tunnusluku, filetunnusluku, saldo;
    FILE *tili;
    
    do {
    printf("Syota tilinumero (ohjelma kysyy numeroa uudestaan jos tilinumeroa ei ole olemassa) > ");
    scanf("%s", tilinumero);
    strcat(tilinumero, ".tili.txt");
    tili = fopen(tilinumero, "r+");
    } while (tili == 0);
   
    fscanf(tili, "%d", &filetunnusluku);
    
    while(tunnusluku != filetunnusluku) {
    printf("Syota tunnusluku > ");
    tunnusluku = lueLuku();
    }
	
	fscanf(tili, "%d", &saldo);
    printf("Saldosi on %d euroa\n", saldo);
    
    fclose(tili);
    tunnusluku = 9999999;
    printf("Kiitos kaytosta, tervetuloa uudelleen!\n\n\n\n");
}

//SYÖTTEENTARKISTUS -funktio
// Funktio tarkistaa onko syötetty syöte varmasti luku. Lähettää virheilmoituksen jos esimerkiksi syöttää kirjaimia.
//
// Lainattu luentodemoista Nopasta.

int lueLuku () {
    int luku = 0;
    int tila = 0;
    do {
        tila = scanf("%d", &luku); while(fgetc(stdin) != '\n') {
            ; }
        if(0 == tila) {
            printf("Virheellinen luku, yrita uudelleen > ");
        }
    } while(0 == tila);
    return luku;
}

