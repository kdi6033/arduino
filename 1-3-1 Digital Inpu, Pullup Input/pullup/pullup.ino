//참조 https://m.blog.naver.com/PostView.nhn?blogId=kaiserkhan21&logNo=221048743608&proxyReferer=https:%2F%2Fwww.google.com%2F

const int In =  D3;
const int Out =  D4;
void setup() {
  pinMode(Out, OUTPUT);
  //pinMode(In, INPUT);
  pinMode(In, INPUT_PULLUP);
  //pinMode(In, INPUT_PULLDOWN); //사용 못함
}

void loop() {
  if(digitalRead(In)==1)
    digitalWrite(Out, LOW);
  else
    digitalWrite(Out, HIGH);
}
