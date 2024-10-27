//
//  SettingsView.swift
//  luau-M-GUI
//
//  Created by Braeden Weaver on 11/5/22.
//

import SwiftUI
import CodeViewer

struct GeneralSettingsView: View {
    @AppStorage("fontSize") private var fontSize = 16.0
    @AppStorage("theme") private var theme = CodeWebView.Theme.terminal

    var body: some View {
        Form {
            Slider(value: $fontSize, in: 9...96) {
                Text("Font Size (\(fontSize, specifier: "%.0f") pts)")
            }
            Picker("Theme", selection: $theme) {
                Group {
                    Text("Ambiance").tag(CodeWebView.Theme.ambiance)
                    Text("Chrome").tag(CodeWebView.Theme.chrome)
                    Text("Clouds Light").tag(CodeWebView.Theme.clouds)
                    Text("Clouds Dark").tag(CodeWebView.Theme.clouds_midnight)
                    Text("Cobalt").tag(CodeWebView.Theme.cobalt)
                    Text("Crimson").tag(CodeWebView.Theme.crimson_editor)
                    Text("Dawn").tag(CodeWebView.Theme.dawn)
                    Text("Dracula").tag(CodeWebView.Theme.dracula)
                    Text("Dream Weaver").tag(CodeWebView.Theme.dreamweaver)
                    Text("Chaos").tag(CodeWebView.Theme.chaos)
                }
                Group {
                    Text("Eclipse").tag(CodeWebView.Theme.eclipse)
                    Text("GitHub").tag(CodeWebView.Theme.github)
                    Text("Gob").tag(CodeWebView.Theme.gob)
                    Text("Gruvbox").tag(CodeWebView.Theme.gruvbox)
                    Text("Idle Fingers").tag(CodeWebView.Theme.idle_fingers)
                    Text("iPlastic").tag(CodeWebView.Theme.iplastic)
                    Text("Katzenmilch").tag(CodeWebView.Theme.katzenmilch)
                    Text("KR Theme").tag(CodeWebView.Theme.kr_theme)
                    Text("Kuroir").tag(CodeWebView.Theme.kuroir)
                    Text("Merbivore").tag(CodeWebView.Theme.merbivore)
                }
                Group {
                    Text("Merbivore Soft").tag(CodeWebView.Theme.merbivore_soft)
                    Text("Mono Industrial").tag(CodeWebView.Theme.mono_industrial)
                    Text("Monokai").tag(CodeWebView.Theme.monokai)
                    Text("Nord Dark").tag(CodeWebView.Theme.nord_dark)
                    Text("Pastel on Dark").tag(CodeWebView.Theme.pastel_on_dark)
                    Text("Solarized Dark").tag(CodeWebView.Theme.solarized_dark)
                    Text("Solarized Light").tag(CodeWebView.Theme.solarized_light)
                    Text("Sql Server").tag(CodeWebView.Theme.sqlserver)
                    Text("Terminal").tag(CodeWebView.Theme.terminal)
                    Text("TextMate").tag(CodeWebView.Theme.textmate)
                }
                Group {
                    Text("Tomorrow").tag(CodeWebView.Theme.tomorrow)
                    Text("Tomorrow Night").tag(CodeWebView.Theme.tomorrow_night)
                    Text("Tomorrow Night Blue").tag(CodeWebView.Theme.tomorrow_night_blue)
                    Text("Tomorrow Night Bright").tag(CodeWebView.Theme.tomorrow_night_bright)
                    Text("Tomorrow Night Eighties").tag(CodeWebView.Theme.tomorrow_night_eighties)
                    Text("Twilight").tag(CodeWebView.Theme.twilight)
                    Text("Vibrant Ink").tag(CodeWebView.Theme.vibrant_ink)
                    Text("XCode").tag(CodeWebView.Theme.xcode)
                }
            }
        }
        .padding(20)
    }
}

struct AdvancedSettingsView: View {
    @AppStorage("forceOpenGL") private var forceOpenGL = true

    var body: some View {
        Form {
            Toggle("Force OpenGL", isOn: $forceOpenGL)
        }
        .padding(20)
    }
}

struct SettingsView: View {
    private enum Tabs: Hashable {
        case general, advanced
    }
    var body: some View {
        TabView {
            GeneralSettingsView()
                .tabItem {
                    Label("General", systemImage: "gear")
                }
                .tag(Tabs.general)
            AdvancedSettingsView()
                .tabItem {
                    Label("Advanced", systemImage: "gear")
                }
                .tag(Tabs.advanced)
        }
        .padding(10)
    }
}
