#include "sassy.h"

int val_from_hex_string (char*);
int val_from_bin_string (char*);
int hv (char);

int  int_from_string (char *str)
    {
    char *vstr;
    int val;

    if (str[0] == '-')
        vstr = str+1;
    else
        vstr = str;

    if (strlen (vstr) > 1)
        switch (vstr[1])
            {
            case 'x' :
                val = val_from_hex_string (vstr);
                break;
            case 'b' :
                val = val_from_bin_string (vstr);
                break;
            default :
                sscanf (vstr, "%d", &val);
            }
    else
        sscanf (vstr, "%d", &val);

    if (vstr != str)
        val = -val;

    return val;
    }

int val_from_bin_string (char *str)
    {
    int ac;
    char *pc;

    assert (str[1] == 'b');

    for (ac=0, pc=str+2; (*pc)!='\0'; pc++)
	{
	ac = ac*2;
	if (*pc == '1')
	    ac++;
	}
    
    return ac;
    }

int val_from_hex_string (char *str)
    {
    int ac;
    char *pc;

    assert (str[1] == 'x');

    for (ac=0, pc=str+2; (*pc)!='\0'; pc++)
	ac = ac*16 + hv (*pc);
    
    return ac;
    }

int hv (char ch)
    {
    switch (ch)
        {
	case '0' : return 0;
	case '1' : return 1;
	case '2' : return 2;
	case '3' : return 3;
	case '4' : return 4;
	case '5' : return 5;
	case '6' : return 6;
	case '7' : return 7;
	case '8' : return 8;
	case '9' : return 9;
	case 'A' :
	case 'a' : return 10;
	case 'B' :
	case 'b' : return 11;
	case 'C' :
	case 'c' : return 12;
	case 'D' :
	case 'd' : return 13;
	case 'E' :
	case 'e' : return 14;
	case 'F' :
	case 'f' : return 15;
	}
    return -1;
    }

int is_hex (char *str)
    {
    if ((strlen (str) > 2) && (str[0] == '0') && (str[1] == 'x'))
        return TRUE;

    return FALSE;
    }

int is_binary (char *str)
    {
    if ((strlen (str) > 2) && (str[0] == '0') && (str[1] == 'b'))
        return TRUE;

    return FALSE;
    }

int is_float (char *str)
    {
    while ((*str)!='\0')
        {
	if (*str == '.')
	    return TRUE;
	str++;
	}
    return FALSE;
    }
