#pragma once

#include "Delegate.h"
#include "MulticastDelegate.h"

#define JE_DEFINE_DELEGATE(Delegate, ReturnValue, ...) using Delegate = JE::FDelegate<ReturnValue __VA_OPT__(, ) __VA_ARGS__>

#define JE_DEFINE_MULTICAST_DELEGATE(Delegate, ...) using Delegate = JE::FMulticastDelegate<__VA_ARGS__>
