#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <iomanip>

//decl
#include "Wrapper.hpp"
#include "Writer.hpp"
#include "Exception.hpp"
#include "Timer.hpp"





using Wrapper::HFile;
using Writer::Error;
using Writer::Log;
using Writer::Warning;
using Writer::eOut::Console;
using Writer::eOut::File;
using Exception::Invoke;

using Size = struct
{
    float Width;
    float Height;
};

//impl
#include "Wrapper.inl"
#include "CoreWindow.hpp"
#include "Writer.inl"
#include "Exception.inl"
#include "Timer.inl"