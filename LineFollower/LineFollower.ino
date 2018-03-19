//piny do sterowania silników
#define ASTBY 9
#define BSTBY 2

//piny do silników
#define AIN1 7
#define AIN2 8
#define BIN1 4
#define BIN2 3
#define APWM 6
#define BPWM 5

//piny do sterowania 
#define LED1 13
#define LED2 11

//piny od czujników
#define LEWY_SKRAJNY A0
#define LEWY A1
#define SRODKOWY A2
#define PRAWY A3
#define PRAWY_SKRAJNY A4

//pin od potencjometru
#define POT A5

//czas
unsigned long czas;
unsigned long czas_poprzedni = 0;


//do odbioru wartości z czujników
int tab_czujniki[5];
int tab_wagi[]={-2,-1,0,1,2};
int Index1;
int Index2;
int Index3;


//do PID
int Kp = 10;
int Docelowa = 0;
int Tp = 255;

// kierunek - czy utrzymać kurs
int kierunek; //0 - bez zmian, 1 - prawo, 2 - lewo


void sort(int a[], int size) 
{
    for(int i=0; i<(size-1); i++) 
    {
        for(int o=0; o<(size-(i+1)); o++) 
        {
                if(a[o] > a[o+1])   
                {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}

void checkIfTurn()
{
  int wynik=0;
  
  czas = millis();

  if((czas - czas_poprzedni) >= 5)
  {
    czas_poprzedni = czas;

    Serial.print(analogRead(LEWY_SKRAJNY));
    tab_czujniki[0] = analogRead(LEWY_SKRAJNY);
    Serial.print("    ");
    
    Serial.print(analogRead(LEWY));
    tab_czujniki[1] = analogRead(LEWY);
    Serial.print("    ");
    
    Serial.print(analogRead(SRODKOWY));
    tab_czujniki[2] = analogRead(SRODKOWY);
    Serial.print("    ");
    
    Serial.print(analogRead(PRAWY));
    tab_czujniki[3] = analogRead(PRAWY);
    Serial.print("    ");
    
    Serial.println(analogRead(PRAWY_SKRAJNY));
    tab_czujniki[4] = analogRead(PRAWY_SKRAJNY);

    
    sort(tab_czujniki, 5);
   
  }

}


void setup() 
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(LEWY_SKRAJNY, INPUT);
  pinMode(LEWY, INPUT);
  pinMode(SRODKOWY, INPUT);
  pinMode(PRAWY, INPUT);
  pinMode(PRAWY_SKRAJNY, INPUT);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(APWM, OUTPUT);
  pinMode(BPWM, OUTPUT);

  pinMode(ASTBY, OUTPUT);
  pinMode(BSTBY, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  digitalWrite(ASTBY, HIGH);
  digitalWrite(BSTBY, HIGH);

  analogWrite(APWM, 0);
  analogWrite(BPWM, 0);

  Serial.begin(9600);
}

void loop() 
{
  checkIfTurn();
  kierunek
  int Error = Docelowa + kierunek;
  int Zmiana = Kp * Error;

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(APWM, Tp+Zmiana);
  
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(BPWM, Tp-Zmiana);
  

}
