#include <Servo.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
float lat = -34.6667;
float lon = -58.6667;
float husohor = -3;
float latm = (husohor * 15);
int contadorDeMinutos = 0;

// Declaramos la variable para controlar el servo
Servo servoElevacion;
Servo servoAzimut;

void setup() {
  Serial.begin(9600);
  if (!rtc.begin()) {       // si falla la inicializacion del modulo
    Serial.println("Modulo RTC no encontrado !");  // muestra mensaje de error
    while (1);         // bucle infinito que detiene ejecucion del programa
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));
}

void loop() {
  DateTime fecha = rtc.now(); // funcion que devuelve fecha y horario en formato // DateTime y asigna a variable fecha
  Serial.print(fecha.day());  // funcion que obtiene el dia de la fecha completa
  Serial.print("/");           // caracter barra como separador
  Serial.print(fecha.month()); // funcion que obtiene el mes de la fecha completa
  Serial.print("/");          // caracter barra como separador
  Serial.print(fecha.year()); // funcion que obtiene el año de la fecha completa
  Serial.print(" ");          // caracter espacio en blanco como separador
  Serial.print(fecha.hour()); // funcion que obtiene la hora de la fecha completa
  Serial.print(":");              // caracter dos puntos como separador
  Serial.print(fecha.minute());   // funcion que obtiene los minutos de la fecha completa
  Serial.print(":");              // caracter dos puntos como separador
  Serial.println(fecha.second()); // funcion que obtiene los segundos de la fecha completa
  delay(1000); // delay para verlo más tranqui en el monitor
  int dia = fecha.day();
  int mes = fecha.month();
  int hora = fecha.hour();
  int minutos = fecha.minute();
  
  // ### Cálculo del dia del mes ### //
  int dn = 0;
  int diaMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  Serial.print(mes);
  Serial.println(" mes");
  for (int i = 0; i < mes; i++) {
    dn += diaMes[i];
  }
  dn += dia;
  Serial.print(dn);
  Serial.println(" dn");

  // ### Divisor resistivo ### //
  float medir=analogRead(A9)*(5.0/1024.0);
  Serial.println(medir);

// Esto despues hay que hacerlo con horas y no con minutos
if( minutos!=contadorDeMinutos ){
  // ### Cálculos del sol ### //
  float decl = (23.45 * sin(PI / 180 * ((0.9863013699) * (dn - 81))));
  float r = (((dn - 1) * 0.9863013699) * (PI / 180));
  float edn = (13.752 * (0.075 + 1.868 * cos(r) - 32.077 * sin(r) - 14.615 * cos(2 * r) - 40.89 * sin(2 * r)));
  float tsv = ((hora * 240 * (latm - lon) + edn) / 3600);
  float anghor = (360 * ((tsv - 12) / 24));
  float elv = (asin((cos(PI / 180 * lat) * cos(PI / 180 * decl) * cos(PI / 180 * anghor) + (sin(PI / 180 * lat) * sin(PI / 180 * decl)))));
  // Si el ángulo horario es negativo , el valor del azimut es absoluto (no importa si es positivo o negativo). Si el ángulo horario es positivo , se debe hacer 360 - lo que da el azimut.
  float azimut = (acos((sin(elv) * sin(PI / 180 * lat) - sin(PI / 180 * decl)) / (cos(elv) * cos(PI / 180 * lat))));
  if (anghor > 0) azimut = 360 - azimut;
  float angInc = (sin(PI / 180 * decl) * (sin(PI / 180 * lat) * cos(elv) - cos(elv) * sin(elv) * cos(azimut) + cos(PI / 180 * decl) * cos(PI / 180 * lat) * cos(elv) * cos(PI / 180 * anghor) + cos(PI / 180 * decl) * sin(elv) * (sin(PI / 180 * lat) * cos(azimut) * cos(PI / 180 * anghor) + sin(azimut) * sin(PI / 180 * anghor))));
  Serial.print(decl);
  Serial.println(" decl");
  Serial.print(tsv);
  Serial.println(" tsv");
  Serial.print(r);
  Serial.println(" r");
  Serial.print(edn);
  Serial.println(" edn");
  Serial.print(anghor);
  Serial.println(" anghor");
  Serial.print(elv);
  Serial.println(" elv");
  Serial.print(azimut);
  Serial.println(" azimut");
  Serial.print(angInc);
  Serial.println(" angInc");
  Serial.print((180 / PI) * elv);
  Serial.println("° elevacion");
  Serial.print((180 / PI) * azimut);
  Serial.println("° azimut");

  // ### Movimiento de servos ### //
  //servoElevacion.attach(5);
  //servoAzimut.attach(10);
  // servoElevacion.write(180);
  // servoAzimut.write(180);
  //servoElevacion.write((180 / PI) * elv);
  //servoAzimut.write((180 / PI) * azimut);
  contadorDeMinutos = minutos;
}
// servoAzimut.attach(10);
// servoAzimut.write(90);
// delay(1000);
// servoAzimut.write(0);
// delay(1000);
// servoAzimut.write(10);
// delay(1000);
// servoAzimut.write(180);
// delay(1000);
// servoAzimut.write(50);
// delay(1000);
}

