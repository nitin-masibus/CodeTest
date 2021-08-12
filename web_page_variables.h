#ifndef _WEB_PAGE_H_
#define _WEB_PAGE_H_

#include "web_login.h"
#include "web_index.h"
#include "web_general.h"
#include "web_communication.h"
#include "web_mqtt.h"
#include "web_in_out.h"
#include "web_hart.h"
#include "web_modbus.h"
#include "web_sms.h"
#include "web_diag.h"
#include "web_logo_default.h"
#include "web_script.h"
#include "web_style.h"

extern const unsigned char Kc_LOGIN[SZ_LOGIN] ;
extern const unsigned char Kc_INDEX[SZ_INDEX];
extern const unsigned char Kc_GENERAL[SZ_GENERAL];
extern const unsigned char Kc_COMMUNICATION[SZ_COMMUNICATION];
extern const unsigned char Kc_MQTT[SZ_MQTT];
extern const unsigned char Kc_IN_OUT[SZ_IN_OUT];
extern const unsigned char Kc_HART[SZ_HART];
extern const unsigned char Kc_MODBUS[SZ_MODBUS];
extern const unsigned char Kc_SMS[SZ_SMS];
extern const unsigned char Kc_DIAG[SZ_DIAG];
extern const unsigned char Kc_LOGO_DEFAULT[SZ_LOGO_DEFAULT];
extern const unsigned char Kc_SCRIPT[SZ_SCRIPT];
extern const unsigned char Kc_STYLE[SZ_STYLE];

#endif