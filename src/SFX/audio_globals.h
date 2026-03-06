#pragma once

// Global audio state (updated by main loop). Used by small subsystems
// to temporarily override master volume (e.g. for forced SFX playback).

extern float g_gameMasterVolume;
extern bool g_gameMuted;

// Force-override timer and previous values used when a subsystem requests a short
// temporary override so a sound plays even when the player muted the game.
extern float g_forceAudioTimer; // seconds remaining for forced override
extern float g_forcePrevMasterVolume; // previous master volume to restore
extern bool  g_forcePrevMuted; // previous mute state to restore

