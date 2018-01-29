void setup(){
pinMode(13,OUTPUT);
}

void loop(){
  digitalWrite(13,LOW);
  delay(3);
  digitalWrite(13,HIGH);
  delay(32);
}
