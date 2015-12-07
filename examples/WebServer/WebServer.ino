/*
 * UIPEthernet WebServer example.
 *
 * UIPEthernet is a TCP/IP stack that can be used with a enc28j60 based
 * Ethernet-shield.
 *
 * UIPEthernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>
 *
 *      -----------------
 *
 * This Hello World example sets up a web server at 192.168.0.125 on default port 80.
 * Based on Arduino -> Ethernet -> WebServer example
 * mod by reaper7
 */
#if (ESP8266) || (ENERGIA)
#include "SPI.h"
#endif
#include <UIPEthernet.h>

EthernetServer server = EthernetServer(80);

void setup()
{
  Serial.begin(9600);

  uint8_t mac[6] = { 0x54, 0x55, 0x58, 0x10, 0x00, 0x25 };
  IPAddress myIP(192,168,0,125);

  Ethernet.begin(mac,myIP);

  server.begin();
  //Serial.print("server is at ");
  //Serial.println(Ethernet.localIP());  
}

void loop()
{
  EthernetClient client = server.available();  // listen for incoming clients
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;         // an http request ends with a blank line
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) { // if you've gotten to the end of the line (received a newline character) and the line is blank, the http request has ended, so you can send a reply     
          client.println("HTTP/1.1 200 OK");   // send a standard http response header
          client.println("Content-Type: text/html");
          client.println("Connection: close"); // the connection will be closed after completion of the response
          client.println("Refresh: 5");        // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          int sensorReading = analogRead(A0);
          client.print("analog input A0 is ");
          client.print(sensorReading);
          client.println("<br />");         
          client.println("</html>");
          break;        
        }
        if (c == '\n') {                       // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {                  // you've gotten a character on the current line
          currentLineIsBlank = false;
        }        
      }
    }
    delay(1);                                  // give the web browser time to receive the data
    client.stop();                             // close the connection
    Serial.println("client disconnected");
  }
}
