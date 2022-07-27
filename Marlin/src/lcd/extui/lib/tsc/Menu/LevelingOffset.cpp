#include "../TSC_Menu.h"
#include "../../../../../module/settings.h"

#if ENABLED(LEVELING_OFFSET)

//1 titl, ITEM_PER_PAGE item
MENUITEMS LevelingOffsetItems = {
// title
LABEL_PROBE_OFFSET,
// icon                        label
 {{ICON_DOWN,                 LABEL_DEC},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_UP,                   LABEL_INC},
  {ICON_EEPROM_SAVE,          LABEL_EEPROM_SAVE},
  {ICON_01_MM,                LABEL_01_MM},
  {ICON_NORMAL_SPEED,         LABEL_VALUE_ZERO},
  {ICON_BACK,                 LABEL_BACK},}
};

typedef struct
{
  const ITEM *list;
  const float *ele;
  uint8_t cur;
  uint8_t totaled;
}ELEMENTS;

#define ITEM_LEVELING_OFFSET_UNIT_NUM 3
const ITEM itemProbeOffsetUnit[ITEM_LEVELING_OFFSET_UNIT_NUM] = {
// icon                       label
  {ICON_001_MM,               LABEL_001_MM},
  {ICON_01_MM,                LABEL_01_MM},
  {ICON_1_MM,                 LABEL_1_MM},
};
const float item_LevelingOffset_unit[ITEM_LEVELING_OFFSET_UNIT_NUM] = {0.01f, 0.1f, 1};

static ELEMENTS elementsUnit;
float LevelingOffset = LODEVA;    // 当前值,默认0.6
float oldLevelingOffset = 0.0f; // 上一个值

static void initElements(uint8_t position)
{
  elementsUnit.totaled = ITEM_LEVELING_OFFSET_UNIT_NUM;
  elementsUnit.list = itemProbeOffsetUnit;
  elementsUnit.ele  = item_LevelingOffset_unit;

  for(uint8_t i=0; i<elementsUnit.totaled; i++)
  {
    if(memcmp(&elementsUnit.list[i], &LevelingOffsetItems.items[position], sizeof(ITEM)) == 0)
    {
      elementsUnit.cur = i;
      break;
    }
  }
}

/* 获取调平补偿 */
float getLevelingOffset(void)
{  
  return LevelingOffset;
}
/* 设置调平补偿 */
void setLevelingOffset(float offset)
{ 
 #if 1
  LevelingOffset += offset;
  if(LevelingOffset>= (BABYSTEP_MAX_HIGH*2))  LevelingOffset =  (BABYSTEP_MAX_HIGH*2);
  if(LevelingOffset<=(-BABYSTEP_MAX_HIGH*2))  LevelingOffset = (-BABYSTEP_MAX_HIGH*2);
 #else
  LevelingOffset = offset;
 #endif
}
// 置零
void resetLevelingOffset(void){
  LevelingOffset = 0.0f;
}
// 将数据保存到存储器
bool saveOffset(){
  float newLevelingOffset = LevelingOffset - oldLevelingOffset; // 求变化量
  if (leveling_is_valid() && ((int)(newLevelingOffset*100) != 0)) {   // 判断调平数据有效 且 变化不小于0.01
    LOOP_L_N(py, GRID_MAX_POINTS_Y) {
      LOOP_L_N(px, GRID_MAX_POINTS_X) {
        z_values[px][py] += newLevelingOffset;
      }
    } // 循环将数组内的数据和并。
    
    oldLevelingOffset = LevelingOffset; // 告知已将偏移值合并到调平数据内
    return settings.save(); //判断是否保存成功
  }
  return false; // 没触发保存，保存失败
}

void showLevelingOffset(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, getLevelingOffset(), 3, 2, RIGHT);
}
void LevelingOffsetReDraw(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, getLevelingOffset(), 3, 2, RIGHT);
}


void menuCallBackLevelingOffset(void)
{
  bool leveling_offset_value_change = false;    // 值是否发生变化
  float ts_value = 0.0f;   // 临时存放数据
  KEY_VALUES key_num =  menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      setLevelingOffset(- elementsUnit.ele[elementsUnit.cur]);
      leveling_offset_value_change = true;
      break;
    case KEY_ICON_3:
      setLevelingOffset(elementsUnit.ele[elementsUnit.cur]);
      leveling_offset_value_change = true;
      break;
    case KEY_ICON_4:
      if(saveOffset())
        popupReminder_SF(textSelect(LABEL_SAVE_POPUP),textSelect(LABEL_EEPROM_SAVE_SUCCESS), true);
      else
        popupReminder_SF(textSelect(LABEL_SAVE_POPUP),textSelect(LABEL_EEPROM_SAVE_FAILED), false);
      
      break;
    case KEY_ICON_5:
      elementsUnit.cur = (elementsUnit.cur + 1) % elementsUnit.totaled;
      LevelingOffsetItems.items[key_num] = elementsUnit.list[elementsUnit.cur];
      menuDrawItem(&LevelingOffsetItems.items[key_num], key_num);
      break;
    case KEY_ICON_6:
      resetLevelingOffset(); // 置零
      leveling_offset_value_change = true;
      break;
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    default :
      break;
  }
  if(leveling_offset_value_change)  // 值发生变化，显示刷新
  {
    leveling_offset_value_change = false;
    LevelingOffsetReDraw();
  }
}

void menuLevelingOffset()
{
  // oldLevelingOffset = LevelingOffset;
  initElements(KEY_ICON_5);
  menuDrawPage(&LevelingOffsetItems);
  showLevelingOffset();
  menuSetFrontCallBack(menuCallBackLevelingOffset);
}


void menuCallBackSetLevelingOffset()
{
  bool leveling_offset_value_change = false;    // 值是否发生变化
  float ts_value = 0.0f;   // 临时存放数据
  KEY_VALUES key_num =  menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      setBabyStepZAxisIncMM(-elementsUnit.ele[elementsUnit.cur]);
      setLevelingOffset(- elementsUnit.ele[elementsUnit.cur]);
      leveling_offset_value_change = true;
      break;
    case KEY_ICON_3:
      setBabyStepZAxisIncMM(elementsUnit.ele[elementsUnit.cur]);
      setLevelingOffset(elementsUnit.ele[elementsUnit.cur]);
      leveling_offset_value_change = true;
      break;
    case KEY_ICON_4:
      if(saveOffset())
        popupReminder_SF(textSelect(LABEL_SAVE_POPUP),textSelect(LABEL_EEPROM_SAVE_SUCCESS), true);
      else
        popupReminder_SF(textSelect(LABEL_SAVE_POPUP),textSelect(LABEL_EEPROM_SAVE_FAILED), false);
      
      break;
    case KEY_ICON_5:
      elementsUnit.cur = (elementsUnit.cur + 1) % elementsUnit.totaled;
      LevelingOffsetItems.items[key_num] = elementsUnit.list[elementsUnit.cur];
      menuDrawItem(&LevelingOffsetItems.items[key_num], key_num);
      break;
    case KEY_ICON_6:
      resetLevelingOffset(); // 置零
      leveling_offset_value_change = true;
      break;
    case KEY_ICON_7:
      mustStoreCmd("M420 S0\n");
      mustStoreCmd("G28\n");
      infoMenu.cur--;
      break;
    default :
      break;
  }
  if(leveling_offset_value_change)  // 值发生变化，显示刷新
  {
    leveling_offset_value_change = false;
    LevelingOffsetReDraw();
  }
}
void menuSetLevelingOffset()
{
  initElements(KEY_ICON_5);
  menuDrawPage(&LevelingOffsetItems);
  showLevelingOffset();
  menuSetFrontCallBack(menuCallBackSetLevelingOffset);
}

// LEVELINGITEMS levelingSetItems = 
uint8_t *levelingSetItems[ITEM_CUBE_NUM] = 
{
  CHAR_INCREASE,
  CHAR_LOWER,
  CHAR_SAVE,
  CHAR_SBACK,
};
void menuCallBackSetLevelingValue()
{
  KEY_VALUES key_num =  menuKeyGetLevelingValue();
  switch(key_num)
  {
    case 0:break;
    case 1:break;
    case 2:break;
    case 3:
      infoMenu.cur--;
    break;
  }
}
void menuSetLevelingValue()
{
  int the_value = 0;
  uint8_t x,y;
  for(y=0; y<GRID_MAX_POINTS_Y; y++){
    for(x=0; x<GRID_MAX_POINTS_X; x++){
      the_value = 0;
      if(leveling_is_valid())the_value = z_values[x][y] * 100;
      if(the_value == 0){
        levelingSetItems[y*GRID_MAX_POINTS_X+x+4] = (uint8_t *)"0";
      }
      else{
        sprintf_P((char*)levelingSetItems[y*GRID_MAX_POINTS_X+x+4], "%d\u002E%d", the_value/100, the_value%100);
      }
    }
  }
  GUI_Clear(BLACK);
  menuDrawCubePage(levelingSetItems);
  menuSetFrontCallBack(menuCallBackSetLevelingValue);
}

#endif
