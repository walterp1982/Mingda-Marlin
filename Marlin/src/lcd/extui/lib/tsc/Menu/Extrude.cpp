#include "../TSC_Menu.h"
#include "Extrude.h"
#if 0
  #include "../../../../../module/stepper/trinamic.h"
#endif

//1title, ITEM_PER_PAGE items
MENUITEMS extrudeItems = {
// title
LABEL_EXTRUDE,
// icon                       label
//  {{ICON_UNLOAD,               LABEL_UNLOAD}, 
 {{ICON_BACKGROUND,           LABEL_BACKGROUND},
  // {ICON_EM_STOP,              LABEL_STOP}, 
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_LOAD,                 LABEL_LOAD},
  // {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_STATUSNOZZLE,         LABEL_NOZZLE},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
  {ICON_E_5_MM,               LABEL_5_MM},
  {ICON_BACK,                 LABEL_BACK},}
};
MENUITEMS extrude2Items = {
// title
LABEL_EXTRUDE,
// icon                       label
 {{ICON_UNLOAD,               LABEL_UNLOAD}, 
//  {{ICON_BACKGROUND,           LABEL_BACKGROUND},
  // {ICON_EM_STOP,              LABEL_STOP}, 
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_LOAD,                 LABEL_LOAD},
  // {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_STATUSNOZZLE,         LABEL_NOZZLE},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
  {ICON_E_5_MM,               LABEL_5_MM},
  {ICON_BACK,                 LABEL_BACK},}
};

#define ITEM_EXTRUDER_NUM EXTRUDERS
static ExtUI::extruder_t item_extruder_i = ExtUI::E0;

#define ITEM_SPEED_NUM 3
const ITEM itemSpeed[ITEM_SPEED_NUM] = {
// icon                       label
  {ICON_SLOW_SPEED,           LABEL_SLOW_SPEED},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
  {ICON_FAST_SPEED,           LABEL_FAST_SPEED},
};
const ITEM itemStop[2] = {
  {ICON_EM_STOP,              LABEL_STOP},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
};
const  uint32_t item_speed[ITEM_SPEED_NUM] = {EXTRUDE_SLOW_SPEED, EXTRUDE_NORMAL_SPEED, EXTRUDE_FAST_SPEED};
static uint8_t  item_speed_i = 1;

#define ITEM_LEN_NUM 3
const ITEM itemLen[ITEM_LEN_NUM] = {
// icon                       label
  {ICON_E_1_MM,               LABEL_1_MM},
  {ICON_E_5_MM,               LABEL_5_MM},
  {ICON_E_10_MM,              LABEL_10_MM},
};
const  uint8_t item_len[ITEM_LEN_NUM] = {1, 5, 10};
static uint8_t item_len_i = 1;
static float extrudeCoordinate = 0.0f;

static int extrudeCoordinate2 = 0;  // ?????????????????????
static int e_add_mm = 0;    // ???????????????????????????????????????????????????
static int e_add_mm_t = 0;  // ??????????????????????????????????????????e_add_mm?????????????????????????????????
static char G1_STR[64] = {0};// ??????Gcode??????

static bool isBusy = false; // ??????????????????????????????
static uint8_t top_info_ai = 0, top_info_bi = 0, top_info_ci = 0;    // ????????????????????????????????????

bool pause_extrude_flag = false;  // ????????????????????????
#define HOTEND_SEPARATOR_X      (START_X + 0 * ICON_WIDTH + 0 * SPACE_X + (ICON_WIDTH-BYTE_WIDTH)/2)
#define STATUS_START_Y          (TITLE_END_Y +  1 * ICON_HEIGHT + 1 * SPACE_Y + SSICON_VAL_Y0)

/************************************************************************************/
void extrudeCoordinateReDraw(void)
{
  const GUI_RECT rect = {exhibitRect.x0, CENTER_Y, exhibitRect.x1, (int16_t)(CENTER_Y+BYTE_HEIGHT)};
  char buf[36];
  // sprintf_P(buf, PSTR("%.2f"), extrudeCoordinate);
  sprintf_P(buf, PSTR("%d"), extrudeCoordinate2);   // ???????????????????????????
  GUI_ClearPrect(&rect);
  GUI_DispStringInPrect(&rect, (uint8_t *)buf);     // ???????????????
}

const char* extruderDisplayID[] = EXTRUDER_ID;

void showExtrudeCoordinate(void)
{
  const GUI_RECT rect = {exhibitRect.x0, (int16_t)(CENTER_Y-BYTE_HEIGHT), exhibitRect.x1, CENTER_Y};
  item_extruder_i = ExtUI::getActiveTool();
  // extrudeCoordinate = ExtUI::getAxisPosition_mm(item_extruder_i);
  extrudeCoordinate2 = ExtUI::getAxisPosition_mm(item_extruder_i);
  GUI_ClearPrect(&rect);
  GUI_DispStringInPrect(&rect, (uint8_t *)extruderDisplayID[item_extruder_i]);
  extrudeCoordinateReDraw();
}

static void redrawHotendAct(int16_t msg)
{
  GUI_DispDec(HOTEND_SEPARATOR_X - 3*BYTE_WIDTH, STATUS_START_Y, msg, 3, RIGHT);
}
static void redrawToolTag(int16_t msg)
{
  GUI_DispDec(HOTEND_SEPARATOR_X + BYTE_WIDTH+1, STATUS_START_Y, msg, 3, LEFT);
}

void menuCallBackExtrude(void)
{
  STATUS_MSG tempMsg;
  statusMsg_GetCurMsg(&tempMsg);
  // Position refresh per 1 sec
  static uint32_t nowTime_ms = 0;
  KEY_VALUES key_num = menuKeyGetValue();

  if(queue.length == 0 && (!pause_extrude_flag)){   // G??????????????????,???????????????????????????????????????????????????????????????????????????G?????????????????????
    isBusy = false;
    if(e_add_mm != 0){
      e_add_mm_t = (getAxisPosition_mm(item_extruder_i) + e_add_mm);
      e_add_mm = 0;

      memset(G1_STR, 0, sizeof(G1_STR));    // ????????????
      switch(item_speed_i){   // ??????????????????
        case 0:
          sprintf(G1_STR, "G1 E%d F%d\n", e_add_mm_t, item_speed[item_speed_i]);
          break;
        case 1:
          sprintf(G1_STR, "G1 E%d F%d\n", e_add_mm_t, item_speed[item_speed_i]);
          break;
        case 2:
          sprintf(G1_STR, "G1 E%d F%d\n", e_add_mm_t, item_speed[item_speed_i]);
          break;
      }
      queue.enqueue_one_now(G1_STR);    // ??????????????????
    }
  }else isBusy = true;

  switch(key_num)
  {
    // case KEY_ICON_0:
    //   if(pause_extrude_flag)  // ??????????????????????????????????????????????????????????????????????????????Gcode
    //     ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(item_extruder_i) - item_len[item_len_i], item_extruder_i, item_speed[item_speed_i]);
    //   else
    //     e_add_mm -= item_len[item_len_i];   // ????????????????????????????????????
    //   break;
    case KEY_ICON_0:
      if(!pause_extrude_flag){
        stop_home = true;
        quickstop_stepper();
      }
      break;
    
    case KEY_ICON_3:
      stop_home = false;
      if(pause_extrude_flag)
        ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(item_extruder_i) + item_len[item_len_i], item_extruder_i, item_speed[item_speed_i]);
      else
        e_add_mm += item_len[item_len_i];   // ????????????????????????????????????
      break;
    
    case KEY_ICON_4:
      item_extruder_i = (ExtUI::extruder_t)((item_extruder_i + 1) % ITEM_EXTRUDER_NUM);
      showExtrudeCoordinate();
      {
        GUI_SetColor(VAL_COLOR);
        GUI_SetBkColor(WHITE);
        GUI_DispString(HOTEND_SEPARATOR_X, STATUS_START_Y, (uint8_t *)"/"); // Ext value
        statusMsg.actHotend = tempMsg.actHotend;
        redrawHotendAct(tempMsg.actHotend);
        statusMsg.tagHotend = tempMsg.tagHotend;
        redrawToolTag(tempMsg.tagHotend);
        GUI_SetColor(WHITE);
        GUI_SetBkColor(VAL_COLOR);
      }
      break;

    case KEY_ICON_5:
      item_speed_i = (item_speed_i+1) % ITEM_SPEED_NUM;
      extrudeItems.items[key_num] = itemSpeed[item_speed_i];
      menuDrawItem(&extrudeItems.items[key_num], key_num);
      break;
    
    case KEY_ICON_6:
      item_len_i = (item_len_i+1) % ITEM_LEN_NUM;
      extrudeItems.items[key_num] = itemLen[item_len_i];
      menuDrawItem(&extrudeItems.items[key_num], key_num);
      break;

    case KEY_ICON_7:
      stop_home = false;
      if(pause_extrude_flag){
        pause_extrude_flag = false;
        // wait_for_user = false;   //wait touch continue button
      }
      infoMenu.cur--; 
      break;
    
    default:
      break;
  }
  
  // if (extrudeCoordinate != ExtUI::getAxisPosition_mm(item_extruder_i)){
  //   extrudeCoordinate = ExtUI::getAxisPosition_mm(item_extruder_i);
  //   extrudeCoordinateReDraw();
  // }

  // ??????????????????
  if (millis() - nowTime_ms > 1000) { // Refresh per 1 sec
    nowTime_ms = millis();
    // Refresh position
    if(isBusy && (!pause_extrude_flag)){
      if(top_info_ai == 0){
        top_info_ai++;  top_info_bi = 0;  top_info_ci = 0;
        GUI_ClearRect(95, 0, LCD_WIDTH_PIXEL, TITLE_END_Y-10);
      }
      drawTopInfo(LABEL_BUSY);  // ??????????????????
    }
   #ifdef PREVENT_COLD_EXTRUSION
    else if(tempMsg.actHotend < EXTRUDE_MINTEMP){
      if(top_info_bi == 0){
        top_info_bi++;  top_info_ai = 0;  top_info_ci = 0;
        GUI_ClearRect(95, 0, LCD_WIDTH_PIXEL, TITLE_END_Y-10);
      }
      drawTopInfo((uint8_t*)"NOZZLE tempreature is too low !");
    }
   #endif
    else{
      if(top_info_ci == 0){
        top_info_ci++;  top_info_bi = 0; top_info_ai = 0;
        GUI_ClearRect(95, 0, LCD_WIDTH_PIXEL, TITLE_END_Y-10);
      }
    }

    GUI_SetColor(VAL_COLOR);
    GUI_SetBkColor(WHITE);
    GUI_DispString(HOTEND_SEPARATOR_X, STATUS_START_Y, (uint8_t *)"/"); // Ext value
    // Refresh hotend temperature
    if (statusMsg.actHotend != tempMsg.actHotend) {
      statusMsg.actHotend = tempMsg.actHotend;
      redrawHotendAct(tempMsg.actHotend);
    }
    if (statusMsg.tagHotend != tempMsg.tagHotend) {
      statusMsg.tagHotend = tempMsg.tagHotend;
      redrawToolTag(tempMsg.tagHotend);
    }
    GUI_SetColor(WHITE);
    GUI_SetBkColor(VAL_COLOR);
  }

  if (extrudeCoordinate2 != (int)ExtUI::getAxisPosition_mm(item_extruder_i)){
    extrudeCoordinate2 = (int)ExtUI::getAxisPosition_mm(item_extruder_i);
    extrudeCoordinateReDraw();
  }
}

void menuExtrude()
{
  e_add_mm = top_info_ai = top_info_bi = top_info_ci = 0;   // ??????????????????????????????
  statusMsg.actHotend = -1;
  statusMsg.tagHotend = -1;
  if(!pause_extrude_flag){
    extrudeItems.items[0] = itemStop[0];
  }else{
    extrudeItems.items[0] = itemStop[1];
  }
  menuDrawPage(&extrudeItems);
  showExtrudeCoordinate();
  menuSetFrontCallBack(menuCallBackExtrude);
}

void menuCallBackExtrude2(void)
{
  STATUS_MSG tempMsg;
  statusMsg_GetCurMsg(&tempMsg);
  // Position refresh per 1 sec
  static uint32_t nowTime_ms = 0;
  KEY_VALUES key_num = menuKeyGetValue();

  if(queue.length == 0 && (!pause_extrude_flag)){   // G??????????????????,???????????????????????????????????????????????????????????????????????????G?????????????????????
    isBusy = false;
    if(e_add_mm != 0){
      e_add_mm_t = (getAxisPosition_mm(item_extruder_i) + e_add_mm);
      e_add_mm = 0;

      memset(G1_STR, 0, sizeof(G1_STR));    // ????????????
      switch(item_speed_i){   // ??????????????????
        case 0:
          sprintf(G1_STR, "G1 E%d F%d\n", e_add_mm_t, item_speed[item_speed_i]);
          break;
        case 1:
          sprintf(G1_STR, "G1 E%d F%d\n", e_add_mm_t, item_speed[item_speed_i]);
          break;
        case 2:
          sprintf(G1_STR, "G1 E%d F%d\n", e_add_mm_t, item_speed[item_speed_i]);
          break;
      }
      queue.enqueue_one_now(G1_STR);    // ??????????????????
    }
  }else isBusy = true;

  switch(key_num)
  {
    case KEY_ICON_0:
     #if 0
      uint32_t sr;
      char str[64];
      write_tmc_reg(0x6C, 0xD4028103);
      sr = read_tmc_reg(0x6C);
      //sprintf_P(str, "reg:%B");
      str[0] = '6';
      str[1] = 'C';
      str[2] = ':';
      itoa(sr, str+3, 16);
      // GUI_SetColor(VAL_COLOR);
      // GUI_SetBkColor(WHITE);
      GUI_SetColor(WHITE);
      GUI_SetBkColor(VAL_COLOR);
      GUI_DispString(180, 5, (uint8_t*)str);
      sr = read_tmc_reg(0x6F);
      //sprintf_P(str, "reg:%B");
      str[0] = '6';
      str[1] = 'F';
      str[2] = ':';
      itoa(sr, str+3, 16);
      GUI_DispString(180, 30, (uint8_t*)str);
     #else
      if(pause_extrude_flag)  // ??????????????????????????????????????????????????????????????????????????????Gcode
        ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(item_extruder_i) - item_len[item_len_i], item_extruder_i, item_speed[item_speed_i]);
      else
        e_add_mm -= item_len[item_len_i];   // ????????????????????????????????????
     #endif
      break;
    // case KEY_ICON_0:
    //   if(!pause_extrude_flag){
    //     stop_home = true;
    //     quickstop_stepper();
    //   }
    //   break;
    
    case KEY_ICON_3:
      stop_home = false;
      if(pause_extrude_flag)
        ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(item_extruder_i) + item_len[item_len_i], item_extruder_i, item_speed[item_speed_i]);
      else
        e_add_mm += item_len[item_len_i];   // ????????????????????????????????????
      break;
    
    case KEY_ICON_4:
      item_extruder_i = (ExtUI::extruder_t)((item_extruder_i + 1) % ITEM_EXTRUDER_NUM);
      showExtrudeCoordinate();
      {
        GUI_SetColor(VAL_COLOR);
        GUI_SetBkColor(WHITE);
        GUI_DispString(HOTEND_SEPARATOR_X, STATUS_START_Y, (uint8_t *)"/"); // Ext value
        statusMsg.actHotend = tempMsg.actHotend;
        redrawHotendAct(tempMsg.actHotend);
        statusMsg.tagHotend = tempMsg.tagHotend;
        redrawToolTag(tempMsg.tagHotend);
        GUI_SetColor(WHITE);
        GUI_SetBkColor(VAL_COLOR);
      }
      break;

    case KEY_ICON_5:
      item_speed_i = (item_speed_i+1) % ITEM_SPEED_NUM;
      extrudeItems.items[key_num] = itemSpeed[item_speed_i];
      menuDrawItem(&extrudeItems.items[key_num], key_num);
      break;
    
    case KEY_ICON_6:
      item_len_i = (item_len_i+1) % ITEM_LEN_NUM;
      extrudeItems.items[key_num] = itemLen[item_len_i];
      menuDrawItem(&extrudeItems.items[key_num], key_num);
      break;

    case KEY_ICON_7:
      mustStoreCmd("M302 P0\n");
      stop_home = false;
      if(pause_extrude_flag){
        pause_extrude_flag = false;
        // wait_for_user = false;   //wait touch continue button
      }
      infoMenu.cur--;
      break;
    
    default:
      break;
  }
  
  // if (extrudeCoordinate != ExtUI::getAxisPosition_mm(item_extruder_i)){
  //   extrudeCoordinate = ExtUI::getAxisPosition_mm(item_extruder_i);
  //   extrudeCoordinateReDraw();
  // }

  // ??????????????????
  if (millis() - nowTime_ms > 1000) { // Refresh per 1 sec
    nowTime_ms = millis();
    // Refresh position
    if(isBusy && (!pause_extrude_flag)){
      if(top_info_ai == 0){
        top_info_ai++;  top_info_bi = 0;  top_info_ci = 0;
        GUI_ClearRect(95, 0, LCD_WIDTH_PIXEL, TITLE_END_Y-10);
      }
      drawTopInfo(LABEL_BUSY);  // ??????????????????
    }
   #if 0 //def PREVENT_COLD_EXTRUSION
    else if(tempMsg.actHotend < EXTRUDE_MINTEMP){
      if(top_info_bi == 0){
        top_info_bi++;  top_info_ai = 0;  top_info_ci = 0;
        GUI_ClearRect(95, 0, LCD_WIDTH_PIXEL, TITLE_END_Y-10);
      }
      drawTopInfo((uint8_t*)"NOZZLE tempreature is too low !");
    }
   #endif
    else{
      if(top_info_ci == 0){
        top_info_ci++;  top_info_bi = 0; top_info_ai = 0;
        GUI_ClearRect(95, 0, LCD_WIDTH_PIXEL, TITLE_END_Y-10);
      }
    }

    GUI_SetColor(VAL_COLOR);
    GUI_SetBkColor(WHITE);
    GUI_DispString(HOTEND_SEPARATOR_X, STATUS_START_Y, (uint8_t *)"/"); // Ext value
    // Refresh hotend temperature
    if (statusMsg.actHotend != tempMsg.actHotend) {
      statusMsg.actHotend = tempMsg.actHotend;
      redrawHotendAct(tempMsg.actHotend);
    }
    if (statusMsg.tagHotend != tempMsg.tagHotend) {
      statusMsg.tagHotend = tempMsg.tagHotend;
      redrawToolTag(tempMsg.tagHotend);
    }
    GUI_SetColor(WHITE);
    GUI_SetBkColor(VAL_COLOR);
  }

  if (extrudeCoordinate2 != (int)ExtUI::getAxisPosition_mm(item_extruder_i)){
    extrudeCoordinate2 = (int)ExtUI::getAxisPosition_mm(item_extruder_i);
    extrudeCoordinateReDraw();
  }
}
void menuExtrude2()
{
  e_add_mm = top_info_ai = top_info_bi = top_info_ci = 0;   // ??????????????????????????????
  statusMsg.actHotend = -1;
  statusMsg.tagHotend = -1;
  menuDrawPage(&extrude2Items);
  showExtrudeCoordinate();
  menuSetFrontCallBack(menuCallBackExtrude2);
}
