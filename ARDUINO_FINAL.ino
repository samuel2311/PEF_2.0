#include <TimerOne.h>
int ge= 0; // Grados eléctricos correspondientes a la onda senoidal
int intensidad=0; //intensidad
int intensidad2=0; //intensidad
int intensidad3=0; //intensidad
int intensidad4=0; //intensidad
int intensidad5=0; //intensidad
int intensidad6=0; //intensidad
int disparo = 5; // pin donde se debe conectar el ch2
int ledB = 9; // pin donde se debe conectar el ch3
int ledC = 10; // pin donde se debe conectar el ch4
int ledD = 11; // pin donde se debe conectar el ch5
int ledE = 12; // pin donde se debe conectar elch6
int ledF = 6; // pin donde se debe conectar el ch7
const byte interruptPin = 2; // pin donde se debe conectar el sync
volatile byte state = LOW; 
float lectura; //lee el número analógico de 0 a 1023
float volt; //convierte el número leído a tensión
String inputString = ""; 
bool stringComplete = false; // si el String esta completo para revisar si ya hemos complteado un comando

//-----------------------------PARTE DE MEDICIONES----------------------
float Sensibilidad = 0.66; // sensibilidad en Voltios // Amperios para sensor de 30A
//float Sensibilidad = 0.100;
//float Sensibilidad = 0.185;
float I = 0.00;
float ajuste = 0.05; // Sólo si es necesario, se usa para corregir posibles desvios en la lectura que se comparará con un amperímetro

void setup()
{
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(interruptPin),cruce_cero,FALLING); //entra cada que lee un RISING no importa que este haciendo en ese momento
  Timer1.initialize(46); // cada vez que pasen 46 us se ejecuta la funcion grado electrico que es lo que dura un grado electrico en cambiar
  Timer1.attachInterrupt(grado_electrico);
  pinMode(disparo,OUTPUT);
  pinMode(ledB,OUTPUT);
  pinMode(ledC,OUTPUT);
  pinMode(ledD,OUTPUT);
  pinMode(ledE,OUTPUT);
  pinMode(ledF,OUTPUT);
}

//Función para generar 500 muestas
float promedio_I(int muestras_I)
{
  float sensorA0;
  float intencidad=0;
  for(int i=0;i<muestras_I;i++)
  {
    sensorA0 = analogRead(A0) * (5.0/ 1023.0); // Leemos el sensor de corriente
    intencidad=intencidad+(sensorA0-2.537)/Sensibilidad; // Cálculo para obtener el valor de la corriente
  }
  intencidad=intencidad/muestras_I;//dividimos por 500 
  return(intencidad);
}

void loop() {
 // digitalWrite(disparo, LOW); // Asegura que el disparo esté en bajo cuando no se reciban nuevos comandos
 if (stringComplete)
{
  inputString.trim(); // Para cortar o eliminar cualquier caracter especial
  Serial.println(inputString); //imprimir para el control en el monitpr serial

//procesar comando
funcionProcesar();
    //
inputString = "";
stringComplete = false;
}

  I=promedio_I(500);//Promedio de 500 muestras para mejorar la presición(llamamos a la función promedio_I()
  if(I>=0.01){//Filtro para eliminar pequeñas oscilasciones(ruido)
  I= ((I*10)+ajuste),2;
  Serial.print("C:");
  Serial.println(I);//Imprime el valor de la corriente consumida 
  delay(1000); 
  }else{
  Serial.print("C:");
  Serial.println("0.00");
}
lectura = analogRead(A1);
  volt = (lectura)*0.053; //vuelve el valor de la lecutura 1 o menos y se multiplica por el factor de escalamiento
  Serial.print("V:");
  Serial.println(volt);
  delay(1000);
}

void funcionProcesar(){
  if (inputString.indexOf("$SA") != -1){
  String holi = inputString.substring(3); // Convierte la cadena a un entero
  intensidad = holi.toInt();
  Serial.println(intensidad);
}
else if (inputString.indexOf("$SB") != -1) {
  String holi2 = inputString.substring(3); // Convierte la cadena a un entero
  intensidad2 = holi2.toInt();
  Serial.println(intensidad2);
}
else if (inputString.indexOf("$SC") != -1) {
  String holi3 = inputString.substring(3); // Convierte la cadena a un entero
  intensidad3 = holi3.toInt();
  Serial.println(intensidad3);
}
else if (inputString.indexOf("$SD") != -1) {
  String holi4 = inputString.substring(3); // Convierte la cadena a un entero
  intensidad4 = holi4.toInt();
  Serial.println(intensidad4);
}
else if (inputString.indexOf("$SE") != -1) {
  String holi5 = inputString.substring(3); // Convierte la cadena a un entero
  intensidad5 = holi5.toInt();
  Serial.println(intensidad5);
}
else if (inputString.indexOf("$SF") != -1) {
  String holi6 = inputString.substring(3); // Convierte la cadena a un entero
  intensidad6 = holi6.toInt();
  Serial.println(intensidad6);
}
}

void cruce_cero()
{
  ge=0; //cada vez que entra el ge es cero
}

void grado_electrico()
{
  ge++;

  if(intensidad==ge) // cuando la intensidad es igual al grado electrico se dispara el triac
  {
   digitalWrite(disparo,HIGH); //62.5 ns
   digitalWrite(disparo,LOW);
  }
  if(intensidad2==ge) // cuando la intensidad es igual al grado electrico se dispara el triac
  {
   digitalWrite(ledB,HIGH); //62.5 ns
   digitalWrite(ledB,LOW);
  }
  if(intensidad3==ge) // cuando la intensidad es igual al grado electrico se dispara el triac
  {
   digitalWrite(ledC,HIGH); //62.5 ns
   digitalWrite(ledC,LOW);
  }
    if(intensidad4==ge) // cuando la intensidad es igual al grado electrico se dispara el triac
  {
   digitalWrite(ledD,HIGH); //62.5 ns
   digitalWrite(ledD,LOW);
  }
    if(intensidad5==ge) // cuando la intensidad es igual al grado electrico se dispara el triac
  {
   digitalWrite(ledE,HIGH); //62.5 ns
   digitalWrite(ledE,LOW);
  }
    if(intensidad6==ge) // cuando la intensidad es igual al grado eléctrico se dispara el triac
  {
   digitalWrite(ledF,HIGH); //62.5 ns
   digitalWrite(ledF,LOW);
  }
}

//Parte de samuel
void serialEvent()
{
	while (Serial.available()) 
	{
		// conseguir el nuevo byte:
		char inChar = (char)Serial.read(); // leer y pasar a tipo char
		if (inChar == '\n') // si llega al final del comando
		{
			stringComplete = true; // se marca true para finalizar
		}
		else
		{
			//Se sigue añadiendo carácteres a la cadena
			inputString += inChar;
			// El operador += suma el valor de su derecha a la variable o propiedad de su izquierda y asigna el resultado a la variable o propiedad de su izquierda
		}
	}
}
