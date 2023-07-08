int val = 0 ;
void setup()
{
Serial.begin(9600); // sensor buart rate  // Yellow Led Connected To D6 Pin
}
void loop()
{
int s1=analogRead(A0); // Water Level Sensor output pin connected A0
Serial.println(s1);  // See the Value In Serial Monitor
delay(100);      // for timer

}
