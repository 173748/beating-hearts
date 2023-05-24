// передатчик
#include <AmperkaKB.h>
#include <nRF24L01.h> 
#include <RF24.h>
#include <SPI.h>
 
// создаём объект для работы с матричной клавиатурой
// указывая номера arduino подключенные к шлейфу клавиатуры
// начиная с первого вывода шлейфа
AmperkaKB KB(5, 4, 3, 2, A2, 8, 7, 6);

RF24 radio(10, 9);               // nRF24L01 (CE,CSN)

int sel = 0;
int num;
 
void setup()
{
  // открываем монитор Serial порта
  Serial.begin(9600);
  // указываем тип клавиатуры
  KB.begin(KB4x4);
  // второй необзятельный параметр:
  // время длительного зажатия кнопки
  // по умолчанию 2000, изменим на 5000 мс
  // KB.begin(KB4x4, 5000);
  // инициализация nRF24L01+
    radio.begin(); 
    // задаем канал для передачи данных (от 0 до 127)
    radio.setChannel(5); 
    // Скорость передачи данных 1Мбит/сек. 
    // Возможны: RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
    radio.setDataRate (RF24_1MBPS); 
    // Мощность передатчика -6dBm. 
    // Возможные можности: 
    // RF24_PA_MIN = -18dBm, RF24_PA_LOW = -12dBm, 
    // RF24_PA_HIGH = -6dBm, RF24_PA_MAX = 0dBm
    radio.setPALevel (RF24_PA_HIGH); 
    // Открываем трубу с идентификатором 0x1234567890 для передачи данных 
    // на одном канале можно открыть 6 разных труб 
    // они должны отличаться только последним байтом
    radio.openWritingPipe (0x1234567890LL);
}
 
void loop()
{ 
  if (millis() / 50 % 2){
  // считывание данных с клавиатуры
  KB.read();
  // опеределяем клик кнопки
  if (KB.justPressed()) {
    // печатаем номер кнопки и её символ в последовательный порт
    Serial.print("Key is press ");
    Serial.print(KB.getNum);
    Serial.print(" = \"");
    Serial.print(KB.getChar);
    Serial.println("\"");
    num = KB.getNum;
    sel = 0;
  }
  // определяем отжатие кнопки
  if (KB.justReleased()) {
    // печатаем номер кнопки и её символ в последовательный порт
    Serial.print("Key is release ");
    Serial.print(KB.getNum);
    Serial.print(" = \"");
    Serial.print(KB.getChar);
    Serial.println("\"");
  }
  // определяем зажатие кнопки на 3 секунды
  if (KB.isHold()) {
    // печатаем номер кнопки и её символ в последовательный порт
    sel = KB.getNum + 100;
    Serial.print(sel);
    Serial.print("Key on long press ");
    Serial.print(KB.getNum);
    Serial.print(" = \"");
    Serial.print(KB.getChar);
    Serial.println("\"");
    num = 0;
    //radio.write(&num, sizeof(num));
  }
  radio.write(&num, sizeof(num));
  radio.write(&sel, sizeof(sel));
}
}
