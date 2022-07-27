#include "Popup.h"
#include "../TSC_Menu.h"
#include "../../../ui_api.h"
#include "../../../../../libs/buzzer.h"
#include "../../../../../gcode/gcode.h"
#include "../../../../../lcd_show_addr.h"
// #include "../../../../../inc/MarlinConfigPre.h"


#define BUTTON_NUM 1

uint8_t ABL_STATUS = ABL_STANDBY;

BUTTON bottomSingleBtn = {
  //button location                       color before pressed   color after pressed
  POPUP_RECT_SINGLE_CONFIRM, NULL, 5, 1, GREEN, BLACK, WHITE,   GREEN, WHITE, BLACK
};
BUTTON bottomDoubleBtn[] = {
  {POPUP_RECT_DOUBLE_CONFIRM, NULL, 5, 1, GREEN, BLACK, WHITE,   GREEN, WHITE, BLACK},
  {POPUP_RECT_DOUBLE_CANCEL,  NULL, 5, 1, GREEN, BLACK, WHITE,   GREEN, WHITE, BLACK},
};
BUTTON bottomThreeBtn[] = {
  {POPUP_RECT_THREE_CONFIRM, NULL, 5, 1, GREEN, BLACK, WHITE,   GREEN, WHITE, BLACK},
  {POPUP_RECT_THREE_CANCEL,  NULL, 5, 1, GREEN, BLACK, WHITE,   GREEN, WHITE, BLACK},
  {POPUP_RECT_THREE_DELETE,  NULL, 5, 1, GREEN, BLACK, WHITE,   GREEN, WHITE, BLACK},
};

GUI_RECT threeBtnRect[] = {POPUP_RECT_THREE_CONFIRM, POPUP_RECT_THREE_CANCEL, POPUP_RECT_THREE_DELETE};
GUI_RECT doubleBtnRect[] = {POPUP_RECT_DOUBLE_CONFIRM, POPUP_RECT_DOUBLE_CANCEL};
GUI_RECT singleBtnRect = POPUP_RECT_SINGLE_CONFIRM;

WINDOW window = {
  POPUP_RECT_WINDOW,       //
  10,                      //
  3,                       //
  0x5D7B,                  //
  {MAGENTA, 0x5D7B, POPUP_TITLE_HEIGHT},   //
  {WHITE, BLACK, POPUP_TEXT_HEIGHT},    //
  {WHITE, GRAY,  POPUP_BOTTOM_HEIGHT},  //
};

static BUTTON *windowButton =  NULL;
static uint16_t buttonNum = 0;
static bool filament_runout_flag = false;   //no filament start
static bool SF_popup = false;               // 是否为提示成功或失败的窗口
static bool SOF = true;                     //success or failed,can change color

void windowReDrawButton(uint8_t positon, uint8_t pressed)
{
  if(positon >= buttonNum)            return;
  if(pressed >= 2)                    return;
  if(windowButton == NULL)            return;
  if(windowButton->context == NULL)   return;

  GUI_DrawButton(windowButton + positon, pressed);
}
void changeSOF(bool _SOF)
{
  SOF = _SOF;
  SF_popup = true;
}
void popupDrawPage(BUTTON *btn, const uint8_t *title, const uint8_t *context, const uint8_t *yes, const uint8_t *no)
{
 #if ENABLED(USART_LCD)
/*
  char dwin_send_Y[16] = {0};
  char dwin_send_N[16] = {0};
  char dwin_send_T[32] = {0};
  char dwin_send_I[256] = {0};
  char dwin_send_S[] = {0x5A, 0xA5, 0x07, 0x82, 0x00, 0x84, 0x5A, 0x01, 0x00, 0x15};

  PORT_REDIRECT(SERIAL_BOTH);
  // yes button
  if(yes == NULL){
    addHeadMess(dwin_send_Y);
    dwin_send_Y[4] = INFO_CONFIRM >> 8;   dwin_send_Y[5] = INFO_CONFIRM & 0xff;
    dwin_send_Y[2] = 10 + 3;
    send_hexPGM(dwin_send_Y, dwin_send_Y[2]+3);
  }
  else{
    addHeadMess(dwin_send_Y);
    dwin_send_Y[4] = INFO_CONFIRM >> 8;   dwin_send_Y[5] = INFO_CONFIRM & 0xff;
    sprintf_P(&dwin_send_Y[6], "%s", yes);
    dwin_send_Y[2] = strlen(&dwin_send_Y[6]) + 3;
    send_hexPGM(dwin_send_Y, dwin_send_Y[2]+3);
  }
  delay(10);
  
  // no button
  if(no == NULL){
    addHeadMess(dwin_send_N);
    dwin_send_N[4] = INFO_BACK >> 8;      dwin_send_N[5] = INFO_BACK & 0xff;
    dwin_send_N[2] = 10 + 3;
    send_hexPGM(dwin_send_N, dwin_send_N[2] + 3);
  }
  else{
    addHeadMess(dwin_send_N);
    dwin_send_N[4] = INFO_BACK >> 8;      dwin_send_N[5] = INFO_BACK & 0xff;
    sprintf_P(&dwin_send_N[6], "%s", no);
    dwin_send_N[2] = strlen(&dwin_send_N[6]) + 3;
    send_hexPGM(dwin_send_N, dwin_send_N[2] + 3);
  }
  delay(10);
  
  // titel
  addHeadMess(dwin_send_T);
  dwin_send_T[4] = INFO_TITEL >> 8;       dwin_send_T[5] = INFO_TITEL & 0xff;
  sprintf_P(&dwin_send_T[6], "%s", title);
  dwin_send_T[2] = 32-3;
  send_hexPGM(dwin_send_T, dwin_send_T[2] + 3);
  delay(10);

  // Info
  addHeadMess(dwin_send_I);
  dwin_send_I[4] = INFO_MESSAGE >> 8;     dwin_send_I[5] = INFO_MESSAGE & 0xff;
  sprintf_P(&dwin_send_I[6], "%s", context);
  dwin_send_I[2] = 100-3;
  send_hexPGM(dwin_send_I, dwin_send_I[2] + 3);
  delay(10);
  
  // changed interface
  send_hexPGM(dwin_send_S, sizeof(dwin_send_S));
*/
 #else

  buttonNum = 0;
  windowButton = btn;
  if(yes)
  {
    windowButton[buttonNum++].context = yes;
  }
  if(no)
  {
    windowButton[buttonNum++].context = no;
  }
  
  TSC_ReDrawIcon = windowReDrawButton;
  if(SF_popup){
    SF_popup = false;
    GUI_DrawWindow_SF(&window, title, context, SOF);
  }
  else
    GUI_DrawWindow(&window, title, context);
  
  for(uint8_t i = 0; i < buttonNum; i++)
    GUI_DrawButton(&windowButton[i], 0);

 #endif
}
void popupDrawPage_T(BUTTON *btn, const uint8_t *title, const uint8_t *context, const uint8_t *yes, const uint8_t *no, const uint8_t *del)
{
  buttonNum = 0;
  windowButton = btn;
  if(yes){
    windowButton[buttonNum++].context = yes;
  }
  if(no){
    windowButton[buttonNum++].context = no;
  }
  if(del){
    windowButton[buttonNum++].context = del;
  }
  
  TSC_ReDrawIcon = windowReDrawButton;
  if(SF_popup){
    SF_popup = false;
    GUI_DrawWindow_SF(&window, title, context, SOF);
  }
  else
    GUI_DrawWindow(&window, title, context);
  
  for(uint8_t i = 0; i < buttonNum; i++)
    GUI_DrawButton(&windowButton[i], 0);
}

void menuCallBackPopup(void)
{
  uint16_t key_num = KEY_GetValue(BUTTON_NUM, &singleBtnRect);
  switch(key_num)
  {            
    case KEY_POPUP_CONFIRM:
      infoMenu.cur--;
      //if no filament, load extruder menu
      if(filament_runout_flag){
        filament_runout_flag = false;
        //load extruder ui
        pause_extrude_flag = true;  // 暂停状态启动挤出界面
        infoMenu.menu[++infoMenu.cur] = menuExtrude;
      }
      break;
    
    default:
      break;            
  }
}
void menuCallBackPopup_B(void)
{
  uint16_t key_num = KEY_GetValue(BUTTON_NUM, &singleBtnRect);
  switch(key_num)
  {            
    case KEY_POPUP_CONFIRM:
      infoMenu.cur--;
      autoCloseBabysetp = true;
      break;
    
    default:
      break;            
  }
}

void menuCallBackPopup_ABL(void)
{
  static uint16_t delay_ms_count = 0;
  static uint8_t temp_silent = infoSettings.silent;
  char context[100];
  
  uint8_t E0_temp = (uint8_t)getActualTemp_celsius(ExtUI::E0);
  uint8_t BED_temp = (uint8_t)getActualTemp_celsius(ExtUI::BED);
  uint16_t key_num = KEY_GetValue(BUTTON_NUM, &singleBtnRect);

  switch(key_num)
  {            
    case 0: 
      if(ABL_STATUS != ABL_LEVELING)
      {
        setTargetTemp_celsius(0,ExtUI::E0);
        setTargetTemp_celsius(0,ExtUI::BED);
        ABL_STATUS = ABL_STANDBY;
        infoMenu.cur--; 
      }
      break;
    
    default:
      switch(ABL_STATUS)
      {
        case ABL_INIT    :  sprintf_P(context, "%s:%3d/150   %s:%3d/50\n%s %s", GET_TEXT(MSG_UBL_HOTEND_TEMP_CUSTOM), (uint8_t)getActualTemp_celsius(ExtUI::E0),
                                                                              GET_TEXT(MSG_UBL_BED_TEMP_CUSTOM), (uint8_t)getActualTemp_celsius(ExtUI::BED),
                                                                              GET_TEXT(MSG_FILAMENT_CHANGE_HEATING), GET_TEXT(MSG_FILAMENT_CHANGE_INIT));
                            popupDrawPage(&bottomSingleBtn , textSelect(LABEL_TIPS), (uint8_t *)context, textSelect(LABEL_CANNEL), NULL);

                            setTargetTemp_celsius(155,ExtUI::E0);
                            setTargetTemp_celsius(50,ExtUI::BED);
                            ABL_STATUS = ABL_HEATING;
                            break;

        case ABL_HEATING :  ExtUI::delay_ms(1);
                            delay_ms_count ++;
                            if(delay_ms_count >= 1000)
                            {
                              delay_ms_count = 0;
                              sprintf_P(context, "%s:%3d/150   %s:%3d/50\n%s %s", GET_TEXT(MSG_UBL_HOTEND_TEMP_CUSTOM), E0_temp,
                                                                                  GET_TEXT(MSG_UBL_BED_TEMP_CUSTOM), BED_temp,
                                                                                  GET_TEXT(MSG_FILAMENT_CHANGE_HEATING), GET_TEXT(MSG_FILAMENT_CHANGE_INIT));
                              
                              GUI_DrawWindow_ABL(&window, textSelect(LABEL_TIPS), (uint8_t *)context);


                              if(E0_temp >= 150 && BED_temp >= 50) ABL_STATUS = ABL_START;
                            }
                            break;

        case ABL_START :    if(queue.length>0){break;}
                            storeCmd("G28");     //home 
                            storeCmd("G29");     //start ABL
                            storeCmd("M500");    //save ABL info
                            storeCmd("G28");     //home

                            sprintf_P(context, "%s %s\n%s", GET_TEXT(MSG_BILINEAR_LEVELING), GET_TEXT(MSG_FILAMENT_CHANGE_LOAD), GET_TEXT(MSG_FILAMENT_CHANGE_INIT));
                            popupDrawPage(NULL , textSelect(LABEL_TIPS), (uint8_t *)context, NULL, NULL);

                            temp_silent = infoSettings.silent;
                            infoSettings.silent = 1;
                            ABL_STATUS = ABL_LEVELING;
                            break;

        case ABL_DONE :     sprintf_P(context, "%s %s!\n%s", GET_TEXT(MSG_BILINEAR_LEVELING), GET_TEXT(MSG_BUTTON_DONE), GET_TEXT(MSG_USERWAIT));
                            popupDrawPage(&bottomSingleBtn , textSelect(LABEL_TIPS), (uint8_t *)context, textSelect(LABEL_CONFIRM), NULL);
                            setTargetTemp_celsius(0,ExtUI::E0);
                            setTargetTemp_celsius(0,ExtUI::BED);
                            infoSettings.silent = temp_silent;
                            ABL_STATUS = ABL_CLOSE_WINDOW;
                            break;

        case ABL_CLOSE_WINDOW : ExtUI::delay_ms(1);
                                delay_ms_count ++;
                                if(delay_ms_count >= 10000)
                                {
                                  delay_ms_count = 0;
                                  ABL_STATUS = ABL_STANDBY;
                                  infoMenu.cur--;  
                                }
                                break;
        default:;
      }
      break;
  }
}

void GUI_DrawWindow_ABL(const WINDOW *window, const uint8_t *title, const uint8_t *inf)//这个函数只更新窗口内容
{
  const uint16_t titleHeight = window->title.height;
  const uint16_t lineWidth = window->lineWidth;
  const uint16_t infoHeight = window->info.height;
  const uint16_t infoBackColor = window->info.backColor;
  const int16_t  sx = window->rect.x0,
                 sy = window->rect.y0,
                 ex = window->rect.x1;
  const uint16_t nowBackColor = GUI_GetBkColor();
  const uint16_t nowFontColor = GUI_GetColor();
  const GUI_TEXT_MODE nowTextMode = GUI_GetTextMode();

  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_SetBkColor(BLACK);
  GUI_SetColor(window->info.fontColor);
  GUI_DispStringInRect_P(sx+lineWidth+BYTE_WIDTH, sy+titleHeight, ex-lineWidth-BYTE_WIDTH, sy+titleHeight+infoHeight, inf);

  GUI_SetBkColor(nowBackColor);
  GUI_SetColor(nowFontColor);
  GUI_SetTextMode(nowTextMode); 
}

void menuPopup(void)
{
  menuSetFrontCallBack(menuCallBackPopup);
}

void menuPopup_B(void)
{
  menuSetFrontCallBack(menuCallBackPopup_B);
}

void menuPopup_ABL(void)
{
  ABL_STATUS = ABL_INIT;
  menuSetFrontCallBack(menuCallBackPopup_ABL);
}

void popupReminder_p(uint8_t* info, uint8_t* context)
{
  filament_runout_flag = true;    //this is no filament
  popupDrawPage(&bottomSingleBtn , info, context, textSelect(LABEL_CONFIRM), NULL);
 #if DISABLED(USART_LCD)
  if(infoMenu.menu[infoMenu.cur] != menuPopup)
  {
    infoMenu.menu[++infoMenu.cur] = menuPopup;
  }
 #endif
}
void popupReminder_B(uint8_t* info, uint8_t* context)
{
  popupDrawPage(&bottomSingleBtn , info, context, textSelect(LABEL_CONFIRM), NULL);
 #if DISABLED(USART_LCD)
  if(infoMenu.menu[infoMenu.cur] != menuPopup_B)
  {
    infoMenu.menu[++infoMenu.cur] = menuPopup_B;
  }
 #endif
}
void popupReminder_SF(uint8_t* info, uint8_t* context, bool _SOF)
{
  SF_popup = true;
  SOF = _SOF;
  popupDrawPage(&bottomSingleBtn , info, context, textSelect(LABEL_CONFIRM), NULL);
 #if DISABLED(USART_LCD)
  if(infoMenu.menu[infoMenu.cur] != menuPopup)
  {
    infoMenu.menu[++infoMenu.cur] = menuPopup;
  }
 #endif
}
void popupReminder(uint8_t* info, uint8_t* context)
{
  popupDrawPage(&bottomSingleBtn , info, context, textSelect(LABEL_CONFIRM), NULL);
 #if DISABLED(USART_LCD)
  if(infoMenu.menu[infoMenu.cur] != menuPopup)
  {
    infoMenu.menu[++infoMenu.cur] = menuPopup;
  }
 #endif
}

