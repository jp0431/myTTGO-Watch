#include "config.h"
#include <stdint.h>
#include <WiFi.h>
#include "time.h"
#include <SPI.h>
#include <TTGO.h>
#include "credentials.h"
const char* ntpServer = "3.es.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
static lv_disp_buf_t disp_buf; //LVGL stuff;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
lv_disp_drv_t disp_drv;
lv_indev_drv_t indev_drv;
TTGOClass *ttgo;  
static lv_obj_t *hora;
static lv_obj_t *data;
static lv_obj_t *batt;
static lv_obj_t *per; 
static lv_obj_t *minut; 
static lv_obj_t *sec; 

static lv_style_t styleBatt;

static lv_obj_t *paginaPrin;
static lv_obj_t *pagOpc;
static lv_obj_t *pagCal;
static lv_obj_t *pagCron;
static lv_obj_t *pagTemp;
lv_calendar_date_t date;
lv_obj_t  * calendar;
lv_obj_t *cal;
lv_obj_t *cron;
lv_obj_t *temp;
lv_obj_t *home;
lv_obj_t *home2;
lv_obj_t *play;
lv_obj_t *pauseBT;
lv_obj_t *plus;
lv_obj_t *minus;
bool btemp;
static bool b_Connectat;
static bool estat= true;
void temporizador(void * parameter){
  for(;;){
    if(btemp){
      String szSec(lv_label_get_text(sec));
      int a=szSec.toInt();
      int b = a;
      for(int i=0;i<b;i++){
        a-=1;
        if(a<10) lv_label_set_text(sec, String("0"+String(a)).c_str());
        else lv_label_set_text(sec, String(String(a)).c_str());
        delay(1000);
        Serial.println(a);
        btemp=false;
            }
    }
 vTaskDelay(10);
  }
  vTaskDelete(NULL);
}
static void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
   // static String szTm=();

 // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
char dayWeek[50];
strftime(dayWeek,50,"%A",&timeinfo);

char monthName[50];
strftime(monthName,50,"%B",&timeinfo);



char dayM[50];
strftime(dayM,50,"%d",&timeinfo);
date.day=timeinfo.tm_mday;
char yearM[50];
strftime( yearM,50,"%Y",&timeinfo);

char timeHour[3];
strftime(timeHour,3, "%H", &timeinfo);
//Serial.println(timeHour);
char timeMin[3];
strftime(timeMin,3, "%M", &timeinfo);
//Serial.println(timeMin);
char timeSec[3];
strftime(timeSec,3, "%S", &timeinfo);
//Serial.println(timeSec);
String a= String(timeHour);
String b=String(timeMin);
String c=String(timeSec);
String dia=String(dayWeek);
String mes=String(monthName);
if(mes=="January"){
  date.month=01;
}
if(mes=="February"){
  date.month=02;
}if(mes=="March"){
  date.month=03;
}if(mes=="April"){
  date.month=04;
}if(mes=="May"){
    date.month=05;

}if(mes=="June"){
    date.month=06;

}if(mes=="July"){
    date.month=07;

}
if(mes=="August"){
    date.month=8;

}if(mes=="September"){
    date.month=9;

}if(mes=="October"){
    date.month=10;

}if(mes=="November"){
    date.month=11;

}if(mes=="December"){
      date.month=12;

}

String diaM=String(dayM);
String yearm=String(yearM);
date.year=yearm.toInt();

String hores=a+":"+b+":"+c;
String fecha=dia +", "+mes+" "+ diaM +" " + yearm;
lv_label_set_text(hora,hores.c_str());
lv_label_set_text(data,fecha.c_str());
}
void apaga() {
  ttgo->displaySleep();
  ttgo->power->setPowerOutPut(AXP202_LDO3, false);
  ttgo->power->setPowerOutPut(AXP202_LDO4, false);
  ttgo->power->setPowerOutPut(AXP202_LDO2, false);
  ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
  ttgo->power->setPowerOutPut(AXP202_DCDC2, false);
  estat = true;
}
void encen() {
  ttgo->displayWakeup();
  ttgo->power->setPowerOutPut(AXP202_LDO3, true);
  ttgo->power->setPowerOutPut(AXP202_LDO4, true);
  ttgo->power->setPowerOutPut(AXP202_LDO2, true);
  ttgo->power->setPowerOutPut(AXP202_EXTEN, true);
  ttgo->power->setPowerOutPut(AXP202_DCDC2, true);


  estat = false;
}
static void obrir_opcions(lv_obj_t *obj, lv_event_t event)
{
 lv_obj_set_hidden(paginaPrin, true);
 lv_obj_set_hidden(pagOpc, false);

}
static void opcions_temp(lv_obj_t *obj, lv_event_t event){
  String szSec(lv_label_get_text(sec));
  String szMin(lv_label_get_text(minut));
  int n_Sec = szSec.toInt();
  int n_Min = szMin.toInt();

  if(obj==plus && event==LV_EVENT_CLICKED){
     n_Sec+=10;
  if(n_Sec>=60){
     n_Min +=1;
      if(n_Min>=90){
      n_Min=90;
    }
    if(n_Min<10){
      String c = "0"+String(n_Min)+":";
      lv_label_set_text(minut, c.c_str());
    }
    else{
      lv_label_set_text(minut, String(String(n_Min) +":").c_str());
    }
  
    n_Sec=0;

  }
  if(n_Sec==0){
    lv_label_set_text(sec, "00");

  }
  else{
  lv_label_set_text(sec, String(n_Sec).c_str());}

  }
  if(obj==minus && event==LV_EVENT_CLICKED){
    if(n_Min>=1&&n_Sec==0){
      n_Min-=1;
      n_Sec=60;
      if(n_Min<10){
        lv_label_set_text(minut, String("0" +String(n_Min)+":").c_str());
      }
      else
        lv_label_set_text(minut, String(n_Min).c_str());
    }
    if(n_Sec>=10){
       n_Sec-=10;
      lv_label_set_text(sec,String(n_Sec).c_str());
      
    }
 
    else{
     
     lv_label_set_text(sec,"00"); 

    }
  }
  if(obj==play && event==LV_EVENT_CLICKED){
    btemp=true;

  }
  if(obj==pauseBT && event==LV_EVENT_CLICKED){
    btemp=false;
    
  }
Serial.println(n_Sec);

}
static void opcions_menu(lv_obj_t *obj, lv_event_t event){
  if(obj==cal && event==LV_EVENT_CLICKED){
     lv_obj_set_hidden(pagOpc, true);
     lv_obj_set_hidden(pagCal, false);
     lv_calendar_set_today_date(calendar, &date);
     lv_calendar_set_showed_date(calendar, &date);
  }
  if(obj==cron && event==LV_EVENT_CLICKED){
    Serial.println("Cronometre");
  }
  if(obj==temp && event==LV_EVENT_CLICKED){
    lv_obj_set_hidden(pagTemp, false);
    lv_obj_set_hidden(pagOpc, true);

  }
   if(obj==home || obj==home2 && event==LV_EVENT_CLICKED){
     lv_obj_set_hidden(pagOpc, true);
      lv_obj_set_hidden(pagTemp, true);

     lv_obj_set_hidden(paginaPrin, false);
    }
}

void createPage(){
static lv_style_t styleHora;
lv_style_init(&styleHora);
static lv_style_t styleData;
lv_style_init(&styleData);
static lv_style_t styleBatt2;
lv_style_init(&styleBatt2);
lv_style_init(&styleBatt);
lv_style_set_text_font(&styleData, LV_STATE_DEFAULT, &lv_font_montserrat_16);
lv_style_set_text_font(&styleHora,LV_STATE_DEFAULT, &lv_font_montserrat_32);
lv_style_set_text_font(&styleBatt,LV_STATE_DEFAULT, &lv_font_montserrat_28);
lv_style_set_text_font(&styleBatt2,LV_STATE_DEFAULT, &lv_font_montserrat_28);

//lv_style_set_text_color(&styleBatt, LV_STATE_DEFAULT, LV_COLOR_GREEN);
paginaPrin =lv_cont_create(lv_scr_act(), NULL);
lv_obj_set_width( paginaPrin, lv_disp_get_hor_res( NULL )  );  
lv_obj_set_height( paginaPrin, lv_disp_get_ver_res( NULL ) );

hora = lv_label_create(paginaPrin, NULL);
data = lv_label_create(paginaPrin, NULL);
batt = lv_label_create(paginaPrin, NULL);
per = lv_label_create(paginaPrin, NULL);


lv_obj_add_style(data,LV_LABEL_PART_MAIN,&styleData);
lv_obj_add_style(hora,LV_LABEL_PART_MAIN,&styleHora);
lv_obj_add_style(batt,LV_LABEL_PART_MAIN,&styleBatt2);
lv_obj_add_style(per,LV_LABEL_PART_MAIN,&styleBatt);


lv_obj_set_pos(hora, 60, 10);
lv_obj_set_pos(data, 25, 50);
lv_obj_set_pos(batt, 50, 80);
lv_obj_set_pos(per, 120, 80);



lv_obj_t * label;



lv_obj_t *btn1 = lv_btn_create(paginaPrin, NULL);
lv_obj_set_size(btn1, 75,50);
lv_obj_set_event_cb(btn1, obrir_opcions);
lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 40);

label = lv_label_create(btn1, NULL);
lv_label_set_text(label, LV_SYMBOL_SETTINGS);


pagOpc =lv_cont_create(lv_scr_act(), NULL);
lv_obj_set_width( pagOpc, lv_disp_get_hor_res( NULL )  );  
lv_obj_set_height( pagOpc, lv_disp_get_ver_res( NULL ) );

cal = lv_btn_create(pagOpc, NULL);
lv_obj_set_event_cb(cal, opcions_menu);
lv_obj_align(cal, NULL, LV_ALIGN_CENTER, 0, -80);
lv_obj_t *label2 = lv_label_create(cal, NULL);
lv_label_set_text(label2, "Calendari");

cron = lv_btn_create(pagOpc, NULL);
lv_obj_set_event_cb(cron, opcions_menu);
lv_obj_align(cron, NULL, LV_ALIGN_CENTER, 0, -20);

lv_obj_t *label3 = lv_label_create(cron, NULL);
lv_label_set_text(label3, "Cronòmetre");

temp = lv_btn_create(pagOpc, NULL);
lv_obj_set_event_cb(temp, opcions_menu);
lv_obj_align(temp, NULL, LV_ALIGN_CENTER, 0, 30);

lv_obj_t *label4 = lv_label_create(temp, NULL);
lv_label_set_text(label4, "Temporitzador");

home = lv_btn_create(pagOpc, NULL);
lv_obj_set_event_cb(home, opcions_menu);
lv_obj_align(home, NULL, LV_ALIGN_CENTER, 20, 80);
static  lv_style_t styleBT;
lv_style_init(&styleBT);
lv_style_set_bg_color(&styleBT,LV_STATE_DEFAULT,LV_COLOR_RED);
lv_style_set_text_color(&styleBT, LV_STATE_DEFAULT, LV_COLOR_WHITE);
lv_obj_add_style(home, LV_LABEL_PART_MAIN,&styleBT);


lv_obj_t *label5 = lv_label_create(home, NULL);
lv_label_set_text(label5, LV_SYMBOL_HOME);
lv_obj_set_size(home, 75,50);

lv_obj_set_hidden(pagOpc, true);

pagCal=lv_cont_create(lv_scr_act(), NULL);

lv_obj_set_width( pagCal, lv_disp_get_hor_res( NULL )  );  
lv_obj_set_height( pagCal, lv_disp_get_ver_res( NULL ) );
calendar = lv_calendar_create(pagCal, NULL);
lv_obj_set_size(calendar, lv_disp_get_hor_res( NULL ), (lv_disp_get_ver_res( NULL )));


lv_obj_set_hidden(pagCal, true);


pagTemp =lv_cont_create(lv_scr_act(), NULL);
lv_obj_set_width( pagTemp, lv_disp_get_hor_res( NULL )  );  
lv_obj_set_height(pagTemp, lv_disp_get_ver_res( NULL ) );

home2 = lv_btn_create(pagTemp, NULL);
lv_obj_set_event_cb(home2, opcions_menu);
lv_obj_align(home2, NULL, LV_ALIGN_CENTER, 30, 80);

lv_obj_add_style(home2, LV_LABEL_PART_MAIN,&styleBT);
lv_obj_t *label6 = lv_label_create(home2, NULL);
lv_label_set_text(label6, LV_SYMBOL_HOME);
lv_obj_set_size(home2, 75,50);

minut = lv_label_create(pagTemp, NULL);
lv_obj_align(minut,NULL,LV_ALIGN_CENTER, -20, -40);
lv_obj_add_style(minut, LV_LABEL_PART_MAIN,&styleHora);
lv_label_set_text(minut, "00:");

sec = lv_label_create(pagTemp, NULL);
lv_obj_align(sec,NULL,LV_ALIGN_CENTER, 30, -40);
lv_obj_add_style(sec, LV_LABEL_PART_MAIN,&styleHora);
lv_label_set_text(sec, "00");

plus = lv_btn_create(pagTemp, NULL);
lv_obj_set_event_cb(plus, opcions_temp);

lv_obj_align(plus,NULL,LV_ALIGN_CENTER, -5, 25);
lv_obj_t *pl = lv_label_create(plus, NULL);
lv_label_set_text(pl, LV_SYMBOL_PLUS);
lv_obj_set_size(plus, 75,50);

minus = lv_btn_create(pagTemp, NULL);
lv_obj_set_event_cb(minus, opcions_temp);

lv_obj_align(minus,NULL,LV_ALIGN_CENTER, 70, 25);
lv_obj_t *mn = lv_label_create(minus, NULL);
lv_label_set_text(mn, LV_SYMBOL_MINUS);
lv_obj_set_size(minus, 75,50);

play = lv_btn_create(pagTemp, NULL);
lv_obj_set_event_cb(play, opcions_temp);

lv_obj_align(play,NULL,LV_ALIGN_CENTER, -5, -80);
lv_obj_t *playbt = lv_label_create(play, NULL);
lv_label_set_text(playbt, LV_SYMBOL_PLAY);
lv_obj_set_size(play, 75,50);

pauseBT = lv_btn_create(pagTemp, NULL);
lv_obj_set_event_cb(pauseBT, opcions_temp);

lv_obj_align(pauseBT,NULL,LV_ALIGN_CENTER, 70, -80);
lv_obj_t *pausebt = lv_label_create(pauseBT, NULL);
lv_label_set_text(pausebt, LV_SYMBOL_PAUSE);
lv_obj_set_size(pauseBT, 75,50);





}
void Sleep(void * parameter){
  Serial.print("estat ");
  Serial.println(estat);
  for(;;){
    if(!estat){
      delay(5000);
      apaga();
    }
  }
}
void getBat(void * parameter){
  for(;;){

    int p = ttgo->power->getBattPercentage();
    String battery=String(p)+"%";
    lv_label_set_text(batt, battery.c_str());
 
    if(p<=100&&p>75){
        lv_label_set_text(per, LV_SYMBOL_BATTERY_FULL);
        lv_style_set_text_color(&styleBatt, LV_STATE_DEFAULT, LV_COLOR_GREEN);

    }
  else  if(p<=75){
      lv_label_set_text(per, LV_SYMBOL_BATTERY_3);
      lv_style_set_text_color(&styleBatt, LV_STATE_DEFAULT, LV_COLOR_YELLOW);

    }
  else  if(p<=50){
      lv_label_set_text(per, LV_SYMBOL_BATTERY_2);
      lv_style_set_text_color(&styleBatt, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
      
    }
 else   if(p<=25){
      lv_label_set_text(per, LV_SYMBOL_BATTERY_1);
      lv_style_set_text_color(&styleBatt, LV_STATE_DEFAULT, LV_COLOR_RED);

    }
else    if(p<=5){
      lv_label_set_text(per, LV_SYMBOL_BATTERY_EMPTY);
      lv_style_set_text_color(&styleBatt, LV_STATE_DEFAULT, LV_COLOR_RED);

    }

      delay(6000);
      vTaskDelay(10);
      }

  
  vTaskDelete(NULL);
}


  
  
void paginaInici(void * parameter){
  if(!b_Connectat){
    b_Connectat=true;
      Serial.printf("Connecting to %s ", ssid);
     WiFi.begin(ssid, password);
     while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  }
  vTaskDelete(NULL);
}

void printTIme(void * parameter){
  for(;;){
    if(b_Connectat){
    printLocalTime();
    delay(1000);
  }
  vTaskDelay(10);}
  vTaskDelete(NULL);
}
void setup() {
lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
      ttgo->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
    ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
    ttgo->power->clearIRQ();

    // Turn off unused power
    ttgo->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
    ttgo->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
    ttgo->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
    ttgo->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);
  ttgo->lvgl_begin();

  Serial.begin(9600);
  Serial.println(__FILE__);
  b_Connectat=false;
  createPage();
  btemp=false;
  xTaskCreate(
    paginaInici,
    "paginaInici",
    5000,
    NULL,
    0,
    NULL
  );
  xTaskCreate(
  printTIme,
  "printTIme",
  5000,
  NULL,
  1,
  NULL
  );
  xTaskCreate(
  getBat,
  "getBat",
  5000,
  NULL,
  1,
  NULL
  );
  
    xTaskCreate(
  temporizador,
  "temporizador",
  10000,
  NULL,
  4,
  NULL
  );
  
  // put your setup code here, to run once:
}


void loop() {
  // put your main code here, to run repeatedly:
  lv_task_handler();
  delay(5);

    ttgo->power->readIRQ();
  if (!estat) {
    if ( ttgo->power->isPEKShortPressIRQ() ) {
      apaga();
      
    }
    
  }
  else if (estat) {
    if ( ttgo->power->isPEKShortPressIRQ()) {
        encen();
        lv_obj_set_hidden(pagCal, true);
        lv_obj_set_hidden(paginaPrin, false);
    }
  }
  ttgo->power->clearIRQ();
}