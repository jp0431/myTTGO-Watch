#include "config.h"
#include <stdint.h>
#include <WiFi.h>
#include "time.h"
#include <SPI.h>
#include <TTGO.h>
#include "credentials.h"
#include "lvgl/lvgl.h"
const char* ntpServer = "3.es.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
static int b;
static int c;
BMA *sensor;
bool irq = false;
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
static lv_obj_t *minut2; 
static lv_obj_t *sec2;
static lv_obj_t *usb;
lv_obj_t * mbox1;
static bool bencen;
char timeHour[3];
char timeMin[3];
char timeSec[3];
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
lv_obj_t *home3;

lv_obj_t *play;
lv_obj_t *pauseBT;
lv_obj_t *play2;
lv_obj_t *pauseBT2;
lv_obj_t *plus;
lv_obj_t *minus;
lv_obj_t *step;

static bool btemp;
static bool bcron;

static bool bpause;

static bool b_Connectat;
static bool estat;
void apaga() {
  ttgo->displaySleep();
  ttgo->power->setPowerOutPut(AXP202_LDO3, false);
  ttgo->power->setPowerOutPut(AXP202_LDO4, false);
  ttgo->power->setPowerOutPut(AXP202_LDO2, false);
  ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
  ttgo->power->setPowerOutPut(AXP202_DCDC2, false);
  estat = true;
  bencen=false;
  
}
void encen() {
  ttgo->displayWakeup();
  ttgo->power->setPowerOutPut(AXP202_LDO3, true);
  ttgo->power->setPowerOutPut(AXP202_LDO4, true);
  ttgo->power->setPowerOutPut(AXP202_LDO2, true);
  ttgo->power->setPowerOutPut(AXP202_EXTEN, true);
  ttgo->power->setPowerOutPut(AXP202_DCDC2, true);
  bencen=true;
  estat = false;
 }

void temporizador(void * parameter){
  
  for(;;){

    if(btemp){
      String szSec(lv_label_get_text(sec));
      String szMin(lv_label_get_text(minut));
      int a=szSec.toInt();
      int nMin=szMin.toInt();
    if(a>=0&&a<60){
      if(nMin>=1&&a==0){
        a=60;
        nMin-=1;
      }
      String Szte="min: " + String(nMin) +" Sec: " +String(a);
      Serial.println(Szte);
      if(nMin<10){
        String szm="0"+String(nMin)+":";
        lv_label_set_text(minut,String(szm).c_str());
      }
      else{
        String szm=String(nMin)+":";
        lv_label_set_text(minut,String(szm).c_str());
      }
      //lv_label_set_text(minut,String(nMin).c_str());
      a-=1;
  //    Serial.println(String(a));
      if(a<10){
        String szm="0"+String(a);
        lv_label_set_text(sec,String(szm).c_str());
      }
      else{
        lv_label_set_text(sec, String(a).c_str());
      }

    }
    if(a==0&&nMin==0){
        //  static const char * btns[] ={"Apply", "Close", ""};

      ttgo->motor->onec(500);
      lv_obj_set_hidden(mbox1, false);

     // lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Hello", "This is a message box with two buttons.", btns, true);
      if(estat){
        encen();
      }
     // static const char * btns[] ={"Apply", "Close", ""};
    
    // lv_obj_t * mbox1=lv_msgbox_create(NULL,cal);
   // lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Hello", "This is a message box with two buttons.", btns, true);
      //lv_obj_align(mbox1);

      btemp=false;

    }
    delay(1000);
    }
    //btemp=false;
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


strftime(timeHour,3, "%H", &timeinfo);
//Serial.println(timeHour);
strftime(timeMin,3, "%M", &timeinfo);
//Serial.println(timeMin);

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
if(!btemp){
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
  }}
  if(obj==play && event==LV_EVENT_CLICKED){
    btemp=true;

  }
  if(obj==pauseBT && event==LV_EVENT_CLICKED){
    btemp=false;
    Serial.println(btemp);
    c++;
    if(c==2){
      lv_label_set_text(sec, "00");
      lv_label_set_text(minut, "00:");
      c=0;

    }
  }

}
static void opcions_menu(lv_obj_t *obj, lv_event_t event){
  if(obj==cal && event==LV_EVENT_CLICKED){
     lv_obj_set_hidden(pagOpc, true);
     lv_obj_set_hidden(pagCal, false);
     lv_calendar_set_today_date(calendar, &date);
     lv_calendar_set_showed_date(calendar, &date);
  }
  if(obj==cron && event==LV_EVENT_CLICKED){
  lv_obj_set_hidden(pagCron, false);
  lv_obj_set_hidden(pagOpc, true);  }
  if(obj==temp && event==LV_EVENT_CLICKED){
    lv_obj_set_hidden(pagTemp, false);
    lv_obj_set_hidden(pagOpc, true);

  }
   if(obj==home || obj==home2 || obj==home3 && event==LV_EVENT_CLICKED){
     lv_obj_set_hidden(pagOpc, true);
      lv_obj_set_hidden(pagTemp, true);
      lv_obj_set_hidden(pagCron, true);


     lv_obj_set_hidden(paginaPrin, false);
    }
}
static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
       lv_obj_set_hidden(pagCal, true);
              lv_obj_set_hidden(paginaPrin, false);

    }
}
static void opcions_cron(lv_obj_t *obj, lv_event_t event){
  if(obj==play2){
    bcron=true;
  }
  if(obj==pauseBT2&&event==LV_EVENT_CLICKED){
  
    bcron=false; 
    b++;
    if(b==2){
      lv_label_set_text(sec2, "00");
      lv_label_set_text(minut2, "00:");
      b=0;

    }
    Serial.println(b);
  }
}
void createPage(){
static lv_style_t styleHora;
lv_style_init(&styleHora);
static lv_style_t styleData;
lv_style_init(&styleData);
static lv_style_t styleBatt2;
lv_style_init(&styleBatt2);
static lv_style_t style2;
lv_style_init(&style2);

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
usb = lv_label_create(paginaPrin, NULL);
step=lv_label_create(paginaPrin, NULL);


lv_obj_add_style(data,LV_LABEL_PART_MAIN,&styleData);
lv_obj_add_style(hora,LV_LABEL_PART_MAIN,&styleHora);
lv_obj_add_style(batt,LV_LABEL_PART_MAIN,&styleBatt2);
lv_obj_add_style(per,LV_LABEL_PART_MAIN,&styleBatt);

lv_obj_add_style(usb,LV_LABEL_PART_MAIN,&styleBatt);

lv_obj_set_pos(hora, 60, 10);
lv_obj_set_pos(data, 25, 50);
lv_obj_set_pos(batt, 50, 80);
lv_obj_set_pos(per, 120, 80);
lv_obj_set_pos(usb, 200, 120);
lv_obj_set_pos(step, 50, 120);
lv_obj_t * label;
lv_obj_t *btn1 = lv_btn_create(paginaPrin, NULL);
lv_obj_set_size(btn1, 75,50);
lv_obj_set_event_cb(btn1, obrir_opcions);
lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 60);

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
  lv_obj_set_event_cb(calendar, event_handler);

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
lv_obj_set_hidden(pagTemp, true);


pagCron =lv_cont_create(lv_scr_act(), NULL);
lv_obj_set_width( pagCron, lv_disp_get_hor_res( NULL )  );  
lv_obj_set_height(pagCron, lv_disp_get_ver_res( NULL ) );
home3 = lv_btn_create(pagCron, NULL);
lv_obj_set_event_cb(home3, opcions_menu);
lv_obj_align(home3, NULL, LV_ALIGN_CENTER, 30, 80);

lv_obj_add_style(home3, LV_LABEL_PART_MAIN,&styleBT);
lv_obj_t *label7 = lv_label_create(home3, NULL);
lv_label_set_text(label7, LV_SYMBOL_HOME);
lv_obj_set_size(home3, 75,50);

minut2 = lv_label_create(pagCron, NULL);
lv_obj_align(minut2,NULL,LV_ALIGN_CENTER, -20, -40);
lv_obj_add_style(minut2, LV_LABEL_PART_MAIN,&styleHora);
lv_label_set_text(minut2, "00:");

sec2 = lv_label_create(pagCron, NULL);
lv_obj_align(sec2,NULL,LV_ALIGN_CENTER, 30, -40);
lv_obj_add_style(sec2, LV_LABEL_PART_MAIN,&styleHora);
lv_label_set_text(sec2, "00");

play2 = lv_btn_create(pagCron, NULL);
lv_obj_set_event_cb(play2, opcions_cron);

lv_obj_align(play2,NULL,LV_ALIGN_CENTER, -5, 25);

lv_obj_t *playbt2 = lv_label_create(play2, NULL);
lv_label_set_text(playbt2, LV_SYMBOL_PLAY);
lv_obj_set_size(play2, 75,50);

pauseBT2 = lv_btn_create(pagCron, NULL);
lv_obj_set_event_cb(pauseBT2, opcions_cron);

lv_obj_align(pauseBT2,NULL,LV_ALIGN_CENTER, 70, 25);
lv_obj_t *pausebt2 = lv_label_create(pauseBT2, NULL);
lv_label_set_text(pausebt2, LV_SYMBOL_PAUSE);
lv_obj_set_size(pauseBT2, 75,50);
lv_obj_set_hidden(pagCron, true);

   static const char * btns[] ={"D'acord", ""};

     mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
    lv_msgbox_set_text(mbox1, "S'ha acabat el temps.");
    lv_msgbox_add_btns(mbox1, btns);
    lv_obj_set_width(mbox1, 200);
        lv_obj_set_height(mbox1, 200);

    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0);
 lv_obj_set_hidden(mbox1, true);


}
void Sleep(void * parameter){
  for(;;){
    if(bencen){
      
        delay(20000);
        apaga();
        bencen=false;
      }
  vTaskDelay(10);}
  vTaskDelete(NULL);
}
void getBat(void * parameter){
  for(;;){
    if(ttgo->power->isChargeing()){
      lv_obj_set_hidden(usb, false);

      lv_label_set_text(usb, LV_SYMBOL_USB);
    }
    else{
      lv_obj_set_hidden(usb, true);
    }
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

    if(p<=25){
      lv_label_set_text(per, LV_SYMBOL_BATTERY_1);
      lv_style_set_text_color(&styleBatt, LV_STATE_DEFAULT, LV_COLOR_RED);

    }
    if(p<=5){
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
void cronometre(void * parameter){
  for(;;){
    if(bcron){
      String szSec(lv_label_get_text(sec2));
      String szMin(lv_label_get_text(minut2));
      int a=szSec.toInt();
      int nMin=szMin.toInt();
      bencen=false;
    if(a>=0){
      a++;
       if(a>=59){
        a=0;
        nMin++;}
      if(a<10){
        String szm="0"+String(a);
        lv_label_set_text(sec2,String(szm).c_str());
      }
      else{
        lv_label_set_text(sec2, String(a).c_str());
      }
     
      if(nMin<10){
        String szm="0"+String(nMin)+":";
        lv_label_set_text(minut2,String(szm).c_str());
      }
      else{
        String szm=String(nMin)+":";
        lv_label_set_text(minut2, szm.c_str());
      
      }
      delay(1000);
     // Serial.println(bcron);
      Serial.println(String(a));
   
      }
   

    }
      vTaskDelay(10);
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
void enableBma(){
      sensor = ttgo->bma;

    // Accel parameter structure
    Acfg cfg;
    /*!
        Output data rate in Hz, Optional parameters:
            - BMA4_OUTPUT_DATA_RATE_0_78HZ
            - BMA4_OUTPUT_DATA_RATE_1_56HZ
            - BMA4_OUTPUT_DATA_RATE_3_12HZ
            - BMA4_OUTPUT_DATA_RATE_6_25HZ
            - BMA4_OUTPUT_DATA_RATE_12_5HZ
            - BMA4_OUTPUT_DATA_RATE_25HZ
            - BMA4_OUTPUT_DATA_RATE_50HZ
            - BMA4_OUTPUT_DATA_RATE_100HZ
            - BMA4_OUTPUT_DATA_RATE_200HZ
            - BMA4_OUTPUT_DATA_RATE_400HZ
            - BMA4_OUTPUT_DATA_RATE_800HZ
            - BMA4_OUTPUT_DATA_RATE_1600HZ
    */
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    /*!
        G-range, Optional parameters:
            - BMA4_ACCEL_RANGE_2G
            - BMA4_ACCEL_RANGE_4G
            - BMA4_ACCEL_RANGE_8G
            - BMA4_ACCEL_RANGE_16G
    */
    cfg.range = BMA4_ACCEL_RANGE_2G;
    /*!
        Bandwidth parameter, determines filter configuration, Optional parameters:
            - BMA4_ACCEL_OSR4_AVG1
            - BMA4_ACCEL_OSR2_AVG2
            - BMA4_ACCEL_NORMAL_AVG4
            - BMA4_ACCEL_CIC_AVG8
            - BMA4_ACCEL_RES_AVG16
            - BMA4_ACCEL_RES_AVG32
            - BMA4_ACCEL_RES_AVG64
            - BMA4_ACCEL_RES_AVG128
    */
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

    /*! Filter performance mode , Optional parameters:
        - BMA4_CIC_AVG_MODE
        - BMA4_CONTINUOUS_MODE
    */
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;

    // Configure the BMA423 accelerometer
    sensor->accelConfig(cfg);

    // Enable BMA423 accelerometer
    // Warning : Need to use steps, you must first enable the accelerometer
    // Warning : Need to use steps, you must first enable the accelerometer
    // Warning : Need to use steps, you must first enable the accelerometer
    sensor->enableAccel();

    pinMode(BMA423_INT1, INPUT);
    attachInterrupt(BMA423_INT1, [] {
        // Set interrupt to set irq value to 1
        irq = 1;
    }, RISING); //It must be a rising edge

    // Enable BMA423 step count feature
    sensor->enableFeature(BMA423_STEP_CNTR, true);

    // Reset steps
    sensor->resetStepCounter();

    // Turn on step interrupt
    sensor->enableStepCountInterrupt();
}
void StepCount(void * parameter){
  for(;;){
      uint32_t steps;
              if((String(timeHour).toInt()==11&&String(timeMin).toInt()==3&&String(timeSec).toInt()==0)){
              steps=0;
            }
     if (irq) {
        irq = 0;
        bool  rlst;
        do {
            // Read the BMA423 interrupt status,
            // need to wait for it to return to true before continuing
            rlst =  sensor->readInterrupt();
        } while (!rlst);

        // Check if it is a step interrupt
        if (sensor->isStepCounter()) {
          
         
            // Get step data from register
           steps = sensor->getCounter();
           lv_label_set_text(step,String(steps).c_str());
     


            Serial.println(steps);
           
        }
    }
    delay(20);
  }
}
void setup() {
lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
 // mpu=ttgo->mpu();
  
      ttgo->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
    ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
    ttgo->power->clearIRQ();
          ttgo->motor_begin();
enableBma();
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
    encen();

  btemp=false;
  estat=true;
  bcron=false;
  bpause=false;
b=0;
c=0;
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
  cronometre,
  "cronometre",
  5000,
  NULL,
  1,
  NULL
  );
    xTaskCreate(
  Sleep,
  "Sleep",
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
  StepCount,
  "StepCount",
  5000,
  NULL,
  0,
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
  vTaskDelay(10);
    ttgo->power->readIRQ();
  if (!estat) {
    if ( ttgo->power->isPEKShortPressIRQ() ) {
      apaga();
      
    }
    
  }
  else if (estat) {
    if ( ttgo->power->isPEKShortPressIRQ()) {
        encen();
        bencen=true;
     
    }
  }
  ttgo->power->clearIRQ();
}