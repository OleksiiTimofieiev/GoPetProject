#define RXD2 16 //RXX2 pin
#define TXD2 17 //TX2 pin

HardwareSerial serial2(2);

void setup(){
  Serial.begin(115200);
  serial2.begin(115200, SERIAL_8N1, TXD2, RXD2);
}

void parse(String input_string) {
//  if (input_string.equals(Led_off) == true) {
//    return 10;
//  }
//  else if (input_string.equals(Led_on) == true) {
//    return 11;
//  }
//  else return 0;
  Serial.println(input_string);
}

void loop(){
  parse(serial2.readString());
  delay(200);
  
}
