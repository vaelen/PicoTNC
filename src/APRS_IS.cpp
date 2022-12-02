/*
Copyright (c) 2022 Andrew C. Young (JJ1OKA / NU8W)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <APRS_IS.h>
#include <console.h>

APRS_IS::APRS_IS(const String &callsign, const String &passcode, const String &tool_name, const String &version)
	: callsign(callsign), passcode(passcode), tool_name(tool_name), version(version) {
}

bool APRS_IS::connect(const String &server, const int port) {
	const String login = "user " + callsign + " pass " + passcode + " vers " + tool_name + " " + version + "\n\r";
	return _connect(server, port, login);
}

bool APRS_IS::connect(const String &server, const int port, const String &filter) {
	const String login = "user " + callsign + " pass " + passcode + " vers " + tool_name + " " + version + " filter " + filter + "\n\r";
	return _connect(server, port, login);
}

bool APRS_IS::_connect(const String &server, const int port, const String &login) {
	if(!client.connect(server.c_str(), port))
	{
		console.print(F("Could not connect to APRS-IS Server: "));
		console.print(server.c_str());
		console.print(F(":"));
		console.print(port);
		console.print('\n');
		return false;
	}
	sendMessage(login);
	while(true)
	{
		String line = client.readStringUntil('\n');
		if(line.indexOf("logresp") != -1)
		{
			if(line.indexOf("unverified") == -1)
			{
				return true;
			}
			else
			{
				console.print(F("Invalid callsign or passcode. Callsign: "));
				console.print(callsign.c_str());
				console.print(F(", Passcode: "));
				console.print(passcode.c_str());
				console.print('\n');
				return false;
			}
		}
	}
	return true;
}

bool APRS_IS::connected() {
	return client.connected();
}

bool APRS_IS::sendMessage(const String & message) {
	if(!connected())
	{
		return false;
	}
	client.println(message);
	return true;
}

int APRS_IS::available() {
	return client.available();
}

String APRS_IS::getMessage() {
	String line;
	if (client.available() > 0)
	{
		line = client.readStringUntil('\n');
	}
	return line;
}
