void* mJumpPoint;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Start");
  do{
  __label__ kk;
  setJump(&&kk);
  Serial.println("setJump(&&kk);");

  kk:
  Serial.println("kk:");
  delay(1000);
  ;
  }while(0);
  goto *mJumpPoint;
}

void loop() {
  // put your main code here, to run repeatedly:

}

void setJump(void* jumpPoint) {
  mJumpPoint = jumpPoint; 
}

void * getJump(){
  return mJumpPoint;
}

