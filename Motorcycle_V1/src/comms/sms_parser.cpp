#include "comms/sms_parser.h"
#include "core/state_machine.h"
#include "comms/sim868.h"

void SmsParser::handleSms(const String& raw, StateMachine& fsm) {
    int first = raw.indexOf('\n');
    int sec   = raw.indexOf('\n', first + 1);
    String sender = raw.substring(first + 1, sec); sender.trim();
    String body   = raw.substring(sec + 1);        body.trim();

    if (!reg->adminSet()) reg->saveAdmin(sender);      // اولین SMS = admin
    if (sender != reg->admin()) return;

    parseCmd(body, fsm);
}

void SmsParser::parseCmd(const String& b, StateMachine& fsm) {
    int comma = b.indexOf(',');
    if (comma < 0) return;

    String pwd  = b.substring(0, comma);
    if (!reg->checkPassword(pwd)) return;

    int code = b.substring(comma + 1).toInt();
    switch (code) {
        case 0:  fsm.postEvent(AlarmEvent::kSmsArm);    break;
        case 1:  fsm.postEvent(AlarmEvent::kSmsDisarm); break;
        case 2:  Sim868::instance().publishLocation();  break;
        case 8:  reg->saveAdmin("");                    break;  // ریست فکتوری ساده
        case 9:  /* حالت Learn ریموت */                break;
        default: break;
    }
}
