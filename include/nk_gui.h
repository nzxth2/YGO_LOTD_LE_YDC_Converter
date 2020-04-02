#pragma once

struct WINDOW_DATA;
struct EVENT_DATA;
struct FILE_DATA;

void SetupGui(WINDOW_DATA &windowData,unsigned int initialWindowWidth, unsigned int initialWindowHeight);
void HandleInput(WINDOW_DATA &windowData);
int HandleEvent(const EVENT_DATA &eventData);
void HandleGui();
void RenderGui();
void CleanupGui();
void UpdateWindowSize(unsigned int width, unsigned int height);