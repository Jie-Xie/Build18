/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 11;

IRsend irsend;
IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  // LED transmits infared
<<<<<<< Updated upstream
  irsend.sendSony(0xa90, 12); 
  irrecv.enableIRIn();
//  irrecv.decode(&results);
//  Serial.println(results.value, HEX);
=======
  irsend.sendSony(0xa90, 12);
  irrecv.enableIRIn();
>>>>>>> Stashed changes
  //receiver receives infared 
//  if (irrecv.decode(&results)) {
//    Serial.println(results.value, HEX);
//  }
  delay(500);
}
