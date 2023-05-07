#include <Arduino.h>
#include <M5Unified.h>
#include "displayControl.hpp"

//=====================
//=====================
void displayButton::draw(M5Canvas canvas)
{
    if (enable)
    {
        canvas.drawPng(iconData, ~0u, x, y, width, height, 0, 0, 1, 1, datum_t::middle_center);
    }
}

bool displayButton::contain(m5::touch_detail_t touchDetail)
{
    return contain(touchDetail.x, touchDetail.y);
}

bool displayButton::contain(int _x, int _y)
{
    return enable && _x >= this->x && _x < (this->x + this->width) && _y >= this->y && _y < (this->y + this->height);
}

displayButton::displayButton()
{
    enable = true;
}

displayButton::displayButton(int x, int y, int width, int height, String name, unsigned char *iconData)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->name = name;
    this->iconData = iconData;
    this->enable = true;
}

//=====================
//=====================
button_Power::button_Power(int x, int y, int width, int height, int offset, int roundsize, int LineWidth, int colorCode)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->offset = offset;
    this->roundsize = roundsize;
    this->colorCode = colorCode;
    this->LineWidth = LineWidth;
}

button_Power::button_Power()
{
    this->x = 0;
    this->y = 0;
    this->width = 48;
    this->height = 48;
    this->offset = 2;
    this->roundsize = 6;
    this->colorCode = YELLOW;
    this->LineWidth = 3;
}

void button_Power::draw(M5Canvas canvas)
{
    int cx = x + width / 2;
    int cy = y + height / 2;
    int a = width > height ? (int)(height * 0.6) / 2 : (int)(width * 0.6) / 2;

    for (int i = 0; i < LineWidth; i++)
    {
        int L = offset + i;
        int R = roundsize - i;
        if (R < 0)
        {
            R = 0;
        }

        canvas.drawRoundRect(x + L, y + L, width - L * 2, height - L * 2, R, colorCode);
    }

    canvas.fillArc(cx, cy, a, a - LineWidth + 1, -60, 240, colorCode);
    canvas.fillRect(cx - LineWidth / 2, cy - a, LineWidth, a, colorCode);
}

//=====================
//=====================
void displaySlider::draw(M5Canvas canvas)
{
    if (enable)
    {
        canvas.drawRect(x, y, width, height, WHITE);
        canvas.fillCircle(x + height * 3 / 4 + (width - height * 3 / 2) * value / (valueRange), y + height / 2, height / 2, YELLOW);
        canvas.drawRoundRect(x + height / 2, y + height / 4, width - height, height / 2, height / 4, ColorCode);
    }
}

bool displaySlider::contain(m5::touch_detail_t touchDetail)
{
    return contain(touchDetail.x, touchDetail.y);
}

bool displaySlider::contain(int _x, int _y)
{
    bool isInside = enable && _x >= this->x && _x < (this->x + this->width) && _y >= this->y && _y < (this->y + this->height);

    if (isInside)
    {
        value = valueMin + (valueRange) * (_x - x) / (width - height);

        if (value < valueMin)
        {
            value = valueMin;
        }
        if (value > valueMax)
        {
            value = valueMax;
        }
    }

    return isInside;
}

int displaySlider::setValueMax(int valueMax)
{
    this->valueMax = valueMax;
    valueRange = valueMax - valueMin;

    return 1;
}

int displaySlider::setValueMin(int valueMin)
{
    this->valueMin = valueMin;
    valueRange = valueMax - valueMin;

    return 1;
}
int displaySlider::setValue(int value)
{

    if (value < valueMin)
    {
        value = valueMin;
    }
    if (value > valueMax)
    {
        value = valueMax;
    }

    this->value = value;

    return 1;
}

displaySlider::displaySlider()
{
    x = 5;
    y = 40;
    width = 200;
    height = 10;
    enable = true;
    ColorCode = 0xffd500;
    valueMin = 0;
    valueMax = 100;
    valueRange = valueMax - valueMin;
    value = valueMin;
}