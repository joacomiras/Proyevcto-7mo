#include <Servo.h> /* Inclusión de la libreria para el control de los servomotores */
#include <Wire.h> /* Esta librería le permite comunicarse al Arduino con dispositivos I2C */
#include <RTClib.h> /* Inclusión de la libreria para poder utilizar el módulo RTC */
/* Creo que habría que borrar la librería TimerOne */

RTC_DS3231 rtc; /* Se inicializa el objeto para el modelo del RTC */

float lat = -34.6667; /* Latitud de Ituzaingó */
float lon = -58.6667; /* Longitud de Ituzaingó */
float husohor = -3; /* Zona horaria de Argentina */
float latm = (husohor * 15); /* La longitud del meridiano central del huso horario donde  se  encuentra  el  observador */
int contadorDeMinutos = 0; /* Contador de minutos del RTC (despues va a ser contador de horas , es provisorio el de minutos) */

// Declaramos las variables para controlar los servomotores
Servo servoElevacion;
Servo servoAzimut;

void setup() {
  Serial.begin(9600);
  if (!rtc.begin()) {  /* Si falla la inicialización del módulo */
    Serial.println("Modulo RTC no encontrado !");  /* Muestra mensaje de error */
    while (1);  /* Bucle infinito que detiene ejecucion del programa */
  }
  rtc.adjust(DateTime(__DATE__, __TIME__)); /* El RTC setea el horario , solo necesita que se ejecute una sola vez */
}

void loop() {
  DateTime fecha = rtc.now(); /* Función que devuelve fecha y horario en formato, DateTime asigna a variable fecha */
  Serial.print(fecha.day());  /* Función que obtiene el dia de la fecha completa */
  Serial.print("/");
  Serial.print(fecha.month()); /* Función que obtiene el mes de la fecha completa */
  Serial.print("/");
  Serial.print(fecha.year()); /* Función que obtiene el año de la fecha completa */
  Serial.print(" ");
  Serial.print(fecha.hour()); /* Función que obtiene la hora de la fecha completa */
  Serial.print(":");
  Serial.print(fecha.minute());  /*  Función que obtiene los minutos de la fecha completa */
  Serial.print(":");
  Serial.println(fecha.second()); /* Función que obtiene los segundos de la fecha completa */
  int dia = fecha.day();
  int mes = fecha.month();
  int hora = fecha.hour();
  int minutos = fecha.minute();
  delay(1000); /* Delay para ver las cosas más tranki en el monitor */
  
  // ### CÁLCULO DEL DÍA DEL MES ### //
  int dn = 0; /* Dias que pasaron desde el dia 0 del año */
  int diaMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; /* Array con la cantidad de dias que tiene cada mes */
  Serial.print(mes);
  Serial.println(" mes");
  for (int i = 0; i < mes; i++) {
    dn += diaMes[i];
  }
  dn += dia;
  Serial.print(dn);
  Serial.println(" dn");
  // ############################## //

  // ### DIVISOR RESISTIVO ### //
  float medir = analogRead(A9)*(5.0/1024.0);
  Serial.println(medir);
  // ######################## //

  // Si el divisor resistivo da una tensión menor a 15 volts , hace otra vez los cálculos del sol , o si nunca hizo los cálculos los hace (el inicio del programa)
  if( medir < 1.5 || contadorDeMinutos == 0) {
    // ### CÁLCULOS DEL SOL ### //
    float decl = (23.45 * sin(PI / 180 * ((0.9863013699) * (dn - 81)))); /* Cálculo de la declinación del sol */
    float r = (((dn - 1) * 0.9863013699) * (PI / 180)); /* Variable para el calculo de edn */
    float edn = (13.752 * (0.075 + 1.868 * cos(r) - 32.077 * sin(r) - 14.615 * cos(2 * r) - 40.89 * sin(2 * r))); /* Es la ecuación del tiempo expresada en segundos, la  cual  determina  la  diferencia  entre  el  tiempo  solar medio y el tiempo solar aparente, y depende del día del año */
    float tsv = ((hora * 240 * (latm - lon) + edn) / 3600); /* Tiempo solar verdadero,  hora definida por la posición del Sol en lugar de la hora reloj */
    float anghor = (360 * ((tsv - 12) / 24)); /* Es el ángulo horario, representa  la  posicion  relativa  del  Sol  a la  vertical  del  observador  en  el  plano  de  su  trayectoria  para un día  dado */
    float elv = (asin((cos(PI / 180 * lat) * cos(PI / 180 * decl) * cos(PI / 180 * anghor) + (sin(PI / 180 * lat) * sin(PI / 180 * decl))))); /* Cálculo de la elevación del sol */
    float azimut = (acos((sin(elv) * sin(PI / 180 * lat) - sin(PI / 180 * decl)) / (cos(elv) * cos(PI / 180 * lat)))); /* Cálculo del azimut del sol */
    if (anghor > 0) azimut = 360 - azimut; /* Si el ángulo horario es negativo , el valor del azimut es absoluto (no importa si es positivo o negativo). Si el ángulo horario es positivo , se debe hacer 360 - lo que da el azimut */
    float angInc = (sin(PI / 180 * decl) * (sin(PI / 180 * lat) * cos(elv) - cos(elv) * sin(elv) * cos(azimut) + cos(PI / 180 * decl) * cos(PI / 180 * lat) * cos(elv) * cos(PI / 180 * anghor) + cos(PI / 180 * decl) * sin(elv) * (sin(PI / 180 * lat) * cos(azimut) * cos(PI / 180 * anghor) + sin(azimut) * sin(PI / 180 * anghor)))); /* Ángulo de inclinación del sol */
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
    // ######################## //

    // ### Movimiento de servos ### //
    // servoElevacion.attach(5);
    // servoAzimut.attach(10);
    // servoElevacion.write((180 / PI) * elv);
    // servoAzimut.write((180 / PI) * azimut);
    // ######################## //

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