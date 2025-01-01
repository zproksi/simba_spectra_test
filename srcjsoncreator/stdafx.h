#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <future>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <regex>
#include <set>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#if !defined(__linux__) && !(defined(__APPLE__) && defined(__MACH__))
#pragma comment(lib, "Ws2_32.lib")
#include <windows.h>
#include <winsock.h>
#else
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#endif

