/*
   Benodigdheden voor het ontvangen:
   LED                x1;
   Infrarood receiver x1;
   Draad              x6;
   Weerstand 330 Ohm  x1;

   Receiver:
   Ga er van uit dat je tegen de voorkant aan kijkt. (Het bolletje)
   Het linker poot sluit je aan op de pin die je in de code aangeeft.
   Het middelste poot sluit je aan op 5V.
   Het recher poot sluit je aan op de ground.

   LED:
   Cathode sluit je aan op de ground (korte poot)
   Anode sluit je via de weerstand aan op de pin die je in de code aangeeft.
*/

#include <IRremote.h>

int main(void)
{
  // Digital pin 11. Output van receiver kan je aanpassen in de library.
  int RECV_PIN = 11; 

  //Library hocus pocus.
  IRrecv irrecv(RECV_PIN);
  decode_results results;
  Serial.begin(9600);
  irrecv.enableIRIn();
  
  // Digital pin 5. LED om te laten knipperen, kan je zonder probleem aanpassen.
  DDRD |= (1 << DDD5); 

  while(1)
  {
    if (irrecv.decode(&results))
    {
      // Hij verwacht een aan/uit knop signaal van een SONY afstandsbediening.
      Serial.println(results.value, HEX);
      
      // Als er signaal binnen komt, toggle digital pin 5.
      PORTD ^= (1 << PD5); 
      irrecv.resume();
    }
  }
}
