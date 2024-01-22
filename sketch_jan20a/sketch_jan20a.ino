#include <SoftwareSerial.h>


SoftwareSerial BT(10,11);


// Motor 1
int ENA = 3;
int IN1 = 9;
int IN2 = 8;
// Motor 2
int ENB = 5;
int IN3 = 7;
int IN4 = 6;

//=================================
// Definir la estructura para almacenar comandos con un nombre
struct ComandoAlmacenado {
  char comando;
  int tiempo;
};
//=================================

// Definir un array para almacenar los últimos comandos (se pueden ajustar según sea necesario)
ComandoAlmacenado AlmacenCommand[2];
float cantidadComandos = 0;
//=================================




void setup() {
 Serial.begin(9600);
 BT.begin(9600);
 pinMode (ENA, OUTPUT);
 pinMode (ENB, OUTPUT);
 pinMode (IN1, OUTPUT);
 pinMode (IN2, OUTPUT);
 pinMode (IN3, OUTPUT);
 pinMode (IN4, OUTPUT);  
}

String command;

char ant ="";
int antT=0;
//=======================================================

void loop() {
  if (BT.available()) {
    command = BT.readString(); // Lee el comando como una cadena
    float tiempo = 0.0;
    char primeraLetra = obtenerPrimeraLetra(command);
        Serial.println(primeraLetra); // Muestra solo la primera letra en el monitor serial
    int time =-1;
    if(numberWordExists(command)){
      tiempo=getNumberWord(command);
      time = static_cast<int>(tiempo*1000);
      
      Serial.println(time); 

      executeCommand(primeraLetra,time);
    }
    else{
       executeCommand(primeraLetra);
    }
    ant=primeraLetra;
    antT=time;
    
    Serial.print(ant); 
    Serial.println(antT); 

  }

  if (Serial.available()) {
    BT.write(Serial.read()); // Lee y envía caracteres al dispositivo Bluetooth
  }
}
//=======================================================
char obtenerPrimeraLetra(String comando) {

    char primeraLetra = comando.charAt(0); // Obtén el primer carácter del String
    return toLowerCase(primeraLetra); 
}


bool numberWordExists(String cadena) {
  for (char c : cadena) {
    if (isDigit(c)) {
      return true;
    }
  }
  return false;
}

// Función para obtener el primer número encontrado en la cadena como un entero
float getNumberWord(String cadena) {
  String numeroStr = "";
  bool puntoDecimalEncontrado = false;

  for (char c : cadena) {
    if (isDigit(c) || c == '.') {
      // Si es un dígito o un punto decimal, lo agregamos a la cadena
      numeroStr += c;

      if (c == '.') {
        puntoDecimalEncontrado = true;
      }
    } else if (numeroStr.length() > 0) {
      // Si ya hemos encontrado dígitos y encontramos otro tipo de caracter, terminamos la búsqueda
      break;
    }
  }

  if (numeroStr.length() > 0 && puntoDecimalEncontrado) {
    // Si se encontró al menos un dígito y se encontró un punto decimal, convertimos la cadena a número decimal
    return numeroStr.toFloat();
  } else if (numeroStr.length() > 0) {
    // Si se encontró al menos un dígito pero no se encontró un punto decimal, convertimos la cadena a número entero
    return numeroStr.toInt();
  } else {
    // Si no se encontraron dígitos, devolvemos un valor indicativo (puedes cambiar esto según tus necesidades)
    return -1.0;
  }
}

//=======================================================

void Adelante(){
 //Dirección motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, HIGH);
 analogWrite (ENA, 255); //Velocidad motor A
 //Dirección motor B
 digitalWrite (IN3, HIGH);
 digitalWrite (IN4, LOW);
 analogWrite (ENB, 255); //Velocidad motor B
}

void Derecha(){
 //Dirección motor A
 digitalWrite (IN1, HIGH);
 digitalWrite (IN2, LOW);
 analogWrite (ENA, 225); //Velocidad motor A
 //Dirección motor B
 digitalWrite (IN3, HIGH);
 digitalWrite (IN4, LOW);
 analogWrite (ENB, 255); //Velocidad motor A
}
void Izquierda(){
 //Dirección motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, HIGH);
 analogWrite (ENA, 255); //Velocidad motor A
 //Dirección motor B
 digitalWrite (IN3, LOW);
 digitalWrite (IN4, HIGH);
 analogWrite (ENB, 255); //Velocidad motor A
}
void Atras(){
 //Dirección motor A
 digitalWrite (IN1, HIGH);
 digitalWrite (IN2, LOW);
 analogWrite (ENA, 255); //Velocidad motor A
 //Dirección motor B
 digitalWrite (IN3, LOW);
 digitalWrite (IN4, HIGH);
 analogWrite (ENB,255); //Velocidad motor B
}
void Parar(){
 //Dirección motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, LOW);
 analogWrite (ENA, 0); //Velocidad motor A
 //Dirección motor B
 digitalWrite (IN3, LOW);
 digitalWrite (IN4, LOW);
 analogWrite (ENB, 0); //Velocidad motor A
} 


//================================================================
// Función para agregar un comando al almacenamiento
void agregarComandoAlmacenado( char comando, int tiempo) {
  for (int i = 1; i >= 0; --i) {
    AlmacenCommand[i + 1] = AlmacenCommand[i];
  }

  // Agregar el nuevo comando en la posición 0
  AlmacenCommand[0] = {comando, tiempo};
  cantidadComandos=cantidadComandos+1;


  Serial.println (cantidadComandos); 

}




// Función para ejecutar el último comando almacenado
void ejecutarUltimoComando() {
  if (cantidadComandos > 0) {
    ComandoAlmacenado ultimoComando = AlmacenCommand[0];
    if (ultimoComando.tiempo==-1){
     executeCommand(ultimoComando.comando);
    }
    else{
     executeCommand(ultimoComando.comando, ultimoComando.tiempo);
    }
  }
}

void ejecutar2UltimoComando() {
  if (cantidadComandos > 1) {
    ComandoAlmacenado ultimoComando = AlmacenCommand[0];
    ComandoAlmacenado AultimoComando = AlmacenCommand[1];

  if (ultimoComando.tiempo==-1 && AultimoComando.tiempo==-1 ){

     executeCommand(ultimoComando.comando);
     executeCommand(AultimoComando.comando);

  }else if(ultimoComando.tiempo!= -1 && AultimoComando.tiempo==-1 ){

     executeCommand(ultimoComando.comando, ultimoComando.tiempo);
     executeCommand(AultimoComando.comando);

  }else if(ultimoComando.tiempo==-1 && AultimoComando.tiempo!=-1 ){

     executeCommand(ultimoComando.comando);
     executeCommand(AultimoComando.comando, AultimoComando.tiempo);

  }
  else{

     executeCommand(ultimoComando.comando, ultimoComando.tiempo);
     executeCommand(AultimoComando.comando, AultimoComando.tiempo);

    }
  }
}

//===================================================================
void executeCommand(char dato,int time) {
  switch (dato) {
         case 'a':
            Adelante();
            delay(time);
            Parar(); // Detiene el movimiento directamente sin realizar ninguna acción

            break;
        case 'r':
            Atras();
            delay(time);
            Parar(); // Detiene el movimiento directamente sin realizar ninguna acción
            break;
        case 'd':
            Derecha();
            delay(time);
            Parar(); // Detiene el movimiento directamente sin realizar ninguna acción
            break;
        case 'i':

            Izquierda();
            delay(time);
            Parar(); // Detiene el movimiento directamente sin realizar ninguna acción
            break;

        case 'p':
            delay(time);
            Parar(); // Detiene el movimiento directamente sin realizar ninguna acción
            break;

  }
}

void executeCommand(char dato) {
    switch(dato){
        case 'a':
            Adelante();
            break;
        case 'r':
            Atras();
            break;
        case 'd':
            Derecha();
            break;
        case 'i':
            Izquierda();
            break;
        case 'p':
            Parar(); // Detiene el movimiento directamente sin realizar ninguna acción
            break;

        case 'g':
          agregarComandoAlmacenado( ant,antT) ;
        break;

        case 'e':
          ejecutarUltimoComando();
        break;

        case 'o':
          ejecutar2UltimoComando();
        break;


    }
}

