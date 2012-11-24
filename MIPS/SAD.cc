
//================================================================================
//SAD.cc 
//Laskee kahden taulukon välisen SAD (sum of absolute differences) arvon.
//HUOM. EI SISÄLLÄ VIRHEENTARKASTUKSIA.
//================================================================================

//INCLUDE-directiivit:
#include <iostream>
#include <string>
#include <cstdlib>

//Avataan standardinimiavaruus käyttöön
using namespace std;

//calcSAD-aliohjelma, joka laskee parametreina annettujen taulukoiden SAD:n ja
//palauttaa sen: 
//Käy läpi jokaisen taulukon alkion ja laskee erotuksen toisen taulukon vastaavan 
//alkion kanssa. Tämän jälkeen ottaa erotuksesta itseisarvon ja lisää sen summa-muuttujaan.
//Kun taulukot on käyty kokonaan läpi, palautetaan summa muuttujan arvo.
int calcSAD(int taulu1[], int taulu2[])
{
	int summa = 0;

	for (int i = 0; i < sizeof(taulu1); ++i)
	{
		int erotus = 0;	
		
		erotus = taulu1[i] - taulu2[i];	
		erotus = abs(erotus);	

		summa = summa + erotus;	
	}

	return summa;	
}

//Pääohjelma, jolla funktiota voi testata:
//Alussa luodaan kaksi taulukkoa, taulukko1 ja taulukko2. Nämä annetaan parametreiksi 
//calcSAD-funktiolle, joka palauttaa taulukoista lasketun SAD:n. SAD tulostetaan käyttäjälle 
//ja ohjelmasta poistutaan.
int main()
{
	int taulu1[] = {11, 77, 250, 113}; 
	int taulu2[] = {121, 89, 249, 97};
	int SAD = 0;	

	SAD = calcSAD(taulu1, taulu2);

	cout << SAD << endl;
	
	return EXIT_SUCCESS; 
}
