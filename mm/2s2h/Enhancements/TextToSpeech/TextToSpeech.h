#pragma once

// 2S2H text-to-speech (Windows SAPI backend).
// Exposed with C linkage so game C files (z_message_nes.c) can call it.
#ifdef __cplusplus
extern "C" {
#endif

// Speak plain UTF-8/ASCII text, interrupting anything currently being spoken.
void TextToSpeech_Speak(const char* text);

// Stop speaking immediately.
void TextToSpeech_Stop(void);

#ifdef __cplusplus
}
#endif
