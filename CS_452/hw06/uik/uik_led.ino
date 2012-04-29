unsigned int LedMode = LOW;


void led_update(void)
{
    digitalWrite(13, LedMode);   // set the LED

    if(LedMode == HIGH)
    {
      LedMode = LOW;
    } 
    else
    {
      LedMode = HIGH; 
    } 
}
