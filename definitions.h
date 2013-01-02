/****************************************************************
 * definitions.h
 * GrblHoming - zapmaker fork on github
 *
 * 15 Nov 2012
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QtGlobal>
#include <QString>
#include <QDateTime>
#include "atomicintbool.h"

#define DEBUG

#define DEFAULT_WAIT_TIME_SEC   100

#define DEFAULT_Z_JOG_RATE      260.0
#define DEFAULT_Z_LIMT_RATE     100.0

#define MM_IN_AN_INCH           25.4
#define PRE_HOME_Z_ADJ_MM       5.0

#define REQUEST_CURRENT_POS             "?"
#define SETTINGS_COMMAND_V08a           "$"
#define SETTINGS_COMMAND_V08c           "$$"
#define REQUEST_PARSER_STATE_V08c       "$G"
#define SET_UNLOCK_STATE_V08c           "$X"

#define REGEXP_SETTINGS_LINE    "(\\d+)\\s*=\\s*([\\w\\.]+)\\s*\\(([^\\)]*)\\)"

#define CLOSE_BUTTON_TEXT               "Close / Reset"

#define GRBL_CONTROLLER_NAME_AND_VERSION    "Grbl Controller 3.1"


extern AtomicIntBool g_enableDebugLog;

#endif // DEFINITIONS_H
