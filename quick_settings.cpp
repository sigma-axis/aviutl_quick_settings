/*
The MIT License (MIT)

Copyright (c) 2024 sigma-axis

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <cstdint>
#include <algorithm>
#include <memory>
#include <vector>
#include <forward_list>
#include <string>
#include <charconv>
#include <map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma comment(lib, "imm32")
#include <CommCtrl.h>
#pragma comment(lib, "comctl32")

#pragma warning(push)
#pragma warning(disable : 6387 26454)
#include <json.h>
#pragma warning(pop)

using byte = uint8_t;
#include <exedit.hpp>

////////////////////////////////
// 主要情報源の変数アドレス．
////////////////////////////////
inline constinit struct AviUtl110 {
	constexpr static int32_t build110 = 11003;
	bool init(HMODULE hinst, AviUtl::SysInfo const& si)
	{
		if (si.build != build110) return false;

		instance = hinst;
		font = si.hfont;
		init_pointers();

		return true;
	}

	HMODULE instance;
	HFONT font;

	// 任意フレーム数移動
	int32_t(*	frame_jump_distance)[4];	// 0x086424

	// 編集のレジューム機能を有効
	int32_t*	resume_enabled;				// 0x086268

	// 関連ウィンドウ同士を移動時にスナップする
	int32_t*	snaps_window;				// 0x086434

	// トラックバーでクリックした位置に直接移動する
	int32_t*	seeks_clicked_point;		// 0x086270

private:
	void init_pointers()
	{
		auto pick_addr = [aviutl_base = reinterpret_cast<uintptr_t>(instance)]
			<class T>(T& target, ptrdiff_t offset) { target = reinterpret_cast<T>(aviutl_base + offset); };

		pick_addr(frame_jump_distance,	0x086424);
		pick_addr(resume_enabled,		0x086268);
		pick_addr(snaps_window,			0x086434);
		pick_addr(seeks_clicked_point,	0x086270);
	}
} aviutl;

enum class time_fmt :int32_t {
	frame = 0,
	hms = 1,
};
inline constinit struct ExEdit092 {
	AviUtl::FilterPlugin* fp;
	constexpr static auto info_exedit092 = "拡張編集(exedit) version 0.92 by ＫＥＮくん";
	bool init(AviUtl::FilterPlugin* this_fp)
	{
		if (fp != nullptr) return true;

		AviUtl::SysInfo si; this_fp->exfunc->get_sys_info(nullptr, &si);
		if (!aviutl.init(this_fp->hinst_parent, si)) return false;

		for (int i = 0; i < si.filter_n; i++) {
			auto that_fp = this_fp->exfunc->get_filterp(i);
			if (that_fp->information != nullptr &&
				0 == std::strcmp(that_fp->information, info_exedit092)) {
				fp = that_fp;
				init_pointers();
				return true;
			}
		}
		return false;
	}

	int32_t*	is_playing;				// 0x1a52ec; 0: editing, 1: playing.

	time_fmt*	tl_time_format;			// 0x178e20
	time_fmt*	dlg_time_format;		// 0x14dec8

	void(*		update_dialog_top)();	// 0x02c580

	int32_t*	layer_size_mode;		// 0x1539d4; 0: large, 1: medium, 2: small.
	int32_t(*	layer_size_preset)[3];	// 0x0a3e08
	int32_t(*	midpt_mk_sz_preset)[3];	// 0x0a3e14
	int32_t*	layer_size;				// 0x0a3e20
	int32_t*	midpt_marker_size;		// 0x0a3e24

	int32_t*	playback_syncs_cursor;	// 0x158d14
	int32_t*	pos_green_line;			// 0x0a3fc0

	int32_t*	scroll_follows_cursor;	// 0x1790d0
	int32_t*	groups_drag_drop;		// 0x179224
	int32_t*	relocates_file_path;	// 0x177a20
	int32_t*	split_calcs_midvalue;	// 0x1460b0
	int32_t*	splits_at_current;		// 0x1538b0
	int32_t*	drags_adjacent_obj;		// 0x14ea00
	int32_t*	split_seps_groups;		// 0x153de0
	int32_t*	clamps_into_rgb_box;	// 0x1538b8

	int32_t*	auto_backup_enabled;	// 0x135aa8

private:
	void init_pointers()
	{
		auto pick_addr = [exedit_base = reinterpret_cast<uintptr_t>(fp->dll_hinst)]
			<class T>(T& target, ptrdiff_t offset) { target = reinterpret_cast<T>(exedit_base + offset); };

		pick_addr(is_playing,				0x1a52ec);

		pick_addr(tl_time_format,			0x178e20);
		pick_addr(dlg_time_format,			0x14dec8);

		pick_addr(update_dialog_top,		0x02c580);

		pick_addr(layer_size_mode,			0x1539d4);
		pick_addr(layer_size_preset,		0x0a3e08);
		pick_addr(midpt_mk_sz_preset,		0x0a3e14);
		pick_addr(layer_size,				0x0a3e20);
		pick_addr(midpt_marker_size,		0x0a3e24);

		pick_addr(playback_syncs_cursor,	0x158d14);
		pick_addr(pos_green_line,			0x0a3fc0);

		pick_addr(scroll_follows_cursor,	0x1790d0);
		pick_addr(groups_drag_drop,			0x179224);
		pick_addr(relocates_file_path,		0x177a20);
		pick_addr(split_calcs_midvalue,		0x1460b0);
		pick_addr(splits_at_current,		0x1538b0);
		pick_addr(drags_adjacent_obj,		0x14ea00);
		pick_addr(split_seps_groups,		0x153de0);
		pick_addr(clamps_into_rgb_box,		0x1538b8);

		pick_addr(auto_backup_enabled,		0x135aa8);
	}
} exedit;


////////////////////////////////
// 文字エンコード変換．
////////////////////////////////
struct Encodes {
	static std::string to_narrow(wchar_t const* wstr)
	{
		auto len = ::WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
		if (len <= 0) return "";
		std::string ret(len - 1, '\0');
		::WideCharToMultiByte(CP_ACP, 0, wstr, -1, ret.data(), len, nullptr, nullptr);
		return ret;
	}
};


////////////////////////////////
// aviutl.ini への書き込み補助．
////////////////////////////////
struct aviutl_ini {
	static char const* ini_path()
	{
		static constinit char path[MAX_PATH] = "";
		if (path[0] == '\0') {
			auto const len = ::GetModuleFileNameA(nullptr, path, std::size(path));
			auto pos = std::string_view{ path, len }.find_last_of("/\\");
			pos = pos == std::string_view::npos ? 0 : pos + 1;
			::strcpy_s(path + pos, std::size(path) - pos, "aviutl.ini");
		}
		return path;
	}

	static void write_int(char const* key, int32_t val)
	{
		write_string(key, std::to_string(val).c_str());
	}
	static void write_bool(char const* key, bool val)
	{
		write_string(key, val ? "1" : "0");
	}
	static void write_string(char const* key, char const* val)
	{
		::WritePrivateProfileStringA("system", key, val, ini_path());
	}
};


////////////////////////////////
// 各種設定項目．
////////////////////////////////
struct setting_item {
	// synchronizes GUI from effective settings.
	virtual void on_update() = 0;
	// synchronizes GUI to effective settings.
	virtual bool on_notify(HWND ctrl, uint32_t code) = 0;

	// used as a menu command.
	virtual bool menu_command() { return false; };
	virtual wchar_t const* menu_title() const { return nullptr; }
	
	virtual std::pair<int, int> measure(HWND parent) = 0;
	virtual void create(HWND parent, int x, int y, int w, int h) = 0;
	virtual void destroy() = 0;
	virtual ~setting_item() {}

	// to be called when GUI recieves updates.
	static bool notify(HWND ctrl, uint32_t code)
	{
		auto that = lookup(ctrl);
		return that != nullptr && that->on_notify(ctrl, code);
	}
	void associate(HWND hwnd) const
	{
		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(this));
	}
	template<std::derived_from<setting_item> ItemT = setting_item>
	static ItemT* lookup(HWND hwnd)
	{
		return dynamic_cast<ItemT*>(reinterpret_cast<setting_item*>(
			::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
	}

	constexpr setting_item() = default;
	setting_item(setting_item const&) = delete;
	setting_item(setting_item&&) = delete;

protected:
	// helper functions to measure window size.
	struct text_measure {
		HWND parent;
		HDC dc;
		HGDIOBJ old_font;
		text_measure(HWND parent, HFONT font) : parent{ parent } {
			dc = ::GetDC(parent);
			old_font = ::SelectObject(dc, font);
		}

		~text_measure() {
			::SelectObject(dc, old_font);
			::ReleaseDC(parent, dc);
		}

		std::pair<int, int> operator()(wchar_t const* text) const
		{
			RECT rc{};
			::DrawTextW(dc, text, -1, &rc, DT_CALCRECT);
			return { rc.right, rc.bottom };
		}

		operator bool() const { return dc != nullptr; }
	};

	// helper functions to create controls.
	static HWND create_label(HWND parent, wchar_t const* text, int x, int y, int w, int h,
		uint32_t extra_style = SS_SIMPLE, uint32_t extra_ex_style = WS_EX_NOPARENTNOTIFY)
	{
		auto ret = ::CreateWindowExW(extra_ex_style, WC_STATICW, text,
			WS_VISIBLE | WS_CHILD | extra_style,
			x, y, w, h, parent, nullptr, aviutl.instance, nullptr);
		::SendMessageW(ret, WM_SETFONT, reinterpret_cast<WPARAM>(aviutl.font), {});
		return ret;
	}

	HWND create_edit(HWND parent, wchar_t const* text, int x, int y, int w, int h,
		uint32_t extra_style = ES_AUTOHSCROLL | ES_LEFT,
		uint32_t extra_ex_style = WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY) const
	{
		// edit control.
		auto ret = ::CreateWindowExW(extra_ex_style, WC_EDITW, text,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | extra_style,
			x, y, w, h, parent, nullptr, aviutl.instance, nullptr);
		::SendMessageW(ret, WM_SETFONT, reinterpret_cast<WPARAM>(aviutl.font), {});

		associate(ret);
		enable_focus_nav(ret, parent);
		return ret;
	}
	template<size_t max_digits = 15>
	static bool try_parse_edit(HWND edit, auto& num)
	{
		// parses input text as a number.
		wchar_t wbuf[max_digits + 1];
		if (::SendMessageW(edit, WM_GETTEXT, std::size(wbuf), reinterpret_cast<LPARAM>(wbuf)) == 0)
			return false;

		auto buf = Encodes::to_narrow(wbuf);
		auto result = std::from_chars(&*buf.cbegin(), &*buf.cend(), num);
		return result.ec == std::errc{} && result.ptr == &*buf.cend();
	}

	HWND create_check(HWND parent, wchar_t const* text, int x, int y, int w, int h,
		uint32_t extra_style = 0, uint32_t extra_ex_style = WS_EX_NOPARENTNOTIFY) const
	{
		// check box control.
		auto ret = ::CreateWindowExW(extra_ex_style, WC_BUTTONW, text,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | extra_style,
			x, y, w, h, parent, nullptr, aviutl.instance, nullptr);
		::SendMessageW(ret, BM_SETCHECK, BST_UNCHECKED, 0);
		::SendMessageW(ret, WM_SETFONT, reinterpret_cast<WPARAM>(aviutl.font), {});

		associate(ret);
		enable_focus_nav(ret, parent);
		return ret;
	}

	HWND create_radio(HWND parent, wchar_t const* text, int x, int y, int w, int h, bool is_first,
		uint32_t extra_style = 0, uint32_t extra_ex_style = WS_EX_NOPARENTNOTIFY) const
	{
		// radio button control.
		auto ret = ::CreateWindowExW(extra_ex_style, WC_BUTTONW, text,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | extra_style
			| (is_first ? WS_GROUP : 0), x, y, w, h, parent,
			nullptr, aviutl.instance, nullptr);
		::SendMessageW(ret, BM_SETCHECK, is_first ? BST_CHECKED : BST_UNCHECKED, 0);
		::SendMessageW(ret, WM_SETFONT, reinterpret_cast<WPARAM>(aviutl.font), {});

		associate(ret);
		enable_focus_nav(ret, parent);
		return ret;
	}

	// helper function to destroy multiple windows.
	static void destroy_windows(auto&... windows)
	{
		constexpr auto destroy = [](HWND& wnd) {
			if (wnd != nullptr) {
				::DestroyWindow(wnd);
				wnd = nullptr;
			}
		};
		(destroy(windows), ...);
	}

private:
	// implement tab navigation.
	static void enable_focus_nav(HWND hwnd, HWND parent) {
		::SetWindowSubclass(hwnd, focus_nav_proc, reinterpret_cast<uintptr_t>(parent), {});
	}
	static LRESULT CALLBACK focus_nav_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, uintptr_t id, DWORD_PTR)
	{
		switch (message) {
		case WM_KEYDOWN:
		{
			HWND next = hwnd;
			switch (wparam) {
			case VK_TAB:
				next = ::GetNextDlgTabItem(reinterpret_cast<HWND>(id), hwnd,
					::GetKeyState(VK_SHIFT) < 0 ? TRUE : FALSE);
				break;
			case VK_LEFT:
			case VK_UP:
			case VK_RIGHT:
			case VK_DOWN:
			{
				if (is_radio(hwnd)) {
					next = ::GetNextDlgGroupItem(reinterpret_cast<HWND>(id), hwnd,
						wparam == VK_LEFT || wparam == VK_UP ? TRUE : FALSE);
					if (!is_radio(next)) next = hwnd;
				}
				break;
			}
			}

			if (next != hwnd) {
				::SetFocus(next);
				return 0;
			}
			break;
		}
		case WM_CHAR:
			if (wparam == L'\t') return 0; // suppress notification sound.
			break;
		case WM_NCDESTROY:
			::RemoveWindowSubclass(hwnd, focus_nav_proc, id);
			break;
		}
		return ::DefSubclassProc(hwnd, message, wparam, lparam);
	}
	static inline bool is_radio(HWND hwnd);
};

// 任意フレーム数移動．
struct setting_item_frame_jump : setting_item {
	constexpr static auto title = L"任意フレーム数移動";
	constexpr static wchar_t const* title_a[] = { L"A", L"B", L"C", L"D" };
	constexpr static char const* ini_keys[] = { "moveA", "moveB", "moveC", "moveD" };
	constexpr static size_t cnt_values = std::size(title_a);

	// returns lengths of  { title, maximum of A/B/C/D, numbers, height }.
	static std::tuple<int, int, int, int> label_size(HWND parent) {
		static constinit int label = -1, label_a = -1, num, height = -1;
		if (label < 0) {
			text_measure measure{ parent, aviutl.font };
			std::tie(label, height) = measure(title);
			num = measure(L"123456").first; // accept at most 6 digits.
			for (auto a : title_a)
				label_a = std::max(label_a, measure(a).first);
		}
		return { label, label_a, num, height };
	}
	static int edit_pad_size() {
		static constinit int padding = -1;
		if (padding < 0) {
			padding = 2 * ::GetSystemMetrics(SM_CXEDGE);
		}
		return padding;
	}

	// GUI elemnts.
	HWND label = nullptr;
	struct gui_unit {
		int32_t gui_val;
		HWND label = nullptr, edit = nullptr;
	} units[cnt_values]{ { 5 }, { 30 }, { 899 }, { 8991 } };

	// metrics for small blanks.
	constexpr static int left_pad = 16, gap_unit = 8, gap_label = 4, gap_v = 0, pad_edit = 4;

	void on_update() override
	{
		if (label == nullptr) return;

		auto focused = ::GetFocus();
		for (size_t i = 0; i < cnt_values; i++) {
			auto& val = units[i];
			if (focused == val.edit) continue;

			auto src = (*aviutl.frame_jump_distance)[i];
			if (val.gui_val == src) continue;
			pull(val, src);
		}
	}
	bool on_notify(HWND ctrl, uint32_t code) override
	{
		if (label != nullptr) {
			switch (code) {
			case EN_SETFOCUS:
				::SendMessageW(ctrl, EM_SETSEL, 0, -1);
				break;
			case EN_KILLFOCUS:
			{
				size_t idx = 0;
				for (; idx < cnt_values; idx++) {
					if (units[idx].edit == ctrl) break;
				}
				if (int value; idx < cnt_values &&
					try_parse_edit(ctrl, value) && value > 0) {
					// valid number was input. write it as an effective setting.
					units[idx].gui_val = value;
					push(idx, value);
				}
				else // invalid number input. rewind the edit state.
					pull(units[idx], units[idx].gui_val);

				break;
			}
			}
		}
		return false;
	}

	std::pair<int, int> measure(HWND parent) override
	{
		auto const pad = edit_pad_size(), pad_h = pad + pad_edit;
		auto const [title_w, lbl_w, num_w, height] = label_size(parent);
		return {
			std::max<int>(title_w, left_pad
				+ cnt_values * (lbl_w + gap_label + pad_h + num_w)
				+ (cnt_values - 1) * gap_unit),
			2 * height + gap_v + pad
		};
	}
	void create(HWND parent, int x, int y, int w, int h) override
	{
		auto const pad = edit_pad_size(), pad_h = pad + pad_edit;
		auto const [title_w, lbl_w, num_w, height] = label_size(parent);

		// title.
		label = create_label(parent, title, x, y, w, height);

		// each number boxes.
		int const unit_w = lbl_w + gap_label + pad_h + num_w + gap_unit,
			Y = y + height + gap_v;
		int X = x + w - cnt_values * unit_w + gap_unit;
		size_t idx = 0;
		for (auto& val : units) {
			// labels A/B/C/D.
			val.label = create_label(parent, title_a[idx],
				X, Y + (pad >> 1), lbl_w + gap_label, height);

			// edit control to input numbers.
			val.edit = create_edit(parent, std::to_wstring(val.gui_val).c_str(),
				X + lbl_w + gap_label, Y, pad_h + num_w, pad + height,
				ES_AUTOHSCROLL | ES_NUMBER | ES_RIGHT);

			X += unit_w; idx++;
		}
	}
	void destroy() override
	{
		[this] <size_t... I>(std::index_sequence<I...>) {
			destroy_windows(label, units[I].label..., units[I].edit...);
		}(std::make_index_sequence<cnt_values>{});
	}
	~setting_item_frame_jump() override { destroy(); }

private:
	// syncs between GUIs and effective settings.
	void pull(gui_unit& unit, int32_t value)
	{
		unit.gui_val = value;
		::SendMessageW(unit.edit, WM_SETTEXT, {},
			reinterpret_cast<LPARAM>(std::to_wstring(value).c_str()));
	}
	void push(size_t idx, int32_t gui_val) {
		auto* target = &(*aviutl.frame_jump_distance)[idx];
		if (*target == gui_val) return;

		*target = gui_val;
		aviutl_ini::write_int(ini_keys[idx], gui_val);
	}
};

// ボタン利用の設定項目 (push_like のフィールド定義が目的)．
struct setting_item_button_base : setting_item {
	static auto min_check_size()
	{
		static constinit int min_width = -1, min_height = -1;
		if (min_width < 0) {
			min_width = ::GetSystemMetrics(SM_CXMENUCHECK) + 2 * ::GetSystemMetrics(SM_CXEDGE);
			min_height = ::GetSystemMetrics(SM_CYMENUCHECK);
		}
		return std::pair{ min_width, min_height };
	}
	static int button_pad_size()
	{
		static constinit int padding = -1;
		if (padding < 0) {
			padding = 2 * ::GetSystemMetrics(SM_CXEDGE);
		}
		return padding;
	}

	bool push_like = false;
};

// チェックボックス形式．
struct setting_item_check_base : setting_item_button_base {
	// target field for the effective setting.
	int32_t*& target;

	// GUI elements.
	wchar_t const* title;
	HWND check = nullptr; bool gui_val = false;
	constexpr setting_item_check_base(int32_t*& target, wchar_t const* title)
		: target{ target }, title{ title } {}

	void on_update() override
	{
		if (check != nullptr && (*target != 0) != gui_val) {
			gui_val = *target != 0;
			::SendMessageW(check, BM_SETCHECK, gui_val ? BST_CHECKED : BST_UNCHECKED, 0);
		}
	}
	bool on_notify(HWND ctrl, uint32_t code) override
	{
		if (check != nullptr && code == BN_CLICKED) {
			gui_val = ::SendMessageW(check, BM_GETCHECK, 0, 0) != BST_UNCHECKED;
			return push(gui_val);
		}
		return false;
	}

	bool menu_command() override
	{
		auto ret = push(*target == 0);
		on_update();
		return ret;
	}
	wchar_t const* menu_title() const override { return title; }

	std::pair<int, int> measure(HWND parent) override
	{
		if (push_like) {
			auto const pad = button_pad_size();
			auto const [W, H] = text_measure{ parent, aviutl.font }(title);
			return { W + pad, H + pad };
		}
		else {
			auto const [w, h] = min_check_size();
			auto const [W, H] = text_measure{ parent, aviutl.font }(title);
			return { w + W, std::max(h, H) };
		}
	}
	void create(HWND parent, int x, int y, int w, int h) override
	{
		check = create_check(parent, title, x, y, w, h, push_like ? BS_PUSHLIKE : 0);
		gui_val = false;
	}
	void destroy() override { destroy_windows(check); }
	~setting_item_check_base() override { destroy(); }

protected:
	virtual bool push(bool value) = 0;
};

template<bool redraw>
struct setting_item_check : setting_item_check_base {
	constexpr setting_item_check(int32_t*& target, wchar_t const* title)
		: setting_item_check_base{ target, title } {}

protected:
	// syncs between GUIs and effective settings.
	bool push(bool val) override
	{
		int32_t const value = val ? 1 : 0;
		if (*target == value) return false;

		*target = value;
		on_push_extra(val);
		return redraw;
	}
	virtual void on_push_extra(bool val) {}
};

// 再生を停止して緑の線を隠す特殊化．
struct setting_item_sync_cursor : setting_item_check_base {
	constexpr setting_item_sync_cursor(wchar_t const* title)
		: setting_item_check_base{ exedit.playback_syncs_cursor, title } {}

protected:
	// syncs between GUIs and effective settings.
	bool push(bool val) override
	{
		int32_t const value = val ? 1 : 0;
		if (*target == value) return false;

		*target = value;

		if (*exedit.is_playing != 0) {
			if (val && *exedit.pos_green_line >= 0) {
				// hide the green line.
				*exedit.pos_green_line = -1;

				// redraw the timeline as well.
				::InvalidateRect(exedit.fp->hwnd, nullptr, FALSE);
			}

			// stops playing by updating the screen.
			return true;
		}
		return false;
	}
};

// aviutl.ini に変更を保存する特殊化．
template<bool redraw>
struct setting_item_check_ini : setting_item_check<redraw> {
	char const* const ini_key = nullptr;
	constexpr setting_item_check_ini(int32_t*& target, wchar_t const* title, char const* key)
		: setting_item_check<redraw>{ target, title }, ini_key{ key } {}

protected:
	// write to .ini file when effective setting updates.
	void on_push_extra(bool val) override {
		aviutl_ini::write_bool(ini_key, val);
	}
};

// ラジオボタン形式の基底クラス．
struct setting_item_radio_base : setting_item_button_base {};
inline bool setting_item::is_radio(HWND hwnd) {
	// checks if it's a radio button control.
	return lookup<setting_item_radio_base>(hwnd) != nullptr;
}

// 時間表示形式の選択．
struct setting_item_time_fmt : setting_item_radio_base {
	constexpr static auto
		label_hms		= L"時分秒",
		label_frame		= L"フレーム";
	// returns a tuple of widths of { hms, frame }.
	static std::tuple<int, int> label_widths(HWND parent) {
		static constinit int hms = -1, frame = -1;
		if (hms < 0) {
			text_measure measure{ parent, aviutl.font };
			hms = measure(label_hms).first;
			frame = measure(label_frame).first;
		}
		return { hms, frame };
	}

	// left/top padding of timeline window.
	constexpr static int
		timeline_layer_header_width = 64,
		timeline_ruler_height = 42;

	// target field for the effective setting.
	time_fmt*& target;

	// GUI elements.
	wchar_t const* title;
	HWND label = nullptr, radio_hms = nullptr, radio_frame = nullptr;
	time_fmt gui_val = time_fmt::hms;

	// metrics for small blanks.
	constexpr static int gap_title = 8, gap_radio = 4, pad_button = 12;

	constexpr setting_item_time_fmt(time_fmt*& target, wchar_t const* title)
		: target{ target }, title{ title } {}

	void on_update() override
	{
		if (label == nullptr) return;

		if (*target != gui_val) {
			// update the check states of radio buttons.
			auto prev_val = std::exchange(gui_val, *target);
			::SendMessageW(choose_radio(prev_val), BM_SETCHECK, BST_UNCHECKED, 0);
			::SendMessageW(choose_radio(gui_val), BM_SETCHECK, BST_CHECKED, 0);
		}
	}
	bool on_notify(HWND ctrl, uint32_t code) override
	{
		if (label != nullptr && code == BN_CLICKED) {
			auto prev_val = std::exchange(gui_val, ctrl == radio_hms ? time_fmt::hms : time_fmt::frame);
			if (prev_val != gui_val) {
				::SendMessageW(choose_radio(prev_val), BM_SETCHECK, BST_UNCHECKED, 0);
				::SendMessageW(ctrl, BM_SETCHECK, BST_CHECKED, 0);
			}
			push(gui_val);
		}
		return false;
	}
	bool menu_command() override
	{
		push(toggle(*target));
		on_update();
		return false;
	}
	wchar_t const* menu_title() const override { return title; }

	std::pair<int, int> measure(HWND parent) override
	{
		auto const [title_w, title_h] = text_measure{ parent, aviutl.font }(title);
		auto const [hms_w, frame_w] = label_widths(parent);
		if (push_like) {
			auto const pad = button_pad_size(), pad_h = pad + pad_button;
			return { title_w + gap_title + 2 * pad_h + hms_w + frame_w, title_h + pad };
		}
		else {
			auto const [radio_w, radio_h] = min_check_size();
			return { title_w + gap_title + 2 * radio_w + hms_w + frame_w + gap_radio, std::max(radio_h, title_h) };
		}
	}
	void create(HWND parent, int x, int y, int w, int h) override
	{
		auto const [hms_w, frame_w] = label_widths(parent);

		if (push_like) {
			auto const pad = button_pad_size(), pad_h = pad + pad_button;

			// title.
			label = create_label(parent, title, x, y + (pad >> 1),
				std::max(0, w - (2 * pad_h + hms_w + frame_w)), h - pad);


			// radio for H:M:S format.
			radio_hms = create_radio(parent, label_hms,
				x + w - (2 * pad_h + hms_w + frame_w), y,
				pad_h + hms_w, h, true, BS_PUSHLIKE);

			// radio for frame count.
			radio_frame = create_radio(parent, label_frame,
				x + w - (pad_h + frame_w), y, pad_h + frame_w, h, false, BS_PUSHLIKE);
		}
		else {
			auto const radio_w = min_check_size().first;

			// title.
			label = create_label(parent, title, x, y,
				std::max(0, w - (2 * radio_w + hms_w + frame_w + gap_radio)), h);

			// radio for H:M:S format.
			radio_hms = create_radio(parent, label_hms,
				x + w - (2 * radio_w + hms_w + frame_w + gap_radio), y,
				radio_w + hms_w, h, true);

			// radio for frame count.
			radio_frame = create_radio(parent, label_frame,
				x + w - (radio_w + frame_w), y, radio_w + frame_w, h, false);
		}

		gui_val = time_fmt::hms;
	}
	void destroy() override { destroy_windows(label, radio_hms, radio_frame); }
	~setting_item_time_fmt() override { destroy(); }

protected:
	HWND choose_radio(time_fmt val) const {
		return val == time_fmt::hms ? radio_hms : radio_frame;
	}
	static constexpr time_fmt toggle(time_fmt val) {
		switch (val) {
		case time_fmt::hms:
		default:				return time_fmt::frame;
		case time_fmt::frame:	return time_fmt::hms;
		}
	}
	virtual void push(time_fmt val) = 0;
};
struct setting_item_tl_time_fmt : setting_item_time_fmt {
	constexpr setting_item_tl_time_fmt(wchar_t const* title)
		: setting_item_time_fmt(exedit.tl_time_format, title) {}

protected:
	// syncs between GUIs and effective settings.
	void push(time_fmt val) override
	{
		if (*target == val) return;

		// update the setting from radio button state.
		*target = val;

		// let redraw the ruler of the timeline.
		RECT rc;
		::GetClientRect(exedit.fp->hwnd, &rc);
		rc.left = timeline_layer_header_width;
		rc.bottom = timeline_ruler_height;
		::InvalidateRect(exedit.fp->hwnd, &rc, FALSE);
	}
};
struct setting_item_dlg_time_fmt : setting_item_time_fmt {
	constexpr setting_item_dlg_time_fmt(wchar_t const* title)
		: setting_item_time_fmt(exedit.dlg_time_format, title) {}

protected:
	// syncs between GUIs and effective settings.
	void push(time_fmt val) override
	{
		if (*target == val) return;

		// update the setting from radio button state.
		*target = val;

		// update the top part of the dialog.
		exedit.update_dialog_top();
	}
};

// レイヤー幅の選択．
struct setting_item_layer_height : setting_item_radio_base {
	constexpr static auto title = L"レイヤーの幅";
	constexpr static wchar_t const* label_btn[] = { L"大", L"中", L"小" };
	// returns the lengths of { title, maximum of label_btn[*], height }.
	static std::tuple<int, int, int> label_size(HWND parent) {
		static constinit int label = -1, button = -1, height;
		if (label < 0) {
			text_measure measure{ parent, aviutl.font };
			std::tie(label, height) = measure(title);
			for (auto str : label_btn)
				button = std::max(button, measure(str).first);
		}
		return { label, button, height };
	}

	constexpr static int num_layers = 100;

	// GUI elements.
	HWND label = nullptr, radio_L = nullptr, radio_M = nullptr, radio_S = nullptr;
	int32_t gui_val = 0;

	// metrics for small blanks.
	constexpr static int gap_title = 8, gap_radio = 12, pad_button = 8;

	void on_update() override
	{
		if (label == nullptr) return;

		if (*exedit.layer_size_mode != gui_val) {
			// update the check states of radio buttons.
			auto prev_val = std::exchange(gui_val, *exedit.layer_size_mode);
			::SendMessageW(choose_radio(prev_val), BM_SETCHECK, BST_UNCHECKED, 0);
			::SendMessageW(choose_radio(gui_val), BM_SETCHECK, BST_CHECKED, 0);
		}
	}
	bool on_notify(HWND ctrl, uint32_t code) override
	{
		if (label != nullptr && code == BN_CLICKED) {
			auto prev_val = std::exchange(gui_val, ctrl != radio_S ? ctrl != radio_M ? 0 : 1 : 2);
			if (prev_val != gui_val) {
				::SendMessageW(choose_radio(prev_val), BM_SETCHECK, BST_UNCHECKED, 0);
				::SendMessageW(ctrl, BM_SETCHECK, BST_CHECKED, 0);
			}
			push(gui_val);
		}
		return false;
	}
	bool menu_command() override
	{
		push(toggle(*exedit.layer_size_mode));
		on_update();
		return false;
	}
	wchar_t const* menu_title() const override { return title; }

	std::pair<int, int> measure(HWND parent)
	{
		auto const [title_w, btn_w, height] = label_size(parent);
		if (push_like) {
			auto const pad = button_pad_size(), pad_h = pad + pad_button;
			return { title_w + gap_title + 3 * (pad_h + btn_w), pad + height };
		}
		else {
			auto const [radio_w, radio_h] = min_check_size();
			return { title_w + gap_title + 3 * (radio_w + btn_w) + 2 * gap_radio, std::max(radio_h, height) };
		}
	}
	void create(HWND parent, int x, int y, int w, int h)
	{
		auto const btn_w = std::get<1>(label_size(parent));
		if (push_like) {
			auto const pad = button_pad_size(), pad_h = pad + pad_button;

			// title.
			label = create_label(parent, title,
				x, y + (pad >> 1), std::max(0, w - 3 * (pad_h + btn_w)), h - pad);

			// radio buttons.
			radio_L = create_radio(parent, label_btn[0],
				x + w - 3 * (pad_h + btn_w), y,
				pad_h + btn_w, h, true, BS_PUSHLIKE);

			radio_M = create_radio(parent, label_btn[1],
				x + w - 2 * (pad_h + btn_w), y,
				pad_h + btn_w, h, false, BS_PUSHLIKE);

			radio_S = create_radio(parent, label_btn[2],
				x + w - (pad_h + btn_w), y, pad_h + btn_w, h, false, BS_PUSHLIKE);
		}
		else {
			auto const radio_w = min_check_size().first;

			// title.
			label = create_label(parent, title,
				x, y, std::max(0, w - (3 * (radio_w + btn_w) + 2 * gap_radio)), h);

			// radio buttons.
			radio_L = create_radio(parent, label_btn[0],
				x + w - (3 * (radio_w + btn_w) + 2 * gap_radio), y,
				radio_w + btn_w, h, true);

			radio_M = create_radio(parent, label_btn[1],
				x + w - (2 * (radio_w + btn_w) + gap_radio), y,
				radio_w + btn_w, h, false);

			radio_S = create_radio(parent, label_btn[2],
				x + w - (radio_w + btn_w), y, radio_w + btn_w, h, false);
		}

		gui_val = 0;
	}
	void destroy() override { destroy_windows(label, radio_L, radio_M, radio_S); }
	~setting_item_layer_height() override { destroy(); }

private:
	static constexpr int32_t toggle(int32_t val) { return (val + 1) % 3; }
	HWND choose_radio(int32_t val) const {
		return val != 2 ? val != 1 ? radio_L : radio_M : radio_S;
	}

	// syncs between GUIs and effective settings.
	void push(int32_t val)
	{
		if (*exedit.layer_size_mode == val) return;

		// apply the chosen preset of values.
		*exedit.layer_size_mode = val;
		*exedit.layer_size = (*exedit.layer_size_preset)[val];
		*exedit.midpt_marker_size = (*exedit.midpt_mk_sz_preset)[val];

		// promote redraw.
		::InvalidateRect(exedit.fp->hwnd, nullptr, FALSE);

		// resize the window to fit with integral number of layers.
		RECT rc;
		::GetWindowRect(exedit.fp->hwnd, &rc);
		::SendMessageW(exedit.fp->hwnd, WM_SIZING, WMSZ_BOTTOM, reinterpret_cast<LPARAM>(&rc));
		::SetWindowPos(exedit.fp->hwnd, nullptr, 0, 0,
			rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	}
};

// セパレータ系．
struct setting_item_gap : setting_item {
	int pad_v = 0;
	constexpr static int min_padding = 0, max_padding = 32;

	void on_update() override {}
	bool on_notify(HWND, uint32_t) override { return false; }
	std::pair<int, int> measure(HWND parent) override { return { 0, pad_v }; }
	void create(HWND parent, int x, int y, int w, int h) override {}
	void destroy() override {}
};

struct setting_item_separator : setting_item_gap {
	HWND hline = nullptr; int pad_h = 0;
	constexpr static int min_width = 2, sep_height = 2;

	std::pair<int, int> measure(HWND parent) override {
		return { min_width + 2 * pad_h, sep_height + 2 * pad_v };
	}
	void create(HWND parent, int x, int y, int w, int h) override
	{
		hline = create_label(parent, L"", x + pad_h, y + pad_v,
			std::max(min_width, w - 2 * pad_h), sep_height, SS_ETCHEDHORZ);
	}
	void destroy() override { destroy_windows(hline); }
	~setting_item_separator() override { destroy(); }
};


////////////////////////////////
// オプション実体の定義．
////////////////////////////////
static setting_item_check<false> // 変数上書きだけで設定変更が成立するもの．
	scroll_follows_cursor	{ exedit.scroll_follows_cursor,	L"カーソル移動時に自動でスクロール" },
	groups_drag_drop		{ exedit.groups_drag_drop,		L"D && D読み込み時に複数オブジェクトをグループ化" },
	relocates_file_path		{ exedit.relocates_file_path,	L"ファイルの場所が変更された時にプロジェクトパスから読む" },
	split_calcs_midvalue	{ exedit.split_calcs_midvalue,	L"中間点追加・分割時に中間の値を反映する" },
	splits_at_current		{ exedit.splits_at_current,		L"中間点追加・分割を常に現在フレームで行う" },
	drags_adjacent_obj		{ exedit.drags_adjacent_obj,	L"オブジェクトの端をつまむ時に隣接するオブジェクトも選択" },
	split_seps_groups		{ exedit.split_seps_groups,		L"分割時にグループを別々にする" };

static setting_item_check<true> // 画面更新で十分なもの．
	clamps_into_rgb_box		{ exedit.clamps_into_rgb_box,	L"RGB色空間で飽和" },
	auto_backup_enabled		{ exedit.auto_backup_enabled,	L"自動バックアップを有効"}; // updating to promote the first backup.

static setting_item_sync_cursor // 緑の線を隠す必要があるもの．
	playback_syncs_cursor	{ L"再生ウィンドウで再生した時にカーソルを連動" };

static setting_item_check_ini<false> // 変数上書きと .ini への書き出しが必要なもの．
	resume_enabled			{ aviutl.resume_enabled,		L"編集のレジューム機能を有効",					"editresume" },
	snaps_window			{ aviutl.snaps_window,			L"関連ウィンドウ同士を移動時にスナップする",		"windowsnap" },
	seeks_clicked_point		{ aviutl.seeks_clicked_point,	L"トラックバーでクリックした位置に直接移動する",	"trackbarclick" };

static constinit setting_item_frame_jump // 任意フレーム数移動の設定 (.ini への書き出しも必要).
	frame_jump_distance{};

// 時間表示形式の設定．
static setting_item_tl_time_fmt
	tl_time_format			{ L"編集画面の時間表示単位" };
static setting_item_dlg_time_fmt
	dlg_time_format			{ L"設定ダイアログの時間表示単位" };

static constinit setting_item_layer_height // レイヤー幅の変更．
	layer_size_mode{};

#define item(id)	{ &id, #id }
static constinit struct {
	setting_item* ptr;
	char const* name;
	setting_item* operator->() const { return ptr; }
} const all_settings[] = {
	item(frame_jump_distance),
	item(resume_enabled),
	item(snaps_window),
	item(seeks_clicked_point),

	item(tl_time_format),
	item(dlg_time_format),
	item(layer_size_mode),

	item(playback_syncs_cursor),
	item(scroll_follows_cursor),
	item(groups_drag_drop),
	item(relocates_file_path),
	item(split_calcs_midvalue),
	item(splits_at_current),
	item(drags_adjacent_obj),
	item(split_seps_groups),
	item(clamps_into_rgb_box),

	item(auto_backup_enabled),
};
#undef item


////////////////////////////////
// プラグインの設定．
////////////////////////////////
static struct Settings {
	struct {
		int max_width, margin_h, margin_v, gap_v;
		constexpr static int min_val = 0, max_val = 32, min_max_width = 128;
	} metrics{ 0, 8, 6, 2 };

	// using forward_list is because their memory locations must not be moved.
	std::forward_list<setting_item_gap> gap_pool{ 2 };
	std::forward_list<setting_item_separator> sep_pool{ 2 };
	std::vector<setting_item*> gui_items{
		&frame_jump_distance,
		&resume_enabled,
		&snaps_window,
		&seeks_clicked_point,
		&init_size(*std::next(sep_pool.begin(), 1), (0, 4)),
		&tl_time_format,
		&dlg_time_format,
		&init_size(sep_pool.front(), 0, 4),
		&layer_size_mode,
		&init_size(*std::next(gap_pool.begin(), 1), 4),
		&playback_syncs_cursor,
		&scroll_follows_cursor,
		&groups_drag_drop,
		&relocates_file_path,
		&split_calcs_midvalue,
		&splits_at_current,
		&drags_adjacent_obj,
		&split_seps_groups,
		&clamps_into_rgb_box,
		&init_size(gap_pool.front(), 4),
		&auto_backup_enabled,
	};

private:
	constexpr static setting_item_separator& init_size(setting_item_separator& sep, int pad_h, int pad_v) {
		sep.pad_h = pad_h; sep.pad_v = pad_v;
		return sep;
	}
	constexpr static setting_item_gap& init_size(setting_item_gap& gap, int pad_v) {
		gap.pad_v = pad_v;
		return gap;
	}

public:
	enum class load_result : uint8_t {
		successful,
		no_file,
		error_file,
	};
	load_result load(char const* jsonpath)
	{
		size_t buf_len = 0;
		auto buf = file_content(jsonpath, buf_len);
		if (!buf) return load_result::no_file;

		std::unique_ptr<json_value_s, void(&)(void*)> json_root{
			::json_parse_ex(buf.get(), buf_len - 1,
				json_parse_flags_allow_json5, nullptr, nullptr, nullptr),
			::free
		};
		buf.reset();
		auto json_obj = json::as_obj(json_root.get());
		if (json_obj == nullptr) return load_result::error_file;

		if (!load_from_json(json_obj)) return load_result::error_file;

		// successful.
		return load_result::successful;
	}

private:
	// reads file content into memory.
	static std::unique_ptr<char[]> file_content(char const* filepath, size_t& buf_len)
	{
		FILE* file = nullptr;
		if (0 != ::fopen_s(&file, filepath, "r") || file == nullptr) {
			buf_len = 0;
			return nullptr;
		}

		std::fseek(file, 0, SEEK_END);
		auto len = std::ftell(file);
		std::fseek(file, 0, SEEK_SET);

		std::unique_ptr<char[]> ret = nullptr;
		if (len > 0) {
			ret = std::make_unique<char[]>(len + 1);
			auto str_len = std::fread(ret.get(), sizeof(char), len, file);
			ret[str_len] = '\0';
			buf_len = str_len + 1;
		}
		else buf_len = 0;
		::fclose(file);

		return ret;
	}

	bool load_from_json(json_object_s* json)
	{
		bool ret = false;
		ret |= load_metrics(json);
		ret |= load_gui_items(json);

		return ret;
	}
	bool load_metrics(json_object_s* json)
	{
		// reads a field like:
		// "metrics" : { "max_width" : 0, "margin_h" : 8, "margin_v" : 6, "gap_v" : 2 }
		auto obj = json::as_obj(find_key(json, "metrics"));
		if (obj == nullptr) return false;

	#define load_int_entry(head, field)	load_int(head##field, obj, #field)
		load_int_entry(metrics., max_width);
	#undef load_int_entry

		auto clamp = [](int x) {
			return std::clamp(x, decltype(metrics)::min_val, decltype(metrics)::max_val);
		};
	#define load_int_entry(head, field, coerce)	load_int(head##field, obj, #field, coerce)
		load_int_entry(metrics., margin_h, clamp);
		load_int_entry(metrics., margin_v, clamp);
		load_int_entry(metrics., gap_v, clamp);
	#undef load_int_entry

		return true;
	}
	bool load_gui_items(json_object_s* json)
	{
		// reads a field like:
		// "gui_items" : [ ... ]
		auto arr = json::as_arr(find_key(json, "gui_items"));
		if (arr == nullptr) return false;

		// clear the default items.
		gui_items.clear();
		gui_items.reserve(arr->length);
		gap_pool.clear();
		sep_pool.clear();

		// prepare an id-to-item mapping.
		std::map<std::string, setting_item*> entries{};
		for (auto& item : all_settings)
			entries.try_emplace(item.name, item.ptr);

		// iterate over the json array.
		for (auto elem = arr->start; elem != nullptr; elem = elem->next) {
			// reads a line like:
			// { "id" : "snaps_window", "push_like" : false }
			auto obj = json::as_obj(elem->value);
			if (obj == nullptr) continue;

			auto str = json::as_str(find_key(obj, "id"));
			if (str == nullptr) continue;

			std::string id{ str->string, str->string_size };
			if (id.starts_with(' ') || id.starts_with('*')) {
				// create a gap in the pool.
				gap_pool.emplace_front();

				// add that gap.
				auto* gap = &gap_pool.front();
				gui_items.push_back(gap);
				load_gap_options(obj, gap);
			}
			else if (id.starts_with('-') || id.starts_with('=')) {
				// create a separator in the pool.
				sep_pool.emplace_front();

				// add that separator.
				auto* sep = &sep_pool.front();
				gui_items.push_back(sep);
				load_separator_options(obj, sep);
			}
			else if (auto entry = entries.find(id); entry != entries.cend()) {
				// recognized a preset item.
				auto* item = entry->second;

				// remove it to avoid double-instance.
				entries.erase(entry);

				// add that item.
				gui_items.push_back(item);
				load_item_options(obj, id, item);
			}
		}

		gui_items.shrink_to_fit();

		return true;
	}

	void load_item_options(json_object_s* json, std::string& id, setting_item* item)
	{
		// load options according to the type of the item.
		if (auto* btn = dynamic_cast<setting_item_button_base*>(item)) {
			auto val = json::as_bool(find_key(json, "push_like"));
			if (val.has_value()) btn->push_like = val.value();
		}
	}
	void load_separator_options(json_object_s* json, setting_item_separator* sep)
	{
		// load options: "pad_h" and "pad_v".
		constexpr auto clamp = [](int x) {
			return std::clamp(x,
				setting_item_separator::min_padding,
				setting_item_separator::max_padding);
		};
	#define load_int_entry(field)	load_int(sep->field, json, #field, clamp)
		load_int_entry(pad_h);
		load_int_entry(pad_v);
	#undef load_int_entry
	}
	void load_gap_options(json_object_s* json, setting_item_gap* sep)
	{
		// load an option: "pad_v".
		constexpr auto clamp = [](int x) {
			return std::clamp(x,
				setting_item_gap::min_padding,
				setting_item_gap::max_padding);
		};
	#define load_int_entry(field)	load_int(sep->field, json, #field, clamp)
		load_int_entry(pad_v);
	#undef load_int_entry
	}

	static json_value_s* find_key(json_object_s* obj, std::string_view const& key)
	{
		// searches for a certain key from the table.
		for (auto elem = obj->start; elem != nullptr; elem = elem->next) {
			if (elem->name->string == key) return elem->value;
		}
		return nullptr;
	}

	// loading a field if it's an integer.
	static bool load_int(int& storage, json_object_s* obj, std::string_view const& key) {
		return load_int(storage, obj, key, [](int x) {return x; });
	}
	static bool load_int(int& storage, json_object_s* obj, std::string_view const& key, auto&& coerce)
	{
		auto jn = json::as_num(find_key(obj, key));
		if (jn == nullptr) return false;
		int num;
		if (std::from_chars(jn->number, jn->number + jn->number_size, num)
			.ec != std::errc{}) return false;
		storage = coerce(num);
		return true;
	}

	// some JSON helper functions.
	struct json {
		static json_object_s* as_obj(json_value_s* val) {
			if (val == nullptr) return nullptr;
			return ::json_value_as_object(val);
		}
		static json_array_s* as_arr(json_value_s* val) {
			if (val == nullptr) return nullptr;
			return ::json_value_as_array(val);
		}
		static json_number_s* as_num(json_value_s* val) {
			if (val == nullptr) return nullptr;
			return ::json_value_as_number(val);
		}
		static json_string_t* as_str(json_value_s* val) {
			if (val == nullptr) return nullptr;
			return ::json_value_as_string(val);
		}
		static std::optional<bool> as_bool(json_value_s* val) {
			if (val == nullptr) return std::nullopt;
			if (::json_value_is_true(val)) return true;
			if (::json_value_is_false(val)) return false;
			return std::nullopt;
		}
	};
} settings;


////////////////////////////////
// AviUtlに渡す関数の定義．
////////////////////////////////
BOOL func_init(AviUtl::FilterPlugin* fp)
{
	// 情報源確保．
	if (!exedit.init(fp)) {
		::MessageBoxA(fp->hwnd, "拡張編集0.92が見つかりませんでした．",
			fp->name, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	for (size_t id = 0; id < std::size(all_settings); id++) {
		auto name = all_settings[id]->menu_title();
		if (name == nullptr) continue;
		fp->exfunc->add_menu_item(fp, (Encodes::to_narrow(name) + "の切替").c_str(),
			fp->hwnd, id, 0, AviUtl::ExFunc::AddMenuItemFlag::None);
	}

	// 設定ロード．
	constexpr char ext_json[] = "json";
	constexpr int len_auf = std::size("auf") - 1, len_json = std::size(ext_json) - 1;

	char path[MAX_PATH];
	auto len = ::GetModuleFileNameA(fp->dll_hinst, path, std::size(path));
	if (len_auf <= len && len < std::size(path) + len_auf - len_json) {
		::strcpy_s(&path[len - len_auf], std::size(path) - (len - len_auf), ext_json);
		auto result = settings.load(path);
		if (result == Settings::load_result::error_file) {
			::MessageBoxA(fp->hwnd, "設定ファイルを正しく読み込めませんでした．デフォルト設定が利用されます．",
				fp->name, MB_OK | MB_ICONEXCLAMATION);
		}
	}

	// コントロールの配置．まずサイズを測る．
	int max_width = 0, sum_height = 0;
	int const cnt_items = settings.gui_items.size();
	auto height_table = std::make_unique<int[]>(cnt_items);
	for (int i = 0; i < cnt_items; i++) {
		auto* item = settings.gui_items[i];
		auto [w, h] = item->measure(fp->hwnd);
		max_width = std::max(max_width, w);
		sum_height += h;
		height_table[i] = h;
	}

	if (settings.metrics.max_width > 0)
		max_width = std::min(max_width, settings.metrics.max_width);
	max_width = std::max(max_width, settings.metrics.min_max_width);

	// ウィンドウサイズを変更．
	RECT rc{ 0, 0, max_width + 2 * settings.metrics.margin_h,
		sum_height + 2 * settings.metrics.margin_v + (cnt_items - 1) * settings.metrics.gap_v };
	::AdjustWindowRect(&rc, ::GetWindowLongW(fp->hwnd, GWL_STYLE), FALSE);
	::SetWindowPos(fp->hwnd, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

	// サイズが決定したので実際にコントロールを配置．
	sum_height = settings.metrics.margin_v;
	for (int i = 0; i < cnt_items; i++) {
		auto* item = settings.gui_items[i];
		auto h = height_table[i];
		item->create(fp->hwnd, settings.metrics.margin_h, sum_height, max_width, h);
		sum_height += h + settings.metrics.gap_v;
	}

	// IME を無効化．
	::ImmReleaseContext(fp->hwnd, ::ImmAssociateContext(fp->hwnd, nullptr));

	return TRUE;
}

BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	switch (message) {
		using AUM = AviUtl::FilterPlugin::WindowMessage;
	case AUM::Init:
	case AUM::ChangeWindow:
	case AUM::Update:
	{
		// synchronize with effective setting values.
		if (fp->exfunc->is_filter_window_disp(fp) != FALSE) {
			for (auto* item : settings.gui_items) item->on_update();
		}
		return FALSE;
	}

	// handle notification messages from GUI controls.
	case WM_COMMAND:
	{
		auto const ctrl = reinterpret_cast<HWND>(lparam);
		if (ctrl != nullptr)
			return setting_item::notify(ctrl, wparam >> 16) ? TRUE : FALSE;
		break;
	}

	// handle menu commands.
	case AUM::Command:
	{
		auto const menu_id = static_cast<size_t>(wparam);
		if (menu_id < std::size(all_settings))
			return all_settings[menu_id]->menu_command() ? TRUE : FALSE;
		break;
	}

	// let the parent window handle shortcut keys.
	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSCHAR:
	case WM_SYSKEYUP:
	{
		::SendMessageW(fp->hwnd_parent, message, wparam, lparam);
		return FALSE;
	}
	}

	return FALSE;
}


////////////////////////////////
// Entry point.
////////////////////////////////
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		::DisableThreadLibraryCalls(hinst);
		break;
	}
	return TRUE;
}


////////////////////////////////
// 看板．
////////////////////////////////
#define PLUGIN_NAME		"クイック設定"
#define PLUGIN_VERSION	"v1.00"
#define PLUGIN_AUTHOR	"sigma-axis"
#define PLUGIN_INFO_FMT(name, ver, author)	(name##" "##ver##" by "##author)
#define PLUGIN_INFO		PLUGIN_INFO_FMT(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

extern "C" __declspec(dllexport) AviUtl::FilterPluginDLL* __stdcall GetFilterTable(void)
{
	// （フィルタとは名ばかりの）看板．
	using Flag = AviUtl::FilterPlugin::Flag;
	static constinit AviUtl::FilterPluginDLL filter{
		.flag = Flag::AlwaysActive | Flag::ExInformation,
		.name = PLUGIN_NAME,

		.func_init = func_init,
		.func_WndProc = func_WndProc,
		.information = PLUGIN_INFO,
	};
	return &filter;
}

