#include "delay.h"
#include "intrins.h"

void Delay2500ms() //@22.1184MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 211;
    j = 30;
    k = 11;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Delay1000ms() //@22.1184MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 85;
    j = 12;
    k = 155;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Delay500ms() //@22.1184MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 43;
    j = 6;
    k = 203;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Delay300ms() //@22.1184MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 26;
    j = 55;
    k = 171;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Delay200ms() //@12.000MHz
{
    unsigned char i, j, k;

    //	_nop_();
    //	_nop_();
    i = 10;
    j = 31;
    k = 147;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Delay150ms() //@22.1184MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 13;
    j = 156;
    k = 83;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Delay100ms() //@22.1184MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 9;
    j = 104;
    k = 139;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

//numberÎª¶¨Ê±50msµÄ¸öÊý
void Delay50ms(unsigned char number) //@22.1184MHz
{
    unsigned char i, j, k;

    while (number-- > 0)
    {
        _nop_();
        _nop_();
        i = 5;
        j = 52;
        k = 195;
        do
        {
            do
            {
                while (--k)
                    ;
            } while (--j);
        } while (--i);
    }
}
