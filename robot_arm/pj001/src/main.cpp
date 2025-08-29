#include <Arduino.h>
#include <Servo.h>

Servo Cintura;       // Declaramos variable para contolar Servo Cintura
Servo Hombro;        // Declaramos variable para contolar Servo Hombro
Servo Codo;          // Declaramos variable para contolar Servo Codo
Servo Pinzas;        // Declaramos variable para contolar Servo Pinzas

int GRADOS_CINTURA = 90;    //  Posición inicial servo cintura  = 90 Grados
int GRADOS_HOMBRO = 20;     //  Posición inicial servo hombro = 20 Grados 
int GRADOS_CODO = 50;       //  Posición inicial servo codo = 50 Grados
int GRADOS_PINZAS = 95;      //  Posición inicial servo pinzas = 0 Grados 


#define VRY_2 A0       //  Asignamos la entrada Y del Joystick 2 a la entrada análoga A0 
#define VRX_2 A1       //  Asignamos la entrada X del Joystick 2 a la entrada análoga A1 
#define VRY_1 A2       //  Asignamos la entrada Y del Joystick 1 a la entrada análoga A2 
#define VRX_1 A3       //  Asignamos la entrada X del Joystick 1 a la entrada análoga A3


void setup() {
  Serial.begin(9600);
  Cintura.attach(6);    // Para control servo Cintura asignamos el pin digital 6
  Hombro.attach(9);     // Para control servo Hombro asignamos el pin digital 9
  Codo.attach(10);      // Para control servo Codo asignamos el pin digital 10
  Pinzas.attach(5);    // Para control servo Pinzas asignamos el pin digital 11

  pinMode(VRY_1, INPUT);    // Declaramos pin análogo A2 como entrada
  pinMode(VRX_1, INPUT);    // Declaramos pin análogo A3 como entrada
  pinMode(VRY_2, INPUT);    // Declaramos pin análogo A0 como entrada
  pinMode(VRX_2, INPUT);    // Declaramos pin análogo A1 como entrada
}

void loop() {
  int LVRY_1 = analogRead(VRY_1);    // Variable para almacenar lectura Y Joystick 1
  int LVRX_1 = analogRead(VRX_1);    // Variable para almacenar lectura X Joystick 1
  int LVRY_2 = analogRead(VRY_2);    // Variable para almacenar lectura Y Joystick 2
  int LVRX_2 = analogRead(VRX_2);    // Variable para almacenar lectura X Joystick 2



  if (LVRY_1 < 340) GRADOS_CINTURA += 3;           // Si lectura Y Joystick 1 es mayor a 680 entonces Aumente 1 grado Servo Cintura
  else if (LVRY_1 > 680) GRADOS_CINTURA -= 3;                // Si lectura Y Joystick 1 es menor a 340 entonces Disminuya 1 grado Servo Cintura
  GRADOS_CINTURA = min(190, max(0, GRADOS_CINTURA));    // El Servo se movera desde un mínimo de 0 grados hasta máximo 175 grados 

  if (LVRX_1 < 340) GRADOS_HOMBRO += 3;
  else if (LVRX_1 > 680) GRADOS_HOMBRO -= 3;
  GRADOS_HOMBRO = min(110, max(10, GRADOS_HOMBRO));

  if (LVRX_2 < 340) GRADOS_CODO -= 4;
  else if (LVRX_2 > 680) GRADOS_CODO += 4;
  GRADOS_CODO = min(80, max(0, GRADOS_CODO));

  if (LVRY_2 < 340) {
    GRADOS_PINZAS -= 6;
    //Serial.println("Abre Pinzas: " + String(LVRY_2));
    //Serial.println("Estado del servo A: " + String(GRADOS_PINZAS));
  } 
  else if (LVRY_2 > 680) {
    GRADOS_PINZAS += 6;
    //Serial.println("Cierra Pinzas: " + String(LVRY_2));
    //Serial.println("Estado del servo B: " + String(GRADOS_PINZAS));
  }

  GRADOS_PINZAS = min(143, max(50, GRADOS_PINZAS));

  Serial.print("Pinza (pot-deg): " + String(LVRY_2));
  Serial.print(" " + String(GRADOS_PINZAS));

  Serial.print("\t || Codo: " + String(LVRX_2));
  Serial.print(" " + String(GRADOS_CODO));

  Serial.print("\t || Hombro: " + String(LVRX_1));
  Serial.print(" " + String(GRADOS_HOMBRO));

  Serial.print("\t || Cintura: " + String(LVRY_1));
  Serial.println(" " + String(GRADOS_CINTURA));


  Cintura.write(GRADOS_CINTURA);     //  Al Servo Cintura asignamos el valor de la variable GRADOS_CINTURA
  Hombro.write(GRADOS_HOMBRO);       //  Al Servo Hombro asignamos el valor de la variable GRADOS_HOMBRO
  Codo.write(GRADOS_CODO);           //  Al Servo Codo asignamos el valor de la variable GRADOS_CODO
  Pinzas.write(GRADOS_PINZAS);       //  Al Servo Pinzas asignamos el valor de la variable GRADOS_PINZAS

  delay(40);                         //  Retardo de 40 milisegundos 
}