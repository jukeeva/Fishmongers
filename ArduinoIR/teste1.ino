const int  n_sensores = 10;
const int  n_linhas = 256;
int sensores_input[n_sensores] = {52, 50, 48, 46, 44, 42, 40, 38, 36, 34};
int(i);
int(j);
int(k);
bool (pack_passando);
int tempo_linha = 5; //ms
bool Matrix[n_linhas][n_sensores];
void exibe_matrix_inteira();
void cria_matrix();

void setup() {
  // put your setup code here, to run once:
  for (i = 0; i < n_sensores; i++)
  {
    pinMode(sensores_input[i], INPUT);
  }
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  pack_passando = false;
  for (j = 0; j < n_sensores; j++)
  {
    if (digitalRead(sensores_input[j]) == 0)
    {
      pack_passando = true;
    }
  }
  if (pack_passando == true)
  {
    cria_matrix();
    exibe_matrix_inteira();
  }
}
void exibe_matrix_inteira()
{
  Serial.println("[---]0123456789012345678");
  for (k = 0; k < n_linhas; k++)
  {
    if (k < 100)
    {
      if (k < 10)
      {
        Serial.print("[00");
        Serial.print(k);
        Serial.print("]");
      }
      else
      {
        Serial.print("[0");
        Serial.print(k);
        Serial.print("]");
      }
    }

    else
    {
      Serial.print("[");
      Serial.print(k);
      Serial.print("]");
    }

    //Serial.println();

    for (i = 0; i < n_sensores; i++)
    {
      Serial.print(Matrix[k][i]);
      //Serial.println(Matrix[k][i]);
    }
    Serial.println();
  }
  Serial.println(" ");
}

void cria_matrix()
{
  for (i = 0; i < n_linhas; i++)
  {
    for (j = 0; j < n_sensores; j++)
    {
      Matrix[i][j] = digitalRead(sensores_input[j]);
    }
    delay(tempo_linha);
  }
}
