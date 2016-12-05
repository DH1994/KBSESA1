/*
  Benodigdheden voor het verzenden:
  Infrarood LED     x1;
  Draad             x2;
  Weerstand 330 Ohm x1;

  LED:
  Cathode sluit je aan op de ground. (Korte poot)
  Anode sluit je via de weerstand aan op pin 5(Lange poot). Dit kan je in de library aanpassen.
*/

#include <IRremote.h>

int main(void)
{
  IRsend irsend;

  while(1)
  {
    for (int i = 0; i < 3; i++)
    {
      // Aan/uit knop van een SONY afstandsbediening wordt hier verstuurd.
      irsend.sendSony(0xa90, 12);
      _delay_ms(40);
    }
  }
}

