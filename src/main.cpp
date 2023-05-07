#include <Arduino.h>
#include <M5ModuleDisplay.h>
#include <M5Unified.h>
#include <EEPROM.h>
#include "displayControl.hpp"
#include "forms.hpp"

static int cycleValueStore;

extern unsigned char icon_Setting[2143];
extern unsigned char icon_Power[1624];
extern unsigned char icon_M5LOGO[13611];
extern unsigned char icon_Save[1624];

extern unsigned char btn_OK[1859];
extern unsigned char btn_CANCEL[2815];
extern unsigned char btn_Right[1407];
extern unsigned char btn_Left[1417];

/// @brief Main Display
M5GFX Display_Main;
M5Canvas Display_Main_Canvas(&Display_Main);

/// @brief Module
M5GFX Display_Module;
M5Canvas Display_Module_Canvas(&Display_Module);

bool DisplayMainLoaded = false;
bool DisplayModuleLoaded = false;

void InitializeComponent()
{

    M5.Lcd.printf("Start MainMonitor Initialize\r\n"); // LCDに表示

    // Main Display initialize
    DisplayMainLoaded = M5.setPrimaryDisplayType(m5gfx::board_M5StackCore2);
    // M5GFX initialize
    Display_Main = M5.Display;
    int w = Display_Main.width();
    int h = Display_Main.height();

    // Create sprite for MainDisplay
    Display_Main_Canvas.createSprite(w, h);
    Display_Main_Canvas.setTextColor(0xffd500);
    Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_12);
    Display_Main_Canvas.setTextColor(0xffd500);

    M5.Lcd.printf("Start Module Monitor Initialize\r\n"); // LCDに表示

    DisplayModuleLoaded = M5.setPrimaryDisplayType(m5gfx::board_M5ModuleDisplay);

    if (!DisplayModuleLoaded)
    {
        delay(1000);
        DisplayModuleLoaded = M5.setPrimaryDisplayType(m5gfx::board_M5ModuleDisplay);
        if (!DisplayModuleLoaded)
        {
            Display_Main_Canvas.drawString("DISPLAY Module not found.\r\n...system reboot...", 2, 2);
            Display_Main_Canvas.pushSprite(0, 0);
            delay(2000);
            ESP.restart();
        }
    }

    // Create sprite for Module Display
    Display_Module = M5.Display;
    w = Display_Module.width();
    h = Display_Module.height();
    Display_Module_Canvas.createSprite(w, h);

    // PrimaryDisplay Reset
    M5.setPrimaryDisplayType(m5gfx::board_M5StackCore2);
}

void Display_Module_Pattern_draw(int stripeCycle)
{
    Display_Module_Canvas.fillScreen(BLACK);
    int iMax = Display_Module_Canvas.width();
    int jMax = Display_Module_Canvas.height();

    for (int i = 0; i < iMax; i += stripeCycle)
    {
        Display_Module_Canvas.fillRect(i, 0, stripeCycle / 2, jMax, WHITE);
    }

    Display_Module_Canvas.pushSprite(0, 0);
}

form *FormView;
form_Top Form_Top;
form_ShutdownMessage Form_ShutdownMessage;
form_SaveMessage Form_SaveMessage;

struct DATA_SET
{
    int cycleValueStored;
};
DATA_SET data;
int cycleValue_LastData;

void setup()
{
    EEPROM.begin(50); // 50byte
    EEPROM.get<DATA_SET>(0, data);

    if (data.cycleValueStored < 2)
    {
        cycleValue_LastData = 20;
    }
    else
    {
        cycleValue_LastData = data.cycleValueStored;
    }
    Serial.println("\nEEPROM " + String(cycleValue_LastData));

    auto cfg = M5.config();
    cfg.serial_baudrate = 19200;

    cfg.external_speaker.module_display = true;
    cfg.external_display.module_display = true;
    cfg.module_display.logical_width = 800;
    cfg.module_display.logical_height = 480;
    //  cfg.module_display.refresh_rate = 60;

    M5.begin(cfg);
    M5.Lcd.setCursor(0, 0);
    Serial.println("\nInitializeComponent");

    InitializeComponent();
    Display_Module_Pattern_draw(cycleValue_LastData);

    Form_Top = form_Top(Display_Main_Canvas, cycleValue_LastData);
    Form_ShutdownMessage = form_ShutdownMessage(Display_Main_Canvas, 0);
    Form_SaveMessage = form_SaveMessage(Display_Main_Canvas, 0);
    FormView = &Form_Top;

    FormView->formEnable = true;
    FormView->draw();
}

static m5::touch_state_t prev_state;
static constexpr const char *state_name[16] =
    {"none", "touch", "touch_end", "touch_begin", "___", "hold", "hold_end", "hold_begin", "___", "flick", "flick_end", "flick_begin", "___", "drag", "drag_end", "drag_begin"};

int prev_x = 0;
int prev_y = 0;
int cycleValue = 0;

void loop(void)
{

    M5.update();
    auto count = M5.Touch.getCount();
    if (count)
    {
        static m5::touch_state_t prev_state;
        auto t = M5.Touch.getDetail();
        if (prev_state != t.state)
        {
            prev_state = t.state;

            if (prev_x != t.x || prev_y != t.y)
            {
                prev_x = t.x;
                prev_y = t.y;

                int touchIndex = FormView->touchCheck(t);

                if (touchIndex && FormView == &Form_Top)
                {
                    Serial.println("\nForm_Top tIndex = " + String(touchIndex));
                    switch (touchIndex)
                    {
                    case 1:
                        FormView = &Form_ShutdownMessage;
                        FormView->draw();
                        break;
                    case 2:
                        FormView = &Form_ShutdownMessage;
                        FormView->draw();
                        break;
                    case 3:
                    case 4:
                    case 5:
                        cycleValue = FormView->valueString.toInt();
                        FormView->draw();
                        break;
                    case 6:
                        FormView = &Form_SaveMessage;
                        FormView->draw();
                        break;
                    default:
                        FormView = &Form_Top;
                        FormView->draw();
                    }

                    if (cycleValue_LastData != cycleValue)
                    {
                        cycleValue_LastData = cycleValue;
                        Display_Module_Pattern_draw(cycleValue);
                    }

                    touchIndex = 0;
                }
                else if (touchIndex && FormView == &Form_ShutdownMessage)
                {
                    Serial.println("\nForm_ShutdownMessage tIndex = " + String(touchIndex));
                    switch (touchIndex)
                    {
                    case 1:
                        M5.Power.powerOff();
                        break;
                    case 2:
                        FormView = &Form_Top;
                        FormView->draw();
                        break;
                    default:
                        FormView = &Form_Top;
                        FormView->draw();
                    }

                    touchIndex = 0;
                }
                else if (touchIndex && FormView == &Form_SaveMessage)
                {
                    Serial.println("\nForm_SaveMessage tIndex = " + String(touchIndex));
                    switch (touchIndex)
                    {
                    case 1:
                        if (cycleValue_LastData != data.cycleValueStored)
                        {
                            data.cycleValueStored = cycleValue_LastData;
                            EEPROM.put<DATA_SET>(0, data);
                            EEPROM.commit();
                        }
                    case 2:
                        FormView = &Form_Top;
                        FormView->draw();
                        break;
                    default:
                        FormView = &Form_Top;
                        FormView->draw();
                    }

                    touchIndex = 0;
                }
            }
        }
    }
    delay(10);
}