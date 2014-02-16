#include <Encoder.h>
#include <LiquidCrystal.h>
#include <FileIO.h>


/*******************************************
 ********* REST Interface ******************
 *******************************************/

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

char* getCurrentStatus() {
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


/*******************************************
 ********* LCD display    ******************
 *******************************************/
#define LCD_LENGTH 16
#define LCD_LINES  2
#define LCD_REFRESH_TIME 800

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

char line0[64];
char line1[64];

char* lines[] = { line0, line1 };
int update[] = {0,0};
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
  for(i=0;i<LCD_LINES;i++) {
    if(update[i]){
      lcd.setCursor(0,i);
      lcd.print(lines[i]);
      update[i] = 0;
    }
    else {
      if(len[i]>LCD_LENGTH) {
        lcd.setCursor(0,i);
        lcd.print(lines[i]+offset[i]);
        offset[i]++;
        if((offset[i]+LCD_LENGTH)>len[i]) {
          offset[i] = 0;
        }
      }
    }
  }
}

/*******************************************
 ********* Rotary Encoder ******************
 *******************************************/

Encoder knob(2,4);
int knob_sw = 3;
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

char* menu0_str[] = { "- Radios", "- Users", "- PlayList", "- Popular", "- Now Playing", "- Configuration" } ;
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
  setLine(0, "Menu/Radio:");
  setLine(1, getRadioName(menu1_radio_view));
  if(pbc && (pbcDuration()>1500)) {
    menu0_selected = MENU0_NONE;
    menu0_view = MENU0_RADIOS;
    menu1_radio_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      playRadio(menu1_radio_view);
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
  setLine(0, "Menu/User:");
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
  sprintf(temp_menu, "Menu/User/%s:",getUserName(menu1_users_selected)); 
  setLine(0, temp_menu);
  setLine(1, getUserSongName(menu1_users_selected, menu2_users_view));
  if(pbc && (pbcDuration()>1500)) {
    menu1_users_view = menu1_users_selected;
    menu1_users_selected = -1;
    menu2_users_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      menu2_users_selected = menu2_users_view;
      playUser(menu1_users_selected,menu2_users_selected); 
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
  setLine(0, "Menu/PL:");
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
  sprintf(temp_menu, "Menu/PL/%s:",getPlaylistName(menu1_pl_selected)); 
  setLine(0, temp_menu);
  setLine(1, getPlaylistSongName(menu1_pl_selected, menu2_pl_view));
  if(pbc && (pbcDuration()>1500)) {
    menu1_pl_view = menu1_pl_selected;
    menu1_pl_selected = -1;
    menu2_pl_view = 0;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
      menu2_pl_selected = menu2_pl_view;
      playPlaylist(menu1_pl_selected,menu2_pl_selected); 
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




void do_menu_nowplaying(int knob, int pbc) {
  setLine(0, "Menu/NowPlaying:");
  setLine(1, getCurrentStatus());
  if(pbc && (pbcDuration()>1500)) {
    menu0_selected = MENU0_NONE;
    menu0_view = MENU0_RADIOS;
  }
  else {
    if(pbc && (pbcDuration()<1500)) {
    }
    else {
      if(knob>0) {
        next();
      }
      if(knob<0) {
        restart();
      }
    }
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
    case MENU0_NOWPLAYING:
      do_menu_nowplaying(knob_value, high_to_low);
    break;
  }
  update_screen();
  delay(80);
  prev_button = button;
}

void start_radio() {    
    radio_process.begin("python");
    radio_process.addParameter("/root/examples-pygrooveshark/yun-radio.py");
    radio_process.runAsynchronously();
}


void stop_radio() {
    radio_process.close();
}
