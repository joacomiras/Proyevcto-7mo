#include <Servo.h>
#include <Wire.h>
#include <RTClib.h>
#include <TimerOne.h>

RTC_DS3231 rtc;
int dn = 0;
float lat = -34.6667;
float lon = -58.6667;
float husohor = -3;
float latm = (husohor * 15);

// Declaramos la variable para controlar el servo
Servo servoElevacion;
Servo servoAzimut;

volatile int contador = 0;
volatile int minutos = 0;

void contador_horas() {
  // contador++;
  // if (contador == 6) {
  //   contador = 0;
  //   noInterrupts();
  //   minutos++;
  // }
  // if (minutos == 6) {
  //   minutos = 0;
  //   noInterrupts();
  //   // debería ir una funcion de hacer lo del reloj
  // }
  // Serial.print(minutos);
  // Serial.println(" minutos");
  // Serial.print(contador);
  // Serial.println(" segundos");
  // // interrupts();
}

void setup() {
  Timer1.initialize(1000000);         // Dispara cada 1000 ms
  Timer1.attachInterrupt(contador_horas);
  Serial.begin(9600);
  if (!rtc.begin()) {       // si falla la inicializacion del modulo
    Serial.println("Modulo RTC no encontrado !");  // muestra mensaje de error
    while (1);         // bucle infinito que detiene ejecucion del programa
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));
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
  int dia = fecha.day();
  int mes = fecha.month();
  int hora = fecha.hour();
  //  Hay que usar millis e interrupciones así no se ejecuta a cada rato y el dia no se suma a cada rato
  
  // ### Cálculo del dia del mes ### //
  int diaMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  Serial.print(mes);
  Serial.println(" mes");
  for (int i = 0; i < mes; i++) {
    dn += diaMes[i];
  }
  dn += dia;
  Serial.print(dn);
  Serial.println(" dn");
  
  // ### Cálculos del sol ### //
  float decl = (23.45 * sin(PI / 180 * ((0.9863013699) * (dn - 81))));
  float r = (((dn - 1) * 0.9863013699) * (PI / 180));
  float edn = (13.752 * (0.075 + 1.868 * cos(r) - 32.077 * sin(r) - 14.615 * cos(2 * r) - 40.89 * sin(2 * r)));
  float tsv = ((hora * 240 * (latm - lon) + edn) / 3600);
  float anghor = (360 * ((tsv - 12) / 24));
  float elv = (asin((cos(PI / 180 * lat) * cos(PI / 180 * decl) * cos(PI / 180 * anghor) + (sin(PI / 180 * lat) * sin(PI / 180 * decl)))));
  float azimut = (acos((sin(elv) * sin(PI / 180 * lat) - sin(PI / 180 * decl)) / (cos(elv) * cos(PI / 180 * lat))));
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

  // servoElevacion.attach(5);
  // servoAzimut.attach(6);
  // servoElevacion.write(180);
  // servoAzimut.write(180);
  // servoElevacion.write((180 / PI) * elv);
  // servoAzimut.write((180 / PI) * azimut);
}

void loop() {
    // delay(1000);
    // Serial.println(contador);
    // if (contador == 60) {
    //     Serial.println("pene");
    //     contador = 0;
    //     minutos++;
    //     Serial.println(contador);
    //     Serial.println(minutos);
    // }
    // Por ahora no entiendo como hacer el contador
}
