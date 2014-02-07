
#include <Encoder.h>


#include <FileIO.h>

#define    GENRE_NONE             0
#define    GENRE_ROCK             12
#define    GENRE_BLUES            230
#define    GENRE_ELECTRONICA      67
#define    GENRE_CLASSICROCK      3529
#define    GENRE_INDIE            136
#define    GENRE_METAL            17


int stations[] = { GENRE_NONE,
    GENRE_ROCK,
    GENRE_BLUES,
    GENRE_ELECTRONICA,
    GENRE_CLASSICROCK,
    GENRE_INDIE,
    GENRE_METAL };


String stations_str[] = { "None",
    "Rock",
    "Blues",
    "Electronica",
    "Classic Rock",
    "Indie",
    "Metal" };
    
int genres;
int val = 0;
int enc_val = 0;
int prevVal = 0;
Process radio_process;
String label_song, label_artist;


// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
Encoder knob(2,4);
int knob_sw = 3;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.print("Grooveshark!!");

  pinMode(knob_sw, INPUT);
  digitalWrite(knob_sw, HIGH);
//  delay(2);
  genres = sizeof(stations)/sizeof(int);
  Serial.begin(9600);
  Bridge.begin();

  start_radio(val);  

}

void loop() {
  long knob_value = knob.read();

  if(knob_value > 0) {
    enc_val++;
    if(enc_val>=genres) enc_val = genres - 1;
  }
  else {
    if(knob_value < 0) {
      enc_val--;
      if(enc_val<0) enc_val = 0;      
    }
  }
  
  int sw_value = digitalRead(knob_sw);  
  
  Serial.println(sw_value);
  if(!sw_value){
    val = enc_val;
  }
  
  if(knob_value!=0 || !sw_value) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Radio:");
    lcd.setCursor(0, 1);
    if(enc_val == val) {  
      lcd.print("-");
    }
    lcd.print(stations_str[enc_val]);
  }
  knob.write(0);

  
  if(val != prevVal) {
    String text;
    text = "Value: ";
    text += val;

    start_radio(val);
    
    prevVal = val;
  } 
  
  char labelbuffer_song[256];
  char labelbuffer_artist[256];
  Bridge.get("songName", labelbuffer_song, 256); 

  if (String(labelbuffer_song).length() > 0 && label_song != String(labelbuffer_song)){
    lcd.clear();
    label_song = String(labelbuffer_song);
    
    lcd.setCursor(0, 1);
    lcd.print(label_song);
    Bridge.get("songArtist", labelbuffer_artist, 256);
    label_artist = String(labelbuffer_artist);
    
    lcd.setCursor(0, 0);
    lcd.print(label_artist);
    
  }
  
//  delay(400);
}

void start_radio(int val) {
    String parameter = "";
    radio_process.close();
    Process killer;
    killer.begin("killall");
    killer.addParameter("mpg123");
    killer.run();
    if(stations[val] != 0) {
      radio_process.begin("python");
      radio_process.addParameter("/root/examples-pygrooveshark/radio_mpg123.py");
      parameter+= stations[val];
      radio_process.addParameter(parameter);
      radio_process.runAsynchronously();
    }
    else {
      radio_process.begin("echo");
      radio_process.addParameter("Do Nothing!!!");
      radio_process.runAsynchronously();
    }
}

