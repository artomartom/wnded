#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream> //
#include <fstream> //
#include <sstream>
#include <iomanip>
#include <assert.h>
#include <compare>
#include <mutex>
#include <tuple>
// decl
#include "Writer.hpp"
#include "Exception.hpp"
#include "Timer.hpp"

using ::Exception::Invoke;
using ::Writer::Error;
using ::Writer::Log;
using ::Writer::Warning;
using ::Writer::Out::Console;
using ::Writer::Out::File;

// impl
#include "Writer.inl"
#include "Exception.inl"
#include "Timer.inl"
