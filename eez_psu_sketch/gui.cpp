/*
 * EEZ PSU Firmware
 * Copyright (C) 2015-present, Envox d.o.o.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "psu.h"

#if OPTION_DISPLAY

#include "channel.h"
#include "channel_dispatcher.h"
#include "actions.h"
#include "devices.h"
#include "sound.h"
#include "event_queue.h"
#if OPTION_ENCODER
#include "encoder.h"
#endif

#include "gui.h"
#include "gui_internal.h"
#include "gui_password.h"
#include "gui_edit_mode.h"
#include "gui_edit_mode_slider.h"
#include "gui_edit_mode_step.h"
#include "gui_edit_mode_keypad.h"
#include "gui_widget_button_group.h"
#include "gui_page_self_test_result.h"
#include "gui_page_main.h"
#include "gui_page_event_queue.h"
#include "gui_page_ch_settings_protection.h"
#include "gui_page_ch_settings_trigger.h"
#include "gui_page_ch_settings_adv.h"
#include "gui_page_ch_settings_info.h"
#include "gui_page_sys_settings.h"
#include "gui_page_sys_info.h"
#include "gui_page_user_profiles.h"
#include "gui_numeric_keypad.h"

#ifdef EEZ_PSU_SIMULATOR
#include "front_panel/control.h"
#endif

#define CONF_GUI_PAGE_NAVIGATION_STACK_SIZE 5

#define CONF_GUI_STANDBY_PAGE_TIMEOUT 10000000UL // 10s
#define CONF_GUI_ENTERING_STANDBY_PAGE_TIMEOUT 5000000UL // 5s
#define CONF_GUI_WELCOME_PAGE_TIMEOUT 2000000UL // 2s
#define CONF_GUI_LONG_TAP_TIMEOUT 1000000UL // 1s

#define CONF_GUI_KEYPAD_AUTO_REPEAT_DELAY 200000UL // 200ms

#define CONF_GUI_TOAST_DURATION_MS 1 * 1000UL // 1sec

#define INTERNAL_PAGE_ID_NONE             -1
#define INTERNAL_PAGE_ID_SELECT_FROM_ENUM -2

#define MAX_EVENTS 16

namespace eez {
namespace psu {
namespace gui {

using namespace lcd;

////////////////////////////////////////////////////////////////////////////////

Styles *g_styles;
Document *g_document;

#if defined(EEZ_PSU_ARDUINO_MEGA)
static Styles g_stylesBuffer;
static Document g_documentBuffer;
#endif

static int g_activePageId;
static Page *g_activePage;

static int g_previousActivePageId;

static struct Event {
    int activePageId;
    Page *activePage;
} g_pageNavigationStack[CONF_GUI_PAGE_NAVIGATION_STACK_SIZE];
static int g_pageNavigationStackPointer = 0;

WidgetCursor g_foundWidgetAtDown;
static WidgetCursor g_foundTouchWidget;

static void (*g_dialogYesCallback)();
static void (*g_dialogNoCallback)();
static void (*g_dialogCancelCallback)();

static void (*g_errorMessageAction)();
static int g_errorMessageActionParam;

static uint32_t g_showPageTime;
static uint32_t g_timeOfLastActivity;
static bool g_touchActionExecuted;

Channel *g_channel;

static data::Cursor g_wasFocusCursor;
static uint8_t g_wasFocusDataId;
data::Cursor g_focusCursor;
uint8_t g_focusDataId;
data::Value g_focusEditValue;

////////////////////////////////////////

static uint32_t g_touchDownTime;
static uint32_t g_lastAutoRepeatEventTime;
static bool g_longTapGenerated;

enum EventType {
    EVENT_TYPE_TOUCH_DOWN,
    EVENT_TYPE_TOUCH_MOVE,
    EVENT_TYPE_LONG_TAP,
    EVENT_TYPE_AUTO_REPEAT,
    EVENT_TYPE_TOUCH_UP
};

static int g_numEvents;
static struct {
    EventType type;
    int x;
    int y;
} g_events[MAX_EVENTS];

////////////////////////////////////////////////////////////////////////////////

Page *createPageFromId(int pageId) {
    switch (pageId) {
    case PAGE_ID_SELF_TEST_RESULT: return new SelfTestResultPage();
    case PAGE_ID_MAIN: return new MainPage();
    case PAGE_ID_EVENT_QUEUE: return new EventQueuePage();
    case PAGE_ID_CH_SETTINGS_PROT: return new ChSettingsProtectionPage();
    case PAGE_ID_CH_SETTINGS_PROT_OVP: return new ChSettingsOvpProtectionPage();
    case PAGE_ID_CH_SETTINGS_PROT_OCP: return new ChSettingsOcpProtectionPage();
    case PAGE_ID_CH_SETTINGS_PROT_OPP: return new ChSettingsOppProtectionPage();
    case PAGE_ID_CH_SETTINGS_PROT_OTP: return new ChSettingsOtpProtectionPage();
    case PAGE_ID_CH_SETTINGS_ADV: return new ChSettingsAdvPage();
    case PAGE_ID_CH_SETTINGS_ADV_LRIPPLE: return new ChSettingsAdvLRipplePage();
    case PAGE_ID_CH_SETTINGS_ADV_RSENSE: return new ChSettingsAdvRSensePage();
    case PAGE_ID_CH_SETTINGS_ADV_RPROG: return new ChSettingsAdvRProgPage();
    case PAGE_ID_CH_SETTINGS_ADV_TRACKING: return new ChSettingsAdvTrackingPage();
    case PAGE_ID_CH_SETTINGS_ADV_COUPLING:
    case PAGE_ID_CH_SETTINGS_ADV_COUPLING_INFO: return new ChSettingsAdvCouplingPage();
    case PAGE_ID_CH_SETTINGS_ADV_VIEW: return new ChSettingsAdvViewPage();
    case PAGE_ID_CH_SETTINGS_TRIGGER: return new ChSettingsTriggerPage();
    case PAGE_ID_CH_SETTINGS_LISTS: return new ChSettingsListsPage();
    case PAGE_ID_CH_SETTINGS_INFO: return new ChSettingsInfoPage();
    case PAGE_ID_SYS_SETTINGS_DATE_TIME: return new SysSettingsDateTimePage();
    case PAGE_ID_SYS_SETTINGS_ETHERNET: return new SysSettingsEthernetPage();
    case PAGE_ID_SYS_SETTINGS_PROTECTIONS: return new SysSettingsProtectionsPage();
    case PAGE_ID_SYS_SETTINGS_AUX_OTP: return new SysSettingsAuxOtpPage();
    case PAGE_ID_SYS_SETTINGS_SOUND: return new SysSettingsSoundPage();
#if OPTION_ENCODER
    case PAGE_ID_SYS_SETTINGS_ENCODER: return new SysSettingsEncoderPage();
#endif
    case PAGE_ID_SYS_SETTINGS_DISPLAY: return new SysSettingsDisplayPage();
    case PAGE_ID_SYS_INFO:
    case PAGE_ID_SYS_INFO2: return new SysInfoPage();
    case PAGE_ID_USER_PROFILES:
    case PAGE_ID_USER_PROFILES2:
    case PAGE_ID_USER_PROFILE_0_SETTINGS:
    case PAGE_ID_USER_PROFILE_SETTINGS: return new UserProfilesPage();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

bool isActivePageInternal() {
    return g_activePageId < INTERNAL_PAGE_ID_NONE;
}

////////////////////////////////////////////////////////////////////////////////

int getActivePageId() {
    return g_activePageId;
}

Page *getActivePage() {
    return g_activePage;
}

int getPreviousActivePageId() {
    return g_previousActivePageId;
}

void doShowPage(int index, Page *page = 0) {
    lcd::turnOn();

    // delete current page
    if (g_activePage) {
        delete g_activePage;
    }

    g_previousActivePageId = g_activePageId;
    g_activePageId = index;

    if (page) {
        g_activePage = page;
    } else {
        g_activePage = createPageFromId(g_activePageId);
    }

    if (g_activePage) {
        g_activePage->pageWillAppear();
    }

    g_showPageTime = micros();
    g_timeOfLastActivity = millis();

    refreshPage();
}

void setPage(int index) {
    // delete stack
    for (int i = 0; i < g_pageNavigationStackPointer; ++i) {
        if (g_pageNavigationStack[i].activePage) {
            delete g_pageNavigationStack[i].activePage;
        }
    }
    g_pageNavigationStackPointer = 0;

    // clear
    g_focusEditValue = data::Value();

    //
    doShowPage(index);
}

void replacePage(int index, Page *page) {
    doShowPage(index, page);
}

void pushPage(int index, Page *page) {
    // push current page on stack
    if (g_activePageId != INTERNAL_PAGE_ID_NONE) {
        if (g_pageNavigationStackPointer == CONF_GUI_PAGE_NAVIGATION_STACK_SIZE) {
            // no more space on the stack

            // delete page on the bottom
            if (g_pageNavigationStack[0].activePage) {
                delete g_pageNavigationStack[0].activePage;
            }

            // move stack one down
            for (int i = 1; i < g_pageNavigationStackPointer; ++i) {
                g_pageNavigationStack[i-1].activePageId = g_pageNavigationStack[i].activePageId;
                g_pageNavigationStack[i-1].activePage = g_pageNavigationStack[i].activePage;
            }

            --g_pageNavigationStackPointer;
        }

        g_pageNavigationStack[g_pageNavigationStackPointer].activePageId = g_activePageId;
        g_pageNavigationStack[g_pageNavigationStackPointer].activePage = g_activePage;
        g_activePage = 0;
        ++g_pageNavigationStackPointer;
    }

    doShowPage(index, page);
}

void popPage() {
    if (g_pageNavigationStackPointer > 0) {
        --g_pageNavigationStackPointer;

        doShowPage(g_pageNavigationStack[g_pageNavigationStackPointer].activePageId,
            g_pageNavigationStack[g_pageNavigationStackPointer].activePage);
    } else {
        doShowPage(PAGE_ID_MAIN);
    }
}

void showWelcomePage() {
    setPage(PAGE_ID_WELCOME);
    flush();
}

void showSelfTestResultPage() {
    setPage(PAGE_ID_SELF_TEST_RESULT);
    flush();
}

void showStandbyPage() {
    setPage(PAGE_ID_STANDBY);
    flush();
}

void showEnteringStandbyPage() {
    if (g_activePageId != PAGE_ID_ENTERING_STANDBY) {
        setPage(PAGE_ID_ENTERING_STANDBY);
        flush();
    }
}

void showEthernetInit() {
    doShowPage(PAGE_ID_ETHERNET_INIT);
    flush();
}

void pushSelectFromEnumPage(const data::EnumItem *enumDefinition, uint8_t currentValue, uint8_t disabledValue, void (*onSet)(uint8_t)) {
    pushPage(INTERNAL_PAGE_ID_SELECT_FROM_ENUM, new SelectFromEnumPage(enumDefinition, currentValue, disabledValue, onSet));
}

void dialogYes() {
    popPage();

    if (g_dialogYesCallback) {
        g_dialogYesCallback();
    }
}

void dialogNo() {
    popPage();

    if (g_dialogNoCallback) {
        g_dialogNoCallback();
    }
}

void dialogCancel() {
    popPage();

    if (g_dialogCancelCallback) {
        g_dialogCancelCallback();
    }
}

void dialogOk() {
    dialogYes();
}

void errorMessageAction() {
    popPage();

    if (g_dialogYesCallback) {
        g_dialogYesCallback();
    }

    g_errorMessageAction();
}

void alertMessage(int alertPageId, data::Value message, void (*ok_callback)()) {
    data::set(data::Cursor(), DATA_ID_ALERT_MESSAGE, message, 0);

    g_dialogYesCallback = ok_callback;

    pushPage(alertPageId);

    if (alertPageId == PAGE_ID_ERROR_ALERT) {
        sound::playBeep();
    }
}

void longAlertMessage(int alertPageId, data::Value message, data::Value message2, void (*ok_callback)()) {
    data::set(data::Cursor(), DATA_ID_ALERT_MESSAGE_2, message2, 0);
    alertMessage(alertPageId, message, ok_callback);
}

void infoMessage(data::Value value, void (*ok_callback)()) {
    alertMessage(PAGE_ID_INFO_ALERT, value, ok_callback);
}

void infoMessageP(const char *message PROGMEM, void (*ok_callback)()) {
    alertMessage(PAGE_ID_INFO_ALERT, data::Value::ProgmemStr(message), ok_callback);
}

void longInfoMessage(data::Value value1, data::Value value2, void (*ok_callback)()) {
    longAlertMessage(PAGE_ID_INFO_LONG_ALERT, value1, value2, ok_callback);
}

void longInfoMessageP(const char *message1 PROGMEM, const char *message2 PROGMEM, void (*ok_callback)()) {
    longInfoMessage(data::Value::ProgmemStr(message1), data::Value::ProgmemStr(message2), ok_callback);
}

void toastMessageP(const char *message1 PROGMEM, const char *message2 PROGMEM, const char *message3 PROGMEM, void (*ok_callback)()) {
    data::set(data::Cursor(), DATA_ID_ALERT_MESSAGE_3, message3, 0);
    longAlertMessage(PAGE_ID_TOAST3_ALERT, message1, message2, ok_callback);
}

void changeLimit(const data::Value& value, float minLimit, float maxLimit, float defLimit, void (*onSetLimit)(float)) {
	NumericKeypadOptions options;

	options.editUnit = value.getType();

	options.min = minLimit;
	options.max = maxLimit;
	options.def = defLimit;

	options.flags.genericNumberKeypad = true;
	options.enableMaxButton();
	options.enableDefButton();
	options.flags.signButtonEnabled = true;
	options.flags.dotButtonEnabled = true;

	NumericKeypad::start(0, value, options, onSetLimit);
}

void onSetVoltageLimit(float limit) {
    Channel& channel = Channel::get(g_errorMessageActionParam);
    channel_dispatcher::setVoltageLimit(channel, limit);
    infoMessageP(PSTR("Voltage limit changed!"), popPage);
}

void changeVoltageLimit() {
    Channel& channel = Channel::get(g_errorMessageActionParam);
	float minLimit = channel_dispatcher::getUMin(channel);
	float maxLimit = channel_dispatcher::getUMax(channel);
	float defLimit = channel_dispatcher::getUMax(channel);
    changeLimit(data::Value(channel_dispatcher::getULimit(channel), data::VALUE_TYPE_FLOAT_VOLT), minLimit, maxLimit, defLimit, onSetVoltageLimit);
}

void onSetCurrentLimit(float limit) {
    Channel& channel = Channel::get(g_errorMessageActionParam);
    channel_dispatcher::setCurrentLimit(channel, limit);
    infoMessageP(PSTR("Current limit changed!"), popPage);
}

void changeCurrentLimit() {
    Channel& channel = Channel::get(g_errorMessageActionParam);
	float minLimit = channel_dispatcher::getIMin(channel);
	float maxLimit = channel_dispatcher::getIMax(channel);
	float defLimit = channel_dispatcher::getIMax(channel);
    changeLimit(data::Value(channel_dispatcher::getILimit(channel), data::VALUE_TYPE_FLOAT_AMPER), minLimit, maxLimit, defLimit, onSetCurrentLimit);
}

void onSetPowerLimit(float limit) {
    Channel& channel = Channel::get(g_errorMessageActionParam);
    channel_dispatcher::setPowerLimit(channel, limit);
    infoMessageP(PSTR("Power limit changed!"), popPage);
}

void changePowerLimit() {
    Channel& channel = Channel::get(g_errorMessageActionParam);
	float minLimit = channel_dispatcher::getPowerMinLimit(channel);
	float maxLimit = channel_dispatcher::getPowerMaxLimit(channel);
	float defLimit = channel_dispatcher::getPowerDefaultLimit(channel);
    changeLimit(data::Value(channel_dispatcher::getPowerLimit(channel), data::VALUE_TYPE_FLOAT_WATT), minLimit, maxLimit, defLimit, onSetPowerLimit);
}

void errorMessage(const data::Cursor& cursor, data::Value value, void (*ok_callback)()) {
    int errorPageId = PAGE_ID_ERROR_ALERT;

    if (value.getType() == data::VALUE_TYPE_SCPI_ERROR_TEXT) {
        void (*action)() = 0;
        const char *actionLabel PROGMEM = 0;

        Channel& channel = Channel::get(cursor.i);

        if (value.getScpiError() == SCPI_ERROR_VOLTAGE_LIMIT_EXCEEDED) {
            if (channel_dispatcher::getULimit(channel) < channel_dispatcher::getUMaxLimit(channel)) {
                action = changeVoltageLimit;
                actionLabel = PSTR("Change voltage limit");
            } else {
                errorPageId = PAGE_ID_ERROR_TOAST_ALERT;
            }
        } else if (value.getScpiError() == SCPI_ERROR_CURRENT_LIMIT_EXCEEDED) {
            if (channel_dispatcher::getILimit(channel) < channel_dispatcher::getIMaxLimit(channel)) {
                action = changeCurrentLimit;
                actionLabel = PSTR("Change current limit");
            } else {
                errorPageId = PAGE_ID_ERROR_TOAST_ALERT;
            }
        } else if (value.getScpiError() == SCPI_ERROR_POWER_LIMIT_EXCEEDED) {
            if (channel_dispatcher::getPowerLimit(channel) < channel_dispatcher::getPowerMaxLimit(channel)) {
                action = changePowerLimit;
                actionLabel = PSTR("Change power limit");
            } else {
                errorPageId = PAGE_ID_ERROR_TOAST_ALERT;
            }
        }

        if (action) {
            data::set(data::Cursor(), DATA_ID_ALERT_MESSAGE_2, actionLabel, 0);
            g_errorMessageAction = action;
            g_errorMessageActionParam = cursor.i;
            errorPageId = PAGE_ID_ERROR_ALERT_WITH_ACTION;
        }
    }

    alertMessage(errorPageId, value, ok_callback);
    sound::playBeep();
}


void errorMessageP(const char *message PROGMEM, void (*ok_callback)()) {
    alertMessage(PAGE_ID_ERROR_ALERT, data::Value::ProgmemStr(message), ok_callback);
    sound::playBeep();
}

void yesNoDialog(int yesNoPageId, const char *message PROGMEM, void (*yes_callback)(), void (*no_callback)(), void (*cancel_callback)()) {
    data::set(data::Cursor(), DATA_ID_ALERT_MESSAGE, data::Value::ProgmemStr(message), 0);

    g_dialogYesCallback = yes_callback;
    g_dialogNoCallback = no_callback;
    g_dialogCancelCallback = cancel_callback;

    pushPage(yesNoPageId);
}

void areYouSure(void (*yes_callback)()) {
    yesNoDialog(PAGE_ID_YES_NO, PSTR("Are you sure?"), yes_callback, 0, 0);
}

void areYouSureWithMessage(const char *message PROGMEM, void (*yes_callback)()) {
    yesNoDialog(PAGE_ID_ARE_YOU_SURE_WITH_MESSAGE, message, yes_callback, 0, 0);
}

static bool isChannelTripLastEvent(int i, event_queue::Event &lastEvent) {
    if (lastEvent.eventId == (event_queue::EVENT_ERROR_CH1_OVP_TRIPPED + i * 3) ||
        lastEvent.eventId == (event_queue::EVENT_ERROR_CH1_OCP_TRIPPED + i * 3) ||
        lastEvent.eventId == (event_queue::EVENT_ERROR_CH1_OPP_TRIPPED + i * 3) ||
        lastEvent.eventId == (event_queue::EVENT_ERROR_CH1_OTP_TRIPPED + i)) 
    {
        return Channel::get(i).isTripped();
    }

    return false;
}

void onLastErrorEventAction() {
    event_queue::Event lastEvent;
    event_queue::getLastErrorEvent(&lastEvent);

    if (lastEvent.eventId == event_queue::EVENT_ERROR_AUX_OTP_TRIPPED && temperature::sensors[temp_sensor::AUX].isTripped()) {
        setPage(PAGE_ID_SYS_SETTINGS_AUX_OTP);
    } else if (isChannelTripLastEvent(0, lastEvent)) {
        g_channel = &Channel::get(0);
        setPage(PAGE_ID_CH_SETTINGS_PROT_CLEAR);
    } else if (isChannelTripLastEvent(1, lastEvent)) {
        g_channel = &Channel::get(1);
        setPage(PAGE_ID_CH_SETTINGS_PROT_CLEAR);
    } else {
        setPage(PAGE_ID_EVENT_QUEUE);
    }
}

static void doUnlockFrontPanel() {
    popPage();

    if (persist_conf::lockFrontPanel(false)) {
        infoMessageP(PSTR("Front panel is unlocked!"));
    }
}

static void checkPasswordToUnlockFrontPanel() {
    checkPassword(PSTR("Password: "), persist_conf::devConf2.systemPassword, doUnlockFrontPanel);
}

void lockFrontPanel() {
    if (persist_conf::lockFrontPanel(true)) {
        infoMessageP(PSTR("Front panel is locked!"));
    }
}

void unlockFrontPanel() {
    if (strlen(persist_conf::devConf2.systemPassword) > 0) {
        checkPasswordToUnlockFrontPanel();
    } else {
        if (persist_conf::lockFrontPanel(false)) {
            infoMessageP(PSTR("Front panel is unlocked!"));
        }
    }
}

bool isWidgetActionEnabled(const Widget *widget) {
    if (widget->action) {
        if (isFrontPanelLocked()) {
            if (g_activePageId == PAGE_ID_INFO_ALERT || g_activePageId == PAGE_ID_ERROR_ALERT || g_activePageId == PAGE_ID_KEYPAD ||
                g_activePageId == PAGE_ID_SCREEN_CALIBRATION_YES_NO || g_activePageId == PAGE_ID_SCREEN_CALIBRATION_YES_NO_CANCEL) {
                return true;
            }

            if (widget->action != ACTION_ID_SYS_FRONT_PANEL_UNLOCK) {
                return false;
            }
        }

        return true;
    }

    return false;
}

int transformStyle(const Widget *widget) {
    if (isFrontPanelLocked()) {
        if (widget->style == STYLE_ID_BOTTOM_BUTTON) {
            if (widget->action != ACTION_ID_SYS_FRONT_PANEL_UNLOCK) {
                return STYLE_ID_BOTTOM_BUTTON_DISABLED;
            }
        } else if (widget->style == STYLE_ID_EDIT_S) {
            return STYLE_ID_DEFAULT_S;
        } else if (widget->style == STYLE_ID_MON_VALUE) {
            return STYLE_ID_DEFAULT;
        } else if (widget->style == STYLE_ID_CHANNEL_OFF_LANDSCAPE) {
            return STYLE_ID_DEFAULT_L_LANDSCAPE;
        } else if (widget->style == STYLE_ID_EDIT_VALUE_ACTIVE_S_RIGHT) {
            return STYLE_ID_EDIT_VALUE_S_RIGHT;
        }
    }

    return widget->style;
}

////////////////////////////////////////////////////////////////////////////////

void selectChannel() {
    if (g_foundWidgetAtDown.cursor.i >= 0) {
        g_channel = &Channel::get(g_foundWidgetAtDown.cursor.i);
    } else {
        g_channel = &Channel::get(0);
    }
}

////////////////////////////////////////////////////////////////////////////////

void setFocusCursor(const data::Cursor& cursor, uint8_t dataId) {
    g_focusCursor = cursor;
    g_focusDataId = dataId;
    g_focusEditValue = data::Value();
}

bool wasFocusWidget(const WidgetCursor &widgetCursor) {
    DECL_WIDGET(widget, widgetCursor.widgetOffset);
    return widgetCursor.cursor == g_wasFocusCursor && widget->data == g_wasFocusDataId;
}

bool isFocusWidget(const WidgetCursor &widgetCursor) {
    if (g_activePageId == PAGE_ID_CH_SETTINGS_LISTS) {
        return ((ChSettingsListsPage *)g_activePage)->isFocusWidget(widgetCursor);
    } else {
        DECL_WIDGET(widget, widgetCursor.widgetOffset);
        return widgetCursor.cursor == g_focusCursor && widget->data == g_focusDataId;
    }
}

bool isFocusChanged() {
    return g_focusEditValue.getType() != data::VALUE_TYPE_NONE;
}

////////////////////////////////////////////////////////////////////////////////

bool isLongPressAction(int actionId) {
    return actionId == ACTION_ID_TURN_OFF ||
        actionId == ACTION_ID_SYS_FRONT_PANEL_LOCK || 
        actionId == ACTION_ID_SYS_FRONT_PANEL_UNLOCK;
}

ActionType getAction(WidgetCursor &widgetCursor) {
    if (isActivePageInternal()) {
        return ((InternalPage *)g_activePage)->getAction(widgetCursor);
    } else {
        DECL_WIDGET(widget, widgetCursor.widgetOffset);
        return widget->action;
    }
}


void executeAction(int actionId) {
    sound::playClick();
    actions[actionId]();
}

////////////////////////////////////////////////////////////////////////////////

#if OPTION_ENCODER

static bool g_isEncoderEnabledInActivePage;

void isEncoderEnabledInActivePageCheckWidget(const WidgetCursor &widgetCursor) {
    DECL_WIDGET(widget, widgetCursor.widgetOffset);
    if (widget->action == ACTION_ID_EDIT) {
        g_isEncoderEnabledInActivePage = true;
    }
}

bool isEncoderEnabledInActivePage() {
    // encoder is enabled if active page contains widget with "edit" action
    g_isEncoderEnabledInActivePage = false;
    enumWidgets(getActivePageId(), 0, 0, isEncoderEnabledInActivePageCheckWidget);
    return g_isEncoderEnabledInActivePage;
}

void onEncoder(int counter, bool clicked) {
    if (isFrontPanelLocked()) {
        return;
    }

    if (clicked) {
        if (g_activePage) {
            if (g_activePage->onEncoderClicked()) {
                return;
            }
        }

        if (isEncoderEnabledInActivePage()) {
            if (g_activePageId == PAGE_ID_EDIT_MODE_KEYPAD || g_activePageId == PAGE_ID_NUMERIC_KEYPAD) {
                getActiveKeypad()->ok();
            } else {
                if (isFocusChanged()) {
                    // confirmation
                    int16_t error;
                    if (!data::set(g_focusCursor, g_focusDataId, g_focusEditValue, &error)) {
                        errorMessage(g_focusCursor, data::Value::ScpiErrorText(error));
                    } else {
                        g_focusEditValue = data::Value();
                    }
                } else if (edit_mode::isActive() && !edit_mode::isInteractiveMode() && edit_mode::getEditValue() != edit_mode::getCurrentValue()) {
                    edit_mode::nonInteractiveSet();
                } else {
                    // selection
                    data::Cursor newCursor = g_focusCursor;
                    int newDataId;

                    if (g_focusDataId == DATA_ID_CHANNEL_U_EDIT) {
                        newDataId = DATA_ID_CHANNEL_I_EDIT;
                    } else {
                        for (int i = 0; i < CH_NUM; ++i) {
                            newCursor.i = (g_focusCursor.i + 1) % CH_NUM;
                            if (Channel::get(g_focusCursor.i).isOk()) {
                                break;
                            }
                        }
                        newDataId = DATA_ID_CHANNEL_U_EDIT;
                    }

                    setFocusCursor(newCursor, newDataId);

                    if (edit_mode::isActive()) {
                        edit_mode::update();
                    }
                }
                sound::playClick();
            }
        }
    }

    if (counter) {
        if (g_activePage) {
            if (g_activePage->onEncoder(counter)) {
                return;
            }
        }

        if (g_activePageId == PAGE_ID_EDIT_MODE_KEYPAD || g_activePageId == PAGE_ID_NUMERIC_KEYPAD) {
            if (((NumericKeypad *)getActiveKeypad())->onEncoder(counter)) {
                return;
            }
        }
    
        if (g_activePageId == PAGE_ID_EDIT_MODE_STEP) {
            edit_mode_step::onEncoder(counter);
            return;
        }

        encoder::enableAcceleration(true);
        encoder::setMovingSpeedMultiplier(
            data::getMax(g_focusCursor, g_focusDataId).getFloat() / data::getMax(g_focusCursor, DATA_ID_CHANNEL_U_SET).getFloat());

        if (g_activePageId == PAGE_ID_EDIT_MODE_SLIDER) {
            edit_mode_slider::increment(counter);
            return;
        }

        if (isEncoderEnabledInActivePage()) {
            data::Value value = data::get(g_focusCursor, g_focusDataId);

            float newValue = value.getFloat() + 0.01f * counter;

            float min = data::getMin(g_focusCursor, g_focusDataId).getFloat();
            if (newValue < min) {
                newValue = min;
            }

            float max = data::getMax(g_focusCursor, g_focusDataId).getFloat();
            if (newValue > max) {
                newValue = max;
            }

            if (persist_conf::devConf2.flags.encoderConfirmationMode) {
                g_focusEditValue = data::Value(newValue, value.getType());
            } else {
                int16_t error;
                if (!data::set(g_focusCursor, g_focusDataId, data::Value(newValue, value.getType()), &error)) {
                    errorMessage(g_focusCursor, data::Value::ScpiErrorText(error));
                }
            }
        }
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////

void init() {
    setFocusCursor(0, DATA_ID_CHANNEL_U_EDIT);

    lcd::init();

#if OPTION_ENCODER
    encoder::init();
#endif

#ifdef EEZ_PSU_SIMULATOR
    if (true || persist_conf::devConf.gui_opened) {
        simulator::front_panel::open();
    }
#endif

    touch::init();

    g_activePageId = INTERNAL_PAGE_ID_NONE;
    g_activePage = 0;

#if defined(EEZ_PSU_ARDUINO_MEGA)
    arduino_util::prog_read_buffer(styles, (uint8_t *)&g_stylesBuffer, sizeof(Styles));
    g_styles = &g_stylesBuffer;

    arduino_util::prog_read_buffer(pages, (uint8_t *)&g_documentBuffer, sizeof(Document));
    g_document = &g_documentBuffer;
#else
    g_styles = (Styles *)styles;
    g_document = (Document *)document;
#endif
}

int getStartPageId() {
    if (devices::anyFailed()) {
        return PAGE_ID_SELF_TEST_RESULT;
    }

    return PAGE_ID_MAIN;
}

////////////////////////////////////////////////////////////////////////////////

void pushEvent(EventType type) {
    // allow only one EVENT_TYPE_AUTO_REPEAT in the stack
    if (type == EVENT_TYPE_AUTO_REPEAT) {
        for (int i = 0; i < g_numEvents; ++i) {
            if (g_events[i].type == EVENT_TYPE_AUTO_REPEAT) {
                return;
            }
        }
    }

    // ignore EVENT_TYPE_TOUCH_MOVE if it is the same as the last event
    if (type == EVENT_TYPE_TOUCH_MOVE) {
        if (g_numEvents > 0 && g_events[g_numEvents-1].type == EVENT_TYPE_TOUCH_MOVE &&
            g_events[g_numEvents-1].x == touch::x || g_events[g_numEvents-1].y == touch::y) {
            return;
        }
    }

    // if events stack is full then remove oldest EVENT_TYPE_TOUCH_MOVE
    if (g_numEvents == MAX_EVENTS) {
        for (int i = 0; i < g_numEvents; ++i) {
            if (g_events[i].type == EVENT_TYPE_TOUCH_MOVE) {
                for (int j = i + 1; j < g_numEvents; ++j) {
                    g_events[j - 1] = g_events[j];
                }
                --g_numEvents;
                break;
            }
        }
    }

    if (g_numEvents < MAX_EVENTS) {
        // push new event on the stack
        g_events[g_numEvents].type = type;
        g_events[g_numEvents].x = touch::x;
        g_events[g_numEvents].y = touch::y;
        ++g_numEvents;
    }
}

void touchHandling(uint32_t tick_usec) {
    if (touch::calibration::isCalibrating()) {
        touch::calibration::tick(tick_usec);
        return;
    }

    if (g_activePageId == PAGE_ID_ENTERING_STANDBY) {
        return;
    }

    if (touch::event_type != touch::TOUCH_NONE) {
        g_timeOfLastActivity = millis();

        if (touch::event_type == touch::TOUCH_DOWN) {
            g_touchDownTime = tick_usec;
            g_lastAutoRepeatEventTime = tick_usec;
            g_longTapGenerated = false;
            pushEvent(EVENT_TYPE_TOUCH_DOWN);
        } else if (touch::event_type == touch::TOUCH_MOVE) {
            pushEvent(EVENT_TYPE_TOUCH_MOVE);

            if (!g_longTapGenerated && tick_usec - g_touchDownTime >= CONF_GUI_LONG_TAP_TIMEOUT) {
                g_longTapGenerated = true;
                pushEvent(EVENT_TYPE_LONG_TAP);
            }
            
            if (tick_usec - g_lastAutoRepeatEventTime >= CONF_GUI_KEYPAD_AUTO_REPEAT_DELAY) {
                pushEvent(EVENT_TYPE_AUTO_REPEAT);
                g_lastAutoRepeatEventTime = tick_usec;
            }
        } else if (touch::event_type == touch::TOUCH_UP) {
            pushEvent(EVENT_TYPE_TOUCH_UP);
        }
    }
}

void processEvents() {
    for (int i = 0; i < g_numEvents; ++i) {
        if (g_activePageId == PAGE_ID_SCREEN_CALIBRATION_INTRO) {
            if (g_events[i].type == EVENT_TYPE_TOUCH_UP) {
                touch::calibration::enterCalibrationMode(PAGE_ID_SCREEN_CALIBRATION_YES_NO_CANCEL, getStartPageId());
            }
        } else {
            if (g_events[i].type == EVENT_TYPE_TOUCH_DOWN) {
                g_touchActionExecuted = false;
                WidgetCursor foundWidget = findWidget(g_events[i].x, g_events[i].y);
                g_foundWidgetAtDown = 0;
                if (foundWidget) {
                    if (isActivePageInternal()) {
                        g_foundWidgetAtDown = foundWidget;
                    } else {
                        DECL_WIDGET(widget, foundWidget.widgetOffset);
                        if (isWidgetActionEnabled(widget)) {
                            g_foundWidgetAtDown = foundWidget;
                        }
                    }

                    if (g_foundWidgetAtDown) {
                        selectWidget(g_foundWidgetAtDown);
                    } else {
                        if (!isActivePageInternal()) {
                            DECL_WIDGET(widget, foundWidget.widgetOffset);
                            if (foundWidget && widget->type == WIDGET_TYPE_BUTTON_GROUP) {
                                widgetButtonGroup::onTouchDown(foundWidget);
                            } else if (g_activePageId == PAGE_ID_EDIT_MODE_SLIDER) {
                                edit_mode_slider::onTouchDown();
                            } else if (g_activePageId == PAGE_ID_EDIT_MODE_STEP) {
                                edit_mode_step::onTouchDown();
                            } else if (foundWidget && widget->type == WIDGET_TYPE_LIST_GRAPH) {
                                g_foundTouchWidget = foundWidget;
                                onTouchListGraph(g_foundTouchWidget, g_events[i].x, g_events[i].y);
                            } 
                        }
                    }
                }
            } else if (g_events[i].type == EVENT_TYPE_TOUCH_MOVE) {
                if (!g_foundWidgetAtDown) {
                    if (g_activePageId == PAGE_ID_EDIT_MODE_SLIDER) {
                        edit_mode_slider::onTouchMove();
                    } else if (g_activePageId == PAGE_ID_EDIT_MODE_STEP) {
                        edit_mode_step::onTouchMove();
                    } else if (g_activePageId == PAGE_ID_SCREEN_CALIBRATION_YES_NO || g_activePageId == PAGE_ID_SCREEN_CALIBRATION_YES_NO_CANCEL) {
        #ifdef CONF_DEBUG
                        int x = g_events[i].x;
                        if (x < 1) x = 1;
                        else if (x > lcd::lcd.getDisplayXSize() - 2) x = lcd::lcd.getDisplayXSize() - 2;

                        int y = g_events[i].y;
                        if (y < 1) y = 1;
                        else if (y > lcd::lcd.getDisplayYSize() - 2) y = lcd::lcd.getDisplayYSize() - 2;

                        lcd::lcd.setColor(VGA_WHITE);
                        lcd::lcd.fillRect(g_events[i].x - 1, g_events[i].y - 1, g_events[i].x + 1, g_events[i].y + 1);
        #endif
                    } else if (g_foundTouchWidget) {
                        DECL_WIDGET(widget, g_foundTouchWidget.widgetOffset);
                        onTouchListGraph(g_foundTouchWidget, g_events[i].x, g_events[i].y);
                    }
                }
            } else if (g_events[i].type == EVENT_TYPE_LONG_TAP) {
                if (!g_touchActionExecuted) {
                    if (g_activePageId == INTERNAL_PAGE_ID_NONE) {
                        g_touchActionExecuted = true;
                        psu::changePowerState(true);
                    } else if (g_activePageId == PAGE_ID_DISPLAY_OFF) {
                        g_touchActionExecuted = true;
                        persist_conf::setDisplayState(1);
                    } else {
                        if (g_foundWidgetAtDown) {
                            ActionType action = getAction(g_foundWidgetAtDown);
                            if (action == ACTION_ID_TURN_OFF) {
                                deselectWidget();
                                g_foundWidgetAtDown = 0;
                                g_touchActionExecuted = true;
                                showEnteringStandbyPage();
                                sound::playClick();
                                psu::changePowerState(false);
                            } else if (action == ACTION_ID_SYS_FRONT_PANEL_LOCK || action == ACTION_ID_SYS_FRONT_PANEL_UNLOCK) {
                                deselectWidget();
                                g_foundWidgetAtDown = 0;
                                g_touchActionExecuted = true;
                                sound::playClick();
                                if (isFrontPanelLocked()) {
                                    unlockFrontPanel();
                                } else {
                                    lockFrontPanel();
                                }
                            }
                        }
                    }
                }
            } else if (g_events[i].type == EVENT_TYPE_AUTO_REPEAT) {
                if (g_foundWidgetAtDown) {
                    ActionType action = getAction(g_foundWidgetAtDown);
                    if (action == ACTION_ID_KEYPAD_BACK || action == ACTION_ID_UP_DOWN) {
                        g_touchActionExecuted = true;
                        executeAction(action);
                    }
                }
            } else if (g_events[i].type == EVENT_TYPE_TOUCH_UP) {
                if (g_foundWidgetAtDown) {
                    deselectWidget();
                    ActionType action = getAction(g_foundWidgetAtDown);
                    if (!isLongPressAction(action)) {
                        executeAction(action);
                    }
                    g_foundWidgetAtDown = 0;
                } else {
                    if (g_activePageId == PAGE_ID_EDIT_MODE_SLIDER) {
                        edit_mode_slider::onTouchUp();
                    } else if (g_activePageId == PAGE_ID_EDIT_MODE_STEP) {
                        edit_mode_step::onTouchUp();
                    }
                }
            }
        }
    }

    g_numEvents = 0;
}

void tick(uint32_t tick_usec) {
    if (g_activePageId == INTERNAL_PAGE_ID_NONE) {
        processEvents();
        return;
    }

    // wait some time for transitional pages
    if (g_activePageId == PAGE_ID_STANDBY && tick_usec - g_showPageTime < CONF_GUI_STANDBY_PAGE_TIMEOUT) {
        return;
    } else if (g_activePageId == PAGE_ID_ENTERING_STANDBY && tick_usec - g_showPageTime < CONF_GUI_ENTERING_STANDBY_PAGE_TIMEOUT) {
        if (!psu::isPowerUp()) {
            uint32_t saved_showPageTime = g_showPageTime;
            showStandbyPage();
            g_showPageTime = saved_showPageTime - (CONF_GUI_STANDBY_PAGE_TIMEOUT - CONF_GUI_ENTERING_STANDBY_PAGE_TIMEOUT);
        }
        return;
    } else if (g_activePageId == PAGE_ID_WELCOME && tick_usec - g_showPageTime < CONF_GUI_WELCOME_PAGE_TIMEOUT) {
        return;
    }

    // turn the screen off if power is down
    if (!psu::isPowerUp()) {
        g_activePageId = INTERNAL_PAGE_ID_NONE;
        g_activePage = 0;
        turnOff();
        return;
    }

    // select page to go after transitional page
    if (g_activePageId == PAGE_ID_WELCOME || g_activePageId == PAGE_ID_STANDBY || g_activePageId == PAGE_ID_ENTERING_STANDBY) {
        if (!touch::calibration::isCalibrated()) {
            // touch screen is not calibrated
            setPage(PAGE_ID_SCREEN_CALIBRATION_INTRO);
        } else {
            setPage(getStartPageId());
        }
        return;
    }

    if (persist_conf::devConf2.flags.displayState == 0 && (g_activePageId != PAGE_ID_DISPLAY_OFF && g_activePageId != PAGE_ID_SELF_TEST_RESULT && touch::calibration::isCalibrated())) {
        setPage(PAGE_ID_DISPLAY_OFF);
        flush();
        return;
    } else if (persist_conf::devConf2.flags.displayState == 1 && g_activePageId == PAGE_ID_DISPLAY_OFF) {
        setPage(PAGE_ID_MAIN);
        return;
    }

    processEvents();

    if (g_activePageId == PAGE_ID_DISPLAY_OFF) {
        return;
    }

#if OPTION_ENCODER
    int counter;
    bool clicked;
    encoder::read(counter, clicked);
    onEncoder(counter, clicked);
#endif

    //
    uint32_t inactivityPeriod = millis() - g_timeOfLastActivity;

#if GUI_BACK_TO_MAIN_ENABLED
    if (g_activePageId == PAGE_ID_EVENT_QUEUE ||
        g_activePageId == PAGE_ID_USER_PROFILES ||
        g_activePageId == PAGE_ID_USER_PROFILES2 ||
        g_activePageId == PAGE_ID_USER_PROFILE_0_SETTINGS ||
        g_activePageId == PAGE_ID_USER_PROFILE_SETTINGS)
    {
        if (inactivityPeriod >= GUI_BACK_TO_MAIN_DELAY * 1000UL) {
            setPage(PAGE_ID_MAIN);
        }
    }
#endif

    if (g_activePageId == PAGE_ID_SCREEN_CALIBRATION_INTRO) {
        if (inactivityPeriod >= 20 * 1000UL) {
            touch::calibration::enterCalibrationMode(PAGE_ID_SCREEN_CALIBRATION_YES_NO_CANCEL, getStartPageId());
            return;
        }
    }

    if (g_activePageId == PAGE_ID_TOAST3_ALERT || g_activePageId == PAGE_ID_ERROR_TOAST_ALERT) {
        if (inactivityPeriod >= CONF_GUI_TOAST_DURATION_MS) {
            dialogOk();
            return;
        }
    }

    // update screen
    if (!touch::calibration::isCalibrating()) {
        drawTick();

        g_wasFocusCursor = g_focusCursor;
        g_wasFocusDataId = g_focusDataId;
    }
}

}
}
} // namespace eez::psu::gui

#endif