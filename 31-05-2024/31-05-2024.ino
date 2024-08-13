/* NVQ_PLC ROBOT PENETRÓMETRO version 1.0
*/
#include <ParserLib.h>
#include <Wire.h>
Parser parser;

// ================ PINOUT =================== //
#define SONDA 2
#define ZMAX 3
#define ZMIN 4
#define XMIN 5
#define XMAX 6
#define PARADA 7
#define PULX 8
#define DIRX 9
#define ENAX 10
#define PULZ 11
#define DIRZ 12
#define ENAZ 13
// ================ VARIABLES ================== //
int FLAG_STATE = 0;
long HOME_PULSES = 1000600;
long UP_PULSES = 60000;
long DOWN_PULSES = 30000;
long ARRIBA_PULSES = 5000;
long ABAJO_PULSES = 5000;
long udelaySensor = 1400;
long dist_frutas = 6000;
long udelay_X = 82; // old 't' var
long udelay_Z = 130; // old 't' var
int num_frutas = 0;
long ajuste = 720;
int fino = 300;

// "<=ERROR=>"
String command = " ";
String cini = "<=";
String cend = "=>";
String p_reader = "";
// =============== FUNCTIONS =================== //
// S100#50#6000#5\n
//funcion ARRIBA hasta final de carrera
void HOMING_UP(long UP_PULSES, long udelay_Z){
  for (long i=0; i<UP_PULSES; i++){ 
      digitalWrite(DIRZ,LOW);
  digitalWrite(ENAZ,HIGH);
    digitalWrite(PULZ,HIGH);
    delayMicroseconds(udelay_Z);
    digitalWrite(PULZ,LOW);
    delayMicroseconds(udelay_Z);
    if (digitalRead(ZMIN) == HIGH) {
      break;}
        if (digitalRead(PARADA) == LOW) {
      break;}}} 
void RIGHT(long ajuste, long udelay_X){  // FUNCIÓN DE DESPLAZAMIENTO A LA DERECHA (de ajuste)    
  for (long i=0; i<ajuste; i++){
      digitalWrite(DIRX,HIGH);
  digitalWrite(ENAX,HIGH);
    digitalWrite(PULX,HIGH);
    delayMicroseconds(udelay_X);
    digitalWrite(PULX,LOW);
    delayMicroseconds(udelay_X);
    if (digitalRead(XMAX) == HIGH) {
      break;}
    if (digitalRead(PARADA) == LOW) {
      break;}}} 
// FUNCIÓN DOWN DE COMIENZO DE SECUENCIA
void DOWN(int DOWN_PULSES, long udelay_Z){
  for (long i=0; i<DOWN_PULSES; i++){
      digitalWrite(DIRZ,HIGH);
  digitalWrite(ENAZ,HIGH);    
    digitalWrite(PULZ,HIGH);
    delayMicroseconds(udelay_Z);
    digitalWrite(PULZ,LOW);
    delayMicroseconds(udelay_Z);
    if (digitalRead(ZMAX) == HIGH){
      break;}
    if (digitalRead(SONDA) == HIGH) {
    break;}
            if (digitalRead(PARADA) == LOW) {
      break;}}}

void bajar(int ABAJO_PULSES, long udelay_Z){
  for (long i=0; i<ABAJO_PULSES; i++){
      digitalWrite(DIRZ,HIGH);
  digitalWrite(ENAZ,HIGH);    
    digitalWrite(PULZ,HIGH);
    delayMicroseconds(udelay_Z);
    digitalWrite(PULZ,LOW);
    delayMicroseconds(udelay_Z);
    if (digitalRead(ZMAX) == HIGH){
      break;}
    if (digitalRead(SONDA) == HIGH) {
    break;}
            if (digitalRead(PARADA) == LOW) {
      break; }}}
       
// FUNCIÓN SUBIDA
void subir(long ARRIBA_PULSES, long udelay_Z){
  for (long i=0; i<ARRIBA_PULSES; i++){ 
      digitalWrite(DIRZ,LOW);
  digitalWrite(ENAZ,HIGH);
    digitalWrite(PULZ,HIGH);
    delayMicroseconds(udelay_Z);
    digitalWrite(PULZ,LOW);
    delayMicroseconds(udelay_Z);
    if (digitalRead(ZMIN) == HIGH) {
      break;}
        if (digitalRead(PARADA) == LOW) {
      break;}}} 
      
// HOMING IZQUIERDA
void izquierda(long HOME_PULSES, long udelay_X){
  for (long i=0; i<HOME_PULSES; i++){
      digitalWrite(DIRX,LOW);
  digitalWrite(ENAX,HIGH);                            
    digitalWrite(PULX,HIGH);
    delayMicroseconds(udelay_X);
    digitalWrite(PULX,LOW);
    delayMicroseconds(udelay_X);
    if (digitalRead(XMIN) == HIGH){
      break;}
            if (digitalRead(PARADA) == LOW) {
      break; }}}
      
void derecha(long dist_frutas, long udelay_X){  // FUNCIÓN DE DESPLAZAMIENTO A LA DERECHA      
  for (long i=0; i<dist_frutas; i++){
      digitalWrite(DIRX,HIGH);
  digitalWrite(ENAX,HIGH);
    digitalWrite(PULX,HIGH);
    delayMicroseconds(udelay_X);
    digitalWrite(PULX,LOW);
    delayMicroseconds(udelay_X);
    if (digitalRead(XMAX) == HIGH) {
      break;}
            if (digitalRead(PARADA) == LOW) {
      break;}}} 

void sensor(long udelaySensor, int fino){ 
  for (int i=0; i<udelaySensor; i++){    //PENETROMETRO TOMA LECTURA
  digitalWrite(DIRZ,HIGH);
  digitalWrite(ENAZ,HIGH);
  digitalWrite(PULZ,HIGH);
  delayMicroseconds(fino);
  digitalWrite(PULZ,LOW);
  delayMicroseconds(fino);
  if (digitalRead(ZMAX) == HIGH) {
    break;}
            if (digitalRead(PARADA) == LOW) {
      break;}}} 

// ================== SETUP ==================== //
void setup()
{
  Wire.begin();
  Serial.begin(115200);
  // TEST
  //
  pinMode (PULX, OUTPUT);
  pinMode (DIRX, OUTPUT);
  pinMode (ENAX, OUTPUT);
  pinMode (PULZ, OUTPUT);
  pinMode (DIRZ, OUTPUT);
  pinMode (ENAZ, OUTPUT);
  pinMode (ZMAX, INPUT);
  pinMode (ZMIN, INPUT);
  pinMode (XMIN, INPUT);
  pinMode (XMAX,INPUT);  
}
//====================================LOOP========================================//
void loop()
{
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    switch (inByte)
    {
      // Sudelay_X#udelay_Z#dist_frutas#num_frutas
      case 'S':
        p_reader = Serial.readStringUntil('\n');
        if (p_reader.length() > 0) {
          parser.Init(p_reader);
          udelay_X = parser.Read_Int16();
          parser.Skip(1);
          udelay_Z = parser.Read_Int16();
          parser.Skip(1);
          dist_frutas = parser.Read_Int16();
          parser.Skip(1);
          num_frutas = parser.Read_Int16();
          // Añadir código penetrómetro
          FLAG_STATE = 1; } // START HOMMING
      break; }}
  // ====== STATE MACHINE =========//
  if (FLAG_STATE == 1){
    command = cini+ FLAG_STATE + cend;
    Serial.println(command);
// secuencia de homing antes de los ciclos    
HOMING_UP(UP_PULSES, udelay_Z);
izquierda(HOME_PULSES, udelay_X);
RIGHT(ajuste, udelay_X); //ajuste inicial a la derecha
DOWN(DOWN_PULSES, udelay_Z);
    //Serial.println("<=HOME=>");
    FLAG_STATE = 2; // START EXPERIMENT  
  }
  // comienza el ciclo
  else if (FLAG_STATE == 2){
for (int ciclo = 0; ciclo < num_frutas; ciclo++) {
  command = cini + FLAG_STATE + cend;
  Serial.println(command);
  bajar(ABAJO_PULSES, udelay_Z);
  sensor(udelaySensor, fino);  
  if (digitalRead(PARADA) == LOW) {
    break;}
  delay(1000);
  subir(ARRIBA_PULSES, udelay_Z);
  if (digitalRead(PARADA) == LOW) {
    break;}
  // Ejecutar la función 'derecha' solo si no es la última iteración
  if (ciclo < num_frutas - 1) {
    derecha(dist_frutas, udelay_X); 
    if (digitalRead(XMAX) == HIGH) {
      break;}
    if (digitalRead(PARADA) == LOW) {
      break;}}}


      
//HOMING_UP(UP_PULSES, udelay_Z);
izquierda(HOME_PULSES, udelay_X);
RIGHT(ajuste, udelay_X);
DOWN(DOWN_PULSES, udelay_Z);
for (int ciclo = 0; ciclo < num_frutas; ciclo++) {
  command = cini + FLAG_STATE + cend;
  Serial.println(command);
  bajar(ABAJO_PULSES, udelay_Z);
  sensor(udelaySensor, fino);
  if (digitalRead(PARADA) == LOW) {
    break;}
  delay(1000);
  subir(ARRIBA_PULSES, udelay_Z);
  if (digitalRead(PARADA) == LOW) {
    break;}
  // Ejecutar la función 'derecha' solo si no es la última iteración
  if (ciclo < num_frutas - 1) {
    derecha(dist_frutas, udelay_X);
    
    if (digitalRead(XMAX) == HIGH) {
      break;} 
    if (digitalRead(PARADA) == LOW) {
      break;}}}
    //Serial.println("<=HOMMING=>");
    FLAG_STATE = 3; // HOME AT END
}
  else if (FLAG_STATE == 3){
HOMING_UP(UP_PULSES, udelay_Z);
    delay(1000);
izquierda(HOME_PULSES, udelay_X);
  digitalWrite(ENAX, LOW);
  digitalWrite(ENAZ, LOW);
    command = cini+ FLAG_STATE + cend;
    Serial.println(command);
    FLAG_STATE = 0; // END EXPERIMENT
  } 
  else{
    command = cini + FLAG_STATE + cend;
    Serial.println(command);
  }
  
  // Serial.print("Distancia Frutas: ");
  // Serial.println(dist_frutas);
  // Serial.print("DelayX: ");
  // Serial.println(udelay_X);
  // Serial.print("DelayY: ");
  // Serial.println(udelay_Z);
  // Serial.print("Numero de Frutas: ");
  // Serial.println(num_frutas);
}
