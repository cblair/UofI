/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xDE, 0x91 };
IPAddress ip(10,42,43,2);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

int web_server_inited = 0; /*false*/

void web_server_init()
{
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  
  web_server_inited = 1; /*true*/
}

void web_server_loop()
{
 int read_count = 0; /*how many times we've read the pins for data*/
  
 UIK_sleep(500);//give time for rest of tasks to start
 
 /*main loop*/
 while(1)
 { 
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          // output the value of each analog input pin
          client.print("{ \n<br />");
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            client.print("\"analog input ");
            client.print(analogChannel);
            client.print("\" :  ");
            client.print(analogRead(analogChannel));
            client.print(", "); 
            client.print("\n<br />");
            
            if(analogChannel >= 5)
            {
              client.print("\"read count\" : ");
              client.print(read_count);
              client.print("\n<br />\n");
              
              client.print("}"); 
            }
          }
          
          led_update_delay13(); 
          read_count++;
          
          client.print("<!-- \"read count\" : ");
          client.print(read_count);
          client.print(" -->");
          client.print("\n<br />\n");
          client.print("\n<br />\n");
          
          break;
        }
        
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
  
  #ifndef UIK_PREEMPT
  UIK_sleep(500);
  #endif
 }/*end main loop*/
}
