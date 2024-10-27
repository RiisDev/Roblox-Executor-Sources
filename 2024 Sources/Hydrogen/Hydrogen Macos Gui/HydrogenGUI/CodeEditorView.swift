//
//  CodeEditorView.swift
//  luau-M-GUI
//
//  Created by Braeden Weaver on 11/5/22.
//

import SwiftUI
import CodeViewer
import UniformTypeIdentifiers

extension Date {
    static func - (lhs: Date, rhs: Date) -> TimeInterval {
        return lhs.timeIntervalSinceReferenceDate - rhs.timeIntervalSinceReferenceDate
    }
}

extension NSImage.Name {
    static let logo = NSImage.Name("Hydrogen")
}

func dateToString(date: Date) -> String {
    let dateFormatter = DateFormatter()
    return dateFormatter.string(from: date)
}

extension TimeInterval{
    func stringFromTimeInterval() -> String {
        let time = NSInteger(self)
        let ms = Int((self.truncatingRemainder(dividingBy: 1)) * 1000)
        let seconds = time % 60
        let minutes = (time / 60) % 60
        let hours = (time / 3600)
        return String(format: "%0.2d hours and %0.2d minutes. ", hours, minutes)

    }
}

struct CodeEditorView: View {
    @State private var hover: Bool = false
    @State private var messagePortName = "com.hydro.app.group.port"
    @State private var do_load: Bool = false
    @AppStorage("fontSize") private var fontSize = 12.0
    @AppStorage("theme") private var theme = CodeWebView.Theme.tomorrow_night_eighties
    @Binding var expiryTime: Date
    
    @SceneStorage("lua")
    private var lua = """
            -- Welcome to Hydrogen!
            """
    
    func onLoadComplete(res: Result<[URL], any Error>) -> Void {
        
    }
    
    func showSavePanel() -> URL? {
        let savePanel = NSSavePanel()
        savePanel.allowedFileTypes = ["lua"]
        savePanel.canCreateDirectories = true
        savePanel.isExtensionHidden = false
        savePanel.allowsOtherFileTypes = false
        savePanel.title = "Save your script"
        savePanel.message = "Choose a place to save your script."
        savePanel.nameFieldLabel = "Script name:"
        let response = savePanel.runModal()
        return response == .OK ? savePanel.url : nil
    }
    
    func showOpenPanel() -> URL? {
        let openPanel = NSOpenPanel()
        openPanel.allowedFileTypes = ["lua"]
        openPanel.allowsMultipleSelection = false
        openPanel.canChooseDirectories = false
        openPanel.canChooseFiles = true
        let response = openPanel.runModal()
        return response == .OK ? openPanel.url : nil
    }
    
    var body: some View {
        VStack(alignment: .trailing) {
            HStack {
                Button(action: self.send, label: { // 1
                    Image(systemName: "play.fill")
                        .foregroundColor(Color(.systemGreen))
                    Text("Execute")
                })
                .padding(EdgeInsets(top: 10, leading: 0, bottom: 0, trailing: 0))
                .onHover { isHovered in
                                    self.hover = isHovered
                                    DispatchQueue.main.async { //<-- Here
                                        if (self.hover) {
                                            NSCursor.pointingHand.push()
                                        } else {
                                            NSCursor.pop()
                                        }
                                    }
                                }
                
                Button(action: {
                    if let saveURL = showSavePanel() {
                        do {
                            try lua.write(to: saveURL, atomically: true, encoding: .utf8)
                        } catch {
                        }
                    }
                }, label: { // 1
                    Image(systemName: "square.and.arrow.down.fill")
                        .foregroundColor(Color(.white))
                    Text("Save")
                })
                .padding(EdgeInsets(top: 10, leading: 0, bottom: 0, trailing: 0))
                .onHover { isHovered in
                                    self.hover = isHovered
                                    DispatchQueue.main.async { //<-- Here
                                        if (self.hover) {
                                            NSCursor.pointingHand.push()
                                        } else {
                                            NSCursor.pop()
                                        }
                                    }
                                }
            }
            
            CodeViewer(
                content: $lua,
                mode: .lua,
                darkTheme: theme,
                lightTheme: theme,
                fontSize: Int(fontSize)
            )
            .fileImporter(isPresented: $do_load, allowedContentTypes: [UTType.text], allowsMultipleSelection: false, onCompletion: onLoadComplete)
            
            HStack {
                Image(systemName: "terminal.fill")
                    .padding(EdgeInsets(top: 0, leading: 0, bottom: 5, trailing: 0))
                Text("Thanks for using Hydrogen! Your key expires in " + (expiryTime - Date()).stringFromTimeInterval())
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(EdgeInsets(top: 0, leading: 0, bottom: 5, trailing: 0))
            }
        }
    }
    
    func send() {
        if ((expiryTime - Date()) < 0) {
            let alert = NSAlert()
            alert.messageText = "Key expired!"
            alert.informativeText = "Your key has expired. Please reopen Hydrogen to continue using it."
            alert.addButton(withTitle: "OK")
            let logoIcon = NSImage(named: .logo)
            alert.icon = logoIcon
            alert.runModal()
            exit(0)
        }
        let utf = lua.compactMap { $0.asciiValue }
        if let serverPort = CFMessagePortCreateRemote(nil, messagePortName as CFString) {
            let bytes: [UInt8] = [UInt8](utf)
            let data = CFDataCreate(nil, bytes, bytes.count)
            let sendResult = CFMessagePortSendRequest(serverPort,
               1,
               data,
               1.0,
               1.0,
               nil,
               nil);
            if sendResult == Int32(kCFMessagePortSuccess) {
               NSLog("Client: success!")
            }
            else {
               NSLog("Client error: \(sendResult)")
            }
        } else {
            NSLog("Client error: Unable to connect to server!")
        }
    }
}
