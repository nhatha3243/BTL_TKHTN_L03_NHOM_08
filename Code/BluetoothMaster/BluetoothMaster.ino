#include <EasyTransfer.h> 
#include <Adafruit_ST7735.h> 
#include <Adafruit_GFX.h> 

#define TFT_CS     10
#define TFT_RST    8                      
#define TFT_DC     9
#define TFT_SCLK   13   
#define TFT_MOSI   11 

char temperatureChar[10];
char humidityChar[10];

struct WEATHER_DATA_STRUCTURE
{
  float temperature ;
  float humidity ;
};

struct ACKNOWLEDGE
{
  boolean received = false;
};

WEATHER_DATA_STRUCTURE data;
ACKNOWLEDGE acknowledge;

EasyTransfer ETin, ETout; 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void setup() {
  pinMode(5, OUTPUT);
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK); 
  printUI();
  ETin.begin(details(data), &Serial);
  ETout.begin(details(acknowledge), &Serial);
}

void loop() {

  if(ETin.receiveData()){

    String temperatureString = String(data.temperature,1);
    temperatureString.toCharArray(temperatureChar,10);
    tft.fillRect(10,26,80,36,ST7735_BLACK);
    printText(temperatureChar, ST7735_WHITE,10,26,3);

    String humidityString = String(data.humidity,1);
    humidityString.toCharArray(humidityChar,10);
    tft.fillRect(10,101,80,106,ST7735_BLACK);
    printText(humidityChar, ST7735_WHITE,10,101,3);
    acknowledge.received = 1;
    ETout.sendData();
    } 
    delay(200);
    acknowledge.received = 0;
}

void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}

void printUI()
{
    printText("TEMPERATURE", ST7735_WHITE,30,5,1);  // Temperature Static Text
    printText("o", ST7735_WHITE,90,19,2);
    printText("C", ST7735_WHITE,105,26,3);

    printText("HUMIDITY", ST7735_WHITE,30,86,1);  // Humidity Static Text
    printText("%", ST7735_WHITE,90,101,3);
}
