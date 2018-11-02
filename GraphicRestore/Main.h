#pragma once

#include "Main.h"
#include "GraphicRestore.h"
#include "Debug.h"

#include <Windows.h>

GraphicRestore* main;

void Main();

static void WINAPI Timer();
