#include <Servo.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
int dn = 0;
float lat = -34.6667;
float lon = -58.6667;
float husohor = -3;
float ta = 0;
float latm = (husohor * 15);

// Declaramos la variable para controlar el servo
Servo servoElevacion;
Servo servoAzimut;
void setup() {
  Serial.begin(9600);
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));
  DateTime fecha = rtc.now(); // funcion que devuelve fecha y horario en formato // DateTime y asigna a variable fecha
  Serial.print(fecha.day());  // funcion que obtiene el dia de la fecha completa
  int dia = fecha.day();
  Serial.print("/");           // caracter barra como separador
  Serial.print(fecha.month()); // funcion que obtiene el mes de la fecha completa
  int mes = fecha.month();
  Serial.print("/");          // caracter barra como separador
  Serial.print(fecha.year()); // funcion que obtiene el año de la fecha completa
  Serial.print(" ");          // caracter espacio en blanco como separador
  Serial.print(fecha.hour()); // funcion que obtiene la hora de la fecha completa
  int hora = fecha.hour();
  Serial.print(":");              // caracter dos puntos como separador
  Serial.print(fecha.minute());   // funcion que obtiene los minutos de la fecha completa
  Serial.print(":");              // caracter dos puntos como separador
  Serial.println(fecha.second()); // funcion que obtiene los segundos de la fecha completa
  ta = hora;                      // QUE CARAJO ES ESTO?
                                  //  Hay que usar millis e interrupciones así no se ejecuta a cada rato y el dia no se suma a cada rato
  
  int diaMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  Serial.println(mes);
  for (int i = 0; i < mes; i++) {
    dn += diaMes[i];
  }
  dn += dia;
  Serial.println(dn);
  Serial.println("dn");
  
  float decl = (23.45 * sin(PI / 180 * ((0.9863013699) * (dn - 81))));
  float r = (((dn - 1) * 0.9863013699) * (PI / 180));
  float edn = (13.752 * (0.075 + 1.868 * cos(r) - 32.077 * sin(r) - 14.615 * cos(2 * r) - 40.89 * sin(2 * r)));
  float tsv = ((ta * 240 * (latm - lon) + edn) / 3600);
  float anghor = (360 * ((tsv - 12) / 24));
  float elv = (asin((cos(PI / 180 * lat) * cos(PI / 180 * decl) * cos(PI / 180 * anghor) + (sin(PI / 180 * lat) * sin(PI / 180 * decl)))));
  float azimut = (acos((sin(elv) * sin(PI / 180 * lat) - sin(PI / 180 * decl)) / (cos(elv) * cos(PI / 180 * lat))));
  float angInc = (sin(PI / 180 * decl) * (sin(PI / 180 * lat) * cos(elv) - cos(elv) * sin(elv) * cos(azimut) + cos(PI / 180 * decl) * cos(PI / 180 * lat) * cos(elv) * cos(PI / 180 * anghor) + cos(PI / 180 * decl) * sin(elv) * (sin(PI / 180 * lat) * cos(azimut) * cos(PI / 180 * anghor) + sin(azimut) * sin(PI / 180 * anghor))));
  /* delay(1000);  */ // demora de 1 segundo
  if (Serial.available()) {
    char inByte = Serial.read();
    if (inByte == '\n') {
      Serial.println(decl);
      Serial.println(tsv);
      Serial.println(r);
      Serial.println(edn);
      Serial.println(anghor);
      Serial.println(elv);
      Serial.println(azimut);
      Serial.println(angInc);
      Serial.println(dia);
      Serial.println(mes);
      Serial.println(hora);
      Serial.println(dn);
    }
  }
  Serial.println((180 / PI) * elv);
  Serial.println((180 / PI) * azimut);
  servoElevacion.attach(5);
  servoAzimut.attach(6);
  servoElevacion.write(180);
  servoAzimut.write(180);
  // servoElevacion.write((180 / PI) * elv);
  // servoAzimut.write((180 / PI) * azimut);
}

void loop()
{
}
