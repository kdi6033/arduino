#include <arduino.h>

int end232,end485 =0;
String inputString232,inputString485;

void setup() {
  Serial.begin(115200);
  
  // RX 16핀, TX 17핀, 8비트, 페리티 비트 없음, Stop 비트 1
  Serial2.begin(115200, SERIAL_8N1, 16, 17); //RS-485
  Serial1.begin(115200, SERIAL_8N1, 26, 27); //RS-232

}

void loop() {    
  /*
  //모니터 프로그램으로 입릭이 들어오면
  if(Serial.available()){
    //들어온 데이터를 Serial2로 그대로 전송
    unsigned char Rcv_Byte = Serial.read();
    Serial2.write(Rcv_Byte);
    Serial1.write(Rcv_Byte);

  }
  */
  serial232Event();
  serial485Event();
}

void serial232Event() {
  if (Serial1.available()) {
    // get the new byte:
    char inChar232 = (char)Serial1.read();
    if(inChar232 == 0x41) // 마지막에 A가 들어오면 출력
      end232=1;
    inputString232 += inChar232;
  }
  if(end232==1) {
    Serial.println("-----RS232 received-----");
    Serial.print("Received data :  ");
    Serial.println(inputString232);
    Serial.println("");
    inputString232="";
    end232=0;
   }

}

void serial485Event() {
  if (Serial2.available()) {
    // get the new byte:
    char inChar485 = (char)Serial2.read();
    if(inChar485 == 0x42) // 마지막에 B 가 들어오면 출력
      end485=1;
    inputString485 += inChar485;
  }
  if(end485==1) {
    Serial.println("-----RS485 received-----");
    Serial.print("Received data :  ");
    Serial.println(inputString485);
    Serial.println("");
    inputString485="";
    end485=0;
   }

}
