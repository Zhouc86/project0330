/* -LICENSE-START-
** Copyright (c) 2016 Blackmagic Design
**
** Permission is hereby granted, free of charge, to any person or organization
** obtaining a copy of the software and accompanying documentation covered by
** this license (the "Software") to use, reproduce, display, distribute,
** execute, and transmit the Software, and to prepare derivative works of the
** Software, and to permit third-parties to whom the Software is furnished to
** do so, all subject to the following:
** 
** The copyright notices in the Software and this entire statement, including
** the above license grant, this restriction and the following disclaimer,
** must be included in all copies of the Software, in whole or in part, and
** all derivative works of the Software, unless such copies or derivative
** works are solely in the form of machine-executable object code generated by
** a source language processor.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
** SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
** FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
** -LICENSE-END-
*/

#ifndef BMD_DECKLINKAPIDECKCONTROL_H
#define BMD_DECKLINKAPIDECKCONTROL_H


#ifndef BMD_CONST
    #if defined(_MSC_VER)
        #define BMD_CONST __declspec(selectany) static const
    #else
        #define BMD_CONST static const
    #endif
#endif

// Type Declarations


// Interface ID Declarations

BMD_CONST REFIID IID_IDeckLinkDeckControlStatusCallback           = /* 53436FFB-B434-4906-BADC-AE3060FFE8EF */ {0x53,0x43,0x6F,0xFB,0xB4,0x34,0x49,0x06,0xBA,0xDC,0xAE,0x30,0x60,0xFF,0xE8,0xEF};
BMD_CONST REFIID IID_IDeckLinkDeckControl                         = /* 8E1C3ACE-19C7-4E00-8B92-D80431D958BE */ {0x8E,0x1C,0x3A,0xCE,0x19,0xC7,0x4E,0x00,0x8B,0x92,0xD8,0x04,0x31,0xD9,0x58,0xBE};

/* Enum BMDDeckControlMode - DeckControl mode */

typedef uint32_t BMDDeckControlMode;
enum _BMDDeckControlMode {
    bmdDeckControlNotOpened                                      = 'ntop',
    bmdDeckControlVTRControlMode                                 = 'vtrc',
    bmdDeckControlExportMode                                     = 'expm',
    bmdDeckControlCaptureMode                                    = 'capm'
};

/* Enum BMDDeckControlEvent - DeckControl event */

typedef uint32_t BMDDeckControlEvent;
enum _BMDDeckControlEvent {
    bmdDeckControlAbortedEvent                                   = 'abte',	// This event is triggered when a capture or edit-to-tape operation is aborted.

    /* Export-To-Tape events */

    bmdDeckControlPrepareForExportEvent                          = 'pfee',	// This event is triggered a few frames before reaching the in-point. IDeckLinkInput::StartScheduledPlayback() should be called at this point.
    bmdDeckControlExportCompleteEvent                            = 'exce',	// This event is triggered a few frames after reaching the out-point. At this point, it is safe to stop playback.

    /* Capture events */

    bmdDeckControlPrepareForCaptureEvent                         = 'pfce',	// This event is triggered a few frames before reaching the in-point. The serial timecode attached to IDeckLinkVideoInputFrames is now valid.
    bmdDeckControlCaptureCompleteEvent                           = 'ccev'	// This event is triggered a few frames after reaching the out-point.
};

/* Enum BMDDeckControlVTRControlState - VTR Control state */

typedef uint32_t BMDDeckControlVTRControlState;
enum _BMDDeckControlVTRControlState {
    bmdDeckControlNotInVTRControlMode                            = 'nvcm',
    bmdDeckControlVTRControlPlaying                              = 'vtrp',
    bmdDeckControlVTRControlRecording                            = 'vtrr',
    bmdDeckControlVTRControlStill                                = 'vtra',
    bmdDeckControlVTRControlShuttleForward                       = 'vtsf',
    bmdDeckControlVTRControlShuttleReverse                       = 'vtsr',
    bmdDeckControlVTRControlJogForward                           = 'vtjf',
    bmdDeckControlVTRControlJogReverse                           = 'vtjr',
    bmdDeckControlVTRControlStopped                              = 'vtro'
};

/* Enum BMDDeckControlStatusFlags - Deck Control status flags */

typedef uint32_t BMDDeckControlStatusFlags;
enum _BMDDeckControlStatusFlags {
    bmdDeckControlStatusDeckConnected                            = 1 << 0,
    bmdDeckControlStatusRemoteMode                               = 1 << 1,
    bmdDeckControlStatusRecordInhibited                          = 1 << 2,
    bmdDeckControlStatusCassetteOut                              = 1 << 3
};

/* Enum BMDDeckControlExportModeOpsFlags - Export mode flags */

typedef uint32_t BMDDeckControlExportModeOpsFlags;
enum _BMDDeckControlExportModeOpsFlags {
    bmdDeckControlExportModeInsertVideo                          = 1 << 0,
    bmdDeckControlExportModeInsertAudio1                         = 1 << 1,
    bmdDeckControlExportModeInsertAudio2                         = 1 << 2,
    bmdDeckControlExportModeInsertAudio3                         = 1 << 3,
    bmdDeckControlExportModeInsertAudio4                         = 1 << 4,
    bmdDeckControlExportModeInsertAudio5                         = 1 << 5,
    bmdDeckControlExportModeInsertAudio6                         = 1 << 6,
    bmdDeckControlExportModeInsertAudio7                         = 1 << 7,
    bmdDeckControlExportModeInsertAudio8                         = 1 << 8,
    bmdDeckControlExportModeInsertAudio9                         = 1 << 9,
    bmdDeckControlExportModeInsertAudio10                        = 1 << 10,
    bmdDeckControlExportModeInsertAudio11                        = 1 << 11,
    bmdDeckControlExportModeInsertAudio12                        = 1 << 12,
    bmdDeckControlExportModeInsertTimeCode                       = 1 << 13,
    bmdDeckControlExportModeInsertAssemble                       = 1 << 14,
    bmdDeckControlExportModeInsertPreview                        = 1 << 15,
    bmdDeckControlUseManualExport                                = 1 << 16
};

/* Enum BMDDeckControlError - Deck Control error */

typedef uint32_t BMDDeckControlError;
enum _BMDDeckControlError {
    bmdDeckControlNoError                                        = 'noer',
    bmdDeckControlModeError                                      = 'moer',
    bmdDeckControlMissedInPointError                             = 'mier',
    bmdDeckControlDeckTimeoutError                               = 'dter',
    bmdDeckControlCommandFailedError                             = 'cfer',
    bmdDeckControlDeviceAlreadyOpenedError                       = 'dalo',
    bmdDeckControlFailedToOpenDeviceError                        = 'fder',
    bmdDeckControlInLocalModeError                               = 'lmer',
    bmdDeckControlEndOfTapeError                                 = 'eter',
    bmdDeckControlUserAbortError                                 = 'uaer',
    bmdDeckControlNoTapeInDeckError                              = 'nter',
    bmdDeckControlNoVideoFromCardError                           = 'nvfc',
    bmdDeckControlNoCommunicationError                           = 'ncom',
    bmdDeckControlBufferTooSmallError                            = 'btsm',
    bmdDeckControlBadChecksumError                               = 'chks',
    bmdDeckControlUnknownError                                   = 'uner'
};

// Forward Declarations

class IDeckLinkDeckControlStatusCallback;
class IDeckLinkDeckControl;

/* Interface IDeckLinkDeckControlStatusCallback - Deck control state change callback. */

class IDeckLinkDeckControlStatusCallback : public IUnknown
{
public:
    virtual HRESULT TimecodeUpdate (/* in */ BMDTimecodeBCD currentTimecode) = 0;
    virtual HRESULT VTRControlStateChanged (/* in */ BMDDeckControlVTRControlState newState, /* in */ BMDDeckControlError error) = 0;
    virtual HRESULT DeckControlEventReceived (/* in */ BMDDeckControlEvent event, /* in */ BMDDeckControlError error) = 0;
    virtual HRESULT DeckControlStatusChanged (/* in */ BMDDeckControlStatusFlags flags, /* in */ uint32_t mask) = 0;

protected:
    virtual ~IDeckLinkDeckControlStatusCallback () {} // call Release method to drop reference count
};

/* Interface IDeckLinkDeckControl - Deck Control main interface */

class IDeckLinkDeckControl : public IUnknown
{
public:
    virtual HRESULT Open (/* in */ BMDTimeScale timeScale, /* in */ BMDTimeValue timeValue, /* in */ bool timecodeIsDropFrame, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT Close (/* in */ bool standbyOn) = 0;
    virtual HRESULT GetCurrentState (/* out */ BMDDeckControlMode *mode, /* out */ BMDDeckControlVTRControlState *vtrControlState, /* out */ BMDDeckControlStatusFlags *flags) = 0;
    virtual HRESULT SetStandby (/* in */ bool standbyOn) = 0;
    virtual HRESULT SendCommand (/* in */ uint8_t *inBuffer, /* in */ uint32_t inBufferSize, /* out */ uint8_t *outBuffer, /* out */ uint32_t *outDataSize, /* in */ uint32_t outBufferSize, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT Play (/* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT Stop (/* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT TogglePlayStop (/* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT Eject (/* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT GoToTimecode (/* in */ BMDTimecodeBCD timecode, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT FastForward (/* in */ bool viewTape, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT Rewind (/* in */ bool viewTape, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT StepForward (/* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT StepBack (/* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT Jog (/* in */ double rate, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT Shuttle (/* in */ double rate, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT GetTimecodeString (/* out */ CFStringRef *currentTimeCode, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT GetTimecode (/* out */ IDeckLinkTimecode **currentTimecode, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT GetTimecodeBCD (/* out */ BMDTimecodeBCD *currentTimecode, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT SetPreroll (/* in */ uint32_t prerollSeconds) = 0;
    virtual HRESULT GetPreroll (/* out */ uint32_t *prerollSeconds) = 0;
    virtual HRESULT SetExportOffset (/* in */ int32_t exportOffsetFields) = 0;
    virtual HRESULT GetExportOffset (/* out */ int32_t *exportOffsetFields) = 0;
    virtual HRESULT GetManualExportOffset (/* out */ int32_t *deckManualExportOffsetFields) = 0;
    virtual HRESULT SetCaptureOffset (/* in */ int32_t captureOffsetFields) = 0;
    virtual HRESULT GetCaptureOffset (/* out */ int32_t *captureOffsetFields) = 0;
    virtual HRESULT StartExport (/* in */ BMDTimecodeBCD inTimecode, /* in */ BMDTimecodeBCD outTimecode, /* in */ BMDDeckControlExportModeOpsFlags exportModeOps, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT StartCapture (/* in */ bool useVITC, /* in */ BMDTimecodeBCD inTimecode, /* in */ BMDTimecodeBCD outTimecode, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT GetDeviceID (/* out */ uint16_t *deviceId, /* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT Abort (void) = 0;
    virtual HRESULT CrashRecordStart (/* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT CrashRecordStop (/* out */ BMDDeckControlError *error) = 0;
    virtual HRESULT SetCallback (/* in */ IDeckLinkDeckControlStatusCallback *callback) = 0;

protected:
    virtual ~IDeckLinkDeckControl () {} // call Release method to drop reference count
};

/* Functions */

extern "C" {


}


#endif /* defined(BMD_DECKLINKAPIDECKCONTROL_H) */
