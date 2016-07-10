/*
Edison wifi web blinking sample
*/
# include <SPI.h>
# include <WiFi.h>

char serviceSID[]	= "sSID";           //  your network serviceSID (name) 
char password[]		= "password";		// your network passwordword
int  wifiStatus		= WL_IDLE_STATUS;
int  keyIndex = 0;                      // your network key Index number (needed only for WEP)

WiFiServer edisonWebServer(8010);       // Our server on port 8010

void setup()
{
	pinMode(13, OUTPUT);                // set the on board led as output
	Serial.begin(19200);                // Our debug monitor
	while (wifiStatus != WL_CONNECTED)
	{
		Serial.print("Try to connect at : ");
		Serial.println(serviceSID);
		Serial.println("Waiting for access point...");
		wifiStatus = WiFi.begin(serviceSID, password);
	}
	// Start our web server
	edisonWebServer.begin();
	sendDebugInfo();
}

void sendDebugInfo()
{
	// give wifi ip adrress to be invoked
	IPAddress ip = WiFi.localIP();
	Serial.print("Invoke EDISON web blinking on http://");
	Serial.print(ip);
	Serial.println(":8010");
}

void loop()
{
	WiFiClient client = edisonWebServer.available();
	if (client)
	{
		String currentLine = "";
		while (client.connected())
		{
			if (client.available())
			{
				char c = client.read();
				if (c == '\n')
				{
					if (currentLine.length() == 0)
					{
						break;
					}
					else
					{
						currentLine = "";
					}
				}
				else if (c != '\r')
				{
					currentLine += c;
				}
			}
		}
		//
		digitalWrite(13, !digitalRead(13));
		//
		client.println("HTTP/1.1 200 OK");
		client.println("Content-Type: text/html");
		client.println("Connection: close");
		client.println("Refresh: 1");
		client.println();
		client.print("Debug led is : ");
		if (digitalRead(13))
		{
			client.println("On!");
		}
		else
		{
			client.println("Off!");
		}
		client.stop();
	}
}
