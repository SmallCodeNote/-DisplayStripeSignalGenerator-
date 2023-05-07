#include <Arduino.h>
#include <M5ModuleDisplay.h>
#include <M5Unified.h>
#include "displayControl.hpp"

#ifndef __InitializeComponent
#define __InitializeComponent

extern unsigned char icon_Setting[2143];
extern unsigned char icon_Power[1624];
extern unsigned char icon_Right[1407];
extern unsigned char icon_Left[1417];
extern unsigned char icon_Save[1624];


class form
{
public:
    int formWidth;
    int formHeight;

    bool formEnable;
    String formName;
    String valueString;
    M5Canvas Display_Main_Canvas;

    form()
    {
        formName = "BaseClass";
        formEnable = true;
    }

    form(M5Canvas formCanvas, int value)
    {
        formName = "BaseClass";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();
    }

    virtual void draw()
    {
    }

    virtual int touchCheck(m5::touch_detail_t t)
    {
        return 0;
    }
};

class form_Top : public form
{
private:
    displayButton BTN_Power;
    displayButton BTN_Config;
    displayButton BTN_UP;
    displayButton BTN_DOWN;
    displayButton BTN_SAVE;
    displaySlider SLD_StripeWidth;

public:
    form_Top()
    {
        formName = "Top";
        formEnable = true;
    }

    /// @brief Initialize Canvas / Button
    form_Top(M5Canvas formCanvas, int value)
    {
        formName = "Top";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();

        BTN_Config = displayButton();
        // Config ICON Button ===================
        BTN_Config.x = 174;
        BTN_Config.y = 187;
        BTN_Config.width = 52;
        BTN_Config.height = 52;
        BTN_Config.iconData = icon_Setting;

        BTN_Power = displayButton();
        // Power ICON Button ===================
        BTN_Power.x = 249;
        BTN_Power.y = 187;
        BTN_Power.width = 52;
        BTN_Power.height = 52;
        BTN_Power.iconData = icon_Power;
        // ================================

        BTN_UP = displayButton();
        // Power ICON Button ===================
        BTN_UP.x = 99;
        BTN_UP.y = 187;
        BTN_UP.width = 52;
        BTN_UP.height = 52;
        BTN_UP.iconData = icon_Right;
        // ================================

        BTN_DOWN = displayButton();
        // Power ICON Button ===================
        BTN_DOWN.x = 24;
        BTN_DOWN.y = 187;
        BTN_DOWN.width = 52;
        BTN_DOWN.height = 52;
        BTN_DOWN.iconData = icon_Left;
        // ================================

        SLD_StripeWidth = displaySlider();
        // SLIDER ===================
        SLD_StripeWidth.x = 35;
        SLD_StripeWidth.y = 120;
        SLD_StripeWidth.width = 250;
        SLD_StripeWidth.height = 36;
        SLD_StripeWidth.enable = true;
        SLD_StripeWidth.setValueMin(2);
        SLD_StripeWidth.setValueMax(100);
        SLD_StripeWidth.value = value;

        // ================================

        BTN_SAVE = displayButton();
        // Power ICON Button ===================
        BTN_SAVE.x = 249;
        BTN_SAVE.y = 2;
        BTN_SAVE.width = 52;
        BTN_SAVE.height = 52;
        BTN_SAVE.iconData = icon_Save;
        // ================================
    }

    void draw() override
    {
        if (formEnable)
        {
            Display_Main_Canvas.fillScreen(BLACK);
            BTN_Config.draw(Display_Main_Canvas);
            BTN_Power.draw(Display_Main_Canvas);
            BTN_UP.draw(Display_Main_Canvas);
            BTN_DOWN.draw(Display_Main_Canvas);
            BTN_SAVE.draw(Display_Main_Canvas);

            SLD_StripeWidth.draw(Display_Main_Canvas);

            Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_16);
            Display_Main_Canvas.setTextColor(0xffd500);
            Display_Main_Canvas.drawString("// DISPLAY 13.2 //", 2, 2);

            Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_28);
            Display_Main_Canvas.setTextColor(0xffd500);
            Display_Main_Canvas.drawCenterString(String(SLD_StripeWidth.value),
                                                 formWidth / 2, 40);

            Display_Main_Canvas.pushSprite(0, 0);
        }
    }

    int touchCheck(m5::touch_detail_t t) override
    {
        if (formEnable)
        {
            if (BTN_Power.contain(t))
            {
                return 1;
            }
            else if (BTN_Config.contain(t))
            {
                return 2;
            }
            else if (SLD_StripeWidth.contain(t))
            {
                valueString = String(SLD_StripeWidth.value);
                return 3;
            }
            else if (BTN_UP.contain(t))
            {
                SLD_StripeWidth.setValue(SLD_StripeWidth.value + 1);
                valueString = String(SLD_StripeWidth.value);
                return 4;
            }
            else if (BTN_DOWN.contain(t))
            {
                SLD_StripeWidth.setValue(SLD_StripeWidth.value - 1);
                valueString = String(SLD_StripeWidth.value);
                return 5;
            }
            else if (BTN_SAVE.contain(t))
            {
                valueString = String(SLD_StripeWidth.value);
                return 6;
            }
        }
        return 0;
    }
};

extern unsigned char btn_OK[1859];
extern unsigned char btn_CANCEL[2815];

class form_ShutdownMessage : public form
{
private:
    displayButton BTN_OK;
    displayButton BTN_CANCEL;

public:
    form_ShutdownMessage()
    {
        formName = "ShutdownMessage";
        formEnable = true;
    }
    /// @brief Initialize Canvas / Button
    form_ShutdownMessage(M5Canvas formCanvas, int value)
    {
        formName = "ShutdownMessage";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();

        BTN_OK = displayButton();
        // OK ICON Button ===================
        BTN_OK.x = 0;
        BTN_OK.y = 187;
        BTN_OK.width = 160;
        BTN_OK.height = 52;
        BTN_OK.iconData = btn_OK;
        BTN_OK.enable = true;

        BTN_CANCEL = displayButton();
        // CANCEL ICON Button ===================
        BTN_CANCEL.x = 160;
        BTN_CANCEL.y = 187;
        BTN_CANCEL.width = 160;
        BTN_CANCEL.height = 52;
        BTN_CANCEL.iconData = btn_CANCEL;
        BTN_CANCEL.enable = true;
        // ================================
    }

    void draw()
    {
        if (formEnable)
        {
            Display_Main_Canvas.fillScreen(BLACK);
            BTN_OK.draw(Display_Main_Canvas);
            BTN_CANCEL.draw(Display_Main_Canvas);

            Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_40);
            Display_Main_Canvas.setTextColor(0xffd500);
            Display_Main_Canvas.drawCenterString("Shutdown ?", formWidth / 2, 40);
            Display_Main_Canvas.pushSprite(0, 0);
        }
    }
    int touchCheck(m5::touch_detail_t t)
    {
        if (BTN_OK.contain(t))
        {
            return 1;
        }
        else if (BTN_CANCEL.contain(t))
        {
            return 2;
        }
        return 0;
    }
};

class form_SaveMessage : public form
{
private:
    displayButton BTN_OK;
    displayButton BTN_CANCEL;

public:
    form_SaveMessage()
    {
        formName = "ShutdownMessage";
        formEnable = true;
    }

    /// @brief Initialize Canvas / Button
    form_SaveMessage(M5Canvas formCanvas, int value)
    {
        formName = "SaveMessage";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();

        BTN_OK = displayButton();
        // OK ICON Button ===================
        BTN_OK.x = 0;
        BTN_OK.y = 187;
        BTN_OK.width = 160;
        BTN_OK.height = 52;
        BTN_OK.iconData = btn_OK;
        BTN_OK.enable = true;

        BTN_CANCEL = displayButton();
        // CANCEL ICON Button ===================
        BTN_CANCEL.x = 160;
        BTN_CANCEL.y = 187;
        BTN_CANCEL.width = 160;
        BTN_CANCEL.height = 52;
        BTN_CANCEL.iconData = btn_CANCEL;
        BTN_CANCEL.enable = true;
        // ================================
    }

    void draw()
    {
        if (formEnable)
        {
            Display_Main_Canvas.fillScreen(BLACK);
            BTN_OK.draw(Display_Main_Canvas);
            BTN_CANCEL.draw(Display_Main_Canvas);

            Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_40);
            Display_Main_Canvas.setTextColor(0xffd500);
            Display_Main_Canvas.drawCenterString("Save ?", formWidth / 2, 40);
            Display_Main_Canvas.pushSprite(0, 0);
        }
    }
    int touchCheck(m5::touch_detail_t t)
    {
        if (BTN_OK.contain(t))
        {
            return 1;
        }
        else if (BTN_CANCEL.contain(t))
        {
            return 2;
        }
        return 0;
    }
};

#endif 