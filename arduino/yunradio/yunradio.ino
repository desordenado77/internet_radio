#include <Encoder.h>
#include <LiquidCrystal.h>
#include <FileIO.h>

char temp[256];
char temp_cmd[32];
char temp_cmd_1[32];

char* getStatus() {
  Bridge.get("lininoStatus", temp, sizeof(temp)); 
  return temp;
}

char* getCurrentSong() {
  Bridge.get("nowPlaying_song", temp, sizeof(temp)); 
  return temp;
}

char* getCurrentAlbum() {
  Bridge.get("nowPlaying_album", temp, sizeof(temp)); 
  return temp;
}


char* getCurrentArtist() {
  Bridge.get("nowPlaying_artist", temp, sizeof(temp)); 
  return temp;
}


char* getCurrentPosition() {
  Bridge.get("nowPlaying_pos", temp, sizeof(temp)); 
  return temp;
}


char* getCurrentDuration() {
  Bridge.get("nowPlaying_dur", temp, sizeof(temp)); 
  return temp;
}

int getUsersNum() {
  Bridge.get("users", temp, sizeof(temp)); 
  return atoi(temp);
}

char* getUserName(int user) {
  sprintf(temp_cmd, "user%dname", user);
  Bridge.get(temp_cmd, temp, sizeof(temp)); 
  return temp;
}

int getUserSongs(int user) {
  sprintf(temp_cmd, "user%dsongs", user);
  Bridge.get(temp_cmd, temp, sizeof(temp)); 
  return atoi(temp);
}

char* getUserSongName(int user, int song) {
  sprintf(temp_cmd, "user%dsong%d", user, song);
  Bridge.get(temp_cmd, temp, sizeof(temp)); 
  return temp;
}

char* getUserSongArtist(int user, int song) {
  sprintf(temp_cmd, "user%dartist%d", user, song);
  Bridge.get(temp_cmd, temp, sizeof(temp)); 
  return temp;
}

int getPlaylitstsNum() {
  Bridge.get("playlists", temp, sizeof(temp)); 
  return atoi(temp);
}

char* getPlaylistName(int pl) {
  sprintf(temp_cmd, "playlist%dname", pl);
  Bridge.get(temp_cmd, temp, sizeof(temp)); 
  return temp;
}

int getPlaylistSongs(int pl) {
  sprintf(temp_cmd, "playlist%dsongs", pl);
  Bridge.get(temp_cmd, temp, sizeof(temp)); 
  return atoi(temp);
}

char* getPlaylistSongName(int pl, int song) {
  sprintf(temp_cmd, "playlist%dsong%d", pl, song);
  Bridge.get(temp_cmd, temp, sizeof(temp)); 
  return temp;
}

char* getPlaylistSongArtist(int pl, int song) {
  sprintf(temp_cmd, "playlist%dartist%d", pl, song);
  Bridge.get(temp_cmd, temp, sizeof(temp)); 
  return temp;
}

int getRadioNum() {
  Bridge.get("radios", temp, sizeof(temp)); 
  return atoi(temp);
}

char* getRadioName(int radio) {
  sprintf(temp_cmd, "radio%dname", radio);
  Bridge.get(temp_cmd, temp, sizeof(temp)); 
  return temp;
}

void sendCommand(char* command, char* commandData1, char* commandData2) {
  Bridge.put("command", command);
  if(commandData1 != NULL) {
    Bridge.put("commandData1", commandData1);
    
    if(commandData2 != NULL) {
      Bridge.put("commandData2", commandData2);
    }
  }
}

void playRadio(int radio) {
  sprintf(temp_cmd, "%d", radio);
  sendCommand("playRadio", temp_cmd, NULL);
}

void playUser(int user, int song) {
  sprintf(temp_cmd, "%d", user);
  sprintf(temp_cmd_1, "%d", song);  
  sendCommand("playRadio", temp_cmd, temp_cmd_1);
}

void playPlaylist(int pl, int song) {
  sprintf(temp_cmd, "%d", pl);
  sprintf(temp_cmd_1, "%d", song);  
  sendCommand("playPlayList", temp_cmd, temp_cmd_1);
}

void playPopularMonthly() {
  sendCommand("playPopular", "monthly", NULL);
}

void playPopularDaily() {
  sendCommand("playPopular", "daily", NULL);
}

void shuffle(int enable) {
  if(enable){
    sendCommand("shuffle", "enable", NULL);
  }
  else {
    sendCommand("shuffle", "disable", NULL);
  }
}

void volume(int perc) {
  sprintf(temp_cmd, "%d", perc);
  sendCommand("volume", temp_cmd, NULL);
}

void pause(){
  sendCommand("pause", NULL, NULL);
}

void next(){
  sendCommand("next", NULL, NULL);
}

void prev(){
  sendCommand("prev", NULL, NULL);
}

void restart(){
  sendCommand("restart", NULL, NULL);
}

void addUser(int id, char* user_name) {
  sprintf(temp_cmd, "%d", id);
  sendCommand("addUser", temp_cmd, user_name);
}

void addRadio(int id, char* user_name) {
  sprintf(temp_cmd, "%d", id);
  sendCommand("addRadio", temp_cmd, user_name);
}

void addPlaylist(int id) {
  sprintf(temp_cmd, "%d", id);
  sendCommand("addPlaylist", temp_cmd, NULL);
}

void setStartingStatus() {
  Bridge.put("lininoStatus", "Starting radio"); 
}

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



LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
Encoder knob(2,4);
int knob_sw = 3;

void setup() {
  Serial.begin(9600);
  Bridge.begin();

  setStartingStatus();
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.print("Grooveshark!!");

  pinMode(knob_sw, INPUT);
  digitalWrite(knob_sw, HIGH);

  genres = sizeof(stations)/sizeof(int);

  start_radio();  
  
  while(1) {
    char* lininostatus = getStatus();
    lcd.setCursor(0,1);
    
    if(strcmp(lininostatus, "Ready") == 0) {
      lcd.print("Ready           ");
      break;
    }
    
    lcd.print(lininostatus);
    

    delay(1);
  }

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

    start_radio();
    
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
  
}

void start_radio() {
    String parameter = "";
    radio_process.close();
    Process killer;
    killer.begin("killall");
    killer.addParameter("mpg123");
    killer.run();
    radio_process.begin("python");
    radio_process.addParameter("/root/examples-pygrooveshark/yun-radio.py");
    radio_process.runAsynchronously();
}

