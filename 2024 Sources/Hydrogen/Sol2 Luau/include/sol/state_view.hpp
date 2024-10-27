// sol2

// The MIT License (MIT)

// Copyright (c) 2013-2022 Rapptz, ThePhD and contributors

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef SOL_STATE_VIEW_HPP
#define SOL_STATE_VIEW_HPP

#include <sol/error.hpp>
#include <sol/table.hpp>
#include <sol/environment.hpp>
#include <sol/load_result.hpp>
#include <sol/state_handling.hpp>

#include <memory>
#include <cstddef>

namespace sol {

	class state_view {
	private:
		lua_State* L;
		table reg;
		global_table global;

		optional<object> is_loaded_package(const std::string& key) {
			auto loaded = reg.traverse_get<optional<object>>("_LOADED", key);
			bool is53mod = loaded && !(loaded->is<bool>() && !loaded->as<bool>());
			if (is53mod)
				return loaded;
#if SOL_LUA_VERSION_I_ <= 501
			auto loaded51 = global.traverse_get<optional<object>>("package", "loaded", key);
			bool is51mod = loaded51 && !(loaded51->is<bool>() && !loaded51->as<bool>());
			if (is51mod)
				return loaded51;
#endif
			return nullopt;
		}

		template <typename T>
		void ensure_package(const std::string& key, T&& sr) {
#if SOL_LUA_VERSION_I_ <= 501
			auto pkg = global["package"];
			if (!pkg.valid()) {
				pkg = create_table_with("loaded", create_table_with(key, sr));
			}
			else {
				auto ld = pkg["loaded"];
				if (!ld.valid()) {
					ld = create_table_with(key, sr);
				}
				else {
					ld[key] = sr;
				}
			}
#endif
			auto loaded = reg["_LOADED"];
			if (!loaded.valid()) {
				loaded = create_table_with(key, sr);
			}
			else {
				loaded[key] = sr;
			}
		}

		template <typename Fx>
		object require_core(const std::string& key, Fx&& action, bool create_global = true) {
			optional<object> loaded = is_loaded_package(key);
			if (loaded && loaded->valid())
				return std::move(*loaded);
			int before = lua_gettop(L);
			action();
			int after = lua_gettop(L);
			if (before == after) {
				// I mean, you were supposed to return
				// something, ANYTHING, from your requires script. I guess I'll just
				// but some trash in here, it's on you after that?
				ensure_package(key, static_cast<void*>(L));
				return object(L, lua_nil);
			}
			stack_reference sr(L, -1);
			if (create_global)
				set(key, sr);
			ensure_package(key, sr);
			return stack::pop<object>(L);
		}

	public:
		using iterator = typename global_table::iterator;
		using const_iterator = typename global_table::const_iterator;

		state_view(lua_State* Ls) : L(Ls), reg(Ls, LUA_REGISTRYINDEX), global(Ls, global_tag) {
		}

		state_view(this_state Ls) : state_view(Ls.L) {
		}

		lua_State* lua_state() const {
			return L;
		}

		iterator begin() const {
			return global.begin();
		}

		iterator end() const {
			return global.end();
		}

		const_iterator cbegin() const {
			return global.cbegin();
		}

		const_iterator cend() const {
			return global.cend();
		}

		global_table lua_globals() const {
			// if we return a reference
			// we'll be screwed a bit
			return global;
		}

		global_table& lua_globals() {
			return global;
		}

		table lua_registry() const {
			return reg;
		}

		std::size_t memory_used() const {
			return total_memory_used(lua_state());
		}

		int stack_top() const {
			return stack::top(L);
		}

		int stack_clear() {
			int s = stack_top();
			lua_pop(L, s);
			return s;
		}

		bool supports_gc_mode(gc_mode mode) const noexcept {
#if SOL_LUA_VERSION_I_ >= 504
			// supports all modes
			(void)mode;
			return true;
#endif
			return mode == gc_mode::default_value;
		}

		bool is_gc_on() const {
#if SOL_LUA_VERSION_I_ >= 502
			return lua_gc(lua_state(), LUA_GCISRUNNING, 0) == 1;
#else
			// You cannot turn it off in Lua 5.1
			return true;
#endif
		}

		void collect_garbage() {
			lua_gc(lua_state(), LUA_GCCOLLECT, 0);
		}

		void collect_gc() {
			collect_garbage();
		}

		bool step_gc(int step_size_kilobytes) {
			// THOUGHT: std::chrono-alikes to map "kilobyte size" here...?
			// Make it harder to give MB or KB to a B parameter...?
			// Probably overkill for now.
#if SOL_LUA_VERSION_I_ >= 504
			// The manual implies that this function is almost always successful...
			// is it?? It could depend on the GC mode...
			return lua_gc(lua_state(), LUA_GCSTEP, step_size_kilobytes) != 0;
#else
			return lua_gc(lua_state(), LUA_GCSTEP, step_size_kilobytes) == 1;
#endif
		}

		void restart_gc() {
			lua_gc(lua_state(), LUA_GCRESTART, 0);
		}

		void stop_gc() {
			lua_gc(lua_state(), LUA_GCSTOP, 0);
		}

		// Returns the old GC mode. Check support using the supports_gc_mode function.
		gc_mode change_gc_mode_incremental(int pause, int step_multiplier, int step_byte_size) {
			// "What the fuck does any of this mean??"
			// http://www.lua.org/manual/5.4/manual.html#2.5.1

			// THOUGHT: std::chrono-alikes to map "byte size" here...?
			// Make it harder to give MB or KB to a B parameter...?
			// Probably overkill for now.
#if SOL_LUA_VERSION_I_ >= 504
			int old_mode = lua_gc(lua_state(), LUA_GCINC, pause, step_multiplier, step_byte_size);
			if (old_mode == LUA_GCGEN) {
				return gc_mode::generational;
			}
			else if (old_mode == LUA_GCINC) {
				return gc_mode::incremental;
			}
#else
			//lua_gc(lua_state(), LUA_GCSETPAUSE, pause);
			lua_gc(lua_state(), LUA_GCSETSTEPMUL, step_multiplier);
			(void)step_byte_size; // means nothing in older versions
#endif
			return gc_mode::default_value;
		}

		// Returns the old GC mode. Check support using the supports_gc_mode function.
		gc_mode change_gc_mode_generational(int minor_multiplier, int major_multiplier) {
#if SOL_LUA_VERSION_I_ >= 504
			// "What does this shit mean?"
			// http://www.lua.org/manual/5.4/manual.html#2.5.2
			int old_mode = lua_gc(lua_state(), LUA_GCGEN, minor_multiplier, major_multiplier);
			if (old_mode == LUA_GCGEN) {
				return gc_mode::generational;
			}
			else if (old_mode == LUA_GCINC) {
				return gc_mode::incremental;
			}
#else
			(void)minor_multiplier;
			(void)major_multiplier;
#endif
			return gc_mode::default_value;
		}

		operator lua_State*() const {
			return lua_state();
		}

		void set_panic(lua_CFunction panic) {
			//lua_atpanic(lua_state(), panic);
		}

		void set_exception_handler(exception_handler_function handler) {
			set_default_exception_handler(lua_state(), handler);
		}

		template <typename... Args, typename... Keys>
		decltype(auto) get(Keys&&... keys) const {
			return global.get<Args...>(std::forward<Keys>(keys)...);
		}

		template <typename T, typename Key>
		decltype(auto) get_or(Key&& key, T&& otherwise) const {
			return global.get_or(std::forward<Key>(key), std::forward<T>(otherwise));
		}

		template <typename T, typename Key, typename D>
		decltype(auto) get_or(Key&& key, D&& otherwise) const {
			return global.get_or<T>(std::forward<Key>(key), std::forward<D>(otherwise));
		}

		template <typename... Args>
		state_view& set(Args&&... args) {
			global.set(std::forward<Args>(args)...);
			return *this;
		}

		template <typename T, typename... Keys>
		decltype(auto) traverse_get(Keys&&... keys) const {
			return global.traverse_get<T>(std::forward<Keys>(keys)...);
		}

		template <typename... Args>
		state_view& traverse_set(Args&&... args) {
			global.traverse_set(std::forward<Args>(args)...);
			return *this;
		}

		template <typename Class, typename... Args>
		usertype<Class> new_usertype(Args&&... args) {
			return global.new_usertype<Class>(std::forward<Args>(args)...);
		}

		template <bool read_only = true, typename... Args>
		state_view& new_enum(const string_view& name, Args&&... args) {
			global.new_enum<read_only>(name, std::forward<Args>(args)...);
			return *this;
		}

		template <typename T, bool read_only = true>
		state_view& new_enum(const string_view& name, std::initializer_list<std::pair<string_view, T>> items) {
			global.new_enum<T, read_only>(name, std::move(items));
			return *this;
		}

		template <typename Fx>
		void for_each(Fx&& fx) {
			global.for_each(std::forward<Fx>(fx));
		}

		template <typename T>
		table_proxy<global_table&, detail::proxy_key_t<T>> operator[](T&& key) {
			return global[std::forward<T>(key)];
		}

		template <typename T>
		table_proxy<const global_table&, detail::proxy_key_t<T>> operator[](T&& key) const {
			return global[std::forward<T>(key)];
		}

		template <typename Sig, typename... Args, typename Key>
		state_view& set_function(Key&& key, Args&&... args) {
			global.set_function<Sig>(std::forward<Key>(key), std::forward<Args>(args)...);
			return *this;
		}

		template <typename... Args, typename Key>
		state_view& set_function(Key&& key, Args&&... args) {
			global.set_function(std::forward<Key>(key), std::forward<Args>(args)...);
			return *this;
		}

		template <typename Name>
		table create_table(Name&& name, int narr = 0, int nrec = 0) {
			return global.create(std::forward<Name>(name), narr, nrec);
		}

		template <typename Name, typename Key, typename Value, typename... Args>
		table create_table(Name&& name, int narr, int nrec, Key&& key, Value&& value, Args&&... args) {
			return global.create(std::forward<Name>(name), narr, nrec, std::forward<Key>(key), std::forward<Value>(value), std::forward<Args>(args)...);
		}

		template <typename Name, typename... Args>
		table create_named_table(Name&& name, Args&&... args) {
			table x = global.create_with(std::forward<Args>(args)...);
			global.set(std::forward<Name>(name), x);
			return x;
		}

		table create_table(int narr = 0, int nrec = 0) {
			return create_table(lua_state(), narr, nrec);
		}

		template <typename Key, typename Value, typename... Args>
		table create_table(int narr, int nrec, Key&& key, Value&& value, Args&&... args) {
			return create_table(lua_state(), narr, nrec, std::forward<Key>(key), std::forward<Value>(value), std::forward<Args>(args)...);
		}

		template <typename... Args>
		table create_table_with(Args&&... args) {
			return create_table_with(lua_state(), std::forward<Args>(args)...);
		}

		static inline table create_table(lua_State* L, int narr = 0, int nrec = 0) {
			return global_table::create(L, narr, nrec);
		}

		template <typename Key, typename Value, typename... Args>
		static inline table create_table(lua_State* L, int narr, int nrec, Key&& key, Value&& value, Args&&... args) {
			return global_table::create(L, narr, nrec, std::forward<Key>(key), std::forward<Value>(value), std::forward<Args>(args)...);
		}

		template <typename... Args>
		static inline table create_table_with(lua_State* L, Args&&... args) {
			return global_table::create_with(L, std::forward<Args>(args)...);
		}
	};
} // namespace sol

#endif // SOL_STATE_VIEW_HPP
