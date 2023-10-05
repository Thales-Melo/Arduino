// Acelerometro ADXL345

// FUNCIONAMENTO DO PROGRAMA
// AS OPCOES SAO MOSTRADAS NO SERIAL MONITOR COM AS RESPECTIVAS FORMAS DE ESCOLHER (direcoes: NORTE, SUL, LESTE, OESTE)

#include <Wire.h>

#define Register_ID 0
#define Register_2D 0x2D
#define Register_X0 0x32
#define Register_X1 0x33
#define Register_Y0 0x34
#define Register_Y1 0x35
#define Register_Z0 0x36
#define Register_Z1 0x37

// Endereco I2C do sensor : 83 em decimal ou 0x53
int ADXAddress = 0x53;  // the default 7-bit slave address

double oldX=0, oldY=1, oldZ=0;
double displacementX=0, displacementY=0, displacementZ=0;
int enable=1, startMeasuring=0;
int startingTime=0, currentTime=0;
char palavra[100];
char alfabeto[26] = "abcdefghijklmnopqrstuvwxyz";
int pI=0, pF=25;
int first_time=1;

int X0,X1,X_out;
int Y0,Y1,Y_out;
int Z1,Z0,Z_out;
double Xg,Yg,Zg;

enum POS_ALFA {
  A=0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};

void setup()
/*
No setup, inicializamos a bilbioteca wire com a função begin e a transmissao serial;

Nao sei por que tem esse delay;

Entao habilitamos a transmissao para o dispositivo especificado pelo endereço ADXAddress, do acelerometro 
(existem outros dispositivos na linha), e permitimos o inicio do processo de leitura, provavelmente por esse write(8) no registrador apontado;
*/
{
  Wire.begin();                
  Serial.begin(9600);    
  delay(100);
  // enable to measute g data
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_2D);
  Wire.write(8);                //measuring enable
  Wire.endTransmission();     // stop transmitting
}

void loop()
/*
O loop atua em 3 instancias, para x, y e z;

Em cada uma ele estabelece a conexão entre o arduino e o acelerometro;

Como a comunicação I2C retorna sempre 8 bits e a leitura do GY-80 16 bits, ela deve ser feita em duas etapas,
retornando a parte menos significativa do numero e depois a mais significativa, e então juntando-as e 
formando o numero original em @_out. Esse processo eh feito acessando diretamente os registradores esquivalentes;

Depois disso é printada a parte mais siginificativa do numero, ou os ultimos 8 bits, ao dividi-lo por 256 (2^8);

Esse processo eh repetido a cada 0.2 segundos;
*/
{
  //--------------X
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_X0);
  Wire.write(Register_X1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    X0 = Wire.read();
    X1 = Wire.read(); 
    X1=X1<<8;
    X_out=X0+X1;   
  }

  //------------------Y
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_Y0);
  Wire.write(Register_Y1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    Y0 = Wire.read();
    Y1 = Wire.read(); 
    Y1=Y1<<8;
    Y_out=Y0+Y1;
  }
  //------------------Z
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_Z0);
  Wire.write(Register_Z1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    Z0 = Wire.read();
    Z1 = Wire.read(); 
    Z1=Z1<<8;
    Z_out=Z0+Z1;
  }
  //
  Xg=X_out/256.0;
  Yg=Y_out/256.0;
  Zg=Z_out/256.0;

  if (first_time) {
    printOpcoes(pI, pF);
    delay(2000);
    first_time=0;
  }

  //Aqui avaliamos se devemos começar a contagem pra leitura (se houve o movimento inicial - trigger)
  if(enable && (Zg) >= -0.4){
    startMeasuring = 1;
    enable = 0;
    startingTime = millis();

    Serial.println("Capturando movimento");
  }

  //Entao esperamos o tempo total reservado para a realização do movimento
  if(startMeasuring){

//debug
    // Serial.print("DIF X: ");
    // Serial.print((Xg-oldX));
    // Serial.print("  DIF y: ");
    // Serial.print((Yg-oldY));
    // Serial.print("  DIF z: ");
    // Serial.println((Zg-oldZ));

    currentTime = millis();

    // Diferença do resultado das diferenças (10 leituras)
    displacementY += (Yg - oldY);
    displacementX += (Xg - oldX);
    displacementZ += (Zg - oldZ);

    //Ao completa-lo, imprimimos a resposta ao movimento na tela a partir da media dos valores obtidos no tempo de leitura (para evitar descontinuidade)
    if((currentTime - startingTime) >= 1000){

      // NORTE
      // Intervalo (A - G)
        // A-B; C-D; E-F; G;
      if (Yg >= 0.8) {
        Serial.println ("NORTE");
        if (pF == Z) {
          pF = G;
        }
        else if (pF == G) {
          pF = B;
        }
        else if (pF == B) {
          pF = A;
        }
        else if (pF == D) {
          pF = C;
        }
        else if (pF == F) {
          pF = E;
        }
        else if (pF == N) {
          pF = I;
        }
        else if (pF == I) {
          pF = H;
        }

        else if (pF == T) {
          pF = P;
        }
        else if (pF == P) {
          pF = O;
        }

        else if (pF == Z && pI == U) {
          pF = V;
        }
        else if (pF == V) {
          pF = U;
        }

        else if (pF == K) {
          pF = J;
        }
        
        else if (pF == M) {
          pF = L;
        }

        else if (pF == R) {
          pF = Q;
        }

        else if (pF == X) {
          pF = W;
        }
        else if (pF == N && pI == H) {
          pF = I;
          pI = H;
        }
        printOpcoes(pI, pF);

      }

      // SUL
      // Intervalo (H - N)
        // H-I; J-K; L-M; N;
      else if (Yg <= -0.8) {
        Serial.println ("SUL");
        if (pF == Z) {
          pI = H;
          pF = N;
        }
        else if (pF == N) {
          pI = J;
          pF = K;
        }
        else if (pF == K) {
          pI = K;
        }

        else if (pF == B) {
          pI = B;
        }

        else if (pF == D) {
          pI = D;
        }

        else if (pF == F) {
          pI = F;
        }

        else if (pF == I) {
          pI = I;
        }

        else if (pF == K) {
          pI = K;
        }

        else if (pF == M) {
          pI = M;
        }

        else if (pF == P) {
          pI = P;
        }

        else if (pF == R) {
          pI = R;
        }

        else if (pF == V) {
          pI = V;
        }

        else if (pF == X) {
          pI = X;
        }
        else if (pF == G) {
          pI = C;
          pF = D;
        }
        else if (pF == T) {
          pF = R;
          pI = Q;
        }
        else if (pF == Z && pI == U) {
          pF = X;
          pI = W;
        }
        printOpcoes(pI, pF);
      }

      // LESTE
      // Intervalo (O - T)
        // O-P; Q-R; S; T;
      else if (Xg <= -0.8) {
        Serial.println ("LESTE");
        if (pF == Z) {
          pI = O;
          pF = T;
        }
        else if (pF == T) {
          pI = S;
          pF = S;
        }

        else if (pF == G) {
          pI = E;
          pF = F;
        }

        else if (pF == N) {
          pI = L;
          pF = M;
        }

        else if (pI == U && pF == Z) {
          pI = Y;
          pF = Y;
        }
        else if (pI == A && pF == G) {
          pI = E;
          pF = F;
        }
        else if (pI == H && pF == N) {
          pI = L;
          pF = M;
        }
        printOpcoes(pI, pF); 
      }

      // OESTE
      // Intervalo (U - Z)
        // U-V; W-X; Y; Z;
      else if (Xg >= 0.8) {
        Serial.println ("OESTE");
        if (pF == Z && pI != U) {
          pI = U;
        }
        else if (pI == U && pF == Z) {
          pI = Z;
          pF = Z;
        }

        else if (pF == T) {
          pI = T;
        }

        else if (pF == N) {
          pI = N;
        }

        else if (pF == G) {
          pI = G;
        }
        
        printOpcoes(pI, pF);
      }

      if (pI == pF) {
        Serial.print("Letra: ");
        Serial.println(alfabeto[pI]);
        sprintf (palavra, "%s%c", palavra, alfabeto[pI]);
        Serial.print("Palavra: ");
        Serial.println(palavra);
        pI = A;
        pF = Z;
        printOpcoes(pI, pF);
      }

      startMeasuring = 0;
      enable = 1;
      displacementX = 0;
      displacementY = 0;


      delay(2000);
    }

  }

  oldX = Xg;
  oldY = Yg;
  oldZ = Zg;

  // Serial.print("X= ");
  // Serial.print(Xg);
  // Serial.print("       ");
  // Serial.print("Y= ");
  // Serial.print(Yg);
  // Serial.print("       ");
  // Serial.print("Z= ");
  // Serial.print(Zg);
  // Serial.println("  ");
  
  delay(50);
}

void printOpcoes (int pI, int pF) {
  if (pI == A && pF == Z) {
    Serial.println ("A-G (NORTE)");
    Serial.println ("H-N (SUL)");
    Serial.println ("O-T (LESTE)");
    Serial.println ("U-Z (OESTE)");
  }
  else if (pI == A && pF == G) {
    Serial.println ("A-B (NORTE)");
    Serial.println ("C-D (SUL)");
    Serial.println ("E-F (LESTE)");
    Serial.println ("G (OESTE)");
  }
  else if (pI == A && pF == B) {
    Serial.println ("A (NORTE)");
    Serial.println ("B (SUL)");
  }
  else if (pI == C && pF == D) {
    Serial.println ("C (NORTE)");
    Serial.println ("D (SUL)");
  }
  else if (pI == E && pF == F) {
    Serial.println ("E (NORTE)");
    Serial.println ("F (SUL)");
  }
  else if (pI == H && pF == N) {
    Serial.println ("H-I (NORTE)");
    Serial.println ("J-K (SUL)");
    Serial.println ("L-M (LESTE)");
    Serial.println ("N (OESTE)");
  }
  else if (pI == H && pF == I) {
    Serial.println ("H (NORTE)");
    Serial.println ("I (SUL)");
  }
  else if (pI == J && pF == K) {
    Serial.println ("J (NORTE)");
    Serial.println ("K (SUL)");
  }
  else if (pI == L && pF == M) {
    Serial.println ("L (NORTE)");
    Serial.println ("M (SUL)");
  }
  else if (pI == O && pF == T) {
    Serial.println ("O-P (NORTE)");
    Serial.println ("Q-R (SUL)");
    Serial.println ("S (LESTE)");
    Serial.println ("T (OESTE)");
  } 
  else if (pI == O && pF == P) {
    Serial.println ("O (NORTE)");
    Serial.println ("P (SUL)");
  }
  else if (pI == Q && pF == R) {
    Serial.println ("Q (NORTE)");
    Serial.println ("R (SUL)");
  }
  else if (pI == U && pF == Z) {
    Serial.println ("U-V (NORTE)");
    Serial.println ("W-X (SUL)");
    Serial.println ("Y (LESTE)");
    Serial.println ("Z (OESTE)");
  } 
  else if (pI == U && pF == V) {
    Serial.println ("U (NORTE)");
    Serial.println ("V (SUL)");
  }
  else if (pI == W && pF == X) {
    Serial.println ("W (NORTE)");
    Serial.println ("X (SUL)");
  }
}




