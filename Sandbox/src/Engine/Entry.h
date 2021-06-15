#pragma once
#include "xpch.h"

#ifndef ENGINE_HEADER
#error must include Engine.h before Entry.h
#endif

//to be defined on the client side
extern ApplicationProperties createApplication();
extern void onStartup();
extern void onShutdown();