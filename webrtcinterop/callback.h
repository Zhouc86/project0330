#pragma once
#include "pch.h"

#include "./include/api.h"

template <typename... Args>
struct Callback {
	/// Type of the raw callback function.
	/// The first parameter is the opaque user data pointer.
	using callback_type = void(*)(void*, Args...);

	/// Pointer to the raw function to invoke.
	callback_type callback_{};

	/// User-provided opaque pointer passed as first argument to the raw function.
	void* user_data_{};
	int32_t peer{};
	int32_t index{};
	/// Check if the callback has a valid function pointer.
	constexpr explicit operator bool() const  {
		return (callback_ != nullptr);
	}

	/// Invoke the callback with the given arguments |args|.
	constexpr void operator()(Args... args) const  {
		if (callback_ != nullptr) {
			(*callback_)(user_data_, std::forward<Args>(args)...);
		}
	}
};