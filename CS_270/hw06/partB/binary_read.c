#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void read_a_line (int fd, char *str, int len);
char read_char (int fd);
void read_data_block (int fd, int sz, void *adr);

void read_binary_file (int fd, void *target_mem, int num_bytes)
    {
    char filetype[256], str[256], s1[256];
    int n_toks;

    /* look at the file type */
    read_a_line (fd, str, 256);
    if (sscanf (str, "%s", filetype) < 1)
	handle_error (ErrBinaryReadFiletype, Deadly, 0, NULL, NULL);

    if (filetype[0] != 'P')
	handle_error (ErrBinaryReadFormat, Deadly, 0, NULL, NULL);

    /* step through the comment lines */
    do  {
        read_a_line (fd, str, 256);
	n_toks = sscanf (str, "%s", s1);
	if (n_toks < 1)
	    handle_error (ErrBinaryReadNoData, Deadly, 0, NULL, NULL);
	} while (strcmp (s1, "#") == 0);

    /* skip over the max-val line */
    read_a_line (fd, str, 256);

    read_data_block (fd, num_bytes, target_mem);
    }

void read_a_line (int fd, char *str, int len)
    {
    char ch;
    int i = 0;

    do  {
	if (i >= len)
	    handle_error (ErrBinaryReadLineLengthOverflow, Deadly, 0, NULL, NULL);
        ch = read_char (fd);
	str[i++] = ch;
        } while (ch != '\n');
    str[i] = '\0';
    }

char read_char (int fd)
    {
    char ch;
    int amt=0, st;

    while (amt < 1) {
        st = read (fd, &ch, 1);
	if (st < 0)
	    handle_error (ErrBinaryReadFail, Deadly, 0, NULL, NULL);
	else if (st == 0)
	    handle_error (ErrBinaryReadNoData, Deadly, 0, NULL, NULL);
	amt += st;
	}
    return ch;
    }

void read_data_block (int fd, int sz, void *adr)
    {
    int amt, cnt=0;

    while (cnt < sz) {
        if ((amt = read (fd, adr+cnt, sz-cnt)) < 0)
	    handle_error (ErrBinaryReadFail, Deadly, 0, NULL, NULL);
        cnt += amt;
        }
    }
