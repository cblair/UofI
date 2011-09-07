#ifndef _add_functions_h
#define _add_functions_h

void string_to_chararray(string &stemp, char ctemp[])
{
  for(int i = 0; i <= stemp.size(); ++i)
    ctemp[i] = stemp[i];
}

#endif
