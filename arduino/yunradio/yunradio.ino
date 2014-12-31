#include <Encoder.h>
#include <LiquidCrystal.h>
#include <FileIO.h>


/*******************************************
 ********* REST Interface ******************
 *******************************************/

char temp[128];
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

char* getCurrentPlayback() {
  int len = sizeof(temp);
  char* ptr = temp;
  Bridge.get("nowPlaying_song", ptr, len);
  Serial.println(ptr);
  ptr+=strlen(temp);
  *ptr = ' ';
  ptr++;
  len--;
  *ptr = '-';
  ptr++;
  len--;
  *ptr = ' ';
  ptr++;
  len--;
  Bridge.get("nowPlaying_artist", ptr, len);
  Serial.println(ptr);
  ptr+=strlen(ptr);
  
  
  return temp;
  /*
  char* ptr = temp;
  Bridge.get("nowPlaying_song", ptr, sizeof(ptr));
  Serial.println(ptr);
  Serial.println(temp);
  ptr += strlen(ptr);
  *ptr = ' ';
  ptr++;
  *ptr = '(';
  ptr++;
  Bridge.get("nowPlaying_artist", ptr, sizeof(ptr)); 
  Serial.println(ptr);
  Serial.println(temp);
  ptr += strlen(ptr);
  *ptr = ')';
  ptr++;
  *ptr = '\0';    

  Serial.println(temp);
  return temp;
  */
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


char* getUserSongNameAndArtirst(int user, int song) {
  int len = sizeof(temp);
  char* ptr = temp;
  sprintf(temp_cmd, "user%dsong%d", user, song);  
  Bridge.get(temp_cmd, ptr, len);
  Serial.println(ptr);
  ptr+=strlen(temp);
  *ptr = ' ';
  ptr++;
  len--;
  *ptr = '-';
  ptr++;
  len--;
  *ptr = ' ';
  ptr++;
  len--;
  sprintf(temp_cmd, "user%dartist%d", user, song);
  Bridge.get(temp_cmd, ptr, len);
  Serial.println(ptr);
  ptr+=strlen(ptr);
  
  return temp;
}


int getPlaylistsNum() {
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

char* getPlaylistSongNameAndArtist(int pl, int song) {
  int len = sizeof(temp);
  char* ptr = temp;
  sprintf(temp_cmd, "playlist%dsong%d", pl, song);  
  Bridge.get(temp_cmd, ptr, len);
  Serial.println(ptr);
  ptr+=strlen(temp);
  *ptr = ' ';
  ptr++;
  len--;
  *ptr = '-';
  ptr++;
  len--;
  *ptr = ' ';
  ptr++;
  len--;
  sprintf(temp_cmd, "playlist%dartist%d", pl, song);
  Bridge.get(temp_cmd, ptr, len);
  Serial.println(ptr);
  ptr+=strlen(ptr);
  
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
  sendCommand("playUser", temp_cmd, temp_cmd_1);
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

void playPopular(int daily) {
  if(daily)
      playPopularDaily();
  else
      playPopularMonthly();
}
void setShuffle(int enable) {
  if(enable){
    sendCommand("shuffle", "enable", NULL);
  }
  else {
    sendCommand("shuffle", "disable", NULL);
  }
}

int prevVolume = 100;

void setVolume(int perc) {
  if(perc != prevVolume) {
    sprintf(temp_cmd, "%d", perc);
    sendCommand("volume", temp_cmd, NULL);
    prevVolume = perc;
  }
  
}

void pause_resume(){
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


/*******************************************
 ********* LCD display    ******************
 *******************************************/
#define LCD_LENGTH 16
#define LCD_LINES  2
#define LCD_REFRESH_TIME 800

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

char line0[64];
char line1[64];
char temp_line[17];

char* lines[] = { line0, line1 };
int update[] = {0,0};
unsigned long update_time[] = {0,0};
int offset[] = {0,0};
int len[] = {0,0}; 
char* clearLine = "                ";

void setLine(int line, char* text) {
//  MsTimer2::stop();
  char* the_line = lines[line];

  if(strcmp(the_line, text)!=0) {
    int line_length = strlen(text);
    len[line] = line_length;
    if(line_length<LCD_LENGTH){
      memcpy(the_line,text,line_length);
      memcpy(the_line+line_length, clearLine, LCD_LENGTH-line_length);
      the_line[LCD_LENGTH] = '\0';
    }
    else {
      strcpy(the_line, text);
    }
    update[line] = 1;
    offset[line] = 0;
    update_screen();
  }
//  MsTimer2::start();
}

void update_screen() {
  int i;
  unsigned long time_now = millis();
  for(i=0;i<LCD_LINES;i++) {
    if(update[i]){
      lcd.setCursor(0,i);
      memcpy(temp_line, lines[i], 16);
      temp_line[17] = '\0';
      lcd.print(temp_line);
      update[i] = 0;
      update_time[i] = time_now;
    }
    else {
      if(time_now<update_time[i]+600){
        continue;
      }
      if(len[i]>LCD_LENGTH) {
        Serial.println(i);
        Serial.println(len[i]);
        lcd.setCursor(0,i);
        memcpy(temp_line, lines[i]+offset[i], 16);
        temp_line[17] = '\0';
        lcd.print(temp_line);
        update_time[i] = time_now;
        offset[i]++;
        if((offset[i]+LCD_LENGTH)>len[i]) {
          offset[i] = 0;
        }
      }
    }
  }
}

void setLineVolume(int line, int volume) {
  temp_line[0] = ' ';
  temp_line[1] = '-';
  temp_line[2] = '|';

  temp_line[15] = ' ';
  temp_line[14] = '-';
  temp_line[13] = '|';
  temp_line[16] = '\0';
  
  int i;
  for(i=0;i<10;i++) {
    if(volume>i*10) {
      temp_line[3+i] = 255;
    }
    else {
      temp_line[3+i] = ' ';
    }      
  }
  Serial.println(volume);
  Serial.println(temp_line);
  setLine(line, temp_line);
}

/*******************************************
 ********* Rotary Encoder ******************
 *******************************************/

Encoder knob(5,7);
int knob_sw = 6;
unsigned long time_since_last_move = 0;
unsigned long time_last_move = 0;
long move = 0;
long prev_move = 0;
int sw_value = 0;
int prev_sw_value = 0;
unsigned long time_since_last_pbc = 0;
unsigned long time_last_pbc = 0;


long knobRead(){
  long ret = knob.read();

  prev_move = move;
  move = ret;

  if(ret != 0) {
    unsigned long time_now = millis();
    time_since_last_move = time_now - time_last_move;
    time_last_move = time_now;
    knob.write(0);
  }
  
  return ret;
}

// 1 for repeat 0  for back
int repeat_or_back() {
  if(move<0 && prev_move<0 && time_since_last_move<1000) {
    return 0;
  }
  return 1;
}

int buttonPressed() {
  int value_read = digitalRead(knob_sw);  
  
  if(value_read != sw_value) {
    prev_sw_value = sw_value;
    sw_value = value_read;

    unsigned long time_now = millis();
    time_since_last_pbc = time_now - time_last_pbc;
    time_last_pbc = time_now;
  }
  
  return !sw_value;
}

unsigned long pbcDuration(){
  return time_since_last_pbc;
}


/*******************************************
 ********* Menus          ******************
 *******************************************/

char temp_menu[128];

typedef enum {
  MENU0_RADIOS = 0,
  MENU0_USERS,
  MENU0_PLAYLIST,
  MENU0_POPULAR,
  MENU0_NOWPLAYING,
  MENU0_CONFIG,
  MENU0_MAXVALUE,
  MENU0_NONE = -1
} MENU0;

typedef enum {
  MENU1_CONFIG_SHUFFLE = 0,
  MENU1_CONFIG_VOLUME,
  MENU1_CONFIG_STATUS,  
} MENU1_CONFIG;

char* menu0_str[] = { "Radios", "Users", "PlayList", "Popular", "Now Playing", "Configuration" } ;
char* menu1_popular_str[] = { "Monthly", "Daily" };
char* menu1_config_str[] = { "Shuffle", "Volume", "Status" };
char* menu2_config_shuffle_str[] = { "disable", "enable" };
int menu0_selected = MENU0_NONE;
int menu0_view = MENU0_RADIOS;

/*
  Radio Name
  User Name
  Playlist Name
  Popular Monthly/Weekly
*/ 
int menu1_radio_selected = -1;
int menu1_radio_view = 0;
int menu1_users_selected = -1;
int menu1_users_view = 0;
int menu2_users_selected = -1;
int menu2_users_view = 0;
int menu1_pl_selected = -1;
int menu1_pl_view = 0;
int menu2_pl_selected = -1;
int menu2_pl_view = 0;
int menu1_config_selected = -1;
int menu1_config_view = 0;
int menu1_popular_selected = -1;
int menu1_popular_view = 0;
int menu2_config_shuffle_view = 0;

int volume = 100;
int shuffle = 0;

/*
  ---
  Song Name
  Song Name
  ---
*/
int menu2_selected = -1;
int menu2_view = 0;

void do_menu0(int knob, int pbc) {
  setLine(0, "Menu:");
  setLine(1, menu0_str[menu0_view]);
Serial.print(menu0_view);
Serial.println(menu0_str[menu0_view]);
  if(pbc) {
    menu0_selected = menu0_view;
  }
  else {
    if(knob>0) {
      menu0_view++;
    }
    if(knob<0) {
      menu0_view--;
    }
    
    if(menu0_view < 0){
      menu0_view = 0;
    }
    if(menu0_view >= MENU0_MAXVALUE){
      menu0_view = MENU0_MAXVALUE - 1;
    }  
  }
}

void do_menu1_radio(int knob, int pbc) {
  setLine(0, "Radio:");
  setLine(1, getRadioName(menu1_radio_view));
  if(pbc && (pbcDuration()>1500)) {
    menu0_selected = MENU0_NONE;
    menu0_view = MENU0_RADIOS;
    menu1_radio_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      playRadio(menu1_radio_view);
      menu0_selected = MENU0_NOWPLAYING;
      menu0_view = MENU0_NOWPLAYING;      
    }
    else {
      if(knob>0) {
        menu1_radio_view++;
      }
      if(knob<0) {
        menu1_radio_view--;
      }
      
      if(menu1_radio_view < 0){
        menu1_radio_view = 0;
      }
      int radionum = getRadioNum();
      if(menu1_radio_view >= radionum){
        menu1_radio_view = radionum - 1;
      }      
    }
  }
}


void do_menu1_users(int knob, int pbc) {
  setLine(0, "User:");
  setLine(1, getUserName(menu1_users_view));
  if(pbc && (pbcDuration()>1500)) {
    menu0_selected = MENU0_NONE;
    menu0_view = MENU0_RADIOS;
    menu1_users_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      menu1_users_selected = menu1_users_view;
    }
    else {
      if(knob>0) {
        menu1_users_view++;
      }
      if(knob<0) {
        menu1_users_view--;
      }
      
      if(menu1_users_view < 0){
        menu1_users_view = 0;
      }
      int usernum = getUsersNum();
      if(menu1_users_view >= usernum){
        menu1_users_view = usernum - 1;
      }      
    }
  }
}


void do_menu2_users(int knob, int pbc) {
  sprintf(temp_menu, "User/%s:",getUserName(menu1_users_selected)); 
  setLine(0, temp_menu);
  setLine(1, getUserSongNameAndArtirst(menu1_users_selected, menu2_users_view));
  if(pbc && (pbcDuration()>1500)) {
    menu1_users_view = menu1_users_selected;
    menu1_users_selected = -1;
    menu2_users_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      menu2_users_selected = menu2_users_view;
      playUser(menu1_users_selected,menu2_users_selected); 
      menu0_selected = MENU0_NOWPLAYING;
      menu0_view = MENU0_NOWPLAYING;
      menu1_users_selected = -1;
    }
    else {
      if(knob>0) {
        menu2_users_view++;
      }
      if(knob<0) {
        menu2_users_view--;
      }
      
      if(menu2_users_view < 0){
        menu2_users_view = 0;
      }
      int usersongsnum = getUserSongs(menu1_users_selected);
      if(menu2_users_view >= usersongsnum){
        menu2_users_view = usersongsnum - 1;
      }      
    }
  }
}


void do_menu1_pl(int knob, int pbc) {
  setLine(0, "PlayList:");
  Serial.println(menu1_pl_view);
  Serial.println(getPlaylistName(0));
  setLine(1, getPlaylistName(menu1_pl_view));
  if(pbc && (pbcDuration()>1500)) {
    menu0_selected = MENU0_NONE;
    menu0_view = MENU0_RADIOS;
    menu1_pl_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      menu1_pl_selected = menu1_pl_view;
    }
    else {
      if(knob>0) {
        menu1_pl_view++;
      }
      if(knob<0) {
        menu1_pl_view--;
      }
      
      if(menu1_pl_view < 0){
        menu1_pl_view = 0;
      }
      int plnum = getPlaylistsNum();
      if(menu1_pl_view >= plnum){
        menu1_pl_view = plnum - 1;
      }      
    }
  }
}


void do_menu2_pl(int knob, int pbc) {
  sprintf(temp_menu, "PlayList/%s:",getPlaylistName(menu1_pl_selected)); 
  setLine(0, temp_menu);
  setLine(1, getPlaylistSongNameAndArtist(menu1_pl_selected, menu2_pl_view));
  if(pbc && (pbcDuration()>1500)) {
    menu1_pl_view = menu1_pl_selected;
    menu1_pl_selected = -1;
    menu2_pl_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      menu2_pl_selected = menu2_pl_view;
      playPlaylist(menu1_pl_selected,menu2_pl_selected); 
      menu0_selected = MENU0_NOWPLAYING;
      menu0_view = MENU0_NOWPLAYING;
      menu1_pl_selected = -1;
    }
    else {
      if(knob>0) {
        menu2_pl_view++;
      }
      if(knob<0) {
        menu2_pl_view--;
      }
      
      if(menu2_pl_view < 0){
        menu2_pl_view = 0;
      }
      int plsongsnum = getPlaylistSongs(menu1_pl_selected);
      if(menu2_pl_view >= plsongsnum){
        menu2_pl_view = plsongsnum - 1;
      }      
    }
  }
}


void do_menu1_popular(int knob, int pbc) {
  setLine(0, "Popular:");
  setLine(1, menu1_popular_str[menu1_popular_view]);
  if(pbc && (pbcDuration()>1500)) {
    menu0_selected = MENU0_NONE;
    menu0_view = MENU0_RADIOS;
    menu1_popular_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      playPopular(menu1_popular_view);
      menu0_selected = MENU0_NOWPLAYING;
      menu0_view = MENU0_NOWPLAYING;
    }
    else {
      if(knob>0) {
        menu1_popular_view++;
      }
      if(knob<0) {
        menu1_popular_view--;
      }
      
      if(menu1_popular_view < 0){
        menu1_popular_view = 0;
      }
      int radionum = sizeof(menu1_popular_str)/sizeof(char*);
      if(menu1_popular_view >= radionum){
        menu1_popular_view = radionum - 1;
      }      
    }
  }
}



void do_menu_nowplaying(int knob, int pbc) {
  setLine(0, "NowPlaying:");
  setLine(1, getCurrentPlayback());
  if(pbc && (pbcDuration()>1500)) {
    menu0_selected = MENU0_NONE;
    menu0_view = MENU0_RADIOS;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      pause_resume();
    }
    else {
      if(knob>0) {
        next();
      }
      if(knob<0) {
        if(repeat_or_back()) {
          restart();
        }
        else {
          prev();
        }
      }
    }
  }
}


void do_menu1_config(int knob, int pbc) {
  setLine(0, "Config:");
  setLine(1, menu1_config_str[menu1_config_view]);
  if(pbc && (pbcDuration()>1500)) {
    menu0_selected = MENU0_NONE;
    menu0_view = MENU0_RADIOS;
    menu1_config_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      menu1_config_selected = menu1_config_view;
    }
    else {
      if(knob>0) {
        menu1_config_view++;
      }
      if(knob<0) {
        menu1_config_view--;
      }
      
      if(menu1_config_view < 0){
        menu1_config_view = 0;
      }
      int confignum = sizeof(menu1_config_str)/sizeof(char*);
      if(menu1_config_view >= confignum){
        menu1_config_view = confignum - 1;
      }      
    }
  }
}

void do_menu2_config_shuffle(int knob, int pbc) {
  setLine(0, "Config/Shuffle:");
  setLine(1, menu2_config_shuffle_str[menu2_config_shuffle_view]);
  if(pbc && (pbcDuration()>1500)) {
    menu1_config_view = menu1_config_selected;
    menu1_config_selected = -1;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      setShuffle( menu2_config_shuffle_view );
      
      menu1_config_selected = menu1_config_view;
      menu1_config_selected = -1;
    }
    else {
      if(knob>0) {
        menu2_config_shuffle_view++;
      }
      if(knob<0) {
        menu2_config_shuffle_view--;
      }
      
      if(menu2_config_shuffle_view < 0){
        menu2_config_shuffle_view = 0;
      }
      int confignum = sizeof(menu2_config_shuffle_str)/sizeof(char*);
      if(menu2_config_shuffle_view >= confignum){
        menu2_config_shuffle_view = confignum - 1;
      }      
    }
  }
}

void do_menu2_config_volume(int knob, int pbc) {
  setLine(0, "Config/Volume:");
  setLineVolume(1, volume);
  if(pbc && (pbcDuration()>1500)) {
    menu1_config_view = menu1_config_selected;
    menu1_config_selected = -1;
  }
  else {
    if(knob>0) {
      volume+=2;
    }
    if(knob<0) {
      volume-=2;
    }
    if(volume < 0){
      volume = 0;
    }
    if(volume >= 100){
      volume = 100;
    }
    setVolume(volume);
  }
}


void do_menu2_config_status(int knob, int pbc) {
  setLine(0, "Config/Status:");
  setLine(1, getStatus());
  if(pbc) {
    menu1_config_view = menu1_config_selected;
    menu1_config_selected = -1;
  }
}


/*******************************************
 ********* Other Globals  ******************
 *******************************************/
Process radio_process;




/*******************************************
 ********* Setup          ******************
 *******************************************/
void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD.
  setLine(0, "YUN Radio!!!");
  setLine(1, "Booting...");
//  lcd.print("Grooveshark!!");
  update_screen();
  
  Serial.begin(9600);
  Bridge.begin();
  setLine(1, "Booting... Done");
  delay(500);
  setStartingStatus();
  
  pinMode(knob_sw, INPUT);

  digitalWrite(knob_sw, HIGH);

  start_radio();  
/*
  while(1) {
    char* lininostatus = getStatus();
//    lcd.setCursor(0,1);
    setLine(1, lininostatus);    
    if(strcmp(lininostatus, "Ready") == 0) {
//      lcd.print("Ready           ");
      break;
    }
    
//    lcd.print(lininostatus);
    update_screen();
    delay(800);
  }
*/
}

/*******************************************
 ********* Loop           ******************
 *******************************************/
int prev_button = 0;

void loop() {
  long knob_value = knobRead();
  int button = buttonPressed();
  
  int high_to_low = (button - prev_button) == -1;

  switch(menu0_selected) {
    case MENU0_NONE:
      do_menu0(knob_value, high_to_low);
    break;
    case MENU0_RADIOS:
      do_menu1_radio(knob_value, high_to_low);
    break;
    case MENU0_USERS:
      if(menu1_users_selected == -1) {
        do_menu1_users(knob_value, high_to_low);
      }
      else {
        do_menu2_users(knob_value, high_to_low);
      }
    break;
    case MENU0_PLAYLIST:
      if(menu1_pl_selected == -1) {
        do_menu1_pl(knob_value, high_to_low);
      }
      else {
        do_menu2_pl(knob_value, high_to_low);
      }
    break;
    case MENU0_POPULAR:
      do_menu1_popular(knob_value, high_to_low);
    break;
    case MENU0_NOWPLAYING:
      do_menu_nowplaying(knob_value, high_to_low);
    break;
    case MENU0_CONFIG:
      if(menu1_config_selected == -1) {
        do_menu1_config(knob_value, high_to_low);
      }
      else {
        switch(menu1_config_selected) {
          case MENU1_CONFIG_SHUFFLE:
            do_menu2_config_shuffle(knob_value, high_to_low);
          break;
          case MENU1_CONFIG_VOLUME:
            do_menu2_config_volume(knob_value, high_to_low);
          break;
          case MENU1_CONFIG_STATUS:
            do_menu2_config_status(knob_value, high_to_low);
          break;
        }
      }
    break;
  }
  update_screen();
  int i;
  for(i=0;i<80;i++) {
    knob.read();
    delay(1);
  }
  prev_button = button;
  // not sure if this is needed, but just in case let's flush the process buffer
  radio_process.flush();
}

void start_radio() {    
    radio_process.begin("python");
    radio_process.addParameter("/root/examples-pygrooveshark/yun-radio.py");
    radio_process.runAsynchronously();
}


void stop_radio() {
    radio_process.close();
}
