#include "comms/sim868.h"
#include "config.h"
#include "comms/sms_parser.h"
 
void Sim868::begin(Registry* r_) {
    reg = r_;
    gsm.begin(9600, SERIAL_8N1, GSM_RX1, GSM_TX1);

    pinMode(GSM_PWRK, OUTPUT);
    digitalWrite(GSM_PWRK, HIGH);     // فرض می‌کنیم روشن است

    at("ATE0");                       // Echo off
    at("AT+CFUN=1");
    at("AT+CSCLK=0");                 // کنترل خواب با MCU
}

bool Sim868::waitFor(const String& key, uint16_t tout) {
    unsigned long t0 = millis();
    String buf;
    while (millis() - t0 < tout) {
        while (gsm.available()) {
            char c = gsm.read();  buf += c;
            if (buf.endsWith(key)) return true;
        }
    }
    return false;
}

bool Sim868::at(const String& cmd, const String& ok, uint16_t tout) {
    send(cmd);
    return waitFor(ok, tout);
}

String Sim868::query(const String& cmd, uint16_t tout) {
    send(cmd);
    waitFor("\r\n", 200);
    String resp = gsm.readStringUntil('\r');
    waitFor("OK", tout);
    return resp;
}

void Sim868::wake()  { at("AT+CFUN=1"); }
void Sim868::sleep() { at("AT+CFUN=0"); }

void Sim868::sendSmsAdmin(const String& txt) {
    if (!reg->adminSet()) return;
    at("AT+CMGF=1");
    at("AT+CSCS=\"GSM\"");
    send("AT+CMGS=\"" + reg->admin() + "\"");
    waitFor(">", 700);
    gsm.print(txt); gsm.write(0x1A);              // Ctrl-Z
    waitFor("OK", 10000);
}

void Sim868::publishLocation() {
    String r = query("AT+CGNSSINF", 1200);        // GNSS info
    int p = 0;
    for (int i = 0; i < 4; i++) p = r.indexOf(',', p) + 1;
    int q = r.indexOf(',', p);
    String lat = r.substring(p, q);
    p = q + 1; q = r.indexOf(',', p);
    String lon = r.substring(p, q);

    String topic   = "/motor/" + reg->admin() + "/event";
    String payload = "{\"lat\":" + lat + ",\"lon\":" + lon + "}";
    at("AT+SMPUB=\"" + topic + "\"," + payload.length() + ",0,0");
    send(payload); gsm.write(0x1A);
    waitFor("OK", 5000);
}

void Sim868::processUrc(SmsParser& sp, StateMachine& fsm) {
    if (!gsm.available()) return;
    String line = gsm.readStringUntil('\n');  line.trim();

    if (line.startsWith("+CMTI:")) {          // پیام جدید
        int idx = line.lastIndexOf(',');
        int pos = line.substring(idx + 1).toInt();
        at("AT+CMGF=1");                      // Text mode
        String sms = query("AT+CMGR=" + String(pos), 2000);
        sp.handleSms(sms, fsm);
    }
}
