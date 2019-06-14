/*
    Joypad support is based on a stripped-down version of SDL's Darwin implementation
    of the Joystick API, under the following license:
*/

/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2017 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <AppKit/AppKit.h>
#include <stdint.h>
#include <stdbool.h>
#include <IOKit/hid/IOHIDLib.h>
#include "GBJoystickListener.h"

typedef signed SDL_JoystickID;
typedef struct _SDL_Joystick SDL_Joystick;

typedef struct _SDL_JoystickAxisInfo
{
    int16_t initial_value;       /* Initial axis state */
    int16_t value;               /* Current axis state */
    int16_t zero;                /* Zero point on the axis (-32768 for triggers) */
    bool has_initial_value; /* Whether we've seen a value on the axis yet */
    bool sent_initial_value; /* Whether we've sent the initial axis value */
} SDL_JoystickAxisInfo;

struct _SDL_Joystick
{
    SDL_JoystickID instance_id; /* Device instance, monotonically increasing from 0 */
    char *name;                 /* Joystick name - system dependent */
    
    int naxes;                  /* Number of axis controls on the joystick */
    SDL_JoystickAxisInfo *axes;
    
    int nbuttons;               /* Number of buttons on the joystick */
    uint8_t *buttons;             /* Current button states */
    
    int nhats;
    uint8_t *hats;
    
    struct joystick_hwdata *hwdata;     /* Driver dependent information */
    
    int ref_count;              /* Reference count for multiple opens */
    
    bool is_game_controller;
    bool force_recentering; /* SDL_TRUE if this device needs to have its state reset to 0 */
    struct _SDL_Joystick *next; /* pointer to next joystick we have allocated */
};

typedef struct {
    uint8_t data[16];
} SDL_JoystickGUID;

struct recElement
{
    IOHIDElementRef elementRef;
    IOHIDElementCookie cookie;
    uint32_t usagePage, usage;      /* HID usage */
    SInt32 min;                   /* reported min value possible */
    SInt32 max;                   /* reported max value possible */
    
    /* runtime variables used for auto-calibration */
    SInt32 minReport;             /* min returned value */
    SInt32 maxReport;             /* max returned value */
    
    struct recElement *pNext;   /* next element in list */
};
typedef struct recElement recElement;

struct joystick_hwdata
{
    IOHIDDeviceRef deviceRef;   /* HIDManager device handle */
    io_service_t ffservice;     /* Interface for force feedback, 0 = no ff */
    
    char product[256];          /* name of product */
    uint32_t usage;                 /* usage page from IOUSBHID Parser.h which defines general usage */
    uint32_t usagePage;             /* usage within above page from IOUSBHID Parser.h which defines specific usage */
    
    int axes;                  /* number of axis (calculated, not reported by device) */
    int buttons;               /* number of buttons (calculated, not reported by device) */
    int hats;
    int elements;              /* number of total elements (should be total of above) (calculated, not reported by device) */
    
    recElement *firstAxis;
    recElement *firstButton;
    recElement *firstHat;

    bool removed;
    
    int instance_id;
    SDL_JoystickGUID guid;
    
    SDL_Joystick joystick;
};
typedef struct joystick_hwdata recDevice;

/* The base object of the HID Manager API */
static IOHIDManagerRef hidman = NULL;

/* static incrementing counter for new joystick devices seen on the system. Devices should start with index 0 */
static int s_joystick_instance_id = -1;

#define SDL_JOYSTICK_AXIS_MAX 32767

void SDL_PrivateJoystickAxis(SDL_Joystick * joystick, uint8_t axis, int16_t value)
{
    /* Make sure we're not getting garbage or duplicate events */
    if (axis >= joystick->naxes) {
        return;
    }
    if (!joystick->axes[axis].has_initial_value) {
        joystick->axes[axis].initial_value = value;
        joystick->axes[axis].value = value;
        joystick->axes[axis].zero = value;
        joystick->axes[axis].has_initial_value = true;
    }
    if (value == joystick->axes[axis].value) {
        return;
    }
    if (!joystick->axes[axis].sent_initial_value) {
        /* Make sure we don't send motion until there's real activity on this axis */
        const int MAX_ALLOWED_JITTER = SDL_JOYSTICK_AXIS_MAX / 80;  /* ShanWan PS3 controller needed 96 */
        if (abs(value - joystick->axes[axis].value) <= MAX_ALLOWED_JITTER) {
            return;
        }
        joystick->axes[axis].sent_initial_value = true;
        joystick->axes[axis].value = value; /* Just so we pass the check above */
        SDL_PrivateJoystickAxis(joystick, axis, joystick->axes[axis].initial_value);
    }
    
    /* Update internal joystick state */
    joystick->axes[axis].value = value;
    
    NSResponder<GBJoystickListener> *responder = (typeof(responder)) [[NSApp keyWindow] firstResponder];
    while (responder) {
        if ([responder respondsToSelector:@selector(joystick:axis:movedTo:)]) {
            [responder joystick:@(joystick->name) axis:axis movedTo:value];
            break;
        }
        responder = (typeof(responder)) [responder nextResponder];
    }
}

void SDL_PrivateJoystickButton(SDL_Joystick *joystick, uint8_t button, uint8_t state)
{
    
    /* Make sure we're not getting garbage or duplicate events */
    if (button >= joystick->nbuttons) {
        return;
    }
    if (state == joystick->buttons[button]) {
        return;
    }
    
    /* Update internal joystick state */
    joystick->buttons[button] = state;
    
    NSResponder<GBJoystickListener> *responder = (typeof(responder)) [[NSApp keyWindow] firstResponder];
    while (responder) {
        if ([responder respondsToSelector:@selector(joystick:button:changedState:)]) {
            [responder joystick:@(joystick->name) button:button changedState:state];
            break;
        }
        responder = (typeof(responder)) [responder nextResponder];
    }
}

void SDL_PrivateJoystickHat(SDL_Joystick *joystick, uint8_t hat, uint8_t state)
{
    
    /* Make sure we're not getting garbage or duplicate events */
    if (hat >= joystick->nhats) {
        return;
    }
    if (state == joystick->hats[hat]) {
        return;
    }
    
    /* Update internal joystick state */
    joystick->hats[hat] = state;
    
    NSResponder<GBJoystickListener> *responder = (typeof(responder)) [[NSApp keyWindow] firstResponder];
    while (responder) {
        if ([responder respondsToSelector:@selector(joystick:button:changedState:)]) {
            [responder joystick:@(joystick->name) hat:hat changedState:state];
            break;
        }
        responder = (typeof(responder)) [responder nextResponder];
    }
}

static void
FreeElementList(recElement *pElement)
{
    while (pElement) {
        recElement *pElementNext = pElement->pNext;
        free(pElement);
        pElement = pElementNext;
    }
}


static recDevice *
FreeDevice(recDevice *removeDevice)
{
    recDevice *pDeviceNext = NULL;
    if (removeDevice) {
        if (removeDevice->deviceRef) {
            IOHIDDeviceUnscheduleFromRunLoop(removeDevice->deviceRef, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
            removeDevice->deviceRef = NULL;
        }

        /* free element lists */
        FreeElementList(removeDevice->firstAxis);
        FreeElementList(removeDevice->firstButton);
        FreeElementList(removeDevice->firstHat);

        free(removeDevice);
    }
    return pDeviceNext;
}

static SInt32
GetHIDElementState(recDevice *pDevice, recElement *pElement)
{
    SInt32 value = 0;

    if (pDevice && pElement) {
        IOHIDValueRef valueRef;
        if (IOHIDDeviceGetValue(pDevice->deviceRef, pElement->elementRef, &valueRef) == kIOReturnSuccess) {
            value = (SInt32) IOHIDValueGetIntegerValue(valueRef);

            /* record min and max for auto calibration */
            if (value < pElement->minReport) {
                pElement->minReport = value;
            }
            if (value > pElement->maxReport) {
                pElement->maxReport = value;
            }
        }
    }

    return value;
}

static SInt32
GetHIDScaledCalibratedState(recDevice * pDevice, recElement * pElement, SInt32 min, SInt32 max)
{
    const float deviceScale = max - min;
    const float readScale = pElement->maxReport - pElement->minReport;
    const SInt32 value = GetHIDElementState(pDevice, pElement);
    if (readScale == 0) {
        return value;           /* no scaling at all */
    }
    return ((value - pElement->minReport) * deviceScale / readScale) + min;
}


static void
JoystickDeviceWasRemovedCallback(void *ctx, IOReturn result, void *sender)
{
    recDevice *device = (recDevice *) ctx;
    device->removed = true;
    device->deviceRef = NULL; // deviceRef was invalidated due to the remove
    FreeDevice(device);
}


static void AddHIDElement(const void *value, void *parameter);

/* Call AddHIDElement() on all elements in an array of IOHIDElementRefs */
static void
AddHIDElements(CFArrayRef array, recDevice *pDevice)
{
    const CFRange range = { 0, CFArrayGetCount(array) };
    CFArrayApplyFunction(array, range, AddHIDElement, pDevice);
}

static bool
ElementAlreadyAdded(const IOHIDElementCookie cookie, const recElement *listitem) {
    while (listitem) {
        if (listitem->cookie == cookie) {
            return true;
        }
        listitem = listitem->pNext;
    }
    return false;
}

/* See if we care about this HID element, and if so, note it in our recDevice. */
static void
AddHIDElement(const void *value, void *parameter)
{
    recDevice *pDevice = (recDevice *) parameter;
    IOHIDElementRef refElement = (IOHIDElementRef) value;
    const CFTypeID elementTypeID = refElement ? CFGetTypeID(refElement) : 0;

    if (refElement && (elementTypeID == IOHIDElementGetTypeID())) {
        const IOHIDElementCookie cookie = IOHIDElementGetCookie(refElement);
        const uint32_t usagePage = IOHIDElementGetUsagePage(refElement);
        const uint32_t usage = IOHIDElementGetUsage(refElement);
        recElement *element = NULL;
        recElement **headElement = NULL;

        /* look at types of interest */
        switch (IOHIDElementGetType(refElement)) {
            case kIOHIDElementTypeInput_Misc:
            case kIOHIDElementTypeInput_Button:
            case kIOHIDElementTypeInput_Axis: {
                switch (usagePage) {    /* only interested in kHIDPage_GenericDesktop and kHIDPage_Button */
                    case kHIDPage_GenericDesktop:
                        switch (usage) {
                            case kHIDUsage_GD_X:
                            case kHIDUsage_GD_Y:
                            case kHIDUsage_GD_Z:
                            case kHIDUsage_GD_Rx:
                            case kHIDUsage_GD_Ry:
                            case kHIDUsage_GD_Rz:
                            case kHIDUsage_GD_Slider:
                            case kHIDUsage_GD_Dial:
                            case kHIDUsage_GD_Wheel:
                                if (!ElementAlreadyAdded(cookie, pDevice->firstAxis)) {
                                    element = (recElement *) calloc(1, sizeof (recElement));
                                    if (element) {
                                        pDevice->axes++;
                                        headElement = &(pDevice->firstAxis);
                                    }
                                }
                                break;
                            case kHIDUsage_GD_Hatswitch:
                                if (!ElementAlreadyAdded(cookie, pDevice->firstHat)) {
                                    element = (recElement *) calloc(1, sizeof (recElement));
                                    if (element) {
                                        pDevice->hats++;
                                        headElement = &(pDevice->firstHat);
                                    }
                                }
                                break;

                            case kHIDUsage_GD_DPadUp:
                            case kHIDUsage_GD_DPadDown:
                            case kHIDUsage_GD_DPadRight:
                            case kHIDUsage_GD_DPadLeft:
                            case kHIDUsage_GD_Start:
                            case kHIDUsage_GD_Select:
                            case kHIDUsage_GD_SystemMainMenu:
                                if (!ElementAlreadyAdded(cookie, pDevice->firstButton)) {
                                    element = (recElement *) calloc(1, sizeof (recElement));
                                    if (element) {
                                        pDevice->buttons++;
                                        headElement = &(pDevice->firstButton);
                                    }
                                }
                                break;
                        }
                        break;

                    case kHIDPage_Simulation:
                        switch (usage) {
                            case kHIDUsage_Sim_Rudder:
                            case kHIDUsage_Sim_Throttle:
                            case kHIDUsage_Sim_Accelerator:
                            case kHIDUsage_Sim_Brake:
                                if (!ElementAlreadyAdded(cookie, pDevice->firstAxis)) {
                                    element = (recElement *) calloc(1, sizeof (recElement));
                                    if (element) {
                                        pDevice->axes++;
                                        headElement = &(pDevice->firstAxis);
                                    }
                                }
                                break;

                            default:
                                break;
                        }
                        break;

                    case kHIDPage_Button:
                    case kHIDPage_Consumer: /* e.g. 'pause' button on Steelseries MFi gamepads. */
                        if (!ElementAlreadyAdded(cookie, pDevice->firstButton)) {
                            element = (recElement *) calloc(1, sizeof (recElement));
                            if (element) {
                                pDevice->buttons++;
                                headElement = &(pDevice->firstButton);
                            }
                        }
                        break;

                    default:
                        break;
                }
            }
            break;

            case kIOHIDElementTypeCollection: {
                CFArrayRef array = IOHIDElementGetChildren(refElement);
                if (array) {
                    AddHIDElements(array, pDevice);
                }
            }
            break;

            default:
                break;
        }

        if (element && headElement) {       /* add to list */
            recElement *elementPrevious = NULL;
            recElement *elementCurrent = *headElement;
            while (elementCurrent && usage >= elementCurrent->usage) {
                elementPrevious = elementCurrent;
                elementCurrent = elementCurrent->pNext;
            }
            if (elementPrevious) {
                elementPrevious->pNext = element;
            } else {
                *headElement = element;
            }

            element->elementRef = refElement;
            element->usagePage = usagePage;
            element->usage = usage;
            element->pNext = elementCurrent;

            element->minReport = element->min = (SInt32) IOHIDElementGetLogicalMin(refElement);
            element->maxReport = element->max = (SInt32) IOHIDElementGetLogicalMax(refElement);
            element->cookie = IOHIDElementGetCookie(refElement);

            pDevice->elements++;
        }
    }
}

static bool
GetDeviceInfo(IOHIDDeviceRef hidDevice, recDevice *pDevice)
{
    const uint16_t BUS_USB = 0x03;
    const uint16_t BUS_BLUETOOTH = 0x05;
    int32_t vendor = 0;
    int32_t product = 0;
    int32_t version = 0;
    CFTypeRef refCF = NULL;
    CFArrayRef array = NULL;
    uint16_t *guid16 = (uint16_t *)pDevice->guid.data;

    /* get usage page and usage */
    refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDPrimaryUsagePageKey));
    if (refCF) {
        CFNumberGetValue(refCF, kCFNumberSInt32Type, &pDevice->usagePage);
    }
    if (pDevice->usagePage != kHIDPage_GenericDesktop) {
        return false; /* Filter device list to non-keyboard/mouse stuff */
    }

    refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDPrimaryUsageKey));
    if (refCF) {
        CFNumberGetValue(refCF, kCFNumberSInt32Type, &pDevice->usage);
    }

    if ((pDevice->usage != kHIDUsage_GD_Joystick &&
         pDevice->usage != kHIDUsage_GD_GamePad &&
         pDevice->usage != kHIDUsage_GD_MultiAxisController)) {
        return false; /* Filter device list to non-keyboard/mouse stuff */
    }

    pDevice->deviceRef = hidDevice;

    /* get device name */
    refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDProductKey));
    if (!refCF) {
        /* Maybe we can't get "AwesomeJoystick2000", but we can get "Logitech"? */
        refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDManufacturerKey));
    }
    if ((!refCF) || (!CFStringGetCString(refCF, pDevice->product, sizeof (pDevice->product), kCFStringEncodingUTF8))) {
        strlcpy(pDevice->product, "Unidentified joystick", sizeof (pDevice->product));
    }

    refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDVendorIDKey));
    if (refCF) {
        CFNumberGetValue(refCF, kCFNumberSInt32Type, &vendor);
    }

    refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDProductIDKey));
    if (refCF) {
        CFNumberGetValue(refCF, kCFNumberSInt32Type, &product);
    }

    refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDVersionNumberKey));
    if (refCF) {
        CFNumberGetValue(refCF, kCFNumberSInt32Type, &version);
    }

    memset(pDevice->guid.data, 0, sizeof(pDevice->guid.data));

    if (vendor && product) {
        *guid16++ = BUS_USB;
        *guid16++ = 0;
        *guid16++ = vendor;
        *guid16++ = 0;
        *guid16++ = product;
        *guid16++ = 0;
        *guid16++ = version;
        *guid16++ = 0;
    } else {
        *guid16++ = BUS_BLUETOOTH;
        *guid16++ = 0;
        strlcpy((char*)guid16, pDevice->product, sizeof(pDevice->guid.data) - 4);
    }

    array = IOHIDDeviceCopyMatchingElements(hidDevice, NULL, kIOHIDOptionsTypeNone);
    if (array) {
        AddHIDElements(array, pDevice);
        CFRelease(array);
    }

    return true;
}

void
SDL_SYS_JoystickUpdate(SDL_Joystick * joystick)
{
    recDevice *device = joystick->hwdata;
    recElement *element;
    SInt32 value;
    int i;
    
    if (!device) {
        return;
    }
    
    if (device->removed) {      /* device was unplugged; ignore it. */
        if (joystick->hwdata) {
            joystick->force_recentering = true;
            joystick->hwdata = NULL;
        }
        return;
    }
    
    element = device->firstAxis;
    i = 0;
    while (element) {
        value = GetHIDScaledCalibratedState(device, element, -32768, 32767);
        SDL_PrivateJoystickAxis(joystick, i, value);
        element = element->pNext;
        ++i;
    }
    
    element = device->firstButton;
    i = 0;
    while (element) {
        value = GetHIDElementState(device, element);
        if (value > 1) {          /* handle pressure-sensitive buttons */
            value = 1;
        }
        SDL_PrivateJoystickButton(joystick, i, value);
        element = element->pNext;
        ++i;
    }
    
    element = device->firstHat;
    i = 0;
    while (element) {
        signed range = (element->max - element->min + 1);
        value = GetHIDElementState(device, element) - element->min;
        if (range == 4) {         /* 4 position hatswitch - scale up value */
            value *= 2;
        } else if (range != 8) {    /* Neither a 4 nor 8 positions - fall back to default position (centered) */
            value = -1;
        }
        if ((unsigned)value >= 8) {
            value = -1;
        }
        
        SDL_PrivateJoystickHat(joystick, i, value);
        
        element = element->pNext;
        ++i;
    }

}

static void JoystickInputCallback(
                  SDL_Joystick *          joystick,
                  IOReturn                result,
                  void * _Nullable        sender,
                  IOHIDReportType         type,
                  uint32_t                reportID,
                  uint8_t *               report,
                  CFIndex                 reportLength)
{
    SDL_SYS_JoystickUpdate(joystick);
}

static void
JoystickDeviceWasAddedCallback(void *ctx, IOReturn res, void *sender, IOHIDDeviceRef ioHIDDeviceObject)
{
    recDevice *device;
    io_service_t ioservice;

    if (res != kIOReturnSuccess) {
        return;
    }

    device = (recDevice *) calloc(1, sizeof(recDevice));

    if (!device) {
        abort();
        return;
    }

    if (!GetDeviceInfo(ioHIDDeviceObject, device)) {
        free(device);
        return;   /* not a device we care about, probably. */
    }

    SDL_Joystick *joystick = &device->joystick;
        
    joystick->instance_id = device->instance_id;
    joystick->hwdata = device;
    joystick->name = device->product;
    
    joystick->naxes = device->axes;
    joystick->nbuttons = device->buttons;
    joystick->nhats = device->hats;

    if (joystick->naxes > 0) {
        joystick->axes = (SDL_JoystickAxisInfo *) calloc(joystick->naxes, sizeof(SDL_JoystickAxisInfo));
    }
    if (joystick->nbuttons > 0) {
        joystick->buttons = (uint8_t *) calloc(joystick->nbuttons, 1);
    }
    if (joystick->nhats > 0) {
        joystick->hats = (uint8_t *) calloc(joystick->nhats, 1);
    }
    
    /* Get notified when this device is disconnected. */
    IOHIDDeviceRegisterRemovalCallback(ioHIDDeviceObject, JoystickDeviceWasRemovedCallback, device);
    static uint8_t junk[80];
    IOHIDDeviceRegisterInputReportCallback(ioHIDDeviceObject, junk, sizeof(junk), (IOHIDReportCallback) JoystickInputCallback, joystick);
    IOHIDDeviceScheduleWithRunLoop(ioHIDDeviceObject, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

    /* Allocate an instance ID for this device */
    device->instance_id = ++s_joystick_instance_id;

    /* We have to do some storage of the io_service_t for SDL_HapticOpenFromJoystick */
    ioservice = IOHIDDeviceGetService(ioHIDDeviceObject);
}

static bool
ConfigHIDManager(CFArrayRef matchingArray)
{
    CFRunLoopRef runloop = CFRunLoopGetCurrent();

    if (IOHIDManagerOpen(hidman, kIOHIDOptionsTypeNone) != kIOReturnSuccess) {
        return false;
    }

    IOHIDManagerSetDeviceMatchingMultiple(hidman, matchingArray);
    IOHIDManagerRegisterDeviceMatchingCallback(hidman, JoystickDeviceWasAddedCallback, NULL);
    IOHIDManagerScheduleWithRunLoop(hidman, runloop, kCFRunLoopDefaultMode);

    return true;  /* good to go. */
}


static CFDictionaryRef
CreateHIDDeviceMatchDictionary(const UInt32 page, const UInt32 usage, int *okay)
{
    CFDictionaryRef retval = NULL;
    CFNumberRef pageNumRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);
    CFNumberRef usageNumRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);
    const void *keys[2] = { (void *) CFSTR(kIOHIDDeviceUsagePageKey), (void *) CFSTR(kIOHIDDeviceUsageKey) };
    const void *vals[2] = { (void *) pageNumRef, (void *) usageNumRef };

    if (pageNumRef && usageNumRef) {
        retval = CFDictionaryCreate(kCFAllocatorDefault, keys, vals, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    }

    if (pageNumRef) {
        CFRelease(pageNumRef);
    }
    if (usageNumRef) {
        CFRelease(usageNumRef);
    }

    if (!retval) {
        *okay = 0;
    }

    return retval;
}

static bool
CreateHIDManager(void)
{
    bool retval = false;
    int okay = 1;
    const void *vals[] = {
        (void *) CreateHIDDeviceMatchDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick, &okay),
        (void *) CreateHIDDeviceMatchDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad, &okay),
        (void *) CreateHIDDeviceMatchDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController, &okay),
    };
    const size_t numElements = sizeof(vals) / sizeof(vals[0]);
    CFArrayRef array = okay ? CFArrayCreate(kCFAllocatorDefault, vals, numElements, &kCFTypeArrayCallBacks) : NULL;
    size_t i;

    for (i = 0; i < numElements; i++) {
        if (vals[i]) {
            CFRelease((CFTypeRef) vals[i]);
        }
    }

    if (array) {
        hidman = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
        if (hidman != NULL) {
            retval = ConfigHIDManager(array);
        }
        CFRelease(array);
    }

    return retval;
}


void __attribute__((constructor)) SDL_SYS_JoystickInit(void)
{
    if (!CreateHIDManager()) {
        fprintf(stderr, "Joystick: Couldn't initialize HID Manager");
    }
}
